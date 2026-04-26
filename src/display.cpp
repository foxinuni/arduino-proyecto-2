#include "display.h"

#include "common.h"
#include <Arduino.h>

#define bit_read(value, bit) (((value) >> (bit)) & 0x01)

const int segment_pins[] = {
    SEGMENT_A, SEGMENT_B, SEGMENT_C,
    SEGMENT_D, SEGMENT_E, SEGMENT_F,
    SEGMENT_G
};

const unsigned char hex_segments[] = {
    0x3F, // 0: 0111111 (a,b,c,d,e,f)
    0x06, // 1: 0000110 (b,c)
    0x5B, // 2: 1011011 (a,b,g,e,d)
    0x4F, // 3: 1001111 (a,b,g,c,d)
    0x66, // 4: 1100110 (f,g,b,c)
    0x6D, // 5: 1101101 (a,f,g,c,d)
    0x7D, // 6: 1111101 (a,f,g,e,c,d)
    0x07, // 7: 0000111 (a,b,c)
    0x7F, // 8: 1111111 (a,b,c,d,e,f,g)
    0x6F, // 9: 1101111 (a,b,c,d,f,g)
    0x77, // A: 1110111 (a,b,c,e,f,g)
    0x7C, // b: 1111100 (c,d,e,f,g)
    0x39, // C: 0111001 (a,d,e,f)
    0x5E, // d: 1011110 (b,c,d,e,g)
    0x79, // E: 1111001 (a,d,e,f,g)
    0x71  // F: 1110001 (a,e,f,g)
};

void setup_display() {
    for (int segment = 0; segment < 7; segment++) {
        pinMode(segment_pins[segment], OUTPUT);
    }
    
    display_number(0);
}

void display_number(int number) {
    if (number < 0 || number > 15) {
        Serial.print("Warning: display_number() called for invalid value -> ");
        Serial.println(number);
        return;
    }

    for (int segment = 0; segment< 7; segment++) {
        unsigned char bitset = hex_segments[number];
        int on = bit_read(bitset, segment);
        digitalWrite(segment_pins[segment], on ? HIGH : LOW);
    }
}