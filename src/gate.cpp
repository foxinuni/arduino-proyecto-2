#include "gate.h"
#include "common.h"

#include <Arduino.h>
#include <Servo.h>

struct gate_state_t {
    bool open = false;
    int opened_time = 0;
};

Servo servo;
gate_state_t state;

void setup_gate() {
    servo.attach(SERVO_PIN);

    while (!servo.attached()) {}
    servo.write(SERVO_MIN);
}

void open_gate() {
    if (state.open) {
        return;
    }

    state.open = true;
    state.opened_time = millis();
    servo.write(SERVO_MAX);
}

void update_gate() {
    if (!state.open) {
        return;
    }

    int delta_ms = millis() - state.opened_time;
    if (delta_ms > SERVO_DELAY) {
        state.open = false;
        servo.write(SERVO_MIN);
    }
}