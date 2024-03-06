#ifndef IRRIGATION_SYSTEM_H
#define IRRIGATION_SYSTEM_H

struct IrrigationProgram {
  unsigned long frequency; /**< Frecuencia de riego en milisegundos: 0 (cada minuto), 1 (cada hora), 2 (cada día), 4 (cada 10 segundos - para pruebas). */
  int duration;            /**< Duración del riego en segundos. */
  int minHumidity;         /**< Humedad mínima requerida para activar el riego. */
  int maxHumidity;         /**< Humedad máxima permitida para activar el riego. */
};

extern IrrigationProgram irrigationProgram;
extern unsigned long lastIrrigation;

void checkAndRunIrrigation();
void sendLowHumidityAlert(int humidity);
void sendBluetoothMessage(const char* label, int value);
bool isHumidityAcceptable();
void activateIrrigation(int duration);
void simulateIrrigation(int duration);
bool isTimeForIrrigation(unsigned long currentTime);
void updateLastIrrigation(unsigned long newTime);

#endif
