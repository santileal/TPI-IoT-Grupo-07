#include "Pantalla.h"

static constexpr uint8_t ALTO_FILA     = 8;
static constexpr uint8_t MAX_CARACTERES = 21;   // 128 px / 6 px por caracter

Pantalla::Pantalla(const Sensores &sensores, const Actuadores &actuadores)
  : _oled(OLED_ANCHO, OLED_ALTO, &Wire, -1),
    _sensores(sensores),
    _actuadores(actuadores) {}

bool Pantalla::iniciar() {
  Wire.begin(PIN_SDA, PIN_SCL);
#if defined(OLED_SH1106)
  bool ok = _oled.begin(OLED_ADDR, true);
#else
  bool ok = _oled.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
#endif
  if (!ok) {
    Serial.println("No se encontro el OLED");
    return false;
  }
  _oled.setTextSize(1);
  _oled.setTextColor(OLED_BLANCO);
  return true;
}

void Pantalla::mostrarMensaje(const char *linea1, const char *linea2) {
  _oled.clearDisplay();
  this->linea(0, linea1);
  this->linea(1, linea2);
  _oled.display();
}

void Pantalla::setVista(Vista vista) {
  _vista = vista;
  dibujar();            // que se vea al instante
}

void Pantalla::mostrarError(const String &comando) {
  _comandoInvalido = comando;
  setVista(Vista::ERROR_CMD);
}

// Corre el historial una posicion y guarda el nuevo comando en [0].
void Pantalla::registrarComando(const String &comando) {
  for (int i = MAX_HISTORIAL - 1; i > 0; i--) {
    _historial[i] = _historial[i - 1];
  }
  _historial[0] = comando;
  dibujar();
}

void Pantalla::actualizar() {
  if (millis() - _ultimoDibujo < INTERVALO_PANTALLA) return;
  dibujar();
}

void Pantalla::dibujar() {
  _ultimoDibujo = millis();
  _oled.clearDisplay();
  dibujarVista();
  dibujarHistorial();
  _oled.display();
}

// ---------------------------------------------------------------------
// Privados
// ---------------------------------------------------------------------
void Pantalla::linea(uint8_t fila, const String &texto) {
  String t = texto;
  if (t.length() > MAX_CARACTERES) t = t.substring(0, MAX_CARACTERES);
  _oled.setCursor(0, fila * ALTO_FILA);
  _oled.print(t);
}

void Pantalla::dibujarVista() {
  switch (_vista) {
    case Vista::LED:
      linea(0, "== LED ==");
      linea(1, String("Led azul ") + (_actuadores.azulEncendido() ? "encendido" : "apagado"));
      linea(2, String("Led verde ") + (_actuadores.verdeEncendido() ? "encendido" : "apagado"));
      break;

    case Vista::POTE:
      linea(0, "== POTENCIOMETRO ==");
      linea(1, "ADC: " + String(_sensores.leerAdc()));
      linea(2, "Voltaje: " + String(_sensores.leerVoltaje(), 2) + " V");
      break;

    case Vista::DHT:
      linea(0, "== DHT22 ==");
      if (_sensores.dhtValido()) {
        linea(1, "Temp: " + String(_sensores.temperatura(), 1) + " C");
        linea(2, "Hum: " + String(_sensores.humedad(), 1) + " %");
      } else {
        linea(1, "Sin lectura valida");
      }
      break;

    case Vista::ERROR_CMD:
      linea(0, "Comando no");
      linea(1, "identificado:");
      linea(2, _comandoInvalido);
      break;

    default:  // INICIO
      linea(0, "TPI IoT - UTN FRC");
      linea(1, "Usa /start en");
      linea(2, "Telegram");
      break;
  }
}

void Pantalla::dibujarHistorial() {
  linea(4, "- Ultimos comandos -");
  for (uint8_t i = 0; i < MAX_HISTORIAL; i++) {
    String texto = _historial[i].length() ? _historial[i] : String("-");
    linea(5 + i, texto);
  }
}