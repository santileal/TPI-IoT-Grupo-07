// =====================================================================
// TPI IoT - Supervisión ambiental y actuación remota (ESP32)
// DHT22 + Potenciómetro + OLED + ThingSpeak + Bot de Telegram
// Todo en un solo archivo para aprender; después se separa en lib/
// =====================================================================
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ThingSpeak.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

// ---------------------------------------------------------------------
// 1) CONFIGURACIÓN (pines, credenciales, tiempos)
// ---------------------------------------------------------------------
// Pines según el enunciado
#define PIN_DHT       33
#define PIN_POTE      32
#define PIN_LED_VERDE 23   // lo uso como "relé / actuador principal"
#define PIN_LED_AZUL  2    // LED de estado (comandos /ledon y /ledoff)

// WiFi (Wokwi). En la placa real, poné tu red.
const char *WIFI_SSID = "Wokwi-GUEST";
const char *WIFI_PASS = "";

// Telegram: NO subas el token a GitHub
const char *BOT_TOKEN = "PONER_TOKEN_DEL_BOT";

// ThingSpeak: NO subas la API key a GitHub
const unsigned long TS_CHANNEL_ID = 0;              // tu Channel ID
const char *TS_WRITE_API_KEY = "PONER_WRITE_API_KEY";

// Tiempos (ms)
const unsigned long INTERVALO_TELEGRAM = 1000;
const unsigned long INTERVALO_DHT      = 2000;   // DHT22: mínimo 2 s
const unsigned long INTERVALO_TS       = 20000;  // ThingSpeak gratis: mínimo 15 s
const unsigned long INTERVALO_PANTALLA = 500;
const unsigned long INTERVALO_ALERTA   = 1000;

// Umbral crítico del potenciómetro (voltios)
const float UMBRAL_CRITICO = 2.8;
const float HISTERESIS     = 0.2;  // para no mandar alertas repetidas

// ---------------------------------------------------------------------
// 2) OBJETOS Y ESTADO GLOBAL
// ---------------------------------------------------------------------
DHT dht(PIN_DHT, DHT22);
Adafruit_SH1106G display(128, 64, &Wire, -1);

WiFiClientSecure clienteTelegram;   // Telegram usa HTTPS
WiFiClient clienteThingSpeak;       // ThingSpeak usa HTTP
UniversalTelegramBot bot(BOT_TOKEN, clienteTelegram);

// Últimos valores del DHT22 (los usa /clima sin volver a leer el sensor)
float temperatura = NAN;
float humedad = NAN;

// Relé no bloqueante
bool releActivo = false;
unsigned long releFin = 0;

// LED azul
bool ledAzulEncendido = false;

// Chat al que se le mandan las alertas (se guarda en el primer mensaje)
String chatIdAlerta = "";
bool alertaActiva = false;

// Qué muestra el OLED
enum ModoDisplay { DISPLAY_NADA, DISPLAY_LED, DISPLAY_POTE, DISPLAY_DHT, DISPLAY_ERROR };
ModoDisplay modoDisplay = DISPLAY_NADA;
String comandoInvalido = "";

// Temporizadores (guardan el último momento en que se ejecutó cada tarea)
unsigned long tTelegram = 0, tDht = 0, tTs = 0, tPantalla = 0, tAlerta = 0;
unsigned long ultimoEnvioTs = 0;

// ---------------------------------------------------------------------
// 3) PROTOTIPOS
// ---------------------------------------------------------------------
void conectarWiFi();
float leerVoltajePote();
void leerDHT();
void releEncender(unsigned long segundos);
void releApagar();
void actualizarRele();
void dibujarPantalla();
bool enviarThingSpeak(bool incluirPote);
void verificarAlerta();
void procesarMensajes(int n);
void ejecutarComando(const String &chatId, const String &texto);
void enviarTecladoInicio(const String &chatId);
void cmdClima(const String &chatId);
void cmdRele(const String &chatId, const String &texto);
void cmdThingUp(const String &chatId);
void cmdDisplay(const String &chatId, const String &texto);
void cmdLed(const String &chatId, bool encender);

