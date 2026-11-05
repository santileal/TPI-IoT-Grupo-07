#pragma once
#include <Arduino.h>
#include <DHTesp.h>

class Sensores {
public:
  Sensores(uint8_t pinDht, uint8_t pinPote);

  void iniciar();
  void actualizar();

  float temperatura() const { return _temp; }
  float humedad() const { return _hum; }

  bool dhtValido() const {
    return !isnan(_temp) && !isnan(_hum);
  }

  int leerAdc() const;
  float leerVoltaje() const;

private:
  DHTesp _dht;

  uint8_t _pinDht;
  uint8_t _pinPote;

  float _temp = NAN;
  float _hum = NAN;

  unsigned long _ultimaLectura = 0;
};