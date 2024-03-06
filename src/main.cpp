#include <Arduino.h>
#include <SoftwareSerial.h>
#include "IrrigationSystem.h"
#include "SensorFunctions.h"

SoftwareSerial bluetooth(6, 5); // RX, TX

IrrigationProgram irrigationProgram = {4, 1, 300};

unsigned long lastIrrigation = 0;

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);

  setupSensors();

  Serial.println("Ready to send data via Bluetooth...");
}

void loop() {
  int humidity = readHumiditySensor();
  int light1 = readLightSensor1();
  int light2 = readLightSensor2();

  sendBluetoothMessage("Humidity: ", humidity);
  sendBluetoothMessage("Light 1: ", light1);
  sendBluetoothMessage("Light 2: ", light2);

  sendLowHumidityNotification(humidity);

  checkAndRunIrrigation();

  delay(1000);
}
