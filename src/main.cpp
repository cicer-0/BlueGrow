#include <Arduino.h>
#include <SoftwareSerial.h>

#define SENSOR_PIN A0   // Pin donde está conectado el sensor de humedad
#define SENSOR_LUZ1_PIN A1   // Pin donde está conectado el sensor de humedad
#define SENSOR_LUZ2_PIN A5   // Pin donde está conectado el sensor de humedad
#define MOTOR_PIN 4      // Pin donde está conectado el motor

SoftwareSerial bluetooth(6, 5); // RX, TX

void setup()
{
  Serial.begin(9600);    // Iniciar comunicación serial para debug
  bluetooth.begin(9600); // Iniciar comunicación serial para Bluetooth

  pinMode(SENSOR_PIN, INPUT); // Configurar el pin del sensor como entrada
  pinMode(SENSOR_LUZ1_PIN, INPUT); // Configurar el pin del sensor como entrada
  pinMode(SENSOR_LUZ2_PIN, INPUT); // Configurar el pin del sensor como entrada
  pinMode(MOTOR_PIN, OUTPUT); // Configurar el pin del motor como salida

  Serial.println("Listo para enviar datos por Bluetooth...");
}

void loop()
{
  // Leer valor del sensor de humedad
  int humedad = analogRead(SENSOR_PIN);
  int humedad = analogRead(SENSOR_LUZ1_PIN);
  int humedad = analogRead(SENSOR_LUZ2_PIN);

  // Enviar valor de humedad por Bluetooth
  bluetooth.print("Humedad: ");
  bluetooth.println(humedad);

  // Verificar si la humedad es baja para encender el motor
  // if (humedad < 500) {
  //   digitalWrite(MOTOR_PIN, HIGH); // Encender el motor
  // } else {
  //   digitalWrite(MOTOR_PIN, LOW);  // Apagar el motor
  // }

  delay(1000); // Esperar 1 segundo antes de enviar otra vez
}
