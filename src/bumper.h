#ifndef BUMPER_H
#define BUMPER_H

#include <Arduino.h>

#include <gameserver.h>

#define BMP_SEN_FRONT   36
#define BMP_SEN_LEFT    34
#define BMP_SEN_RIGHT   39
#define BMP_SEN_REAR    35

#define FRONT   0x01
#define LEFT    0x02
#define RIGHT   0x04
#define REAR    0x08

void BumperTask(void* parameter);

#endif