// ---------------------------------------------------------------------
// 4) SETUP
// ---------------------------------------------------------------------
void setup() {
  Serial.begin(115200);

  pinMode(PIN_LED_VERDE, OUTPUT);
  pinMode(PIN_LED_AZUL, OUTPUT);
  digitalWrite(PIN_LED_VERDE, LOW);
  digitalWrite(PIN_LED_AZUL, LOW);

  analogReadResolution(12);  // 0..4095

  dht.begin();

  display.begin(0x3c, true);
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Conectando WiFi...");
  display.display();

  conectarWiFi();

  clienteTelegram.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  ThingSpeak.begin(clienteThingSpeak);

  leerDHT();  // primera lectura
  Serial.println("Sistema listo");
}

// ---------------------------------------------------------------------
// 5) LOOP: solo tareas periódicas con millis(), sin delay()
// ---------------------------------------------------------------------
void loop() {
  unsigned long ahora = millis();

  // Relé: se apaga solo cuando se cumple el tiempo
  actualizarRele();

  // Leer DHT22
  if (ahora - tDht >= INTERVALO_DHT) {
    tDht = ahora;
    leerDHT();
  }

  // Revisar mensajes de Telegram
  if (ahora - tTelegram >= INTERVALO_TELEGRAM) {
    tTelegram = ahora;
    int n = bot.getUpdates(bot.last_message_received + 1);
    while (n) {
      procesarMensajes(n);
      n = bot.getUpdates(bot.last_message_received + 1);
    }
  }

  // Envío periódico a ThingSpeak (temperatura y humedad)
  if (ahora - tTs >= INTERVALO_TS) {
    tTs = ahora;
    enviarThingSpeak(false);
  }

  // Alerta automática por voltaje crítico
  if (ahora - tAlerta >= INTERVALO_ALERTA) {
    tAlerta = ahora;
    verificarAlerta();
  }

  // Refrescar pantalla
  if (ahora - tPantalla >= INTERVALO_PANTALLA) {
    tPantalla = ahora;
    dibujarPantalla();
  }
}

// ---------------------------------------------------------------------
// 6) WIFI
// ---------------------------------------------------------------------
void conectarWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi conectado");
  Serial.println(WiFi.localIP());
}

// ---------------------------------------------------------------------
// 7) SENSORES
// ---------------------------------------------------------------------
float leerVoltajePote() {
  int adc = analogRead(PIN_POTE);
  return (adc * 3.3) / 4095.0;
}

void leerDHT() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  if (isnan(t) || isnan(h)) {
    Serial.println("Error leyendo el DHT22");
    return;  // conservo los últimos valores válidos
  }
  temperatura = t;
  humedad = h;
}

// ---------------------------------------------------------------------
// 8) ACTUADORES (relé no bloqueante)
// ---------------------------------------------------------------------
void releEncender(unsigned long segundos) {
  digitalWrite(PIN_LED_VERDE, HIGH);
  releActivo = true;
  releFin = millis() + segundos * 1000UL;
}

void releApagar() {
  digitalWrite(PIN_LED_VERDE, LOW);
  releActivo = false;
}

void actualizarRele() {
  // La resta con signo evita problemas cuando millis() da la vuelta
  if (releActivo && (long)(millis() - releFin) >= 0) {
    releApagar();
    if (chatIdAlerta != "") {
      bot.sendMessage(chatIdAlerta, "Rele apagado automaticamente (tiempo cumplido)");
    }
  }
}

