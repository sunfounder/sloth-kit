/*******************************************************************
  This is the program for Ardunio Nano. The upper machine esp01s (or other)
  establishes websockets service to communicate with APP Sunfounder Controller,
  and returns data from the serial port to Nano, so as to complete the
  remote control of the robot Sloth.

  Dependent libraries:
    - ArduinoJson
    - NewPing

  Board tools:
    - Ardunio Nano

  Version: 1.1.3
    -- https://github.com/sunfounder/sloth-kit

  Author: Sunfounder
  Website: http://www.sunfounder.com
           https://docs.sunfounder.com
 *******************************************************************/
#define VERSION "1.1.3"

#include <Arduino.h>
#include "string.h"
#include "VarSpeedServo.h"
#include "ws.h"
#include "servos_control.h"
#include "ultrasonic.h"
#include "voice_control.h"
#include <EEPROM.h>

/************************** Configure ******************************/
/* Enable test mode */
#define TEST 0

/* Enable print WS receive */
#define PRINT_WS_RECV 0

/* Configure Wifi mode, ssid, password */
// #define WIFI_MODE WIFI_MODE_STA
// #define SSID "YOUR_SSID"
// #define PASSWORD "YOUR_PASSWORD"

/* WIFI AP mode */
#define WIFI_MODE WIFI_MODE_AP
#define SSID "Nano Sloth"
#define PASSWORD "12345678"

/* websockets port */
#define PORT "8765"

/* Define Name & Type */
#define NAME "Nano Sloth"
#define TYPE "Nano Sloth"

/* Configure the distance of Avoid mode, Follow mode and Keep Distance mode */
#define AVOID_DISTANCE 10
#define FOLLOW_DISTANCE 20
#define KEEP_DISTANCE_DISTANCE 5

/* Define built-in LED pin */
#define BUILT_IN_LED 13

/*********************** Global variables ****************************/
WS ws = WS();

float distance = 0;

extern int speed;
extern int delay_step;

uint8_t voice_step = 0;
bool autonomous_mode = false;
bool last_obstacle_state = false;
uint8_t fall_left_or_right = 0; // 0:left, 1:right

#define ACTION_NONE 0
#define ACTION_STAND 1
#define ACTION_DANCE 2
#define ACTION_HAPPY 3
#define ACTION_SAD 4
#define ACTION_SHY 5
#define ACTION_FALL 6
#define ACTION_CONFUSE 7
#define ACTION_FEAR 8

uint8_t current_action = ACTION_NONE;
uint8_t last_action = ACTION_NONE;
bool is_action = false;
int8_t voice_current_action = -1;

/* define mode variables */
#define MODE_NONE 0
#define MODE_AVOID 1
#define MODE_FOLLOW 2
#define MODE_KEEP_DISTANCE 3
#define MODE_CALIBRATION 4

uint8_t mode = MODE_NONE;

/* define calibration variables */
uint8_t cali_select = 0; // 0 left, 1 right
int8_t cali_arry_temp[4] = {0, 0, 0, 0}; // 0, Right upper; 1, Right lower； 2, Left upper; 3, Left lower;
uint8_t cali_max = 20;

/* define key variables */
int key_A = 0; // slider -100 ~ 100, init value 0
int key_B = 0;
int key_D = 0;
int key_H = 0;

bool key_J_Calibration = false;
bool key_I = false; 

bool key_E = false;
bool key_F = false;
bool key_G = false;

char key_J_Voice[20];

uint8_t key_K = 0;

bool key_M = false;
bool key_N = false;
bool key_O = false;
bool key_P = false;
bool key_Q = false;
bool key_R = false;
bool key_S = false;
bool key_T = false;

/*********************** setup() & loop() ************************/
void setup()
{
    int m = millis();

    Serial.begin(115200);
    Serial.println("Nano Sloth App Control "VERSION);

    pinMode(BUILT_IN_LED, OUTPUT); // Set LED pin as output
    // digitalWrite(BUILT_IN_LED, HIGH);  // 1:turn off LED
    digitalWrite(BUILT_IN_LED, LOW);  // 1:turn off LED

    servos_init();

#if TEST == 0
    while (millis() - m < 500) { // Wait for peripherals to be ready
        delay(1);
    }
    ws.begin(SSID, PASSWORD, WIFI_MODE, PORT, NAME, TYPE);
    ws.setOnReceived(onReceive);
#endif
}

