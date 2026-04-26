#pragma once

#include <Arduino.h>

#define BUTTON_DEBOUNCE 500 //ms

class Button {
private:
    int pin;
    unsigned long last_pressed;
public:
    Button(int pin) : pin(pin), last_pressed(millis()) {}
    
    inline void setup() {
        pinMode(this->pin, INPUT);
    }

    bool pressed() {
        bool time_passed = millis() - this->last_pressed > BUTTON_DEBOUNCE;
        bool pressed = digitalRead(pin);

        if (time_passed && pressed) {
            this->last_pressed = millis();
            return true;
        }

        return false;
    }
};