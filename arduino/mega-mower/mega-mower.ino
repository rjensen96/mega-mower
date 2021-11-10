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
  Serial.begin(115200);
  Serial.println("hey I made it to setup");
  
  // Set the spinning direction CW/CCW:
//  digitalWrite(dirPin, HIGH);

  // send pulse forward
  for(int i = 0; i < 400; i++) {
    delay(10);
    sendPulse(true);
  }
  
  delay(1000);
  
  // rotate back
  for(int i = 0; i < 400; i++) {
    delay(10);
    sendPulse(false);
  }
}

void sendPulse(bool forward) {
  digitalWrite(dirPin, forward ? 1 : 0);
  
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(600);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(600); 
}

void loop() {
  Serial.println("We're here in the main loop!");

  /*            ^
   *    o     < X >   <- arrow keypad controls DIRECTION LEFT/RIGHT
   *    |       V
   *  Joystick: controls MAGNITUDE of speed and FORWARD/BACK.
   *  
   */

   /*
    * EACH FRAME IN HERE:
    * Determine forward/back direction (or zero) -> set pulse rate
    * Determine right/left direction -> set # of pulses to send each motor
    * If turning LEFT, send RIGHT [5?] and LEFT[1?] ... some ratio.
    * Opposite for turning RIGHT.
    */
  
  delay(4000);
}
