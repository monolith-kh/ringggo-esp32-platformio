#include "led.h"

void LedInit()
{
    log_i("Init Led");

    xQueueLed = xQueueCreate(10, sizeof(uint8_t[7]));

    colorList[0] = COLOR_RINGGGO;
    colorList[1] = COLOR_WHITE;
    colorList[2] = COLOR_YELLOW;
    colorList[4] = COLOR_RED;
    colorList[4] = COLOR_DARKRED;
    colorList[5] = COLOR_LIME;
    colorList[6] = COLOR_GREEN;
    colorList[7] = COLOR_BLUE;
    colorList[8] = COLOR_AQUA;

    ledIndex = LED_ALL;
    type = LED_BLINK;
    ledTime = 500;
    color = COLOR_RINGGGO;

    FastLED.addLeds<P9813, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);  // BGR ordering is typical
}

void LedOn(uint8_t index, uint32_t color)
{
    if(index == LED_ALL)
    {
        leds[0] = color;
        leds[1] = color;
    }
    else
    {
        leds[index] = color;
    }
    FastLED.show();
}

void LedOn(uint8_t index, uint32_t color, uint32_t msec)
{
    for(int i=0; i<256; i++)
    {
        leds[0].r = (i/256.0) * RED(color);
        leds[0].g = (i/256.0) * GREEN(color);
        leds[0].b = (i/256.0) * BLUE(color);
        leds[1].r = (i/256.0) * RED(color);
        leds[1].g = (i/256.0) * GREEN(color);
        leds[1].b = (i/256.0) * BLUE(color);
        FastLED.show();
        vTaskDelay(5);
    }
}

void LedOff(uint8_t index)
{
    if(index == LED_ALL)
    {
        leds[0] = CRGB::Black;
        leds[1] = CRGB::Black;
    }
    else
    {
        leds[index] = CRGB::Black;
    }
    FastLED.show();
}

void Blink(uint8_t index, uint32_t color, uint16_t msec)
{
    LedOn(index, color);
    vTaskDelay(msec);
    LedOff(index);
    vTaskDelay(msec);
}

void LedTask(void* parameter)
{
    BaseType_t xStatus;

    uint8_t packetBody[7] = { 0, };

    LedInit();
  
    for (;;)
    {
        xStatus = xQueueReceive(xQueueLed, packetBody, 100);
        if(xStatus == pdPASS)
        {
            Serial.printf("led queue received: %d, %d, %d, %d, %d, %d, %d\n", packetBody[0], packetBody[1], packetBody[2], packetBody[3], packetBody[4], packetBody[5], packetBody[6]);
            ledIndex = packetBody[0];
            type = packetBody[1];
            ledTime = (packetBody[2] | packetBody[3] << 8);
            color = RGB(packetBody[4], packetBody[5], packetBody[6]);
        }
        
        if(type == LED_OFF) {
            LedOff(ledIndex);
        } else if(type == LED_ON) {
            if(ledTime >= 500)
            {
                LedOn(ledIndex, color, ledTime);
            }
            else
            {
                LedOn(ledIndex, color);
            }
        } else if (type == LED_BLINK) {
            Blink(ledIndex, color, ledTime);
            log_d("Blink leds");
        } else if (type == LED_BLINK_RANDOM) {
            Blink(ledIndex, colorList[rand()%sizeof(colorList)], ledTime);
        } else {
            log_d("invalid led type");
        }

    }
    vTaskDelete(NULL);
}
