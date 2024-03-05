#include <Arduino.h>
#include <SoftwareSerial.h>

#define MOTOR_PIN 4  // Pin donde está conectado el motor

SoftwareSerial bluetooth(6, 5); // RX, TX

void setup()
{
  Serial.begin(9600);    // Iniciar comunicación serial para debug
  bluetooth.begin(9600); // Iniciar comunicación serial para Bluetooth

  pinMode(MOTOR_PIN, OUTPUT); // Configurar el pin del motor como salida

  Serial.println("Listo para recibir y enviar datos por Bluetooth...");
}

void loop()
{
  // Leer datos desde el dispositivo Bluetooth
  if (bluetooth.available())
  {
    char receivedChar = (char)bluetooth.read();
    Serial.print("Caracter recibido desde Bluetooth: ");
    Serial.println(receivedChar);

    // Verificar si se recibió "up" para encender el motor
    if (receivedChar == 'u')
    {
      if (bluetooth.available() && bluetooth.read() == 'p')
      {
        digitalWrite(MOTOR_PIN, HIGH); // Encender el motor
        Serial.println("Motor encendido!");
        bluetooth.println("Motor Encendido"); // Enviar mensaje por Bluetooth
      }
    }

    // Verificar si se recibió "down" para apagar el motor
    else if (receivedChar == 'd')
    {
      if (bluetooth.available() && bluetooth.read() == 'o' && bluetooth.available() && bluetooth.read() == 'w' && bluetooth.available() && bluetooth.read() == 'n')
      {
        digitalWrite(MOTOR_PIN, LOW); // Apagar el motor
        Serial.println("Motor apagado!");
        bluetooth.println("Motor Apagado"); // Enviar mensaje por Bluetooth
      }
    }
  }

  // Enviar datos al dispositivo Bluetooth
  if (Serial.available())
  {
    char sendChar = (char)Serial.read();
    Serial.print("Enviando caracter a Bluetooth: ");
    Serial.println(sendChar);
    bluetooth.write(sendChar);
  }
}
