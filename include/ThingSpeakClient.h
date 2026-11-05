
#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <ThingSpeak.h>

class ThingSpeakClient {
public:
  ThingSpeakClient();

  void iniciar();

  // Envia temperatura y humedad a ThingSpeak.
  // Devuelve true si el servidor confirma el envio.
  bool enviar(float temperatura, float humedad);

private:
  WiFiClient _cliente;

  unsigned long _ultimoIntento = 0;
  bool _huboIntento = false;
};
