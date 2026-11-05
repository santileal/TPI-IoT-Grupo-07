#pragma once
#include <Arduino.h>

// LED verde = indicador de modo automatico activo.
// LED azul  = actuador: en modo auto se enciende si el voltaje del pote
//             supera (linea base + umbral).
class Actuadores {
public:
  Actuadores(uint8_t pinVerde, uint8_t pinAzul);

  void  iniciar();

  // --- Umbral ---
  // Devuelve true si el valor estaba en [UMBRAL_MIN; UMBRAL_MAX].
  // Si esta fuera de rango, el umbral vuelve al valor original (UMBRAL_DEFECTO).
  bool  setUmbral(float valor);
  float umbral() const { return _umbral; }

  // --- Modo automatico ---
  // Al activarlo toma el voltaje actual como linea base.
  void  activarAuto(float voltajeActual);
  void  desactivarAuto();
  bool  modoAuto()      const { return _auto; }
  float lineaBase()     const { return _base; }

  // Llamar periodicamente con el voltaje del potenciometro.
  // Solo actua si el modo automatico esta activo.
  void  actualizar(float voltaje);

  // --- LED azul manual / estado ---
  void  setAzul(bool encendido);
  bool  azulEncendido() const { return _azul; }
  bool  verdeEncendido() const { return _auto; }

private:
  uint8_t _pinVerde, _pinAzul;
  float   _umbral;
  float   _base = 0.0f;
  bool    _auto = false;
  bool    _azul = false;
};