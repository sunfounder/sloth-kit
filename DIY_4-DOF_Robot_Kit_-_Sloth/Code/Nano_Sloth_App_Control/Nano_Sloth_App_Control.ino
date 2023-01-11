#include "VarSpeedServo.h"  //include the VarSpeedServo library
#include "ws.h"       
#include "servos_control.h"
#include "ultrasonic.h"
#include "voice_control.h"

#define VERSION "1.0.2"

#define TEST 0
#define PRINT_WS_RECV 1 // 1, enable print WS receive
#define VOICE_CONTROL_STEP 3
uint8_t voice_step = VOICE_CONTROL_STEP;
extern int speed;
extern int delay_step;

bool autonomous_mode = false;

#define AVOID_DISTANCE 10
#define FOLLOW_DISTANCE 20
#define KEEP_DISTANCE_DISTANCE 5
bool last_obstacle_state = false;

uint8_t fall_left_or_right = 0; // 0:left, 1:right
/**
* Define built-in LED pin
*/
#define BUILT_IN_LED 13
 
/** Configure Wifi mode, ssid, password aand ws port*/
#define WIFI_MODE WIFI_MODE_STA
#define SSID "Your SSID"
#define PASSWORD "Your password"
#define PORT "8765"


// WS ws = WS(NAME, TYPE);
WS ws = WS();

float distance = 0;

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

bool key_E = false;
bool key_F = false;
bool key_G = false;

bool key_M = false;
bool key_N = false;
bool key_O = false;
bool key_P = false;
bool key_Q = false;
bool key_R = false;
bool key_S = false;
bool key_T = false;

char* voice_text_buffer[20];
int8_t voice_current_action = -1;


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
    ws.begin(SSID, PASSWORD, WIFI_MODE, PORT);
    ws.setOnReceived(onReceive);
#endif
}

void loop()
{

#if TEST == 0
    ws.loop();
    distance = ultrasonic_read();
#else
    delay(50);
    // distance = ultrasonic_read();
    // Serial.print("distance: "); Serial.println(distance);
    // delay(100);

    // servo_move(0, 0);
    // servo_move(1, 0);
    // servo_move(2, 0);
    // servo_move(3, 0);

    // forward();
    // backward();
    // turn_left();
    // turn_right();
    // stand();
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

void avoid() {
    // if (distance < 2) {
    //     last_obstacle_state = true;
    //     stand();
    // } else 
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


/**
 * websocket received data processing
 */
void onReceive() {
    #if PRINT_WS_RECV == 1
        Serial.print("onRecv:");
        serializeJson(ws.recv_doc, Serial);
        Serial.print("\n");
    #endif

    /*-------- data to display --------*/
    ws.send_doc["I"] = distance;

    /*-------- remote control --------*/
    // speed
    // int key_A = ws.recv_doc["A"];
    // if(key_A != NULL) {
    //     // speed = map(key_A, 0, 100, 0, 255);
    //     speed = key_A;
    // }
    // // delay_step
    // int key_B = ws.recv_doc["B"];
    // if(key_B != NULL) {
    //     delay_step = key_B;
    // }

    // ---  need to run block (would return) ---

#if 1   // autonomous mode: "obstacle avoid" or "obstacle follow" or "keep diatance"
     
    if (!ws.recv_doc["E"].isNull()) key_E = ws.recv_doc["E"];
    if (!ws.recv_doc["F"].isNull()) key_F = ws.recv_doc["F"];
    if (!ws.recv_doc["G"].isNull()) key_G = ws.recv_doc["G"];

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
#endif

    // ---  need to run non-blocking (would not return) ---
    // movement
    const char* key_K = ws.recv_doc["K"];
    if(key_K != NULL) {
        if (strcmp(key_K, "forward") == 0) {
            forward();
        } else if (strcmp(key_K, "backward") == 0) {
            backward();
        } else if (strcmp(key_K, "left") == 0) {
            turn_left();
        } else if (strcmp(key_K, "right") == 0) {
            turn_right();
        } else {
            stop();
        }
    }

    // ---------------- actions ----------------
    if(!ws.recv_doc["M"].isNull()) key_M = ws.recv_doc["M"];
    if(!ws.recv_doc["N"].isNull()) key_N = ws.recv_doc["N"];
    if(!ws.recv_doc["O"].isNull()) key_O = ws.recv_doc["O"];
    if(!ws.recv_doc["P"].isNull()) key_P = ws.recv_doc["P"];
    if(!ws.recv_doc["Q"].isNull()) key_Q = ws.recv_doc["Q"];
    if(!ws.recv_doc["R"].isNull()) key_R = ws.recv_doc["R"];
    if(!ws.recv_doc["S"].isNull()) key_S = ws.recv_doc["S"];
    if(!ws.recv_doc["T"].isNull()) key_T = ws.recv_doc["T"];

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
    }

    if(last_action != current_action) {
        action_step_reset();
        last_action = current_action;
        if(current_action == ACTION_FALL) {
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


    Serial.print("current_action");Serial.println(current_action);
    if(current_action != ACTION_NONE) {
        voice_current_action = -1;
        return;
    }

    // voice control
    // const char* key_J = ws.recv_doc["J"];
    // int8_t code = -1;
    // if (key_J != NULL) {
    //     code  = text_2_cmd_code(key_J);
    // }

    bool key_J = ws.recv_doc["J"];
    int8_t code = -1;
    if (key_J) {
        code  = 1;
        Serial.println("");
    }

    if(code != -1) {
        voice_current_action = code;
        voice_step = VOICE_CONTROL_STEP;
        cmd_fuc_table[voice_current_action]();
        voice_step --;
    } else {
        if (voice_current_action != -1 && voice_step > 0) {
            cmd_fuc_table[voice_current_action]();
            voice_step --;
        }
    }

}

