#include "network.h"
#include "servo.h"

TaskHandle_t Task1;
TaskHandle_t Task2;

int incomingByte;
uint8_t local_pong[8] = "o";
void udpTask(void *pvParameters) {
  Udp.begin(4210);
  MDNS.addService("http", "udp", 4210);
  while (true) {
    if (Udp.parsePacket()) {
      incomingByte = Udp.read();
      if (incomingByte == 0) {
        Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
        Udp.write(local_pong, 1);
        Udp.endPacket();
      }
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
      for (int i = 0; i < 12; i++) {
        int new_right_speed_level = getRightSpeedLevel(incomingByte);
        if (new_right_speed_level != right_speed_level && new_right_speed_level != 0) {
          right_speed_level = new_right_speed_level;
        }
        int new_left_speed_level = getLeftSpeedLevel(incomingByte);
        if (new_left_speed_level != left_speed_level && new_left_speed_level != 0) {
          left_speed_level = new_left_speed_level;
        }
        rotationCycle(
          left_speed_level,
          right_speed_level
        );
      }
    }
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
