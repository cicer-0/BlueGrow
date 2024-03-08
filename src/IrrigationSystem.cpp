#include <Arduino.h>
#include <SoftwareSerial.h>
#include "IrrigationSystem.h"
#include "SensorFunctions.h"

void checkAndRunIrrigation(int humidity)
{
    unsigned long currentTime = millis();
    Serial.print("Current time: ");
    Serial.println(currentTime);

    if (isTimeToIrrigate(currentTime))
    {
        Serial.println("It's time to irrigate!");
        if (isHumidityInAllowedRange(humidity))
        {
            Serial.println("Humidity is in allowed range. Activating irrigation system...");
            activateIrrigationSystem(irrigationProgram.duration);
        }
        else
        {
            Serial.println("Humidity is not in allowed range. Simulating irrigation system...");
            simulateIrrigationSystem(irrigationProgram.duration);
        }
        updateLastIrrigationTime(currentTime);
    }
    else
    {
        Serial.println("It's not time to irrigate yet.");
        if (isLowHumidity(humidity))
        {
            Serial.println("Low humidity detected!");
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
