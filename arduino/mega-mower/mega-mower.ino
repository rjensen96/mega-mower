/* Example sketch to control a stepper motor with TB6600 stepper motor driver and Arduino without a library: continuous rotation. More info: https://www.makerguides.com */

// Define stepper motor connections:
#define dirPin 19
#define stepPin 23

// in our rig, RED = 19; ORANGE = 23; BLACK = GND; BROWN = IGNORE;
// side of stepper driver switches:
// ON: 1, 2, 4, 6.

void setup() {
  // Declare pins as output:
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  Serial.println("hey I made it to setup");
  
  // Set the spinning direction CW/CCW:
//  digitalWrite(dirPin, HIGH);

  // send pulse forward
  for(int i = 0; i < 800; i++) {
    delay(10);
    sendPulse(true);
  }
  
  delay(1000);
  
  // rotate back
  for(int i = 0; i < 800; i++) {
    delay(10);
    sendPulse(false);
  }
}

void sendPulse(bool forward) {
  digitalWrite(dirPin, forward ? 1 : 0);
  
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(800);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(800); 
}

void loop() {
  
}
