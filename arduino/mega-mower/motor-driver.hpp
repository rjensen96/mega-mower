#pragma once

// Define stepper motor connections:
#define rightDirPin 19
#define rightStepPin 23

#define leftDirPin 33
#define leftStepPin 34

// in our rig, RED = 19; ORANGE = 23; BLACK = GND; BROWN = IGNORE;
// side of stepper driver switches:
// ON: 1, 2, 4, 6.

// state variables for motor power
// 0 -> stopped
// 1 -> step every tick
// 2 -> step tick % 2 == 0 (every other tick)
// 3 -> step tick % 3 == 0
// . . .
uint8_t leftPower = 0;
uint8_t rightPower = 0;
bool IS_FORWARD = true; // todo: rename this to isForward after cleaning up the other functions here.

void setMotorState(uint8_t left, uint8_t right, bool directionIsForward) {
  leftPower = left;
  rightPower = right;
  IS_FORWARD = directionIsForward;
}

// implemented logic improvements to get this running more smoothly.
// https://forum.arduino.cc/t/2-stepper-motors-running-at-same-time-arduino-uno/91717

/*
 * Best way to do this is to have the WS messages just change state variables here.
 * The program loop then just sends whatever is in the state variables to the motors.
 * Before, the motor was really jerky because the motor was basically reading straight from the websocket.
 * 
 * The motor should just run smoothly based on whatever the last state update from the WebSocket says.
 * 
 * TODO: need to set state to stopped if we have gone too long without getting a websocket message.
 */

void sendPulse(bool isForward, bool isLeft) {
  if(isLeft){ 
    digitalWrite(leftDirPin, isForward ? 1 : 0);
    digitalWrite(leftStepPin, HIGH);
    delayMicroseconds(700);
    digitalWrite(leftStepPin, LOW);
    delayMicroseconds(700); 
  } else {
    digitalWrite(rightDirPin, isForward ? 1 : 0);
    digitalWrite(rightStepPin, HIGH);
    delayMicroseconds(700);
    digitalWrite(rightStepPin, LOW);
    delayMicroseconds(700); 
  }
}

void driveTick(uint8_t counter) {
  if(leftPower > 0 && counter % leftPower == 0) {
    sendPulse(IS_FORWARD, true);
  }

  if(rightPower > 0 && counter % rightPower == 0) {
    sendPulse(IS_FORWARD, false);
  }
}

void handleDriveCommand(uint8_t driveAngle){
  switch(driveAngle){
    case 0 :
      setMotorState(0, 0, true); // stopped
      break;
    case 1: 
      // FULL left, 0 right
      setMotorState(1, 0, true);
      break;
    case 2:
      // FULL left, 0.5 right
      setMotorState(1, 2, true);
      break;
    case 3: 
      // FULL left, FULL right
      setMotorState(1, 1, true);
      break;
    case 4: 
      // 0.5 left, FULL right
      setMotorState(2, 1, true);
      break;
    case 5:
      // 0 left, FULL right
      setMotorState(0, 1, true);
      break;
    case 6:
      // -0.5 left, -FULL right
      setMotorState(2, 1, false);
      break;
    case 7:
      // -FULL left, -FULL right
      setMotorState(1, 1, false);
      break;
    case 8:
      // -FULL left, -0.5 right
      setMotorState(1, 2, false);
      break;
    default:
      break;
  }
}
