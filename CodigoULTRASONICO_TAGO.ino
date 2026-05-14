#include <HTTPClient.h>
#include <WiFi.h>

// Configuración TagoIO
char serverAddress[] = "https://api.tago.io/data";
char contentHeader[] = "application/json";
char tokenHeader[] = "d9ab1b2e-d712-4635-8d90-c3ef4e09dfbf";

// WiFi
const char* ssid = "RED MECATRONICA";
const char* password = "MECA2026@.;

// Sensor ultrasónico (HC-SR04)
#define TRIG_PIN 5
#define ECHO_PIN 18

void setup() {
  Serial.begin(115200);

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

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {

  char postData[200];

  // Generar pulso ultrasónico
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Leer duración del eco
  long duration = pulseIn(ECHO_PIN, HIGH);

  // Calcular distancia en cm
  float distance = duration * 0.034 / 2;

  Serial.print("Distancia: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Enviar a TagoIO
  HTTPClient client;

  sprintf(postData,
    "{ \"variable\": \"Distance\", \"value\": %.2f, \"unit\": \"cm\" }",
    distance
  );

  client.begin(serverAddress);
  client.addHeader("Content-Type", contentHeader);
  client.addHeader("Device-Token", tokenHeader);

  int statusCode = client.POST(postData);

  Serial.print("Status: ");
  Serial.println(statusCode);

  client.end();

  delay(10000); // cada 10 segundos
}
