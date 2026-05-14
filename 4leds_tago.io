/*
  ============================================================
   Control de 4 LEDs con Thinger.io — ESP32
  ============================================================

  LIBRERÍAS NECESARIAS (Gestor de Librerías del IDE Arduino):
    1. "Thinger.io"  →  buscar: ThingerESP32
    2. Board package: "esp32 by Espressif Systems"

  CONEXIONES:
    LED 1  →  GPIO 5   + resistencia 220Ω → GND
    LED 2  →  GPIO 4   + resistencia 220Ω → GND
    LED 3  →  GPIO 18  + resistencia 220Ω → GND
    LED 4  →  GPIO 19  + resistencia 220Ω → GND

  RECURSOS en Thinger.io (widgets tipo "Switch"):
    "led1", "led2", "led3", "led4", "todos"
  ============================================================
*/

#include <WiFi.h>
#include <ThingerESP32.h>

// ── Credenciales Thinger.io ──────────────────────────────────
#define USERNAME          "Vanny"
#define DEVICE_ID         "leds4"
#define DEVICE_CREDENTIAL "12345678"

// ── Credenciales WiFi ────────────────────────────────────────
#define SSID          "RED MECATRONICA"
#define WIFI_PASSWORD "MECA2026@."

// ── Pines GPIO ───────────────────────────────────────────────
#define PIN_LED1  5
#define PIN_LED2  4
#define PIN_LED3  18
#define PIN_LED4  19

// ── Objeto Thinger.io ────────────────────────────────────────
ThingerESP32 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

// ── Estado de cada LED ───────────────────────────────────────
bool led1 = false;
bool led2 = false;
bool led3 = false;
bool led4 = false;

// ── Función auxiliar para aplicar estado a un pin ───────────
void setLed(uint8_t pin, bool estado) {
  digitalWrite(pin, estado ? HIGH : LOW);
}

// ────────────────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);
  Serial.println("\n=== Thinger.io - Control 4 LEDs (ESP32) ===");

  // Configura pines
  pinMode(PIN_LED1, OUTPUT);
  pinMode(PIN_LED2, OUTPUT);
  pinMode(PIN_LED3, OUTPUT);
  pinMode(PIN_LED4, OUTPUT);

  // Apaga todos al arrancar
  setLed(PIN_LED1, false);
  setLed(PIN_LED2, false);
  setLed(PIN_LED3, false);
  setLed(PIN_LED4, false);

  // Conecta al WiFi
  thing.add_wifi(SSID, WIFI_PASSWORD);

  // ── Recursos Thinger.io ─────────────────────────────────────
  // Cada recurso responde lectura (envía estado actual) y
  // escritura (recibe el valor del widget Switch del dashboard).

  thing["led1"] << [](pson& in) {
    if (in.is_empty()) {
      in = led1;
    } else {
      led1 = (bool)in;
      setLed(PIN_LED1, led1);
      Serial.printf("LED 1 (GPIO %d): %s\n", PIN_LED1, led1 ? "ON" : "OFF");
    }
  };

  thing["led2"] << [](pson& in) {
    if (in.is_empty()) {
      in = led2;
    } else {
      led2 = (bool)in;
      setLed(PIN_LED2, led2);
      Serial.printf("LED 2 (GPIO %d): %s\n", PIN_LED2, led2 ? "ON" : "OFF");
    }
  };

  thing["led3"] << [](pson& in) {
    if (in.is_empty()) {
      in = led3;
    } else {
      led3 = (bool)in;
      setLed(PIN_LED3, led3);
      Serial.printf("LED 3 (GPIO %d): %s\n", PIN_LED3, led3 ? "ON" : "OFF");
    }
  };

  thing["led4"] << [](pson& in) {
    if (in.is_empty()) {
      in = led4;
    } else {
      led4 = (bool)in;
      setLed(PIN_LED4, led4);
      Serial.printf("LED 4 (GPIO %d): %s\n", PIN_LED4, led4 ? "ON" : "OFF");
    }
  };

  // Recurso para controlar todos a la vez
  thing["todos"] << [](pson& in) {
    if (in.is_empty()) {
      in = (led1 && led2 && led3 && led4);
    } else {
      bool estado = (bool)in;
      led1 = led2 = led3 = led4 = estado;
      setLed(PIN_LED1, estado);
      setLed(PIN_LED2, estado);
      setLed(PIN_LED3, estado);
      setLed(PIN_LED4, estado);
      Serial.printf("Todos los LEDs: %s\n", estado ? "ON" : "OFF");
    }
  };

  Serial.println("Recursos registrados. Conectando...");
}

// ────────────────────────────────────────────────────────────
void loop() {
  thing.handle();  // mantiene la conexión con Thinger.io
}
