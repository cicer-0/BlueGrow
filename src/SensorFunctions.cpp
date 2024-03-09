#include <Arduino.h>
#include "SensorFunctions.h"

void setupSensors()
{
  pinMode(HUMIDITY_SENSOR_PIN, INPUT);
  pinMode(LIGHT_SENSOR_PIN_1, INPUT);
  pinMode(LIGHT_SENSOR_PIN_2, INPUT);
  pinMode(MOTOR_PIN, OUTPUT);
}

int readHumiditySensor()
{
  return analogRead(HUMIDITY_SENSOR_PIN);
}

int readLightSensor1()
{
  return analogRead(LIGHT_SENSOR_PIN_1);
}

int readLightSensor2()
{
  return analogRead(LIGHT_SENSOR_PIN_2);
}
