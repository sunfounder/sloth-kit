#include "ws.h"
#include "string.h"

/**
*  functions for manipulating string 
*/
#define IsStartWith(str, prefix) (strncmp(str, prefix, strlen(prefix)) == 0)
#define StrAppend(str, suffix) uint32_t len=strlen(str); str[len] = suffix; str[len+1] = '\0';
#define StrClear(str) str[0] = 0

/**
 * @brief Set wifi and websocket port to esp32-cam, 
 *        block and wait for the setting to succeed
 *        
 * @param ssid  wifi ssid
 * @param password wifi password
 * @param wifiMode  0,None; 1, STA; 2, AP
 * @param wsPort websocket server port
 * 
 */
void WS::begin(
    const char* ssid,
    const char* password, 
    const char* wifi_mode, 
    const char* ws_port,
    const char* name,
    const char* type) 
{
    #if(CUSTOM_SERIAL == 1)
        DateSerial.begin(115200);
    #endif

    char ip[15];

    Serial.println("reset ESP8266 module ...");
    this->set("RESET");

    this->set("SSID", ssid);
    this->set("PSK",  password);
    this->set("MODE", wifi_mode);
    this->set("PORT", ws_port);
    this->set("SMD", "1");  // 0:send original text; 1:send simplified text
    this->set("NAME", name);
    this->set("TYPE", type);

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
    StrClear(readBuffer);
    this->readInto(readBuffer);
    int len = strlen(readBuffer);
    if (len != 0) {
        ws_debug("readBuffer: ");ws_debugln(readBuffer);
        if (IsStartWith(readBuffer, WS_HEADER) and __on_receive__ != NULL) {
            this->subString(readBuffer, strlen(WS_HEADER));
            int lenCheck = getIntOf(readBuffer, REGION_LEN);
            ws_debug("len:");ws_debugln(len);
            ws_debug("lenCheck:");ws_debugln(lenCheck);
            if (lenCheck == len) {
                StrClear(recvBuffer);
                strcpy(recvBuffer, readBuffer);
                __on_receive__();
                this->sendData();
            } 
        }
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
        if (count > WS_BUFFER_SIZE) {
            finished = true;
            break;
        }

        incomingChar = (char)DateSerial.read();

        if (incomingChar == '\n') {
            finished = true;
            buffer[count] = '\0';
            delayMicroseconds(200);
            ws_debug("recv: ");
            ws_debugln(buffer);
            break;
        } else if (incomingChar == '\r') {
            continue;
        } else if ((int)incomingChar > 31 && (int)incomingChar < 127) {
            // StrAppend(buffer, incomingChar);
            // ws_debugln(incomingChar);
            buffer[count] = incomingChar;
            delayMicroseconds(100);
            count ++;
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
        delay(20); // Wait for StrAppend() complete
    }
}

/** 
 * @brief Use the comand() function to set up the ESP32-CAM
 * 
 * @param command command keyword
 */
void WS::set(const char* command) {
    char result[64];
    this->command(command, "", result);
}

/** 
 * @brief Use the comand() function to set up the ESP32-CAM
 * 
 * @param command command keyword
 * @param value
 * 
 * @code {.cpp}
 * set("NAME", "Nano_Sloth");
 * set("TYPE", "Nano_Sloth");
 * set("SSID", "Nano_Sloth");
 * set("PSK",  "12345678");
 * set("MODE", WIFI_MODE_AP);
 * set("PORT", "8765");
 * @endcode
 * 
 */
void WS::set(const char* command, const char* value) {
    char result[64];
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
void WS::sendData(char* sendBuffer) {
  DateSerial.print(F(WS_HEADER));
  DateSerial.println(sendBuffer);
}

/** 
 * @brief Serial port sends data, automatically adds header (WS_HEADER)
 *         
 * @param sendBuffer  Pointer to the character value of the data buffer to be sent
 */
void WS::sendData() {
    //send static content
    // DateSerial.print(F(WS_HEADER));
    // DateSerial.print(F(
    //     "{"
    //     "\"Name\": \"" NAME "\","
    //     "\"Type\": \"" TYPE "\","
    //     "\"Check\": \"" CHECK "\""
    //     "}"
    // )); 
    // DateSerial.print("\n");

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

/* -------------------------------  get key value ---------------------------------- */

/** 
 * @brief Split the string by a cdivider,
 *         and return characters of the selected index
 *
 * @param str string pointer to be split  
 * @param index which index do you wish to return
 * @param result char array pointer to hold the result
 * @param divider
 */
void WS::getStrOf(char* str, uint8_t index, char* result, char divider) {
    uint8_t start, end;
    uint8_t length = strlen(str);
    uint8_t i, j;
    // Get start index
    if (index == 0) {
        start = 0;
    } else {
        for (start = 0, j = 1; start < length; start++) {
            if (str[start] == divider) {
                if (index == j) {
                start++;
                break;
                }
                j++;
            }
        }
    }
    // Get end index
    for (end = start, j = 0; end < length; end++) {
        if (str[end] == divider) {
            break;
        }
    }
    // Copy result
    for (i = start, j = 0; i < end; i++, j++) {
        result[j] = str[i];
    }
    result[j] = '\0';
}

/** 
 * @brief Split the string by a cdivider,
 *         and return characters of the selected index.
 *         Further, the content is converted to int type.
 *
 * @param buf string pointer to be split  
 * @param index which index do you wish to return
 * @param divider
 * 
 * @return int16_t, int value
 */
int16_t WS::getIntOf(char* buf, uint8_t index, char divider=';') {
    int16_t result;
    char strResult[6];
    getStrOf(buf, index, strResult, divider);
    result = String(strResult).toInt();
    return result;   
}

bool WS::getBoolOf(char* str, uint8_t index) {
    char strResult[5];
    getStrOf(str, index, strResult, ';');
    return String(strResult).toInt(); 
}

double WS::getDoubleOf(char* str, uint8_t index) {
  double result;
  char strResult[20];
  getStrOf(str, index, strResult, ';');
  result = String(strResult).toDouble();
  return result;   
}

int16_t WS::getSlider(uint8_t region) {
  int16_t value = getIntOf(recvBuffer, region);
  return value;
}

bool WS::getButton(uint8_t region) {
  bool value = getBoolOf(recvBuffer, region);
  return value;
}

bool WS::getSwitch(uint8_t region) {
  bool value = getBoolOf(recvBuffer, region);
  return value;
}

bool WS::getMusicSwitch(uint8_t region) {
  bool value = getBoolOf(recvBuffer, region);
  return value;
}

int16_t WS::getJoystick(uint8_t region, uint8_t axis) {
  char valueStr[10];
  char xStr[4];
  char yStr[4];
  int16_t x, y, angle, radius;
  getStrOf(recvBuffer, region, valueStr, ';');
  x = getIntOf(valueStr, 0, ',');
  y = getIntOf(valueStr, 1, ',');
  angle = atan2(x, y) * 180.0 / PI;
  radius = sqrt(y * y + x * x);
  switch (axis) {
    case JOYSTICK_X: return x;
    case JOYSTICK_Y: return y;
    case JOYSTICK_ANGLE: return angle;
    case JOYSTICK_RADIUS: return radius;
    default: return 0;
  }
}

uint8_t WS::getDPad(uint8_t region) {
  char value[10];
  getStrOf(recvBuffer, region, value, ';');
  uint8_t result;
  if ((String)value == (String)"forward") result = DPAD_FORWARD;
  else if ((String)value == (String)"backward") result = DPAD_BACKWARD;
  else if ((String)value == (String)"left") result = DPAD_LEFT;
  else if ((String)value == (String)"right") result = DPAD_RIGHT;
  else if ((String)value == (String)"stop") result = DPAD_STOP;
  return result;
}

int16_t WS::getThrottle(uint8_t region) {
  int16_t value = getIntOf(recvBuffer, region);
  return value;
}

void WS::getGreyscale(uint8_t region, char* result);

void WS::getSpeech(uint8_t region, char* result) {
  getStrOf(recvBuffer, region, result, ';');

}


