#include "BluetoothSerial.h"

BluetoothSerial BT;

// LEDs
int led1 = 2;
int led2 = 4;
int led3 = 5;
int led4 = 18;

// Botones
int btn1 = 12;
int btn2 = 13;
int btn3 = 14;
int btn4 = 27;

// Estados LEDs
bool estado1 = false;
bool estado2 = false;
bool estado3 = false;
bool estado4 = false;

// Habilitación individual
bool hab1 = false;
bool hab2 = false;
bool hab3 = false;
bool hab4 = false;

// Lectura anterior (antirrebote)
bool last1 = HIGH;
bool last2 = HIGH;
bool last3 = HIGH;
bool last4 = HIGH;

void setup() {
  BT.begin("ESP32_Cajamarca"); // Nombre del Bluetooth

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);

  pinMode(btn1, INPUT_PULLUP);
  pinMode(btn2, INPUT_PULLUP);
  pinMode(btn3, INPUT_PULLUP);
  pinMode(btn4, INPUT_PULLUP);
}

void loop() {

  //  CONTROL DESDE APP (Bluetooth)
  if (BT.available()) {
    char dato = BT.read();

    // BTN1
    if (dato == 'A') hab1 = true;
    if (dato == 'a') {
      hab1 = false;
      estado1 = false;
      last1 = HIGH; // 
    }

    // BTN2
    if (dato == 'B') hab2 = true;
    if (dato == 'b') {
      hab2 = false;
      estado2 = false;
      last2 = HIGH;
    }

    // BTN3
    if (dato == 'C') hab3 = true;
    if (dato == 'c') {
      hab3 = false;
      estado3 = false;
      last3 = HIGH;
    }

    // BTN4
    if (dato == 'D') hab4 = true;
    if (dato == 'd') {
      hab4 = false;
      estado4 = false;
      last4 = HIGH;
    }
  }

  //  BOTONES (cada uno independiente)

  bool b1 = digitalRead(btn1);
  if (hab1 && last1 == HIGH && b1 == LOW) {
    estado1 = !estado1;
  }
  last1 = b1;

  bool b2 = digitalRead(btn2);
  if (hab2 && last2 == HIGH && b2 == LOW) {
    estado2 = !estado2;
  }
  last2 = b2;

  bool b3 = digitalRead(btn3);
  if (hab3 && last3 == HIGH && b3 == LOW) {
    estado3 = !estado3;
  }
  last3 = b3;

  bool b4 = digitalRead(btn4);
  if (hab4 && last4 == HIGH && b4 == LOW) {
    estado4 = !estado4;
  }
  last4 = b4;

  // 🟢 SALIDA LEDs
  digitalWrite(led1, estado1);
  digitalWrite(led2, estado2);
  digitalWrite(led3, estado3);
  digitalWrite(led4, estado4);
}
