#include "ws.h"
#include "string.h"

char recvBuffer[WS_BUFFER_SIZE + strlen(WS_HEADER)];

/**
*  functions for manipulating string 
*/
#define IsStartWith(str, prefix) (strncmp(str, prefix, strlen(prefix)) == 0)
#define StrAppend(str, suffix) uint32_t len=strlen(str); str[len] = suffix; str[len+1] = '\0'
#define StrClear(str) str[0] = 0

/**
 * @brief instantiate WS Class, set name and type
 * @param name set name
 * @param type set type
 */
// WS::WS(const char* name, const char* type) {
    
//     send_doc["Name"] = name;
//     send_doc["Type"] = type;
//     send_doc["Check"] = CHECK;
// }

/**
 * @brief Set wifi and websocket port to esp32-cam, 
 *        block and wait for the setting to succeed
 *        
 * @param ssid  wifi ssid
 * @param password wifi password
 * @param wifiMode  0,None; 1, STA; 2, AP
 * @param wsPort websocket server port
 */
void WS::begin(const char* ssid, const char* password, const char* wifi_mode, const char* ws_port) {
    #if(CUSTOM_SERIAL == 1)
        DateSerial.begin(115200);
    #endif

    char ip[15];

    Serial.println("reset ESP8266 module ...");
    this->set("RESET");

    if(send_doc["Name"] == NULL || send_doc["Type"] == NULL) {
        Serial.println("Please set the Name and TYPE at first.");
        while (1);
    }

    // this->set("NAME", send_doc["Name"]);
    // this->set("TYPE", send_doc["Type"]);
    this->set("SSID", ssid);
    this->set("PSK",  password);
    this->set("MODE", wifi_mode);
    this->set("PORT", ws_port);

    this->get("START", ip);

    Serial.print("WebServer started on ws://");
    Serial.print(ip);
    Serial.print(":");
    Serial.println(ws_port);

}

/**
* Declare the receive callback function
*/
// void (*__on_receive__)(char*, char*);
void (*__on_receive__)();


/**
 * @brief Set callback function method
 *         
 * @param func  callback function pointer
 */
// void WS::setOnReceived(void (*func)(char*, char*)) { __on_receive__ = func; }
void WS::setOnReceived(void (*func)()) { __on_receive__ = func; }


/**
 * @brief Receive and process serial port data in a loop
 */

void WS::loop() {
    StrClear(recvBuffer);
    this->readInto(recvBuffer);
    
    if (strlen(recvBuffer) != 0) {
        if (__on_receive__ != NULL) {
            deserializeJson(recv_doc, recvBuffer);
            // __on_receive__(recvBuffer, sendBuffer);
            __on_receive__();

        }
        this->sendData();
    }
}

/** 
 * @brief Store the data read from the serial port into the buffer
 *       
 * @param buffer  Pointer to the character value of the stored data
 */
void WS::readInto(char* buffer) {
    bool finished = false;
    char incomingChar;
    StrClear(buffer);
    uint32_t count = 0;
    
    while (DateSerial.available()) {
        count += 1;
        if (count > WS_BUFFER_SIZE) {
            finished = true;
            break;
        }

        incomingChar = (char)DateSerial.read();

        if (incomingChar == '\n') {
            finished = true;
            #if DEBUG == 1
                Serial.print("recv: ");
                Serial.println(buffer);
            #endif
            break;
        } else if (incomingChar == '\r') {
            // Serial.println();
            continue;
        } else if (incomingChar == '{') {
            StrClear(buffer);
            StrAppend(buffer, '{');
        } else if ((int)incomingChar > 31 && (int)incomingChar < 127) {
            StrAppend(buffer, incomingChar);
            delayMicroseconds(100); // This delay is necessary, wait for operation complete 
        }
    }
}


/** 
 * @brief Send command to ESP32-CAM with serial
 *         
 * @param command command keyword
 * @param value
 * @param result returned information from serial
 */
void WS::command(const char* command, const char* value, char* result) {
    bool is_ok = false;

    DateSerial.print("SET+");
    DateSerial.print(command);
    DateSerial.print(value);
    DateSerial.print("\n");

    while(1) {
        this->readInto(result);
        if (IsStartWith(result, OK_FLAG)){
            is_ok = true;
            Serial.println("OK");
            this->subString(result, strlen(OK_FLAG) + 1); // Add 1 for Space
            return;
        }
    }
}

/** 
 * @brief Use the comand() function to set up the ESP32-CAM
 * 
 * @param command command keyword
 */
void WS::set(const char* command) {
    char result[10];
    this->command(command, "", result);
}

/** 
 * @brief Use the comand() function to set up the ESP32-CAM
 * 
 * @param command command keyword
 * @param value
 * 
 * @code {.cpp}
 * set("NAME", "Zeus_Car");
 * set("TYPE", "Zeus_Car");
 * set("SSID", "Zeus_Car");
 * set("PSK",  "12345678");
 * set("MODE", WIFI_MODE_AP);
 * set("PORT", "8765");
 * @endcode
 * 
 */
void WS::set(const char* command, const char* value) {
    char result[10];
    this->command(command, value, result);
}


/** 
 * @brief Use the comand() function to set up the ESP32-CAM,
 *        and receive return information
 * 
 * @param command command keyword
 * @param value
 * @param result returned information from serial
 * @code {.cpp}
 * char ip[15];
 * get("START", ip);
 * @endcode
 */
void WS::get(const char* command, char* result) {
  this->command(command, "", result);
}

/** 
 * @brief Use the comand() function to set up the ESP32-CAM,
 *        and receive return information
 * 
 * @param command command keyword
 * @param value
 * @param result returned information from serial
 */
void WS::get(const char* command, const char* value, char* result) {
  this->command(command, value, result);
}

/** 
 * @brief Serial port sends data, automatically adds header (WS_HEADER)
 *         
 * @param sendBuffer  Pointer to the character value of the data buffer to be sent
 */
void WS::sendData() {
    //send static content
    DateSerial.print(F(WS_HEADER));
    DateSerial.print(F(
        "{"
        "\"Name\": \"" NAME "\","
        "\"Type\": \"" TYPE "\","
        "\"Check\": \"" CHECK "\""
        "}"
    )); 
    DateSerial.print("\n");    

    //send dynamic content
    DateSerial.print(F(WS_HEADER));
    serializeJson(send_doc, DateSerial);
    DateSerial.print("\n");    
}

/** 
 * @brief subtract part of the string
 *         
 * @param buf string pointer to be subtract  
 * @param start start position of content to be subtracted
 * @param end end position of Content to be subtracted
 */
void WS::subString(char* str, int16_t start, int16_t end) {
  uint8_t length = strlen(str);
  if (end == -1) {
    end = length;
  }
  for (uint8_t i = 0; i < end; i++) {
    if (i + start < end) {
      str[i] = str[i + start];
    } else {
      str[i] = '\0';
    }
  }
}
