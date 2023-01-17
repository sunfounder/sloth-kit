#ifndef WS_H
#define WS_H

#include <Arduino.h>
#include <ArduinoJson.h>

/* Enable debug */
#define WS_DEBUG 0

/* Enable Serial data length check*/
#define LEN_CHECK 1

/* Define built-in LED pin */
#define BUILT_IN_LED 13

/* Use custom serial port */
#define CUSTOM_SERIAL 0
#if(CUSTOM_SERIAL == 1)
  #include <SoftwareSerial.h>
  SoftwareSerial dSerial(10, 11); // RX, TX
  #define DateSerial dSerial
  #define DebugSerial Serial
#else
  #define DateSerial Serial
  #define DebugSerial Serial
#endif

#if WS_DEBUG == 1
  #define ws_debug DebugSerial.print
  #define ws_debugln DebugSerial.println
#else
  #define ws_debug(...) ;
  #define ws_debugln(...) ;
#endif

 /* Set SERIAL_TIMEOUT & WS_BUFFER_SIZE */
#define SERIAL_TIMEOUT 100
#define WS_BUFFER_SIZE 200

/* Some keywords for communication with ESP8266 */
/** Configure product name and type*/
#define NAME "Nano Sloth"
#define TYPE "Nano Sloth"
#define CHECK "SC"
#define OK_FLAG "[OK]"
#define WS_HEADER "WS+"

#define WIFI_MODE_NONE "0"
#define WIFI_MODE_STA  "1"
#define WIFI_MODE_AP   "2"

#define DPAD_STOP     0
#define DPAD_FORWARD  1
#define DPAD_BACKWARD 2
#define DPAD_LEFT     3
#define DPAD_RIGHT    4


#define JOYSTICK_X       0
#define JOYSTICK_Y       1
#define JOYSTICK_ANGLE   2
#define JOYSTICK_RADIUS  3

#define REGION_LEN 0
#define REGION_A 1
#define REGION_B 2
#define REGION_C 3
#define REGION_D 4
#define REGION_E 5
#define REGION_F 6
#define REGION_G 7
#define REGION_H 8
#define REGION_I 9
#define REGION_J 10
#define REGION_K 11
#define REGION_L 12
#define REGION_M 13
#define REGION_N 14
#define REGION_O 15
#define REGION_P 16
#define REGION_Q 17
#define REGION_R 18
#define REGION_S 19
#define REGION_T 20
#define REGION_U 21
#define REGION_V 22
#define REGION_W 23
#define REGION_X 24
#define REGION_Y 25
#define REGION_Z 26

class WS {
    public:
        char readBuffer[WS_BUFFER_SIZE];
        char recvBuffer[WS_BUFFER_SIZE];
        StaticJsonDocument<200> send_doc;

        void begin(
            const char* ssid, 
            const char* password, 
            const char* wifi_mode,
            const char* ws_port="8765"
        );
        // void setOnReceived(void (*func)(char*, char*));
        void setOnReceived(void (*func)());
        void loop();
        void readInto(char* buffer);
        void sendData(char* buf);
        void sendData();

        void set(const char* command);
        void set(const char* command, const char* value);
        void get(const char* command, char* result);
        void get(const char* command, const char* value, char* result);

        int16_t getSlider(uint8_t region);
        bool getButton(uint8_t region);
        bool getSwitch(uint8_t region);
        bool getMusicSwitch(uint8_t region);
        int16_t getJoystick(uint8_t region, uint8_t axis);
        uint8_t getDPad(uint8_t region);
        int16_t getThrottle(uint8_t region);
        void getGreyscale(uint8_t region, char* result);
        void getSpeech(uint8_t region, char* result);

    private:
        void command(const char* command, const char* value, char* result);
        void subString(char* str, int16_t start, int16_t end=-1);

        void getStrOf(char* str, uint8_t index, char* result, char divider);
        int16_t getIntOf(char* str, uint8_t index, char divider=';');
        bool getBoolOf(char* str, uint8_t index);
        double getDoubleOf(char* str, uint8_t index);
};


#endif
