#include <Arduino.h>

#include "common.h"
#include "button.h"
#include "music.h"
#include "gate.h"
#include "display.h"
#include "temperature.h"

#define LIGHT_LEVEL 4.2 // volts
#define TEMP_LEVEL 28.0 // °C
#define UPDATE_INTERVAL 1000 // ms

#define MOTOR_MIN 128
#define MOTOR_MAX 255

Button entry_button(ENTRY_BUTTON);
Button leave_button(LEAVE_BUTTON);
TempSensor temperature_sensor(TEMPERATURE_PIN);

int counter = 0;

void setup() {
	Serial.begin(9600);

    // pin setup
	entry_button.setup();
    leave_button.setup();

    // buzzer setup
    pinMode(BUZZER_PIN, OUTPUT);

    // servo setup
    setup_gate();

    // display setup
    setup_display();

    // motor setup
    pinMode(MOTOR_PIN, OUTPUT);
    pinMode(LIGHT_PIN, INPUT);

    // temp sennsor
    temperature_sensor.setup();

    Serial.println("Setup done!");
}

void loop() {
    static unsigned long last_update = 0;

    if (entry_button.pressed()) {
        if (counter < 15) {
            counter++;

            display_number(counter);
            set_melody(&megalovania);
            start_melody();
            open_gate();
        } else {
            set_melody(&warning_beeps);
            start_melody();
        }
    } else if (leave_button.pressed()) {
        if (counter > 0) {
            counter--;

            display_number(counter);
            open_gate();
        } else {
            set_melody(&warning_beeps);
            start_melody();
        }
    }

    float light_level = (float) analogRead(LIGHT_PIN) / 1023.0 * 5.0;

    // Hola profe, se que esto esta en espanglish, no lo hizo chatsito,
    // solo que me da flojera cambiar los nombres y me acabo de dar cuenta :D

    bool parqueadero_saturado = counter >= 7;
    bool luz_saturada = light_level >= LIGHT_LEVEL;
    bool temperatura_saturada = temperature_sensor.get_temperature() >= TEMP_LEVEL;

    bool ventilador_prendido = parqueadero_saturado || luz_saturada || temperatura_saturada;
    int ventilador_parqueadero = MAP_RANGE(counter, 7, 15, MOTOR_MIN, MOTOR_MAX);
    int ventilador_luz = MAP_RANGE(light_level, LIGHT_LEVEL, 5.0, MOTOR_MIN, MOTOR_MAX);
    int ventilador_temp = MAP_RANGE(temperatura_saturada, TEMP_LEVEL, 50.0, MOTOR_MIN, MOTOR_MAX);

    int velocidad_ventilador = max(max(ventilador_parqueadero, ventilador_luz), ventilador_temp);
    if (velocidad_ventilador < MOTOR_MIN) {
        velocidad_ventilador = MOTOR_MIN;
    } else if (velocidad_ventilador > MOTOR_MAX) {
        velocidad_ventilador = MOTOR_MAX;
    }
    
    unsigned long now = millis();
    if (now - last_update >= UPDATE_INTERVAL) {
        Serial.println("-------------------------");
        Serial.print("Car Counter: ");
        Serial.println(counter);
        Serial.print("Temperature: ");
        Serial.println(temperature_sensor.get_temperature());
        Serial.print("Light Level: ");
        Serial.println(light_level);
        Serial.print("Estado Ventilador: ");
        Serial.println(ventilador_prendido);
        Serial.print("Velocidad Ventilador: ");
        Serial.println(velocidad_ventilador);

        last_update = now;
    }

    if (ventilador_prendido) {
        analogWrite(MOTOR_PIN, velocidad_ventilador);
    } else {
        analogWrite(MOTOR_PIN, 0);
    }

    update_melody();
    update_gate();
}