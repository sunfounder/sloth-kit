#ifndef WS_H
#define WS_H

#include "Arduino.h"
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


class WS {
    public:
        StaticJsonDocument<200> recv_doc;
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
        // void sendData(char* buf);
        void sendData();

        void set(const char* command);
        void set(const char* command, const char* value);
        void get(const char* command, char* result);
        void get(const char* command, const char* value, char* result);

    private:
        void command(const char* command, const char* value, char* result);
        void subString(char* str, int16_t start, int16_t end=-1);
};


#endif
