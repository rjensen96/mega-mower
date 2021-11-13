#include "wsconfig.hpp"
#include "motor-driver.hpp"

/* Example sketch to control a stepper motor with TB6600 stepper motor driver and Arduino without a library: continuous rotation. More info: https://www.makerguides.com */

// Define stepper motor connections:
// #define dirPin 19
// #define stepPin 23

// WiFi Credentials are in wsconfig.hpp

// in our rig, RED = 19; ORANGE = 23; BLACK = GND; BROWN = IGNORE;
// side of stepper driver switches:
// ON: 1, 2, 4, 6.

void connectToWifi() {
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");
  Serial.println(WiFi.localIP());
}

void setup() {
  // Declare pins as output:
  pinMode(rightStepPin, OUTPUT);
  pinMode(rightDirPin, OUTPUT);
  Serial.begin(115200);

  connectToWifi();
  initWebSocket();
}

uint8_t counter = 0;

void loop() {
  driveTick(counter); // counter is how to determine ratio of steps for left/right
  
  // clamp counter to low values to fit in uint8_t
  if(counter == 250){
    counter = 1;
  } else {
    counter++;
  }
  
  delay(0); // this is smoothest.
  
  // idea for joystick movement: (not in use)
  /*            ^
   *    o     < X >   <- arrow keypad controls DIRECTION LEFT/RIGHT
   *    |       V
   *  Joystick: controls MAGNITUDE of speed and FORWARD/BACK.
   *  
   */

//  ws.cleanupClients(); // this might be necessary?
}
