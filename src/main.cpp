#include "network.h"
#include "servo.h"

TaskHandle_t Task1;
TaskHandle_t Task2;

int incomingByte;

void udpTask(void *pvParameters) {
  Udp.begin(4210);
  MDNS.addService("http", "udp", 4210);
  while (true) {
    if (Udp.parsePacket()) {
      incomingByte = Udp.read();
    } 
    else {
      incomingByte = 0;
    }
    delay(100);
  }
}

void servoTask(void *pvParameters) {
  while (true) {
    int right_speed_level = getRightSpeedLevel(incomingByte);
    int left_speed_level = getLeftSpeedLevel(incomingByte);

    if (right_speed_level || left_speed_level) {
      moveServosSimplified(
        left_speed_level,
        right_speed_level
      );
    }
    delay(10);
  }
}

void setup() {
    Serial.begin(115200);
    wifiConnectionInit();
    mdnsInit();

    xTaskCreatePinnedToCore(
    udpTask,
    "udpTask",
    4096, // TODO: figure out what does it mean
    NULL,
    1,
    &Task1,
    0
  );

  servoInit();

  xTaskCreatePinnedToCore(
    servoTask,
    "servoTask",
    4096,
    NULL,
    1,
    &Task2,
    1
  );
}

void loop() {}
