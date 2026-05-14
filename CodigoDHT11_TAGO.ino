#include "DHT.h"
#include <HTTPClient.h>
#include <WiFi.h>

// Configuración TagoIO
char serverAddress[] = "https://api.tago.io/data";
char contentHeader[] = "application/json";
char tokenHeader[] = "a79e58e1-cdd9-4a19-92a0-5ee00e8834a5";

// WiFi
const char* ssid = "RED MECATRONICA";
const char* password = "MECA2026@.";

// DHT
#define DHTPIN 5
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  delay(10);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Conectando");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  dht.begin();
}

void loop() {

  delay(2000);

  char postData[300];
  char buffer[30];

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Error leyendo el DHT");
    return;
  }

  float hic = dht.computeHeatIndex(t, h, false);

  HTTPClient client;

  // ================= HUMEDAD =================
  Serial.print("Humedad: ");
  Serial.println(h);

  sprintf(postData,
    "{ \"variable\": \"Humidity\", \"value\": %.2f, \"unit\": \"%%\" }",
    h
  );

  client.begin(serverAddress);
  client.addHeader("Content-Type", contentHeader);
  client.addHeader("Device-Token", tokenHeader);

  int statusCode = client.POST(postData);

  Serial.print("Status: ");
  Serial.println(statusCode);
  client.end();

  delay(30000);

  // ================= TEMPERATURA =================
  Serial.print("Temperatura: ");
  Serial.println(t);

  sprintf(postData,
    "{ \"variable\": \"Temperature\", \"value\": %.2f, \"unit\": \"C\" }",
    t
  );

  client.begin(serverAddress);
  client.addHeader("Content-Type", contentHeader);
  client.addHeader("Device-Token", tokenHeader);

  statusCode = client.POST(postData);

  Serial.print("Status: ");
  Serial.println(statusCode);
  client.end();

  delay(30000);

  // ================= ÍNDICE DE CALOR =================
  Serial.print("Heat Index: ");
  Serial.println(hic);

  sprintf(postData,
    "{ \"variable\": \"HIndex\", \"value\": %.2f, \"unit\": \"C\" }",
    hic
  );

  client.begin(serverAddress);
  client.addHeader("Content-Type", contentHeader);
  client.addHeader("Device-Token", tokenHeader);

  statusCode = client.POST(postData);

  Serial.print("Status: ");
  Serial.println(statusCode);
  client.end();

  delay(30000);
}
