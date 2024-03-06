#ifndef IRRIGATION_SYSTEM_H
#define IRRIGATION_SYSTEM_H

struct IrrigationProgram {
  int frequency;
  int duration;
  int minHumidity;
};

extern IrrigationProgram irrigationProgram;
extern unsigned long lastIrrigation;

void checkAndRunIrrigation();
void runIrrigation();
void sendLowHumidityNotification(int humidity);
void sendBluetoothMessage(const char* label, int value);

#endif
