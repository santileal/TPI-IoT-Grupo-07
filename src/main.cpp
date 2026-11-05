
#include <Arduino.h>
#include <WiFi.h>

#include "Config.h"
#include "Sensores.h"
#include "Actuadores.h"
#include "Pantalla.h"
#include "ThingSpeakClient.h"
#include "BotTelegram.h"

// --------------------------------------------------
// Objetos del sistema
// --------------------------------------------------

Sensores sensores(PIN_DHT, PIN_POTE);

Actuadores actuadores(
  PIN_LED_VERDE,
  PIN_LED_AZUL
);

Pantalla pantalla(sensores, actuadores);

ThingSpeakClient thingSpeak;

BotTelegram botTelegram(
  sensores,
  actuadores,
  pantalla,
  thingSpeak
);

// --------------------------------------------------
// Control de tiempos
// --------------------------------------------------

unsigned long ultimaActualizacionTS = 0;
unsigned long ultimoIntentoWiFi = 0;

// --------------------------------------------------
// SETUP
// --------------------------------------------------

void setup() {
  Serial.begin(115200);

  Serial.println();
  Serial.println("Iniciando sistema IoT - UTN FRC");

  // Inicializar modulos
  actuadores.iniciar();
  sensores.iniciar();

  if (pantalla.iniciar()) {
    pantalla.setVista(Pantalla::Vista::INICIO);
  }

  // Configurar Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  Serial.println("Conectando a Wi-Fi...");

  // Inicializar clientes de comunicacion
  thingSpeak.iniciar();
  botTelegram.iniciar();

  // Evita enviar datos inmediatamente al arrancar.
  ultimaActualizacionTS = millis();

  Serial.println("Sistema iniciado");
}

// --------------------------------------------------
// LOOP
// --------------------------------------------------

void loop() {

  // 1. Mantener actualizadas las lecturas de sensores
  sensores.actualizar();

  // 2. Actualizar el control automatico de los LEDs
  float voltaje = sensores.leerVoltaje();
  actuadores.actualizar(voltaje);

  // 3. Procesar comandos recibidos por Telegram
  botTelegram.actualizar();

  // 4. Actualizar la pantalla OLED
  pantalla.actualizar();

  // 5. Reintentar conexion Wi-Fi si se desconecto
  if (WiFi.status() != WL_CONNECTED) {

    if (millis() - ultimoIntentoWiFi >= 10000) {
      ultimoIntentoWiFi = millis();

      Serial.println("Wi-Fi desconectado. Reintentando...");
      WiFi.reconnect();
    }

    return;
  }

  // 6. Enviar telemetria periodicamente a ThingSpeak
  if (millis() - ultimaActualizacionTS >= INTERVALO_TS) {

    ultimaActualizacionTS = millis();

    if (sensores.dhtValido()) {

      bool enviado = thingSpeak.enviar(
        sensores.temperatura(),
        sensores.humedad()
      );

      if (enviado) {
        Serial.println("Telemetria enviada a ThingSpeak");
      }

    } else {
      Serial.println("ThingSpeak: lecturas DHT no validas");
    }
  }
}
