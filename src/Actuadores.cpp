#include "Actuadores.h"
#include "Config.h"

Actuadores::Actuadores(uint8_t pinVerde, uint8_t pinAzul)
  : _pinVerde(pinVerde), _pinAzul(pinAzul), _umbral(UMBRAL_DEFECTO) {}

void Actuadores::iniciar() {
  pinMode(_pinVerde, OUTPUT);
  pinMode(_pinAzul, OUTPUT);
  digitalWrite(_pinVerde, LOW);
  setAzul(false);
}

bool Actuadores::setUmbral(float valor) {
  if (valor >= UMBRAL_MIN && valor <= UMBRAL_MAX) {
    _umbral = valor;
    return true;
  }
  _umbral = UMBRAL_DEFECTO;   // fuera de rango -> valor original
  return false;
}

void Actuadores::activarAuto(float voltajeActual) {
  _base = voltajeActual;
  _auto = true;
  digitalWrite(_pinVerde, HIGH);
}

void Actuadores::desactivarAuto() {
  _auto = false;
  digitalWrite(_pinVerde, LOW);
  setAzul(false);             // vuelve a manual con el azul apagado
}

void Actuadores::actualizar(float voltaje) {
  if (!_auto) return;
  float disparo = _base + _umbral;
  if (voltaje > disparo)                     setAzul(true);
  else if (voltaje < disparo - HISTERESIS)   setAzul(false);
}

void Actuadores::setAzul(bool encendido) {
  _azul = encendido;
  digitalWrite(_pinAzul, encendido ? HIGH : LOW);
}