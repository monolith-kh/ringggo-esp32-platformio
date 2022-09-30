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

    xQueueBumper = xQueueCreate(10, sizeof(uint8_t));

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
            // xStatus = xQueueSendToFront(xQueueBumper, &bumper, 100);
            // if(xStatus == pdPASS)
            // {
            //     log_i("send game server queue: %x", bumper);
            // }
            SendBumper(bumper);
            bumper = 0;
            pressed = false;
        }
        
        vTaskDelay(200);
    }

    vTaskDelete(NULL);
}
