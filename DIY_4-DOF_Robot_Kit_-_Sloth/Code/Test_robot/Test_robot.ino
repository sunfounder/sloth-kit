#include <Servo.h>
#include <NewPing.h>

Servo myservo1;
Servo myservo2;
Servo myservo3;
Servo myservo4;
int i = 0;

#define TRIGGER_PIN  4  // Arduino pin 4 tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     3  // Arduino pin 3 tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

#define SERVO
//#define ULTRASONIC

void setup()
{
  myservo1.attach(9);    // Connect the signal wire of servo to pin 9 
  myservo2.attach(10);    // Connect the signal wire of servo to pin 10 
  myservo3.attach(11);    // Connect the signal wire of servo to pin 11 
  myservo4.attach(12);    // Connect the signal wire of servo to pin 12 
  Serial.begin(115200);  // Open serial monitor at 115200 baud to see ping results.
}

void loop()
{
#ifdef ULTRASONIC
  delay(50);                      // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  unsigned int uS = sonar.ping(); // Send ping, get ping time in microseconds (uS).
  Serial.print("Ping: ");
  Serial.print(uS / US_ROUNDTRIP_CM); // Convert ping time to distance in cm and print result (0 = outside set distance range)
  Serial.println("cm");
#endif

#ifdef SERVO
  for ( i = 0; i < 180; i++ )
  {
    myservo1.write(i);
    myservo2.write(i);
    myservo3.write(i);
    myservo4.write(i);
    delay(20);
  }
  
  for ( i = 180; i > 0; i-- )
  {
    myservo1.write(i);
    myservo2.write(i);
    myservo3.write(i);
    myservo4.write(i);
    delay(20);
  }
#endif
}