// ---------------------------------------------------------------------
// 9) PANTALLA OLED
// ---------------------------------------------------------------------
void dibujarPantalla() {
  display.clearDisplay();
  display.setCursor(0, 0);

  switch (modoDisplay) {
    case DISPLAY_LED:
      display.println("== LED ==");
      display.println(ledAzulEncendido ? "Led encendido" : "Led apagado");
      break;

    case DISPLAY_POTE:
      display.println("== POTENCIOMETRO ==");
      display.print("ADC: ");
      display.println(analogRead(PIN_POTE));
      display.print("Voltaje: ");
      display.print(leerVoltajePote(), 2);
      display.println(" V");
      break;

    case DISPLAY_DHT:
      display.println("== DHT22 ==");
      display.print("Temp: ");
      display.print(temperatura, 1);
      display.println(" C");
      display.print("Hum: ");
      display.print(humedad, 1);
      display.println(" %");
      break;

    case DISPLAY_ERROR:
      display.println("Comando no");
      display.println("identificado:");
      display.println(comandoInvalido);
      break;

    default:
      display.println("TPI IoT - UTN FRC");
      display.println("Usa /start en Telegram");
      break;
  }
  display.display();
}

// ---------------------------------------------------------------------
// 10) THINGSPEAK
// ---------------------------------------------------------------------
// Campos del canal: 1 = temperatura, 2 = humedad, 3 = voltaje del pote
bool enviarThingSpeak(bool incluirPote) {
  if (isnan(temperatura) || isnan(humedad)) return false;

  ThingSpeak.setField(1, temperatura);
  ThingSpeak.setField(2, humedad);
  if (incluirPote) {
    ThingSpeak.setField(3, leerVoltajePote());
  }

  int codigo = ThingSpeak.writeFields(TS_CHANNEL_ID, TS_WRITE_API_KEY);
  if (codigo == 200) {
    ultimoEnvioTs = millis();
    Serial.println("ThingSpeak OK");
    return true;
  }
  Serial.printf("ThingSpeak error: %d\n", codigo);
  return false;
}

// ---------------------------------------------------------------------
// 11) ALERTA AUTOMÁTICA (sin que el usuario la pida)
// ---------------------------------------------------------------------
void verificarAlerta() {
  float v = leerVoltajePote();

  if (v >= UMBRAL_CRITICO && !alertaActiva) {
    alertaActiva = true;
    if (chatIdAlerta != "") {
      bot.sendMessage(chatIdAlerta,
        "ALERTA: voltaje critico " + String(v, 2) + " V (umbral " + String(UMBRAL_CRITICO, 1) + " V)");
    }
  } else if (v < UMBRAL_CRITICO - HISTERESIS) {
    alertaActiva = false;  // rearmo la alerta
  }
}

// ---------------------------------------------------------------------
// 12) BOT DE TELEGRAM
// ---------------------------------------------------------------------
void procesarMensajes(int n) {
  for (int i = 0; i < n; i++) {
    String chatId = bot.messages[i].chat_id;
    // Si viene de un botón inline, "text" trae el callback_data
    String texto = bot.messages[i].text;
    texto.trim();

    chatIdAlerta = chatId;  // a este chat se le mandan las alertas
    Serial.println("Mensaje: " + texto);
    ejecutarComando(chatId, texto);
  }
}

void ejecutarComando(const String &chatId, const String &texto) {
  if (texto == "/start") {
    enviarTecladoInicio(chatId);
  } else if (texto == "/clima") {
    cmdClima(chatId);
  } else if (texto.startsWith("/releon") || texto.startsWith("/releoff")) {
    cmdRele(chatId, texto);
  } else if (texto == "/thingup") {
    cmdThingUp(chatId);
  } else if (texto.startsWith("/display")) {
    cmdDisplay(chatId, texto);
  } else if (texto == "/ledon") {
    cmdLed(chatId, true);
  } else if (texto == "/ledoff") {
    cmdLed(chatId, false);
  } else {
    bot.sendMessage(chatId, "Comando no reconocido. Usa /start");
  }
}

