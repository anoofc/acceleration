#define DEBUG 1

#define PULSE_PIN 26
#define DIR_PIN   25

#include <Arduino.h>

uint16_t currentPosition = 0;

void movemotor(uint16_t newPosition) {
  if (DEBUG) { Serial.print("\t  New Position:" + String (newPosition));}
  int16_t steps = newPosition - currentPosition;
  if (DEBUG) { Serial.println("  \t Steps:" + String (steps));}
  currentPosition = newPosition;
  if (steps > 0) { digitalWrite(DIR_PIN, HIGH);} 
  else           { digitalWrite(DIR_PIN, LOW); }
  steps = abs(steps); 

  int acceleration = 100; // Number of steps to accelerate
  int deceleration = 100; // Number of steps to decelerate
  int maxSpeedDelay = 1; // Minimum delay between steps (max speed)
  int minSpeedDelay = 10; // Maximum delay between steps (min speed)

  for (int i = 0; i < steps; i++) {
    int delayTime = maxSpeedDelay;
    if (i < acceleration) {
      delayTime = minSpeedDelay - (i * (minSpeedDelay - maxSpeedDelay) / acceleration);
    } else if (i >= steps - deceleration) {
      delayTime = minSpeedDelay - ((steps - i - 1) * (minSpeedDelay - maxSpeedDelay) / deceleration);
    }
    digitalWrite(PULSE_PIN, HIGH);
    delay(delayTime);
    digitalWrite(PULSE_PIN, LOW);
    delay(delayTime);
  }
}

void readSerial() {
  if (Serial.available()) {
    String incoming = Serial.readStringUntil('\n');
    incoming.trim();
    if (incoming.startsWith("P")) {
      uint16_t pos = incoming.substring(1).toInt();
      if (DEBUG) { Serial.print("Current Position:" + String (currentPosition));}
      movemotor(pos);
    }  
  }
}



void motorTest(){
  digitalWrite(DIR_PIN, HIGH);
  for (int i = 0; i < 200; i++) {
    digitalWrite(PULSE_PIN, HIGH);
    delay(1);
    digitalWrite(PULSE_PIN, LOW);
    delay(1);
  }
  delay(1000);
  digitalWrite(DIR_PIN, LOW);
  for (int i = 0; i < 200; i++) {
    digitalWrite(PULSE_PIN, HIGH);
    delay(1);
    digitalWrite(PULSE_PIN, LOW);
    delay(1);
  }
  delay(1000);
}

void setup() {
  Serial.begin(9600);
  pinMode(PULSE_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
}

void loop() {
  readSerial();
}

