#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

#include "Sensores.h"
#include "Actuadores.h"
#include "Pantalla.h"
#include "ThingSpeakClient.h"

class BotTelegram {
public:
  BotTelegram(
    Sensores &sensores,
    Actuadores &actuadores,
    Pantalla &pantalla,
    ThingSpeakClient &thingSpeak
  );

  void iniciar();
  void actualizar();

private:
  WiFiClientSecure _clienteSeguro;
  UniversalTelegramBot _bot;

  Sensores &_sensores;
  Actuadores &_actuadores;
  Pantalla &_pantalla;
  ThingSpeakClient &_thingSpeak;

  unsigned long _ultimaConsulta = 0;

  void procesarMensaje(int indice);
  void enviarMensaje(const String &chatId, const String &texto);
  void registrarComando(const String &comando);
};