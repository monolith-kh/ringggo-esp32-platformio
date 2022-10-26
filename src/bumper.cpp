#include "bumper.h"


void BumperTask(void* parameter)
{
    BaseType_t xStatus;

    uint8_t bumper;
    bool pressed;

    bumper = 0;
    pressed = false;

    pinMode(BMP_SEN_FRONT, INPUT);
    pinMode(BMP_SEN_LEFT, INPUT);
    pinMode(BMP_SEN_RIGHT, INPUT);
    pinMode(BMP_SEN_REAR, INPUT);

    for (;;)
    {

        if(digitalRead(BMP_SEN_FRONT) == LOW)
        {
            bumper |= FRONT;
            pressed = true;
        }
        if(digitalRead(BMP_SEN_RIGHT) == LOW)
        {
            bumper |= RIGHT;
            pressed = true;
        }
        if(digitalRead(BMP_SEN_LEFT) == LOW)
        {
            bumper |= LEFT;
            pressed = true;
        }
        if(digitalRead(BMP_SEN_REAR) == LOW)
        {
            bumper |= REAR;
            pressed = true;
        }

        if(bumper && pressed)
        {
            SendBumper(bumper);
            bumper = 0;
            pressed = false;
        }
        
        vTaskDelay(10);
    }

    vTaskDelete(NULL);
}
