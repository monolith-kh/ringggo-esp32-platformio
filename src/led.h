#ifndef LED_H
#define LED_H

#include <FastLED.h>

#define NUM_LEDS 2

#define DATA_PIN 4
#define CLOCK_PIN 15

#define LED_0               0
#define LED_1               1
#define LED_ALL             255

#define LED_OFF             0
#define LED_ON              1
#define LED_BLINK           2
#define LED_BLINK_RANDOM    255


#define COLOR_RINGGGO   0x00AC30
#define COLOR_WHITE     0x808080
#define COLOR_YELLOW    0xFFFF00
#define COLOR_RED       0xFF0000
#define COLOR_DARKRED   0x8B0000
#define COLOR_LIME      0x00FF00
#define COLOR_GREEN     0x008000
#define COLOR_BLUE      0x0000FF
#define COLOR_AQUA      0x00FFFF


#define RGB(r,g,b)      (r<<16|g<<8|b)

#define RED(color)      (color>>16)
#define GREEN(color)    (color>>8 & 0x000000ff)
#define BLUE(color)     (color & 0x000000ff)


extern CRGB leds[NUM_LEDS];

extern uint32_t colorList[9];

extern xQueueHandle xQueueLed;

void LedInit();
void LedOn(uint8_t index, uint32_t color);
void LedOn(uint8_t index, uint32_t color, uint32_t msec);
void LedOff(uint8_t index);
void Blink(uint8_t index, uint32_t color, uint16_t msec);
void LedTask(void* parameter);

#endif
