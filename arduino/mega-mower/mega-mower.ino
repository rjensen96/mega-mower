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
//  ws.cleanupClients(); // this might be necessary?
  delay(10000);
}
