#include "servos_control.h"
#include <EEPROM.h>

VarSpeedServo RU;  //Right Upper
VarSpeedServo RL;  //Right Lower
VarSpeedServo LU;  //Left Upper
VarSpeedServo LL;  //Left Lower

int8_t array_cal[SERVOS_NUM] = ARRAY_CALIBRATION_INIT;


// const int vel = 20;
// const int delay_Forward = 750;

// const int speed = 100;
// const int delay_step = 100;
int speed = 120;
int delay_step = 180;

int8_t action_step = 0;
int8_t action_max_step = 0;
typedef void (*Func)(void);

void servos_init() {
    RU.attach(9);
    RL.attach(10);
    LU.attach(11);
    LL.attach(12);
    delay(20);

    #if EEPROM_CALIBRATION == 1
        for (uint8_t i = 0; i < SERVOS_NUM; i++) {
            array_cal[i] = EEPROM.read(EEPROM_CALIBRATION_ADDRESS+i);
        }
    #endif

    Serial.print("arry_cal: ");
    Serial.print(array_cal[0]);Serial.print(", ");
    Serial.print(array_cal[1]);Serial.print(", ");
    Serial.print(array_cal[2]);Serial.print(", ");
    Serial.print(array_cal[3]);Serial.println();

    servo_move(0, 0);
    servo_move(1, 0);
    servo_move(2, 0);
    servo_move(3, 0);
    delay(100);
}

void set_servo_calibration(int8_t cali_angles[SERVOS_NUM]) {
    Serial.print("Set Calibration: ");
    for (uint8_t i = 0; i < SERVOS_NUM; i++) {
        EEPROM.write(EEPROM_CALIBRATION_ADDRESS+i, cali_angles[i]);
        array_cal[i] = cali_angles[i];
        Serial.print(array_cal[i]);
        Serial.print(", ");
    }
    Serial.println();
}

void stop() {
    RU.stop();
    RL.stop();
    LU.stop();
    LL.stop();
}

void servo_move_no_cali(uint8_t servo_num, int8_t angle) {
    switch (servo_num) {
        case 0:
            RU.slowmove(+ angle, speed);
            delay(delay_step);
            break;
        case 1:
            RL.slowmove(angle, speed);
            delay(delay_step);
            break;
        case 2:
            LU.slowmove(angle, speed);
            delay(delay_step);
            break;
        case 3:
            LL.slowmove(angle, speed);
            delay(delay_step);
            break;
        default:
            break;
    }
}

void servo_move(uint8_t servo_num, int8_t angle) {
    switch (servo_num) {
        case 0:
            RU.slowmove(int8_t(array_cal[0]) + angle, speed);
            delay(delay_step);
            break;
        case 1:
            RL.slowmove(int8_t(array_cal[1]) + angle, speed);
            delay(delay_step);
            break;
        case 2:
            LU.slowmove(int8_t(array_cal[2]) + angle, speed);
            delay(delay_step);
            break;
        case 3:
            LL.slowmove(int8_t(array_cal[3]) + angle, speed);
            delay(delay_step);
            break;
        default:
            break;
    }
}


void do_action(int8_t len, int32_t array_addr) {
    for(int i=0; i<len; i++) {
        RU.slowmove(int8_t(array_cal[0]) + int8_t(pgm_read_byte(array_addr+SERVOS_NUM*i+0)) , speed);    
        RL.slowmove(int8_t(array_cal[1]) + int8_t(pgm_read_byte(array_addr+SERVOS_NUM*i+1)) , speed);
        LU.slowmove(int8_t(array_cal[2]) + int8_t(pgm_read_byte(array_addr+SERVOS_NUM*i+2)) , speed);
        LL.slowmove(int8_t(array_cal[3]) + int8_t(pgm_read_byte(array_addr+SERVOS_NUM*i+3)) , speed);
        delay(delay_step);
    }
}

void do_action2(int8_t len, int8_t (*array_addr)) {
    for(int i=0; i<len; i++) {
        
        Serial.print("0: ");Serial.println(int8_t(*(array_addr+SERVOS_NUM*i+0)));
        Serial.print("1: ");Serial.println(int8_t(*(array_addr+SERVOS_NUM*i+1)));
        Serial.print("2: ");Serial.println(int8_t(*(array_addr+SERVOS_NUM*i+2)));
        Serial.print("3: ");Serial.println(int8_t(*(array_addr+SERVOS_NUM*i+3)));

        RU.slowmove(int8_t(array_cal[0]) + int8_t(*(array_addr+SERVOS_NUM*i+0)) , speed);   
        RL.slowmove(int8_t(array_cal[1]) + int8_t(*(array_addr+SERVOS_NUM*i+1)) , speed);
        LU.slowmove(int8_t(array_cal[2]) + int8_t(*(array_addr+SERVOS_NUM*i+2)) , speed);
        LL.slowmove(int8_t(array_cal[3]) + int8_t(*(array_addr+SERVOS_NUM*i+3)) , speed);
        delay(delay_step);
    }
}

