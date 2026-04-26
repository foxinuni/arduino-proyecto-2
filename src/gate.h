#pragma once

#define SERVO_MIN 3
#define SERVO_MAX 80
#define SERVO_DELAY 10 * 1000 // 10 seconds

void setup_gate();
void open_gate();
void update_gate();