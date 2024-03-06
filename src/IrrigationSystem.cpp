#include <Arduino.h>
#include <SoftwareSerial.h>
#include "IrrigationSystem.h"
#include "SensorFunctions.h"

void checkAndRunIrrigation()
{
    unsigned long currentTime = millis();

    if (isTimeForIrrigation(currentTime))
    {
        if (isHumidityAcceptable())
        {
            activateIrrigation(irrigationProgram.duration);
        }
        else
        {
            simulateIrrigation(irrigationProgram.duration);
        }
        updateLastIrrigation(millis());
    }
}

bool isHumidityAcceptable()
{
    int currentHumidity = readHumiditySensor();
    return currentHumidity <= irrigationProgram.maxHumidity;
}

void activateIrrigation(int duration)
{
    digitalWrite(MOTOR_PIN, HIGH);
    delay(duration * 1000);
    digitalWrite(MOTOR_PIN, LOW);
}

void simulateIrrigation(int duration)
{
    delay(duration * 1000);
}

void sendLowHumidityAlert(int humidity)
{
    if (humidity < irrigationProgram.minHumidity)
    {
        bluetooth.println("ALERT! Low humidity detected!");
        activateIrrigation(irrigationProgram.duration);
        // updateLastIrrigation(millis());
    }
}

void sendBluetoothMessage(const char *label, int value)
{
    bluetooth.print(label);
    bluetooth.println(value);
}

bool isTimeForIrrigation(unsigned long currentTime)
{
    return currentTime - lastIrrigation >= irrigationProgram.frequency;
}

void updateLastIrrigation(unsigned long newTime)
{
    lastIrrigation = newTime;
}
