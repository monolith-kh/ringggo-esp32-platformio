#include "gameserver.h"

void sleep(int millis)
{
  const static int tick = 100;
  int loop_count = millis / tick;
  while (loop_count)
  {
    vTaskDelay(tick);
    loop_count--;
  }
}


void GameServerInit()
{
  const static int try_amount = 100;

  log_i("Init GameServer");
  healthcheckTime = millis();

  float delay = 1.0;
  int try_count = try_amount;

  while (!gameClient.connect(GAME_HOST, GAME_PORT))
  {
    if (try_count)
    {
      log_i("connection failed %d of %d", try_count, try_amount);
      log_i("try reconnect after %d seconds", int(delay));
      sleep(int(delay));

      try_count--;
      delay *= 1.1;
    } else {
      log_i("tried over 100 times");
      log_i("reset...");
      ESP.restart();
      return;
    }
  }

  log_i("connected GameServer");
  gameClient.setNoDelay(true);


  log_i("try to handshake");
  uint8_t packetHeader[8] = { 0, };
  uint8_t packetBody[8] = { 0, };

  gameClient.readBytes(packetHeader, 8);
  log_i("%x %x %x %x %x %x %x %x ", packetHeader[0], packetHeader[1], packetHeader[2], packetHeader[3], packetHeader[4], packetHeader[5], packetHeader[6], packetHeader[7]);

  if (packetHeader[0] == PK_WHO_ARE_YOU_ANS) {
    log_i("whoyouare received");
    Protocol_t protocol = {PK_IAM_ANS, CAR, 8, carNumber, 0, };
    gameClient.write((const uint8_t *)&protocol, sizeof(protocol));
    log_i("send answer");
    healthcheckTime = millis();
  } else {
    log_i("wrong handshake process");
    log_i("retry GameServerInit");
    gameClient.stop();
    GameServerInit();
  }
}

void GameServerTask(void* parameter)
{
  BaseType_t xStatus;

  uint8_t packetHeader[8] = { 0, };
  uint8_t packetBody[8] = { 0, };
  
  for (;;)
  {
    GameServerInit();

    for (;;)
    {
      if ((millis() - healthcheckTime) > RECONNECT_DELAY)
      {
        log_i("health check failed");
        log_i("%d %d", millis(), healthcheckTime);
        gameClient.stop();
        break;
      }

      while (gameClient.available())
      {
        gameClient.readBytes(packetHeader, 8);
        log_i("%x %x %x %x %x %x %x %x ", packetHeader[0], packetHeader[1], packetHeader[2], packetHeader[3], packetHeader[4], packetHeader[5], packetHeader[6], packetHeader[7]);
        healthcheckTime = millis();
        
        if (packetHeader[0] == PK_CHECK_CONNECTION_REQ) {
          log_i("check received");
          Protocol_t protocol = {PK_CHECK_CONNECTION_ANS, CAR, 8, carNumber, 0, };
          gameClient.write((const uint8_t *)&protocol, sizeof(protocol));
          log_i("send answer");
        } else if (packetHeader[0] == PK_CARLED_NOTI) {
          gameClient.readBytes(packetBody, 7);
          log_i("car led received: %d, %d, %d, %d, %d, %d, %d", packetBody[0], packetBody[1], packetBody[2], packetBody[3], packetBody[4], packetBody[5], packetBody[6]);
          xStatus = xQueueSendToFront(xQueueLed, packetBody, 100);
          if(xStatus == pdPASS)
          {
              log_i("led queue send");
          }
        } else if (packetHeader[0] == PK_CARSOUND_NOTI) {
          gameClient.readBytes(packetBody, 2);
          log_i("car sound received: %d, %d", packetBody[0], packetBody[1]);
          xStatus = xQueueSendToFront(xQueueMp3, packetBody, 100);
          if(xStatus == pdPASS)
          {
              log_i("mp3 queue send");
          }
        } else if (packetHeader[0] == PK_CARACTIVEMODE_NOTI) {
          gameClient.readBytes(packetBody, 1);
          log_i("car start/stop received: %d\n", packetBody[0]);
          SetEventMode(packetBody[0]);
          // xStatus = xQueueSendToFront(xQueueSubBoard, packetBody, 100);
          // if(xStatus == pdPASS)
          // {
          //     log_i("subboard queue send");
          // }
        } else {
          log_i("invalid req code");
        }
      }
      vTaskDelay(100);
    }
  }
  vTaskDelete(NULL);
}

void SendBattery(uint8_t percentage)
{
    Protocol_bt protocol = {PK_BATTERY_NOTI, CAR, 8, carNumber, 0, percentage};
    gameClient.write((const uint8_t *)&protocol, sizeof(protocol));
    log_i("send battery: %d%", percentage);
}

void SendNfc(char wrbId[], uint8_t uid[])
{
    Protocol_nfct protocol = {PK_NFC_NOTI, CAR, 8, carNumber, 0,};
    memcpy(protocol.wrbId, wrbId, 13);
    protocol.sep = ',';
    memcpy(protocol.uid, uid, 7);
    gameClient.write((const uint8_t *)&protocol, sizeof(protocol));
    log_i("send nfc: %s", protocol.wrbId);
}

void SendBumper(int bumper)
{
    Protocol_bumpert protocol = {PK_BUMP_NOTI, CAR, 8, carNumber, 0, (uint8_t)bumper};
    gameClient.write((const uint8_t *)&protocol, sizeof(protocol));
    log_i("send bumper: 0x%x", bumper);
}

