#pragma once

#include <Arduino.h>

#define NOTE_NULL -1
#define NOTE_D3  147
#define NOTE_D4  294
#define NOTE_A4  440
#define NOTE_GS4 415
#define NOTE_G4  392
#define NOTE_F4  349
#define NOTE_C4  262
#define NOTE_B3  247

struct note_t {
    short note;
    int duration_ms;
};

struct melody_t {
    note_t* notes = NULL;
    int length       = 0;
    int current_note = 0;
    int last_update  = 0;
    bool playing     = false;

    melody_t(note_t* n, int l) 
        : notes(n), length(l), current_note(0), last_update(0), playing(false) {}
};

extern melody_t megalovania;
extern melody_t warning_beeps;

void set_melody(melody_t* melody);
void reset_melody();
void start_melody();
void update_melody();