void loop()
{

#if TEST == 0
    ws.loop();

#else /* if TEST == 1, Select the function you need to test by removing the comment */
    delay(50);
    // distance = ultrasonic_read();
    // Serial.print("distance: "); Serial.println(distance);
    // delay(100);

    // servo_move(0, 0);
    // servo_move(1, 0);
    // servo_move(2, 0);
    // servo_move(3, 0);

    // int8_t custom_step[][4] = {
    //     {0, 40, 0, 15},
    //     {30, 40, 30, 15},
    //     {30, 0, 30, 0},

    //     {0, -15, 0, -40},
    //     {-30, -15, -30, -40},
    //     {-30, 0, -30, 0},
    // };
    // do_action2(6, &custom_step[0][0]);

    // forward();
    // backward();
    // turn_left();
    // turn_right();
    stand();
    // moon_walk_left();
    // moon_walk_right();
    // hook();
    // big_swing();
    // swing();
    // walk_boldly();
    // walk_backward_boldly();
    // walk_shyly();
    // walk_backward_shyly();
    // stomp_left();
    // stomp_right();
    // tiptoe_left();
    // tiptoe_right();
    // fall_left();
    // fall_right();

    // dance_step_reset();
    // dance();

    // servo_move(1, -30);
    // servo_move(3, 30);
    // delay(100);
    // stand();
    // delay(1000);

    // happy();
    // sad();
    // shy();
    // confuse();
    // fear();
    // fall();

    // unsigned int uS = sonar.ping();
    // distance = uS / US_ROUNDTRIP_CM;
    // Serial.println(distance);
    // avoid();
    // follow();
    // keep_distance();
#endif
}

/*------------------ avoid & follow & keep_distance ---------------------*/
void avoid() {
    if(distance <= AVOID_DISTANCE) {
        if (last_obstacle_state == false) {
            hook();
            stand();
        }
        last_obstacle_state = true;
        turn_left();
    } else {
        last_obstacle_state = false;
        forward();
    }
}

void follow() {
    if (distance < 5) {
        last_obstacle_state = false;
        stand();
    } else if(distance < FOLLOW_DISTANCE) {
        if (last_obstacle_state == false) {
            hook();
            stand();
        }
        last_obstacle_state = true;
        forward();
    } else {
        last_obstacle_state = false;
        stand();
    }
}

void keep_distance() {
    if (distance < KEEP_DISTANCE_DISTANCE) {
        if (last_obstacle_state == false) {
            hook();
            stand();
        }
        last_obstacle_state = true;
        backward();
    } else if(distance > KEEP_DISTANCE_DISTANCE + 5 && distance < 100) {
        last_obstacle_state = false;
        forward();
    } else {
        last_obstacle_state = false;
        stand();
    }
}

int map_value (int input, int input_min, int input_max, int output_min, int output_max) {
    int output = 0; 
    // limit input
    if (input < input_min) {
        input = input_min;
    } else if (input > input_max) {
        input = input_max;
    }
    // remap
    output = output_min + (output_max - output_min)*(input-input_min)/(input_max-input_min);
    return output;
}

void calibrate() {
    Serial.println("Calibrating: ");
    int intput_min = -100;
    int intput_max = 100;
    int output_min = 90 - cali_max;
    int output_max = 90 + cali_max;
    cali_arry_temp[0] = map_value(key_D, intput_min, intput_max, output_min, output_max);
    cali_arry_temp[1] = map_value(key_H, intput_min, intput_max, output_min, output_max);
    cali_arry_temp[2] = map_value(key_A, intput_min, intput_max, output_min, output_max);
    cali_arry_temp[3] = map_value(key_B, intput_min, intput_max, output_min, output_max);
    Serial.print(cali_arry_temp[0]);Serial.print(", ");
    Serial.print(cali_arry_temp[1]);Serial.print(", ");
    Serial.print(cali_arry_temp[2]);Serial.print(", ");
    Serial.print(cali_arry_temp[3]);Serial.println();

    if (key_I) { // save calibration
        set_servo_calibration(cali_arry_temp);
        servo_move_no_cali(0, output_min);
        servo_move_no_cali(1, output_min);
        servo_move_no_cali(2, output_min);
        servo_move_no_cali(3, output_min);       
    }

    servo_move_no_cali(0, cali_arry_temp[0]);
    servo_move_no_cali(1, cali_arry_temp[1]);
    servo_move_no_cali(2, cali_arry_temp[2]);
    servo_move_no_cali(3, cali_arry_temp[3]);
}

