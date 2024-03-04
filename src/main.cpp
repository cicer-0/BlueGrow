#include <SoftwareSerial.h>
#include <Arduino.h>

// Definición de pines
#define sensorHumedadPin A0
#define sensorLuzPin A1
#define motorRiegoPin 9

// Definición de umbrales y valores
int umbralHumedad = 500; // Ajusta según tus necesidades
int umbralLuz = 500;     // Ajusta según tus necesidades

// Configuración del módulo Bluetooth
SoftwareSerial bluetoothSerial(2, 3); // RX, TX

void setup()
{
  // Inicialización de pines
  pinMode(sensorHumedadPin, INPUT);
  pinMode(sensorLuzPin, INPUT);
  pinMode(motorRiegoPin, OUTPUT);

  // Inicialización de comunicación serial para módulo Bluetooth
  Serial.begin(9600);
  bluetoothSerial.begin(9600);
}

void loop()
{
  // Lectura de sensores
  int humedad = analogRead(sensorHumedadPin);
  int luz = analogRead(sensorLuzPin);

  // Envío de datos al módulo Bluetooth
  enviarBluetooth(humedad, luz);

  // Control de riego
  if (humedad < umbralHumedad && luz > umbralLuz)
  {
    activarRiego();
  }
  else
  {
    desactivarRiego();
  }

  delay(1000); // Espera 1 segundo entre cada ciclo
}

void enviarBluetooth(int humedad, int luz)
{
  bluetoothSerial.print("H:");
  bluetoothSerial.print(humedad);
  bluetoothSerial.print(", L:");
  bluetoothSerial.println(luz);
}

void activarRiego()
{
  digitalWrite(motorRiegoPin, HIGH);
  Serial.println("Riego activado");
}

void desactivarRiego()
{
  digitalWrite(motorRiegoPin, LOW);
  Serial.println("Riego desactivado");
}
