#include <Arduino.h>
#include <SoftwareSerial.h>

// Definir constantes para los pines de los sensores y del motor
#define SENSOR_HUMEDAD_PIN A0
#define SENSOR_LUZ_PIN_1 A1
#define SENSOR_LUZ_PIN_2 A5
#define MOTOR_PIN 4

SoftwareSerial bluetooth(6, 5); // RX, TX

void setup()
{
  Serial.begin(9600);    // Iniciar comunicación serial para debug
  bluetooth.begin(9600); // Iniciar comunicación serial para Bluetooth

  // Configurar pines de los sensores como entradas y el pin del motor como salida
  pinMode(SENSOR_HUMEDAD_PIN, INPUT);
  pinMode(SENSOR_LUZ_PIN_1, INPUT);
  pinMode(SENSOR_LUZ_PIN_2, INPUT);
  pinMode(MOTOR_PIN, OUTPUT);
  Serial.println("Listo para enviar datos por Bluetooth...");
}

// Función para leer el valor de un sensor dado su pin
int leerSensor(int pin)
{
  return analogRead(pin);
}

// Función para enviar un mensaje por Bluetooth
void enviarMensajeBluetooth(const char* mensaje, int valor)
{
  bluetooth.print(mensaje);
  bluetooth.println(valor);
}

void loop()
{
  // Leer valores de los sensores
  int humedad = leerSensor(SENSOR_HUMEDAD_PIN);
  int luz1 = leerSensor(SENSOR_LUZ_PIN_1);
  int luz2 = leerSensor(SENSOR_LUZ_PIN_2);

  // Enviar valores por Bluetooth
  enviarMensajeBluetooth("Humedad: ", humedad);
  enviarMensajeBluetooth("Luz 1: ", luz1);
  enviarMensajeBluetooth("Luz 2: ", luz2);

  delay(1000); // Esperar 1 segundo antes de realizar la próxima lectura y envío
}
