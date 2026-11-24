#include <Arduino.h>
#define IR_USE_AVR_TIMER1 
//This forces the IR receiver to use timer 1, as the motors uses timer 2.

#include <IRremote.hpp>
#include "motor.h"

volatile bool emergencyStop = false; //ensure it is set to false immedaitely.
volatile bool isRepeat = false;

void setup() {
  Serial.begin(9600);

  initMotors();
  motorHalt(LMOTOR);
  motorHalt(RMOTOR);

  IrReceiver.begin(2, ENABLE_LED_FEEDBACK); // Pin 2 is especially important since we are using timer 1 on the arduino mega.
  resetPID();
  Serial.println("STARTING");
}

void loop() {
  if (IrReceiver.decode()) { //Decodes the signal
    uint8_t command = IrReceiver.decodedIRData.command; //makes the decoded signal the command
    bool isRepeat = IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT; // Checks if the command is a repeat
    IrReceiver.printIRResultShort(&Serial); //Prints the data received
    IrReceiver.resume(); //Allows to check for the next signal once the arduino loops

    if (isRepeat == false) { 

      // EMERGENCY STOP: Button 5
      if (command == 0x1C) {
        emergencyStop = true; // By setting this to true, the function that is in progress gets a false, which stops it.
        motorHalt(LMOTOR);
        motorHalt(RMOTOR);
        Serial.println("EMERGENCY STOP!");
        emergencyStop = false;
        Serial.println("restarting");
        IrReceiver.resume(); // Resume receiving IR signals after resetting the emergency stop
      }

      // MOVEMENT COMMANDS
      else if (emergencyStop == false) { // If no emergency stop, then proceed as normal.

        // FORWARD: Button 2
        if (command == 0x18) {
          Serial.println("Going Forward");
          driveDistance(200);
        }

        // BACKWARD: Button 8
        else if (command == 0x52) {
          Serial.println("Going Backward");
          driveDistance(-200);
        }

        // RIGHT: Button 6
        else if (command == 0x5A) {
          Serial.println("Rotating Right");
          rotateCCW(-30);
        }

        // LEFT: Button 4
        else if (command == 0x08) {
          Serial.println("Rotating Left");
          rotateCCW(30);
        }
      }
    }
  }
}
