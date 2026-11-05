#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Config.h"
#include "Sensores.h"
#include "Actuadores.h"

// El controlador del OLED se elige en Config.h (OLED_SSD1306 / OLED_SH1106).
#if defined(OLED_SH1106)
  #include <Adafruit_SH110X.h>
  typedef Adafruit_SH1106G OledDriver;
  #define OLED_BLANCO SH110X_WHITE
#else
  #include <Adafruit_SSD1306.h>
  typedef Adafruit_SSD1306 OledDriver;
  #define OLED_BLANCO SSD1306_WHITE
#endif

// Layout (8 filas de texto de 8 px, 21 caracteres por fila):
//   filas 0-3 : estado del componente pedido (/displayled, /displaypote, ...)
//   fila  4   : encabezado del historial
//   filas 5-7 : ultimos 3 comandos recibidos con exito (el mas nuevo primero)
class Pantalla {
public:
  enum class Vista { INICIO, LED, POTE, DHT, ERROR_CMD };
  static constexpr uint8_t MAX_HISTORIAL = 3;

  Pantalla(const Sensores &sensores, const Actuadores &actuadores);

  bool iniciar();

  // Mensaje de pantalla completa (arranque, errores de red). No toca el historial.
  void mostrarMensaje(const char *linea1, const char *linea2 = "");

  void setVista(Vista vista);
  void mostrarError(const String &comando);   // comando /display... no identificado
  void registrarComando(const String &comando);  // guardar SOLO comandos exitosos

  void actualizar();   // llamar en loop(); redibuja cada INTERVALO_PANTALLA
  void dibujar();      // redibujo inmediato

private:
  void linea(uint8_t fila, const String &texto);
  void dibujarVista();
  void dibujarHistorial();

  OledDriver _oled;
  const Sensores &_sensores;
  const Actuadores &_actuadores;

  Vista  _vista = Vista::INICIO;
  String _comandoInvalido;
  String _historial[MAX_HISTORIAL];   // [0] = mas reciente
  unsigned long _ultimoDibujo = 0;
};