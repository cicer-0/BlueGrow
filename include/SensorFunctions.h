#ifndef SENSOR_FUNCTIONS_H
#define SENSOR_FUNCTIONS_H

#define HUMIDITY_SENSOR_PIN A0
#define LIGHT_SENSOR_PIN_1 A1
#define LIGHT_SENSOR_PIN_2 A5
#define MOTOR_PIN 4

void setupSensors();
int readHumiditySensor();
int readLightSensor1();
int readLightSensor2();

#endif
