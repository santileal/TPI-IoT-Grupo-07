#pragma once
#include <Arduino.h>
#include "Secrets.h"

// ---------------- Pines (segun enunciado) ----------------
constexpr uint8_t PIN_DHT       = 33;
constexpr uint8_t PIN_POTE      = 32;   // ADC
constexpr uint8_t PIN_LED_VERDE = 23;
constexpr uint8_t PIN_LED_AZUL  = 2;
constexpr uint8_t PIN_SDA       = 21;
constexpr uint8_t PIN_SCL       = 22;

// ---------------- OLED ----------------
// Descomentar el controlador que tenga la placa UTN (en Wokwi es SSD1306).
#define OLED_SSD1306
// #define OLED_SH1106
constexpr uint8_t OLED_ANCHO = 128;
constexpr uint8_t OLED_ALTO  = 64;
constexpr uint8_t OLED_ADDR  = 0x3C;

// ---------------- Credenciales (vienen de Secrets.h) ----------------
constexpr const char *WIFI_SSID     = WIFI_SSID_VALOR;
constexpr const char *WIFI_PASS     = WIFI_PASS_VALOR;
constexpr const char *BOT_TOKEN     = BOT_TOKEN_VALOR;
constexpr unsigned long TS_CHANNEL_ID = TS_CHANNEL_ID_VALOR;
constexpr const char *TS_WRITE_KEY  = TS_WRITE_KEY_VALOR;

// ---------------- Tiempos (ms) ----------------
constexpr unsigned long INTERVALO_TELEGRAM = 1000;
constexpr unsigned long INTERVALO_DHT      = 2000;    // DHT22: mínimo 2 s
constexpr unsigned long INTERVALO_TS       = 20000;   // ThingSpeak gratis: mínimo 15 s
constexpr unsigned long INTERVALO_PANTALLA = 500;
constexpr unsigned long INTERVALO_AUTO     = 200;
constexpr unsigned long TS_MIN_ENTRE_ENVIOS = 15000;

// ---------------- Umbral del potenciometro (V) ----------------
constexpr float UMBRAL_DEFECTO = 0.5f;
constexpr float UMBRAL_MIN     = 1.0f;
constexpr float UMBRAL_MAX     = 1.5f;
constexpr float HISTERESIS     = 0.05f;  // evita parpadeo del LED azul en modo auto
 
// ---------------- ADC ----------------
constexpr float ADC_VREF = 3.3f;
constexpr int   ADC_MAX  = 4095;