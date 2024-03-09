#include <Arduino.h>
#include <SoftwareSerial.h>
#include "IrrigationSystem.h"
#include "SensorFunctions.h"
void checkAndRunIrrigation(int humidity)
{
    unsigned long currentTime = millis();
    currentTime = adjustTimeIfNecessary(currentTime);

    if (isTimeToIrrigate(currentTime) && irrigationProgram.frequency > 1)
    {
        if (isHumidityInAllowedRange(humidity))
        {
            activateIrrigationSystem(irrigationProgram.duration);
        }
        else
        {
            simulateIrrigationSystem(irrigationProgram.duration);
        }
        updateLastIrrigationTime(millis());
    }
    else
    {
        if (isLowHumidity(humidity))
        {
            bluetooth.println("ALERT!: Low humidity detected!");
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
    sendBluetoothMessage("MOTOR ACTIVE: ", true);

    delay(duration * MILLISECONDS_IN_A_SECOND);

    digitalWrite(MOTOR_PIN, LOW);
    sendBluetoothMessage("MOTOR INACTIVE: ", false);
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

void sendBluetoothMessage(const char *label, const char *message)
{
    bluetooth.print(label);
    bluetooth.println(message);
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
    sendBluetoothMessage("CONFIG: ", message);
}

void receiveBluetoothMessage()
{
    while (bluetooth.available() > 0)
    {
        bluetooth.read();
    }
    String receivedMessage = bluetooth.readStringUntil('\n');
    // Verifica si el mensaje recibido comienza con la palabra clave para seguridad
    if (receivedMessage.startsWith("CONFIG: "))
    {
        processConfigMessage(receivedMessage);
    }
}

void processConfigMessage(String message)
{
    // "CONFIG: 15,2,10,80";
    message.replace("CONFIG: ", ""); // Valores separados por comas: frequency, duration, minHumidity, maxHumidity

    // Separar los valores del mensaje
    int values[4]; // Array para almacenar los valores
    int index = 0; // Índice para recorrer el array
    char *value = strtok((char *)message.c_str(), ",");
    while (value != NULL && index < 4)
    {
        // values[index++] = atoi(value); // Convertir cada valor a entero y almacenarlo en el array
        if (*value != '\0')
        {
            values[index++] = atoi(value);
        }
        else
        {
            sendBluetoothMessageConfig("INVALID MESSAGE FORMAT");
            return;
        }
        value = strtok(NULL, ",");
    }

    // Verificar si se obtuvieron los 4 valores
    if (index < 4)
    {
        sendBluetoothMessageConfig("INVALID MESSAGE FORMAT");
        return;
    }

    // Asignar los valores a las variables correspondientes
    int frequency = values[0];
    int duration = values[1];
    int minHumidity = values[2];
    int maxHumidity = values[3];

    // Verificar y actualizar cada parámetro de configuración
    if (frequency >= 0 && duration >= 0 && minHumidity >= 0 && maxHumidity >= 0)
    {
        irrigationProgram.frequency = frequency;
        irrigationProgram.maxHumidity = maxHumidity;
        irrigationProgram.minHumidity = minHumidity;
        irrigationProgram.duration = duration;
        Serial.println(irrigationProgram.to_string());
        // Envía confirmación de actualización
        sendBluetoothMessageConfig("CONFIG UPDATED");
    }
    else
    {
        irrigationProgram.frequency = 0;
        irrigationProgram.duration = 0;
        irrigationProgram.minHumidity = 0;
        irrigationProgram.maxHumidity = 100;
        sendBluetoothMessageConfig("INVALID PARAMETERS");
    }
    lastIrrigation = millis();
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