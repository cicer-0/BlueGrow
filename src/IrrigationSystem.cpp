#include <Arduino.h>
#include <SoftwareSerial.h>
#include "IrrigationSystem.h"
#include "SensorFunctions.h"

void checkAndRunIrrigation(int humidity)
{
    unsigned long currentTime = millis();
    if (isTimeToIrrigate(currentTime))
    {
        if (isHumidityInAllowedRange(humidity))
        {
            activateIrrigationSystem(irrigationProgram.duration);
        }
        else
        {
            simulateIrrigationSystem(irrigationProgram.duration);
        }
        updateLastIrrigationTime(currentTime);
    }
    else
    {
        if (isLowHumidity(humidity))
        {
            bluetooth.println("ALERT! Low humidity detected!");
            activateLowHumidityIrrigation(irrigationProgram.duration, currentTime);
        }
    }
}

bool isHumidityInAllowedRange(int humidity)
{
    return map(humidity, MAP_MAX, MAP_MIN, MAP_OUTPUT_MIN, MAP_OUTPUT_MAX) < irrigationProgram.maxHumidity;
}

void activateIrrigationSystem(int duration)
{
    digitalWrite(MOTOR_PIN, HIGH);
    delay(duration * MILLISECONDS_IN_A_SECOND);
    digitalWrite(MOTOR_PIN, LOW);
}

void simulateIrrigationSystem(int duration)
{
    delay(duration * MILLISECONDS_IN_A_SECOND);
}

bool isLowHumidity(int humidity)
{
    return map(humidity, MAP_MAX, MAP_MIN, MAP_OUTPUT_MIN, MAP_OUTPUT_MAX) < irrigationProgram.minHumidity;
}

void activateLowHumidityIrrigation(int duration, unsigned long currentTime)
{
    unsigned long remainingTimeForNextIrrigation =
        (irrigationProgram.frequency * MILLISECONDS_IN_A_SECOND) - (currentTime - lastIrrigation);

    Serial.print("currentTime: ");
    Serial.println(currentTime);
    Serial.print("lastIrrigation: ");
    Serial.println(lastIrrigation);
    Serial.print("remainingTimeForNextIrrigation: ");
    Serial.println(remainingTimeForNextIrrigation);
    if (remainingTimeForNextIrrigation > static_cast<unsigned long>(irrigationProgram.duration * MILLISECONDS_IN_A_SECOND))
    {
        Serial.println("Activating irrigation system...");
        activateIrrigationSystem(duration);
    }
}

void sendBluetoothMessage(const char *label, int value)
{
    bluetooth.print(label);
    bluetooth.println(value);
}

bool isTimeToIrrigate(unsigned long currentTime)
{
    return currentTime - lastIrrigation >= static_cast<unsigned long>(irrigationProgram.frequency * MILLISECONDS_IN_A_SECOND);
}

void updateLastIrrigationTime(unsigned long newTime)
{
    lastIrrigation = newTime;
}
