#ifndef IRRIGATION_SYSTEM_H
#define IRRIGATION_SYSTEM_H
#define MILLISECONDS_IN_A_SECOND 1000
#define MAP_MIN 0
#define MAP_MAX 1023
#define MAP_OUTPUT_MIN 0
#define MAP_OUTPUT_MAX 100
struct IrrigationProgram
{
  unsigned long int frequency;
  unsigned int duration;
  int minHumidity;
  int maxHumidity;
};

extern IrrigationProgram irrigationProgram;
extern unsigned long lastIrrigation;
extern SoftwareSerial bluetooth;

void checkAndRunIrrigation(int humidity);
bool isLowHumidity(int humidity);
void sendBluetoothMessage(const char *label, int value);
bool isHumidityInAllowedRange(int humidity);
void activateIrrigationSystem(int duration);
void simulateIrrigationSystem(int duration);
void activateLowHumidityIrrigation(int duration, unsigned long currentTime);
bool isTimeToIrrigate(unsigned long currentTime);
void updateLastIrrigationTime(unsigned long newTime);

#endif
