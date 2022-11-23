#ifndef _SERVOS_CONTROL_H
#define _SERVOS_CONTROL_H

#include "VarSpeedServo.h"
#include <NewPing.h>

/*Number of servos*/
#define SERVOS_NUM 4

/*Define pins of servos*/
#define UP_RIGHT_SERVO 9
#define LOW_RIGHT_SERVO 10
#define UP_LEFT_SERVO 11
#define LOW_LEFT_SERVO 12

/*Servos initial angle calibration value*/
const int8_t array_cal[SERVOS_NUM] PROGMEM = {95, 90, 90, 92};  // RU, RL, LU, LL

/*Define various action steps
The value represents the rotation angle of the servo based on the initial position
*/
const int8_t array_forward[6][SERVOS_NUM] PROGMEM = {
    {0, 40, 0, 15},
    {-30, 40, -30, 15},
    {-30, 0, -30, 0},

    {0, -15, 0, -40},
    {30, -15, 30, -40},
    {30, 0, 30, 0},
};

const int8_t array_backward[6][SERVOS_NUM] PROGMEM = {
    {0, 40, 0, 15},
    {30, 40, 30, 15},
    {30, 0, 30, 0},

    {0, -15, 0, -40},
    {-30, -15, -30, -40},
    {-30, 0, -30, 0},
};

const int8_t array_turn_left[6][SERVOS_NUM] PROGMEM = {
    {0, 40, 0, 20},
    {0, 40, 20, 20},
    {0, 0, 20, 0},

    {0, -20, 0, -40},
    {0, -20, -20, -40},
    {0, 0, -20, 0},
};

const int8_t array_turn_right[6][SERVOS_NUM] PROGMEM = {
    {0, -20, 0, -40},
    {-20, -20, 0, -40},
    {-20, 0, 0, 0},

    {0, 40, 0, 20},
    {20, 40, 0, 20},
    {20, 0, 0, 0},
};

const int8_t array_stand[1][SERVOS_NUM] PROGMEM = {
    {0,0,0,0},
};

const int8_t array_moon_walk_left[5][SERVOS_NUM] PROGMEM = {
    {0, 0, 0, -30},
    {0, 30, 0, -60},
    {0, 60, 0, -30},
    {0, 30, 0, 0},
    {0, 0, 0, 0},
};

const int8_t array_moon_walk_right[5][SERVOS_NUM] PROGMEM = {
    {0, 30, 0, 0},
    {0, 60, 0, -30},
    {0, 30, 0, -60},
    {0, 0, 0, -30},
    {0, 0, 0, 0},
};

const int8_t array_hook[1][SERVOS_NUM] PROGMEM = {
    {0, 50, 0, -50},
};

const int8_t array_big_swing[1][SERVOS_NUM] PROGMEM = {
    {0, -90, 0, 90},
};

const int8_t array_swing[1][SERVOS_NUM] PROGMEM = {
    {0, -40, 0, 40},
};

const int8_t array_walk_boldly[6][SERVOS_NUM] PROGMEM = {
    {-15, -15, 15, -40},
    {10, -30, 40, -40},
    {10, 0, 40, 0},

    {-15, 40, 15, 15},
    {-40, 40, -10, 30},
    {-40, 0, -10, 0},
};

const int8_t array_walk_backward_boldly[6][SERVOS_NUM] PROGMEM = {
    {-15, -15, 15, -40},
    {-40, -30, -10, -40},
    {-40, 0, -10, 0},

    {-15, 40, 15, 15},
    {10, 40, 40, 30},
    {10, 0, 40, 0},
};

const int8_t array_walk_shyly[6][SERVOS_NUM] PROGMEM = {
    {10, -15, -10, -40},
    {25, -30, -5, -40},
    {25, 0, -5, 0},

    {10, 40, -10, 15},
    {5, 40, -25, 30},
    {5, 0, -25, 0},
};

const int8_t array_walk_backward_shyly[6][SERVOS_NUM] PROGMEM = {
    {10, -15, -10, -40},
    {5, -30, -25, -40},
    {5, 0, -25, 0},

    {10, 40, -10, 15},
    {25, 40, -5, 30},
    {25, 0, -5, 0},
};

const int8_t array_stomp_left[5][SERVOS_NUM] PROGMEM = {
    {0, 0, 0, -15},
    {0, 15, 0, -30},
    {0, 30, 0, -15},
    {0, 15, 0, 0},
    {0, 0, 0, 0},
};

