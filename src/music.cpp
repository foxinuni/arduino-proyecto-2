#include "music.h"
#include "common.h"

note_t megalovania_notes[] = {
    {NOTE_D4, 150}, {NOTE_D4, 150},
    {NOTE_D4, 300}, {NOTE_A4, 300},
    {NOTE_GS4, 300}, {NOTE_G4, 300},
    {NOTE_F4, 300}, {NOTE_D4, 150},
    {NOTE_F4, 150}, {NOTE_G4, 150},

    {NOTE_C4, 150}, {NOTE_C4, 150},
    {NOTE_D4, 300}, {NOTE_A4, 300},
    {NOTE_GS4, 300}, {NOTE_G4, 300},
    {NOTE_F4, 300}, {NOTE_D4, 150},
    {NOTE_F4, 150}, {NOTE_G4, 150},

    {NOTE_B3, 150}, {NOTE_B3, 150},
    {NOTE_D4, 300}, {NOTE_A4, 300},
    {NOTE_GS4, 300}, {NOTE_G4, 300},
    {NOTE_F4, 300}, {NOTE_D4, 150},
    {NOTE_F4, 150}, {NOTE_G4, 150},
};

melody_t megalovania(megalovania_notes, sizeof(megalovania_notes) / sizeof(note_t));

note_t warning_notes[] = {
    {NOTE_A4, 500}, {0, 500},  // Beep 1 + short silence
    {NOTE_A4, 500}, {0, 500},  // Beep 2 + short silence
    {NOTE_A4, 1000}            // Beep 3 (longer for emphasis)
};

melody_t warning_beeps(warning_notes, sizeof(warning_notes) / sizeof(note_t));

static melody_t* current_melody = NULL;

void set_melody(melody_t* melody) {
    if (current_melody != NULL) {
        reset_melody();
    }

    current_melody = melody;
}

void reset_melody() {
    noTone(BUZZER_PIN);

    if (current_melody != NULL) {
        current_melody->current_note = 0;
        current_melody->playing = false;
    }
}

void start_melody() {
    reset_melody();
    
    if (current_melody != NULL) {

        current_melody->playing = true;
        current_melody->current_note = 0;
        current_melody->last_update = millis();

        if (current_melody->length > 0) {
            note_t next_note = current_melody->notes[current_melody->current_note];
            noTone(BUZZER_PIN);

            if (next_note.note != 0) {
                tone(BUZZER_PIN, next_note.note, next_note.duration_ms);
            }
        }
    }
}

void update_melody() {
    if (current_melody == NULL || !current_melody->playing) {
        return;
    }

    note_t note = current_melody->notes[current_melody->current_note];
    int delta_ms = millis() - current_melody->last_update;

    if (note.duration_ms < (delta_ms * 1.1)) {
        current_melody->current_note++;
        current_melody->last_update = millis();

        if (current_melody->current_note >= current_melody->length) {
            reset_melody();
        } else {
            note_t next_note = current_melody->notes[current_melody->current_note];
            noTone(BUZZER_PIN);

            if (next_note.note != 0) {
                tone(BUZZER_PIN, next_note.note, next_note.duration_ms);
            }
        }
    }
}