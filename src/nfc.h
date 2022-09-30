#ifndef NFC_H
#define NFC_H

#include <Adafruit_PN532.h>

#include "gameserver.h"

#define PN532_SS    (5)
#define PN532_MOSI  (23)
#define PN532_MISO  (19)
#define PN532_SCK   (18)

#define GET_NFC_DATA    1
#define MAX_PACKET      32

extern Adafruit_PN532 nfc;

void NfcInit();
void NfcTask(void* parameter);

#endif
