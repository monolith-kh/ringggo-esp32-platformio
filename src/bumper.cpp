#include "bumper.h"

void BumperTask(void* parameter)
{
    pinMode(BMP_SEN_FRONT, INPUT);
    pinMode(BMP_SEN_LEFT, INPUT);
    pinMode(BMP_SEN_RIGHT, INPUT);
    pinMode(BMP_SEN_REAR, INPUT);
    
    uint8_t bumperFront = HIGH;
    uint8_t bumperLeft = HIGH;
    uint8_t bumperRight = HIGH;
    uint8_t bumperRear = HIGH;

    uint8_t prevBumperFront = HIGH;
    uint8_t prevBumperLeft = HIGH;
    uint8_t prevBumperRight = HIGH;
    uint8_t prevBumperRear = HIGH;

    uint8_t bumper = 0x00;

    for (;;)
    {
        bumperFront = digitalRead(BMP_SEN_FRONT);
        if(bumperFront == LOW) {
            if(prevBumperFront == HIGH) {
                SendBumper(FRONT);
            }
            prevBumperFront = bumperFront;
        } else {
            prevBumperFront = bumperFront;
        }

        bumperLeft = digitalRead(BMP_SEN_LEFT);
        if(bumperLeft == LOW) {
            if(prevBumperLeft == HIGH) {
                SendBumper(LEFT);
            }
            prevBumperLeft = bumperLeft;
        } else {
            prevBumperLeft = bumperLeft;
        }

        bumperRight = digitalRead(BMP_SEN_RIGHT);
        if(bumperRight == LOW) {
            if(prevBumperRight == HIGH) {
                SendBumper(RIGHT);
            }
            prevBumperRight = bumperRight;
        } else {
            prevBumperRight = bumperRight;
        }

        bumperRear = digitalRead(BMP_SEN_REAR);
        if(bumperRear == LOW) {
            if(prevBumperRear == HIGH) {
                SendBumper(REAR);
            }
            prevBumperRear = bumperRear;
        } else {
            prevBumperRear = bumperRear;
        }

        vTaskDelay(10);
    }

    vTaskDelete(NULL);
}