/*-------------------- websocket received data to key values -----------------------*/
void onReceive() {
    #if PRINT_WS_RECV == 1
        Serial.print("onRecv:");
        Serial.println(ws.recvBuffer);
    #endif

#if 0 /*-------------- speed control ----------------*/
    // speed
    int key_A = ws.getSlider(REGION_A);
    speed = key_A;
    // delay_step
    int key_B = ws.getSlider(REGION_B);
    delay_step = key_B;
#endif

    /*-------------- EEPROM_CALIBRATION ----------------*/
#if EEPROM_CALIBRATION == 1 
    key_J_Calibration = ws.getSwitch(REGION_J);
    // ws.getSpeech(REGION_J, key_J);

    key_A = ws.getSlider(REGION_A); // left upper calibrate slider
    key_B = ws.getSlider(REGION_B); // left lower
    key_D = ws.getSlider(REGION_D); // right upper
    key_H = ws.getSlider(REGION_H); // right lower
    key_I = ws.getButton(REGION_I); // save calibration

    if (key_J_Calibration) {
    // if (String(key_J).indexOf("1") != -1) {
        calibrate();
        return;
    }

#endif

    /*---------------- data to display ----------------*/
    distance = ultrasonic_read();
    ws.send_doc["I"] = distance;
    
    /*------------------ mode ----------------------*/
    key_E = ws.getSwitch(REGION_E);
    key_F = ws.getSwitch(REGION_F);
    key_G = ws.getSwitch(REGION_G);
    
    if (key_E) {
        autonomous_mode = true;
        avoid();
        return;
    } else if (key_F) {
        autonomous_mode = true;
        follow();
        return;
    } else if (key_G) {
        autonomous_mode = true;
        keep_distance();
        return;
    } else {
        if (autonomous_mode) {
            if (key_E != NULL && key_F != NULL && key_G != NULL) {
                autonomous_mode = false;
                last_obstacle_state = false;
                stand();
                return;
            }
        }
    }

    /*-------------------- movement -----------------*/
    uint8_t key_K = ws.getDPad(REGION_K);
    // If there is a movement, it will return after execution,
    // and "action control" and "voice control" will not be executed
    switch (key_K) {
        case DPAD_STOP:
            stop();
            break;
        case DPAD_FORWARD:
            forward();
            return;
        case DPAD_BACKWARD:
            backward();
            return;
        case DPAD_LEFT:
            turn_left();
            return;
        case DPAD_RIGHT:
            turn_right();
            return;
        default:
            break;
    }

    /*-------------------- actions control ------------------*/
    key_M = ws.getMusicSwitch(REGION_M);
    key_N = ws.getMusicSwitch(REGION_N);
    key_O = ws.getMusicSwitch(REGION_O);
    key_P = ws.getMusicSwitch(REGION_P);
    key_Q = ws.getMusicSwitch(REGION_Q);
    key_R = ws.getMusicSwitch(REGION_R);
    key_S = ws.getMusicSwitch(REGION_S);
    key_T = ws.getMusicSwitch(REGION_T);

    if(key_M) current_action = ACTION_STAND;
    else if(key_N) current_action = ACTION_HAPPY;
    else if(key_O) current_action = ACTION_SAD;
    else if(key_P) current_action = ACTION_SHY;
    else if(key_Q) current_action = ACTION_DANCE;
    else if(key_R) current_action = ACTION_FALL;
    else if(key_S) current_action = ACTION_CONFUSE;
    else if(key_T) current_action = ACTION_FEAR;
    else {
        current_action = ACTION_NONE;
        if (last_action != ACTION_STAND or last_action != ACTION_NONE) {
            last_action = ACTION_NONE;
            stand();
        }
    }

    if (current_action != ACTION_NONE) {
        // voice_action = -1;
        voice_step = 0;
        if(last_action != current_action) {
            action_step_reset();
            last_action = current_action;
            if(current_action == ACTION_FALL) {
                // The fall action randomly falls to the left or right 
                uint32_t t = millis();
                if (t&1) fall_left_or_right = 1;
                else fall_left_or_right = 0;
            }
        }

        switch(current_action) {
            case ACTION_STAND:
                stand();
                break;
            case ACTION_HAPPY:
                happy();
                break;
            case ACTION_SAD:
                sad();
                break;
            case ACTION_SHY:
                shy();
                break;
            case ACTION_DANCE:
                dance();
                break;
            case ACTION_FALL:
                if (fall_left_or_right) fall_right();
                else fall_left();
                break;
            case ACTION_CONFUSE:
                confuse();
                break;
            case ACTION_FEAR:
                fear();
                break;
            default:
                break;
        }
    }

    /*-------------------- voice control ------------------*/
    // char key_J[20];
    int8_t code = -1;
    ws.getSpeech(REGION_J, key_J_Voice);
    Serial.print("voice len: ");Serial.println(strlen(key_J_Voice));
    if (strlen(key_J_Voice) > 0) {
        code  = text_2_cmd_code(key_J_Voice);
        if (code != -1) {
            if (voice_current_action != code) {
                voice_current_action = code;
                voice_step = voice_action_max_step[code]*voice_action_time[code];
            }
        }
    }
    Serial.print("voice_step: ");Serial.print(voice_step);
    Serial.print("voice_current_action: ");Serial.println(voice_current_action);

    if (voice_step > 0 && voice_current_action != -1) {
        voice_action(voice_current_action);
        voice_step --;
        ws.send_doc["J"] = 1;
    } else {
        voice_current_action = -1;
        voice_step = 0;
        ws.send_doc["J"] = 0;
    }

}