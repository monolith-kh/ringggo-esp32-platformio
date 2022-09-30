#ifndef MP3_H
#define MP3_H

#include <DFRobotDFPlayerMini.h>

#define STOP      0
#define PLAY      1
#define EFFECT    2
#define VOLUME    3

#define MAX_VOLUME  20

extern xQueueHandle xQueueMp3;
extern DFRobotDFPlayerMini mp3Player;
extern int volume;

void Mp3Init();
void Mp3Play(int number);
void Mp3Stop();
void Mp3Effect(int number);
void Mp3Pause();
void Mp3Loop(int number);
void Mp3Volume(int number);
void Mp3Task(void* parameter);

#endif
