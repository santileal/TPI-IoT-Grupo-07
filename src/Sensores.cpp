#include "Sensores.h"
#include "Config.h"

Sensores::Sensores(uint8_t pinDht, uint8_t pinPote)
    : _pinDht(pinDht), _pinPote(pinPote) {}

void Sensores::iniciar()
{
  analogReadResolution(12);

  _dht.setup(_pinDht, DHTesp::DHT22);

  _ultimaLectura = millis();
}

void Sensores::actualizar()
{
  if (millis() - _ultimaLectura < INTERVALO_DHT)
    return;

  _ultimaLectura = millis();

  TempAndHumidity datos = _dht.getTempAndHumidity();

  Serial.print("Estado DHT22: ");
  Serial.println(_dht.getStatusString());

  Serial.print("Temperatura: ");
  Serial.println(datos.temperature);

  Serial.print("Humedad: ");
  Serial.println(datos.humidity);

  if (isnan(datos.temperature) || isnan(datos.humidity))
  {
    Serial.println("Error leyendo el DHT22");
    return;
  }

  _temp = datos.temperature;
  _hum = datos.humidity;
}

int Sensores::leerAdc() const
{
  return analogRead(_pinPote);
}

float Sensores::leerVoltaje() const
{
  return (leerAdc() * ADC_VREF) / ADC_MAX;
}