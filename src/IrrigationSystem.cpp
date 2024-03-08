#include <Arduino.h>
#include <SoftwareSerial.h>
#include "IrrigationSystem.h"
#include "SensorFunctions.h"
#include <ArduinoJson.h>

void checkAndRunIrrigation(int humidity)
{
    unsigned long currentTime = millis();
    currentTime = adjustTimeIfNecessary(currentTime);

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

    if (remainingTimeForNextIrrigation > static_cast<unsigned long>(irrigationProgram.duration * MILLISECONDS_IN_A_SECOND))
    {
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

void updateLastIrrigationTime(unsigned long currentTime)
{
    lastIrrigation = currentTime;
}

unsigned long adjustTimeIfNecessary(unsigned long currentTime)
{
    if (currentTime < lastIrrigation)
    {
        lastIrrigation = 0L;
        currentTime = currentTime + (4294967295 - lastIrrigation);
    }
    return currentTime;
}

void sendBluetoothMessageConfig(const char *message)
{
    // Implementación de seguridad para enviar mensajes Bluetooth
    bluetooth.print("CONFIG: ");
    bluetooth.println(message);
}

void receiveBluetoothMessage()
{
    while (bluetooth.available() > 0)
    {
        Serial.print(bluetooth.read());
        Serial.println();
    }
    if (bluetooth.available())
    {
        Serial.println("bluetooth.available");

        String receivedMessage = bluetooth.readStringUntil('\n');
        receivedMessage.trim();
        Serial.println(receivedMessage);
        // Verifica si el mensaje recibido comienza con la palabra clave para seguridad
        if (receivedMessage.startsWith("CONFIG: "))
        {
            Serial.println("JSON");
            receivedMessage.remove(0, 8); // Remueve "SECURE: " del mensaje

            // Procesa el mensaje seguro
            processConfigMessage(receivedMessage);
        }
    }
}

void processConfigMessage(String message)
{
    // Mensaje seguro recibido, procesa y actualiza la configuración de irrigationProgram

    // Parsea el mensaje JSON
    JsonDocument doc; // Documento JSON
    DeserializationError error = deserializeJson(doc, message);

    if (error)
    {
        sendBluetoothMessageConfig("INVALID JSON");
        Serial.println("INVALID JSON");
        return;
    }

    JsonObject config = doc.as<JsonObject>();

    // Verifica y actualiza cada parámetro de configuración
    if (config.containsKey("frequency") && config.containsKey("maxHumidity") && config.containsKey("minHumidity") && config.containsKey("duration"))
    {
        irrigationProgram.frequency = config["frequency"];
        irrigationProgram.maxHumidity = config["maxHumidity"];
        irrigationProgram.minHumidity = config["minHumidity"];
        irrigationProgram.duration = config["duration"];
        Serial.println(irrigationProgram.to_string());
        // Envía confirmación de actualización
        sendBluetoothMessageConfig("CONFIG UPDATED");
    }
    else
    {
        sendBluetoothMessageConfig("MISSING PARAMETERS");
    }
}

String IrrigationProgram::to_string() const
{
    String result = "";

    result += "Frequency: ";
    result += frequency;
    result += "\n";

    result += "Duration: ";
    result += duration;
    result += "\n";

    result += "Minimum humidity: ";
    result += minHumidity;
    result += "\n";

    result += "Maximum humidity: ";
    result += maxHumidity;
    result += "\n";

    return result;
}
