#ifndef _DEBUG_H
#define _DEBUG_H

#include "Arduino.h"

/* Set the Debug Level */
#define DEBUG_LEVEL CAM_DEBUG_LEVEL_DEBUG
#define CAM_DEBUG_LEVEL_OFF 0
#define CAM_DEBUG_LEVEL_ERROR 1
#define CAM_DEBUG_LEVEL_INFO 2
#define CAM_DEBUG_LEVEL_DEBUG 3
#define CAM_DEBUG_LEVEL_ALL 4


void debug(String msg);
void info(String msg);
void error(String msg);

void debug(String msg, String data);
void info(String msg, String data);
void error(String msg, String data);

#endif
