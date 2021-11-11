#pragma once

// Define stepper motor connections:
#define dirPin 19
#define stepPin 23

// in our rig, RED = 19; ORANGE = 23; BLACK = GND; BROWN = IGNORE;
// side of stepper driver switches:
// ON: 1, 2, 4, 6.

void sendPulse(bool isForward) {
  digitalWrite(dirPin, isForward ? 1 : 0);
  
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(600);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(600); 
}

void testDrive(bool isForward) {
  for(int i = 0; i < 400; i++) {
    delay(10);
    sendPulse(isForward);
  }
}
