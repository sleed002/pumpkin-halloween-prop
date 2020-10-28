#define CANDLELED 11
#define REDLED 8
#define BUZZER 5
#define PROXIMITY_THRESHOLD 20
#define PROXIMITY_CONSECUTIVE_READINGS 1
#define BUZZER_FREQUENCY 38
#define FLICKER_INTERVAL 25
#include <Servo.h> 
 
int servoPin = 9;
int angle = 0;   // servo position in degrees 

long previousMillis = 0;
long closeReadings = 0;

int trigPin = 4;    // Trigger
int echoPin = 2;    // Echo
long duration, cm, inches;

bool test; 

Servo servo;  

void setup() {
  pinMode(CANDLELED, OUTPUT);
  pinMode(REDLED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin (9600);
  delay(1000); // Allow the proximity sensor to initialize
  servo.attach(servoPin);
}
void loop() {
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
 
  // Convert the time into a distance
  cm = (duration/2) / 29.1;     // Divide by 29.1 or multiply by 0.0343
  inches = (duration/2) / 74;   // Divide by 74 or multiply by 0.0135
  
  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  
  delay(250);
  if (inches < PROXIMITY_THRESHOLD) // Is someone close?
  {
    closeReadings++;
    if (closeReadings >= PROXIMITY_CONSECUTIVE_READINGS) // require n consecutive "close" readings before going into EVIL mode. This prevents little blips of the buzzer.
      {
        digitalWrite(CANDLELED, LOW);
        digitalWrite(REDLED, HIGH);
        tone(BUZZER, BUZZER_FREQUENCY);
        { 
          // scan from 0 to 75 degrees
          for(angle = 0; angle < 75; angle++)  
          {                                  
            servo.write(angle);
            delay(20);                              
          } 
          // now scan back from 75 to 0 degrees
          for(angle = 75; angle > 0; angle--)    
          {                                
            servo.write(angle);
            delay(20);                 
          } 
        } 
      }
  }
  else
  { //regular candle flicker. Based on Arduino example BlinkWithoutDelay.
    noTone(BUZZER);
    digitalWrite(REDLED, LOW);
    closeReadings = 0;
    unsigned long currentMillis = millis();
    if(currentMillis - previousMillis > FLICKER_INTERVAL)
    {
      previousMillis = currentMillis;
      analogWrite(CANDLELED, random(0, 256));
    }
  }
}