void do_action2(int8_t len, int8_t (*array_addr)[SERVOS_NUM]) {
    for(int i=0; i<len; i++) {
        
        Serial.print("0: ");Serial.println(int8_t(*(array_addr+SERVOS_NUM*i+0)));
        Serial.print("1: ");Serial.println(int8_t(*(array_addr+SERVOS_NUM*i+1)));
        Serial.print("2: ");Serial.println(int8_t(*(array_addr+SERVOS_NUM*i+2)));
        Serial.print("3: ");Serial.println(int8_t(*(array_addr+SERVOS_NUM*i+3)));

        RU.slowmove(int8_t(array_cal[0]) + int8_t(array_addr[SERVOS_NUM*i+0]) , speed);   
        RL.slowmove(int8_t(array_cal[1]) + int8_t(array_addr[SERVOS_NUM*i+1]) , speed);
        LU.slowmove(int8_t(array_cal[2]) + int8_t(array_addr[SERVOS_NUM*i+2]) , speed);
        LL.slowmove(int8_t(array_cal[3]) + int8_t(array_addr[SERVOS_NUM*i+3]) , speed);
        delay(delay_step);
    }
}

void forward() {do_action(ARRY_FORWARD_LENGTH, &array_forward);}
void backward() {do_action(ARRY_BACKWARD_LENGTH, &array_backward);}
void turn_left() {do_action(ARRY_TURN_LEFT_LENGTH, &array_turn_left);}
void turn_right() {do_action(ARRY_TURN_RIGHT_LENGTH, &array_turn_right);}
void stand() {do_action(ARRY_STAND_LENGTH, &array_stand);}
void moon_walk_left() {do_action(ARRY_MOON_LEFT_LENGTH, &array_moon_walk_left);}
void moon_walk_right() {do_action(ARRY_MOON_RIGHT_LENGTH, &array_moon_walk_right);}
void hook() {do_action(ARRY_HOOK_LENGTH, &array_hook);}
void sit() {do_action(ARRY_SIT_LENGTH, &array_sit);}
void swing() {do_action(1, &array_swing);}
void walk_boldly() {do_action(6, &array_walk_boldly);}
void walk_backward_boldly() {do_action(6, &array_walk_backward_boldly);}
void walk_shyly() {do_action(6, &array_walk_shyly);}
void walk_backward_shyly() {do_action(6, &array_walk_backward_shyly);}
void stomp_left() {do_action(5, &array_stomp_left);}
void stomp_right() {do_action(5, &array_stomp_right);}
void pigeon_toed() {do_action(1, &array_pigeon_toed);}
void splay_feet() {do_action(1, &array_splay_feet);}
void tiptoe_left() {do_action(2, &array_tiptoe_left);}
void tiptoe_right() {do_action(2, &array_tiptoe_right);}
void fall_left() {do_action(1, &array_fall_left);}
void fall_right() {do_action(1, &array_fall_right);}
void go_up_and_down() {do_action(2, &array_go_up_and_down);}

void action_step_reset() {
    action_step = 0;
}

void do_fuc_action(uint8_t max_step, uint16_t* action_arry_addr) {
    action_max_step = max_step - 1;
    if (action_step < action_max_step) {
        action_step ++;
    } else {
        action_step = 0;
    }
    Func func_addr = pgm_read_word(&action_arry_addr[action_step]);
    func_addr();
}

void do_fuc_action_block(uint8_t max_step, uint16_t* action_arry_addr) {
    for (int i = 0; i < max_step -1; i++) {
        Func func_addr = pgm_read_word(&action_arry_addr[i]);
        func_addr();
    }
}

void dance() {do_fuc_action(ARRY_DANCE_LENGTH, array_dance);}
void happy() {do_fuc_action(ARRY_HAPPY_LENGTH, array_happy);}
void sad() {do_fuc_action(ARRY_SAD_LENGTH, array_sad);}
void shy() {do_fuc_action(ARRY_SHY_LENGTH, array_shy);}
void confuse() {do_fuc_action(ARRY_CONFUSE_LENGTH, array_confuse);}
void fear() {do_fuc_action(ARRY_FEAR_LENGTH, array_fear);}