void enviarTecladoInicio(const String &chatId) {
  // Cada botón: "text" = lo que se ve, "callback_data" = comando que llega al ESP32
  String teclado =
    "[[{\"text\":\"Ver Clima\",\"callback_data\":\"/clima\"},"
    "{\"text\":\"Encender Rele 10s\",\"callback_data\":\"/releon 10\"}],"
    "[{\"text\":\"Ver Display\",\"callback_data\":\"/displaydht\"},"
    "{\"text\":\"Enviar a ThingSpeak\",\"callback_data\":\"/thingup\"}]]";

  bot.sendMessageWithInlineKeyboard(chatId, "Bienvenido al bot ESP32. Elegi una opcion:", "", teclado);
}

void cmdClima(const String &chatId) {
  if (isnan(temperatura) || isnan(humedad)) {
    bot.sendMessage(chatId, "Todavia no hay lecturas validas del DHT22");
    return;
  }
  String msg = "Temperatura: " + String(temperatura, 1) + " C\n";
  msg += "Humedad: " + String(humedad, 1) + " %";
  bot.sendMessage(chatId, msg);
}

// Formatos: "/releon 10" (10 segundos) | "/releon" (5 s por defecto) | "/releoff"
void cmdRele(const String &chatId, const String &texto) {
  if (texto.startsWith("/releoff")) {
    releApagar();
    bot.sendMessage(chatId, "Rele apagado");
    return;
  }

  long segundos = texto.substring(7).toInt();  // lo que viene después de "/releon"
  if (segundos <= 0) segundos = 5;

  releEncender(segundos);
  bot.sendMessage(chatId, "Rele encendido por " + String(segundos) + " segundos");
}

void cmdThingUp(const String &chatId) {
  float v = leerVoltajePote();
  String msg = "Voltaje del potenciometro: " + String(v, 2) + " V\n";

  // ThingSpeak gratis acepta una escritura cada 15 s
  unsigned long desdeUltimo = millis() - ultimoEnvioTs;
  if (ultimoEnvioTs != 0 && desdeUltimo < 15000) {
    msg += "Espera " + String((15000 - desdeUltimo) / 1000 + 1) + " s para volver a enviar";
  } else if (enviarThingSpeak(true)) {
    msg += "Enviado a ThingSpeak";
  } else {
    msg += "Error al enviar a ThingSpeak";
  }
  bot.sendMessage(chatId, msg);

  // Evaluación del umbral crítico
  if (v >= UMBRAL_CRITICO) {
    bot.sendMessage(chatId, "ALERTA DE SEGURIDAD: voltaje por encima del umbral critico");
  }
}

// /displayled | /displaypote | /displaydht | cualquier otra cosa -> error en pantalla
void cmdDisplay(const String &chatId, const String &texto) {
  String cmd = texto.substring(8);  // lo que viene después de "/display"
  cmd.toLowerCase();

  if (cmd == "led") {
    modoDisplay = DISPLAY_LED;
    bot.sendMessage(chatId, "Mostrando estado del LED en el display");
  } else if (cmd == "pote") {
    modoDisplay = DISPLAY_POTE;
    bot.sendMessage(chatId, "Mostrando potenciometro en el display");
  } else if (cmd == "dht") {
    modoDisplay = DISPLAY_DHT;
    bot.sendMessage(chatId, "Mostrando DHT22 en el display");
  } else {
    modoDisplay = DISPLAY_ERROR;
    comandoInvalido = texto;
    bot.sendMessage(chatId, "Comando de display no identificado (se informa en la pantalla)");
  }
  dibujarPantalla();  // que se vea al instante
}

void cmdLed(const String &chatId, bool encender) {
  ledAzulEncendido = encender;
  digitalWrite(PIN_LED_AZUL, encender ? HIGH : LOW);
  bot.sendMessage(chatId, encender ? "LED azul encendido" : "LED azul apagado");
}