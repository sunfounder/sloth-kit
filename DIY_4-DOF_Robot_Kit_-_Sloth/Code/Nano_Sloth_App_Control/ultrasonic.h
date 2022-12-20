#ifndef _ULTRASONIC_H
#define _ULTRASONIC_H

#include <NewPing.h>        //include the NewPing library

#define TRIGGER_PIN  4  // Arduino pin 4 tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     3  // Arduino pin 3 tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

#define US_ROUNDTRIP_CM 57

int ultrasonic_read();

#endif
