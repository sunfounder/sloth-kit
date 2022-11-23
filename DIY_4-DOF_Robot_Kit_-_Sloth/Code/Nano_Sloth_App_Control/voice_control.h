#ifndef __VOICE_CONTROL_H__
#define __VOICE_CONTROL_H__

#include <Arduino.h>
#include "servos_control.h"

#define CMD_SUM 9

// Pay attention to the order,
// eg: 'left forward' needs to be placed before' left' and 'forward'
const char cmd_str_0[] PROGMEM = "stop";
const char cmd_str_1[] PROGMEM = "forward";
const char cmd_str_2[] PROGMEM = "backward";
const char cmd_str_3[] PROGMEM = "turn left";
const char cmd_str_4[] PROGMEM = "turn right";
const char cmd_str_5[] PROGMEM = "stand";
const char cmd_str_6[] PROGMEM = "sit";
const char cmd_str_7[] PROGMEM = "jump";
const char cmd_str_8[] PROGMEM = "dance";

const char *const cmd_str_table[CMD_SUM] PROGMEM = {
    cmd_str_0, cmd_str_1, cmd_str_2, cmd_str_3,
    cmd_str_4, cmd_str_5, cmd_str_6, cmd_str_7,
    cmd_str_8,
};

void (*cmd_fuc_table [])(){
    stop,
    forward,
    backward,
    turn_left,
    turn_right,
    stand,
    big_swing,
    happy,
    dance,
};

int8_t text_2_cmd_code(char *text) {
    String str = String(text);
    char buffer[20];

    for(uint8_t i = 0; i < CMD_SUM; i++) {
        strcpy_P(buffer, (char *)pgm_read_word(&cmd_str_table[i]));
        // if (str == String(pgm_read_word(&cmd_str_table[i])) ) {
        if (str.indexOf(buffer) != -1) {
            // cmd_fuc_table[i]();
            return i;
        }
    }
    return -1;
}

#endif
