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
    servoRight.write(SERVO_ROTATION_ANGLE);
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
    servoRight.write(SERVO_ROTATION_ANGLE - pos);
    delay(map(max(left_speed_level, right_speed_level), 0, 10, 55, 15));
  }
}

// new implementation
int lDirection = 1;
int rDirection = -1;

int l_i = 10;
int r_i = 10;

void rotationCycle(int left_speed_level, int right_speed_level) {
  if (left_speed_level) {
    int l_pos = (SERVO_ROTATION_ANGLE * (1-lDirection)/2) + l_i * lDirection;
    servoLeft.write(l_pos);
    if (l_i >= SERVO_ROTATION_ANGLE) {
      l_i = 0;
      lDirection *= -1;
    }
    l_i += 10;
  }
  if (right_speed_level) {
    int r_pos = (SERVO_ROTATION_ANGLE * (1-rDirection)/2) + r_i * rDirection;
    servoRight.write(r_pos);
    if (r_i >= SERVO_ROTATION_ANGLE) {
      r_i = 0;
      rDirection *= -1;
    }
    r_i += 10;
  }
  delay(map(max(left_speed_level, right_speed_level), 0, 10, 55, 15));
}
