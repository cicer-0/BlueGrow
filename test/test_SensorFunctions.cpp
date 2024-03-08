#include <Arduino.h>
#include "unity.h"
#include "SensorFunctions.h"

// Definir pines de prueba
#define TEST_HUMIDITY_SENSOR_PIN A0
#define TEST_LIGHT_SENSOR_PIN_1 A1
#define TEST_LIGHT_SENSOR_PIN_2 A2
#define TEST_MOTOR_PIN 5

// Función de configuración de Unity
void setUp(void)
{
  // Inicializar los pines de prueba como INPUT
  pinMode(TEST_HUMIDITY_SENSOR_PIN, INPUT);
  pinMode(TEST_LIGHT_SENSOR_PIN_1, INPUT);
  pinMode(TEST_LIGHT_SENSOR_PIN_2, INPUT);
  pinMode(TEST_MOTOR_PIN, OUTPUT);
}

// Función para limpiar después de cada prueba (opcional)
void tearDown(void)
{
  // Limpiar después de cada prueba si es necesario
}

void setPinMode(int pin, int mode)
{
  ::pinMode(pin, mode);
}

void setPinInput(int pin)
{
  ::pinMode(pin, INPUT);
}

void setPinOutput(int pin)
{
  ::pinMode(pin, OUTPUT);
}

void test_setupSensors_humiditySensorPinInput(void)
{
  setPinInput(TEST_HUMIDITY_SENSOR_PIN);
  setupSensors();
  TEST_ASSERT_EQUAL(INPUT, digitalRead(TEST_HUMIDITY_SENSOR_PIN));
}

void test_setupSensors_lightSensorPin1Input(void)
{
  setPinInput(TEST_LIGHT_SENSOR_PIN_1);
  setupSensors();
  TEST_ASSERT_EQUAL(INPUT, digitalRead(TEST_LIGHT_SENSOR_PIN_1));
}

void test_setupSensors_lightSensorPin2Input(void)
{
  setPinInput(TEST_LIGHT_SENSOR_PIN_2);
  setupSensors();
  TEST_ASSERT_EQUAL(INPUT, digitalRead(TEST_LIGHT_SENSOR_PIN_2));
}

void test_setupSensors_motorPinOutput(void)
{
  setPinOutput(TEST_MOTOR_PIN);
  setupSensors();
  TEST_ASSERT_EQUAL(OUTPUT, digitalRead(TEST_MOTOR_PIN));
}

// Prueba para leer el sensor de humedad
void test_readHumiditySensor(void)
{
  // Simular un valor de lectura de humedad
  analogWrite(TEST_HUMIDITY_SENSOR_PIN, 500);

  int humidityValue = readHumiditySensor();
  TEST_ASSERT_EQUAL_INT(500, humidityValue);
}

// Prueba para leer el primer sensor de luz
void test_readLightSensor1(void)
{
  // Simular un valor de lectura de luz 1
  analogWrite(TEST_LIGHT_SENSOR_PIN_1, 700);

  int lightValue = readLightSensor1();
  TEST_ASSERT_EQUAL_INT(700, lightValue);
}

// Prueba para leer el segundo sensor de luz
void test_readLightSensor2(void)
{
  // Simular un valor de lectura de luz 2
  analogWrite(TEST_LIGHT_SENSOR_PIN_2, 300);

  int lightValue = readLightSensor2();
  TEST_ASSERT_EQUAL_INT(300, lightValue);
}

// Definir la función principal que ejecuta todas las pruebas
int main()
{
  // delay(2000); // Iniciar retardo para establecer conexión serial si es necesario
  UNITY_BEGIN();
  Serial.println("Iniciando pruebas...");
  // Ejecutar las pruebas una por una
  RUN_TEST(test_setupSensors_humiditySensorPinInput);
  RUN_TEST(test_setupSensors_lightSensorPin1Input);
  RUN_TEST(test_setupSensors_lightSensorPin2Input);
  RUN_TEST(test_setupSensors_motorPinOutput);
  RUN_TEST(test_readHumiditySensor);
  RUN_TEST(test_readLightSensor1);
  RUN_TEST(test_readLightSensor2);
  Serial.println("Pruebas finalizadas.");
  UNITY_END();
}

// No se necesita loop() ya que las pruebas se ejecutan una vez y luego terminan
