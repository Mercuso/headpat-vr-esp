#include "servo.h"

int pos = 0;
int direction = 1;

Servo servoLeft;
Servo servoRight;

void servoInit(){
  #if defined(SERVO_ROTATION_ANGLE)
    servoLeft.attach(servo_left_pin);
    servoRight.attach(servo_right_pin);
    servoLeft.write(0);
    servoRight.write(0);
    delay(500);
  #else
    #error "Missing define SERVO_ROTATION_ANGLE"
  #endif
}

int getLeftSpeedLevel(int byte) {
  return byte >> 4;
}

int getRightSpeedLevel (int byte) {
  return byte & 0x0F;
}

void moveServosSimplified (int left_speed_level, int right_speed_level) {
  // move servos with the same spped
  // speed - max speed from l and r speeds
  for (int i = 10; i<= SERVO_ROTATION_ANGLE; i += 10) {
    pos = (SERVO_ROTATION_ANGLE * (1-direction)/2) + i * direction;
    servoLeft.write(pos);
    servoRight.write(pos);
    delay(map(max(left_speed_level, right_speed_level), 0, 10, 55, 15));
  }
  direction *= (-1);
}
