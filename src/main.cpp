#include <Arduino.h>

#include "common.h"
#include "button.h"
#include "music.h"
#include "gate.h"
#include "display.h"
#include "temperature.h"

// Constantes utilidas como umbrales
#define LIGHT_LEVEL 4.2 // volts
#define TEMP_LEVEL 28.0 // °C

// Cantidad de tiempo antes de actualizar
// el monitor serial.
#define UPDATE_INTERVAL 1000 // ms

// Valores maximos y minimos de velocidad del motor
#define MOTOR_MIN 128
#define MOTOR_MAX 255

// Se inicializan los botones de entrada y salida
Button entry_button(ENTRY_BUTTON);
Button leave_button(LEAVE_BUTTON);

// Se inicializa el sensor de temperatura
TempSensor temperature_sensor(TEMPERATURE_PIN);

// Contador de vehiculos adentro del parqueadero
int counter = 0;

void setup() {
    // Se configura el monitor serial
	Serial.begin(9600);

    // Configuracion de botones
	entry_button.setup();
    leave_button.setup();
    
    // Configuracion del servo motor
    setup_gate();

    // Configuracion del display de 7 segmentos
    setup_display();
    
    pinMode(BUZZER_PIN, OUTPUT); // Configuracion del buzzer
    pinMode(MOTOR_PIN, OUTPUT);  // Configuracion del motor
    pinMode(LIGHT_PIN, INPUT);   // Configuracion de la foto resistencia

    // Sensor de temperatura
    temperature_sensor.setup();

    Serial.println("Setup done!");
}

void loop() {
    // Contador en milisegundos desde el ultimo mensaje
    // el monitor serial
    static unsigned long last_update = 0;

    // Si el boton de entrada esta siendo presionado
    if (entry_button.pressed()) {
        if (counter < 15) {
            counter++; // Se incrementa el contador

            display_number(counter); // Se muestra el numero en el display
            set_melody(&megalovania); // Se pone la melodia de entrada
            start_melody(); // Se inicia la melodia
            open_gate(); // Se abre la compuerta
        } else {
            set_melody(&warning_beeps); // Se pone la melodia de advertencia
            start_melody(); // Se incia la melodia
        }
    } else if (leave_button.pressed()) {
        if (counter > 0) {
            counter--; // Se decrementa el contador

            display_number(counter); // Se muestra el numero en el display
            open_gate(); // Se abre la puerta
        } else {
            set_melody(&warning_beeps); // Se pone la melodia de advertencia
            start_melody(); // Se incia la melodia
        }
    }

    // Se lee el voltaje de la fotocelda
    float light_level = (float) analogRead(LIGHT_PIN) / 1023.0 * 5.0;

    // Hola profe, se que esto esta en espanglish, no lo hizo chatsito,
    // solo que me da flojera cambiar los nombres y me acabo de dar cuenta :D

    // Se miran los condicionales para ver si se pasan los umbrales necesarios
    // para encender el motor DC.
    bool parqueadero_saturado = counter >= 7;
    bool luz_saturada = light_level >= LIGHT_LEVEL;
    bool temperatura_saturada = temperature_sensor.get_temperature() >= TEMP_LEVEL;

    // Si el motor DC esta prendido se utiliza map para conseguir la velocidad del motor
    // por cada variable.
    bool ventilador_prendido = parqueadero_saturado || luz_saturada || temperatura_saturada;
    int ventilador_parqueadero = MAP_RANGE(counter, 7, 15, MOTOR_MIN, MOTOR_MAX);
    int ventilador_luz = MAP_RANGE(light_level, LIGHT_LEVEL, 5.0, MOTOR_MIN, MOTOR_MAX);
    int ventilador_temp = MAP_RANGE(temperatura_saturada, TEMP_LEVEL, 50.0, MOTOR_MIN, MOTOR_MAX);

    // Se configura la velocidad del ventilador a la mayor entre todas las variables.
    int velocidad_ventilador = max(max(ventilador_parqueadero, ventilador_luz), ventilador_temp);
    if (velocidad_ventilador < MOTOR_MIN) {
        velocidad_ventilador = MOTOR_MIN;
    } else if (velocidad_ventilador > MOTOR_MAX) {
        velocidad_ventilador = MOTOR_MAX;
    }
    
    // Si se ha pasado el tiempo de actualizacion,
    // se manda el mensaje sobre el estado en el monitor serial.
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

    // Se prende el ventilador en la velocidad requerida.
    if (ventilador_prendido) {
        analogWrite(MOTOR_PIN, velocidad_ventilador);
    } else {
        analogWrite(MOTOR_PIN, 0);
    }

    update_melody(); // Se actualiza la melodia
    update_gate(); // Se actualiza el estado del servo motor
}