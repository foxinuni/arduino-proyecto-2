#pragma once

#include <Arduino.h>
#include <DallasTemperature.h>
#include <OneWire.h>

#define TEMPERATURE_INTERVAL 1000 // ms

class TempSensor {
private:
    int pin;
    unsigned long last_tick;

    float cached_temperature = 0.0;
    OneWire* wire = NULL;
    DallasTemperature* sensor = NULL;


public:
    TempSensor(int pin) : pin(pin), last_tick(millis()) {}

    void setup() {
        this->wire = new OneWire(this->pin);
        this->sensor = new DallasTemperature(this->wire);
        this->sensor->begin();
        this->sensor->setWaitForConversion(false);
    }
    
    float get_temperature() {
        if (this->sensor == NULL) {
            Serial.println("warning: temp sensor not initialized!");
            return 0.0;
        }

        unsigned long now = millis();
        if (now - this->last_tick >= TEMPERATURE_INTERVAL) {
            this->cached_temperature = this->sensor->getTempCByIndex(0);
            this->sensor->requestTemperatures();
            this->last_tick = now;
        }

        return this->cached_temperature;
    }
};