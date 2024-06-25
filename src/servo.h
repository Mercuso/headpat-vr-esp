#ifndef HEADPATVR_SERVO
#define HEADPATVR_SERVO

#include <ESP32Servo.h>

#define servo_left_pin 12
#define servo_right_pin 14

void servoInit();

int getLeftSpeedLevel(int incomingByte);

int getRightSpeedLevel (int incomingByte);

void moveServosSimplified (int left_speed_level, int right_speed_level);

#endif // HEADPATVR_SERVO
