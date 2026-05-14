#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME680.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// WiFi
const char* WIFI_SSID     = "RED MECATRONICA";
const char* WIFI_PASSWORD = "MECA2026@.";

// TagoIO
const char* TAGO_TOKEN = "5c7ca4de-63a5-475a-8be8-babccc2b5bdc";
const char* TAGO_URL   = "https://api.tago.io/data";

// Sensor
#define BME680_ADDR 0x76
#define SEALEVELPRESSURE_HPA 1013.25

Adafruit_BME680 bme;

void conectarWiFi();
bool enviarTagoIO(float temp, float hum, float pres, float alt, float gas);

// ─────────────────────────────
void setup() {
  Serial.begin(115200);

  if (!bme.begin(BME680_ADDR)) {
    Serial.println("ERROR: BME680 no encontrado");
    while (1);
  }

  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150);

  conectarWiFi();
}

// ─────────────────────────────
void loop() {

  if (WiFi.status() != WL_CONNECTED) {
    conectarWiFi();
  }

  if (!bme.performReading()) {
    Serial.println("Error al leer BME680");
    delay(2000);
    return;
  }

  float temperatura = bme.temperature;
  float humedad     = bme.humidity;
  float presion     = bme.pressure / 100.0;
  float altitud     = bme.readAltitude(SEALEVELPRESSURE_HPA);
  float gas         = bme.gas_resistance / 1000.0;

  Serial.println("-----");
  Serial.println(temperatura);
  Serial.println(humedad);
  Serial.println(presion);
  Serial.println(altitud);
  Serial.println(gas);

  if (enviarTagoIO(temperatura, humedad, presion, altitud, gas)) {
    Serial.println("Datos enviados ✔");
  } else {
    Serial.println("Error envío ✘");
  }

  delay(2000);
}

// ─────────────────────────────
void conectarWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Conectando");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi conectado");
}

// ─────────────────────────────
bool enviarTagoIO(float temp, float hum, float pres, float alt, float gas) {

  DynamicJsonDocument doc(512);
  JsonArray arr = doc.to<JsonArray>();

  JsonObject t = arr.createNestedObject();
  t["variable"] = "temperature";
  t["value"] = temp;
  t["unit"] = "C";

  JsonObject h = arr.createNestedObject();
  h["variable"] = "humidity";
  h["value"] = hum;
  h["unit"] = "%";

  JsonObject p = arr.createNestedObject();
  p["variable"] = "pressure";
  p["value"] = pres;
  p["unit"] = "hPa";

  JsonObject a = arr.createNestedObject();
  a["variable"] = "altitude";
  a["value"] = alt;
  a["unit"] = "m";

  JsonObject g = arr.createNestedObject();
  g["variable"] = "gas";
  g["value"] = gas;
  g["unit"] = "KOhm";

  String payload;
  serializeJson(doc, payload);

  WiFiClientSecure client;
  client.setInsecure();  // 🔥 solución simple

  HTTPClient https;
  https.begin(client, TAGO_URL);

  https.addHeader("Content-Type", "application/json");
  https.addHeader("Device-Token", TAGO_TOKEN);

  int httpCode = https.POST(payload);

  if (httpCode > 0) {
    Serial.println(httpCode);
    https.end();
    return true;
  } else {
    Serial.println("Error HTTP");
  }

  https.end();
  return false;
}
