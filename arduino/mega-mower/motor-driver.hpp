#pragma once

// Define stepper motor connections:
#define rightDirPin 19
#define rightStepPin 23

#define leftDirPin 33
#define leftStepPin 34

// in our rig, RED = 19; ORANGE = 23; BLACK = GND; BROWN = IGNORE;
// side of stepper driver switches:
// ON: 1, 2, 4, 6.

void sendPulse(bool isForward, bool isLeft) {
  if(isLeft){ 
    digitalWrite(leftDirPin, isForward ? 1 : 0);
    digitalWrite(leftStepPin, HIGH);
    delayMicroseconds(600);
    digitalWrite(leftStepPin, LOW);
    delayMicroseconds(600); 
  } else {
    digitalWrite(rightDirPin, isForward ? 1 : 0);
    digitalWrite(rightStepPin, HIGH);
    delayMicroseconds(600);
    digitalWrite(rightStepPin, LOW);
    delayMicroseconds(600); 
  }
}

void testDrive(bool isForward, bool isLeft) {
  for(int i = 0; i < 2; i++) {
    delay(4);
    sendPulse(isForward, isLeft);
  }
}

/**
 * @param float leftPower: -1.0 ... 1.0 indicating power magnitude
 * @param float rightPower: -1.0 ... 1.0 indicating power magnitude
 */
void driveAtAngle(uint8_t leftPower, uint8_t rightPower, bool isForward){

  // use ratio to determine how often to send pulses to the RIGHT motor, since ratio is left to right
  // if ratio == 1, then right and left will always get equal pulses.

  // todo: below does not actually balance with the ratio.
  // well, it might if the ratio is 2:1, but don't count on it.
  while(leftPower + rightPower > 0) {
      
      if(leftPower > 0) {
        testDrive(isForward, true);
        leftPower--;
      }

      if(rightPower > 0) {
        testDrive(isForward, false);
        rightPower--;
      }
  }
}

void handleDriveCommand(uint8_t driveAngle){
  switch(driveAngle){
    case 1: 
      // 1 left, 0 right
      driveAtAngle(2, 0, true);
      break;
    case 2:
      // 1 left, 0.5 right
      driveAtAngle(2, 1, true);
      break;
    case 3: 
      // 1 left, 1 right
      driveAtAngle(2, 2, true);
      break;
    case 4: 
      // 0.5 left, 1 right
      driveAtAngle(1, 2, true);
      break;
    case 5:
      // 0 left, 1 right
      driveAtAngle(0, 2, true);
      break;
    case 6:
      // -0.5 left, -1 right
      driveAtAngle(1, 2, false);
      break;
    case 7:
      // -1 left, -1 right
      driveAtAngle(2, 2, false);
      break;
    case 8:
      // -1 left, -0.5 right
      driveAtAngle(2, 1, false);
      break;
    default:
      break;
  }
}
