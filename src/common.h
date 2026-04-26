#pragma once

#include <Arduino.h>

#define ENTRY_BUTTON    2
#define LEAVE_BUTTON    3
#define BUZZER_PIN      4
#define SERVO_PIN       5
#define MOTOR_PIN       A5
#define LIGHT_PIN       A4
#define TEMPERATURE_PIN 13

#define SEGMENT_A 6
#define SEGMENT_B 7
#define SEGMENT_C 8
#define SEGMENT_D 9
#define SEGMENT_E 10
#define SEGMENT_F 11
#define SEGMENT_G 12

// esto si lo hizo chatsito
#define MAP_RANGE(x, in_min, in_max, out_min, out_max) ({ \
    __typeof__(x) _x = (x); \
    __typeof__(in_min) _in_min = (in_min); \
    __typeof__(in_max) _in_max = (in_max); \
    __typeof__(out_min) _out_min = (out_min); \
    __typeof__(out_max) _out_max = (out_max); \
    (_x - _in_min) * (_out_max - _out_min) / (_in_max - _in_min) + _out_min; \
})