const int8_t array_stomp_right[5][SERVOS_NUM] PROGMEM = {
    {0, 15, 0, 0},
    {0, 30, 0, -15},
    {0, 15, 0, -30},
    {0, 0, 0, -15},
    {0, 0, 0, 0},
};

const int8_t array_pigeon_toed[1][SERVOS_NUM] PROGMEM = {
    {30, 0, -30, 0},
};

const int8_t array_splay_feet[1][SERVOS_NUM] PROGMEM = {
    {-30, 0, 30, 0},
};

const int8_t array_tiptoe_left[2][SERVOS_NUM] PROGMEM = {
    {-20, 35, -20, 15},
    {-20, 15, -20, 15},
};

const int8_t array_tiptoe_right[2][SERVOS_NUM] PROGMEM = {
    {20, -15, 20, -35},
    {20, -15, 20, -15},
};

const int8_t array_fall_left[1][SERVOS_NUM] PROGMEM = {
    // {40, -30, 40, -70},
    {40, -30, 40, -30},
};

const int8_t array_fall_right[1][SERVOS_NUM] PROGMEM = {
    // {-40, 70, -40, 30},
    {-40, 30, -40, 30},
};

const int8_t array_go_up_and_down[2][SERVOS_NUM] PROGMEM = {
    {0, 50, 0, -50},
    {0, 0, 0, 0},
};


/* Functions */
void servos_init();
void stop();
void do_action(int8_t len, int32_t array_addr);
void servo_move(uint8_t servo_num, int8_t angle);

void forward();
void backward();
void turn_left();
void turn_right();
void stand();
void moon_walk_left();
void moon_walk_right();
void hook();
void big_swing();
void swing();
void walk_boldly();
void walk_backward_boldly();
void walk_shyly();
void walk_backward_shyly();
void stomp_left();
void stomp_right();
void pigeon_toed();
void splay_feet();
void tiptoe_left();
void tiptoe_right();
void fall_left();
void fall_right();
void go_up_and_down();

void action_step_reset();
void do_fuc_action(uint8_t max_step, uint16_t* action_arry_addr);
void do_fuc_action_block(uint8_t max_step, uint16_t* action_arry_addr);

#define ARRY_DANCE_LENGTH 60
const int16_t array_dance[ARRY_DANCE_LENGTH] PROGMEM = {
    // part1
    stomp_left, stomp_left, stomp_left,
    stomp_right, stomp_right, stomp_right,
    moon_walk_left, moon_walk_left, moon_walk_left,
    moon_walk_right, moon_walk_right, moon_walk_right, // 12

    swing, stand, swing, stand, swing, stand, // 6
    pigeon_toed, stand, splay_feet, stand,  
    pigeon_toed, stand, splay_feet, stand,
    pigeon_toed, stand, splay_feet, stand,   // 12
    tiptoe_left, tiptoe_right, stand, // 3

    // part2
    stomp_left, stomp_left, stomp_left,
    stomp_right, stomp_right, stomp_right,
    moon_walk_left, moon_walk_left, moon_walk_left,
    moon_walk_right, moon_walk_right, moon_walk_right, // 12

    hook, stand, hook, stand, hook, stand,
    swing, big_swing, swing, big_swing, swing, big_swing, // 12
    tiptoe_left, tiptoe_right, stand, // 3
};
void dance();

#define ARRY_HAPPY_LENGTH 2
const int16_t array_happy[ARRY_HAPPY_LENGTH] PROGMEM = {
    hook, stand,
};
void happy();

#define ARRY_SAD_LENGTH 1
const int16_t array_sad[ARRY_SAD_LENGTH] PROGMEM = {
    big_swing,
};
void sad();

#define ARRY_SHY_LENGTH 1
const int16_t array_shy[ARRY_SHY_LENGTH] PROGMEM = {
    pigeon_toed,
};
void shy();

#define ARRY_CONFUSE_LENGTH 1
const int16_t array_confuse[ARRY_CONFUSE_LENGTH] PROGMEM = {
    hook,
};
void confuse();

#define ARRY_FEAR_LENGTH 2
const int16_t array_fear[ARRY_FEAR_LENGTH] PROGMEM = {
    go_up_and_down, walk_backward_boldly,
};
void fear();


#endif
