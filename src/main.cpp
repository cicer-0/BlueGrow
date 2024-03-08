#include <Arduino.h>
#include <SoftwareSerial.h>
#include "IrrigationSystem.h"
#include "SensorFunctions.h"

SoftwareSerial bluetooth(6, 5); // RX, TX

IrrigationProgram irrigationProgram = {0, 0, 10, 50};

unsigned long lastIrrigation = 0L;

void setup()
{
  Serial.begin(9600);
  bluetooth.begin(9600);

  setupSensors();
}

void loop()
{
  int humidity = readHumiditySensor();
  int light1 = readLightSensor1();
  int light2 = readLightSensor2();

  sendBluetoothMessage("Humidity %: ", map(humidity, MAP_MAX, MAP_MIN, MAP_OUTPUT_MIN, MAP_OUTPUT_MAX));
  sendBluetoothMessage("Light 1: ", light1);
  sendBluetoothMessage("Light 2: ", light2);

  checkAndRunIrrigation(humidity);
}
