#include "bumper.h"


void BumperTask(void* parameter)
{
    BaseType_t xStatus;

    uint8_t bumper;
    bool pressed;
    bool prevPressed;

    bumper = 0;
    pressed = false;
    prevPressed = false;

    pinMode(BMP_SEN_FRONT, INPUT);
    pinMode(BMP_SEN_LEFT, INPUT);
    pinMode(BMP_SEN_RIGHT, INPUT);
    pinMode(BMP_SEN_REAR, INPUT);

    for (;;)
    {
        if(digitalRead(BMP_SEN_FRONT) == LOW && (prevPressed == false))
        {
            bumper |= FRONT;
            pressed = true;
        }
        if(digitalRead(BMP_SEN_RIGHT) == LOW && (prevPressed == false))
        {
            bumper |= RIGHT;
            pressed = true;
        }
        if(digitalRead(BMP_SEN_LEFT) == LOW && (prevPressed == false))
        {
            bumper |= LEFT;
            pressed = true;
        }
        if(digitalRead(BMP_SEN_REAR) == LOW && (prevPressed == false))
        {
            bumper |= REAR;
            pressed = true;
        }

        if(bumper && pressed)
        {
            SendBumper(bumper);
            bumper = 0;
            pressed = false;
            prevPressed = false;
        }
        prevPressed = pressed;
        
        vTaskDelay(10);
    }

    vTaskDelete(NULL);
}
