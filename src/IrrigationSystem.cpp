#include <Arduino.h>
#include <SoftwareSerial.h>
#include "IrrigationSystem.h"
#include "SensorFunctions.h"

// Declaración externa de la variable bluetooth
extern SoftwareSerial bluetooth;

void checkAndRunIrrigation()
{
    unsigned long now = millis();
    unsigned long interval = 0;

    switch (irrigationProgram.frequency)
    {
    case 0: // Cada minuto
        interval = 60000;
        break;
    case 1: // Cada hora
        interval = 3600000;
        break;
    case 2: // Cada día
        interval = 86400000;
        break;
    case 4: // Cada día
        interval = 10000;
        break;
    default:
        break;
    }

    if (now - lastIrrigation >= interval)
    {
        runIrrigation();
        lastIrrigation = now;
    }
}

void runIrrigation()
{
    digitalWrite(MOTOR_PIN, HIGH);
    delay(irrigationProgram.duration * 1000); // Convertir a milisegundos
    digitalWrite(MOTOR_PIN, LOW);
}

void sendLowHumidityNotification(int humidity)
{
    if (humidity < irrigationProgram.minHumidity)
    {
        bluetooth.println("ALERT: Low humidity detected!");
    }
}

void sendBluetoothMessage(const char *label, int value)
{
    bluetooth.print(label);
    bluetooth.println(value);
}
