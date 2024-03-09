#include <Arduino.h>
#include <SoftwareSerial.h>
#include "IrrigationSystem.h"
#include "SensorFunctions.h"

SoftwareSerial bluetooth(6, 5); // RX, TX

IrrigationProgram irrigationProgram = {0, 0, 0, 100};

unsigned long lastIrrigation = 0L;

void setup()
{
  Serial.begin(9600);
  bluetooth.begin(9600);

  setupSensors();

  sendBluetoothMessageConfig("READY");
}

void loop()
{
  int humidity = readHumiditySensor();
  int light1 = readLightSensor1();
  int light2 = readLightSensor2();
  sendBluetoothMessage("HUMIDITY %: ", map(humidity, MAP_MAX, MAP_MIN, MAP_OUTPUT_MIN, MAP_OUTPUT_MAX));
  sendBluetoothMessage("LIGHT %: ", (map(light1, MAP_MIN, MAP_MAX, MAP_OUTPUT_MIN, MAP_OUTPUT_MAX) + map(light2, MAP_MIN, MAP_MAX, MAP_OUTPUT_MIN, MAP_OUTPUT_MAX)) / 2);

  // Revisa si hay mensajes nuevos por Bluetooth
  receiveBluetoothMessage();

  checkAndRunIrrigation(humidity);
}