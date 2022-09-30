#ifndef BATTERY_H
#define BATTERY_H

#include <Arduino.h>

#include "gameserver.h"

#define EXTERNAL_NOISE       0.00001          // 외부노이즈
#define MEASUREMENT_NOISE    0.05             // 측정노이즈

#define BATTERY_DELAY       60

extern float g_fBatteryVolt;
extern uint8_t g_dBatteryValue;

void BatteryInit();
void BatteryTask(void* parameter);

#endif
