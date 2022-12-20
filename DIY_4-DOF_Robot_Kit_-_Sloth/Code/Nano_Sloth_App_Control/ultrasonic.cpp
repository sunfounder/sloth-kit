#include "ultrasonic.h"

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

int ultrasonic_read()
{
    return sonar.ping() / US_ROUNDTRIP_CM;
}
