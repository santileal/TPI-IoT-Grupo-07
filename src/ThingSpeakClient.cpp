
#include "ThingSpeakClient.h"
#include "Config.h"

ThingSpeakClient::ThingSpeakClient() {}

void ThingSpeakClient::iniciar() {
  ThingSpeak.begin(_cliente);
  Serial.println("Cliente ThingSpeak iniciado");
}

bool ThingSpeakClient::enviar(float temperatura, float humedad) {

  // Verificar conexion Wi-Fi.
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("ThingSpeak: sin conexion Wi-Fi");
    return false;
  }

  // Respetar el minimo de 15 segundos entre intentos.
  if (_huboIntento &&
      millis() - _ultimoIntento < TS_MIN_ENTRE_ENVIOS) {

    Serial.println("ThingSpeak: esperar intervalo minimo");
    return false;
  }

  _ultimoIntento = millis();
  _huboIntento = true;

  // Preparar los campos del canal.
  ThingSpeak.setField(1, temperatura);
  ThingSpeak.setField(2, humedad);

  // Enviar informacion.
  int respuesta = ThingSpeak.writeFields(
    TS_CHANNEL_ID,
    TS_WRITE_KEY
  );

  if (respuesta == 200) {
    Serial.println("ThingSpeak: envio exitoso");
    return true;
  }

  Serial.print("ThingSpeak: error HTTP ");
  Serial.println(respuesta);

  return false;
}
