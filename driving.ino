#include <Arduino.h>
#include <IRremote.hpp>

// The below function basically check if we got an emergency 
// stop signal every iteration of the function.
// If we did get an emergency stop, then itll 
// set emergency stop to true. This causes the loop to break.

bool checkEmergencyStop() {
  if (IrReceiver.decode()) { // Check if an IR signal was received
    uint8_t command = IrReceiver.decodedIRData.command; // Get the decoded IR command
    IrReceiver.resume(); // Resume receiving IR signals

    if (command == 0x1C) { // stop command if true
      emergencyStop = true;
      motorHalt(LMOTOR);
      motorHalt(RMOTOR);
      Serial.println("EMERGENCY STOP!");
    }
  }

  return emergencyStop;
}


// Wheel specifications
const float WHEEL_RADIUS = 4.0;
const float ENCODER_PPR = 1632.67;
const float PI_VAL = 3.14159265;

float wheelCircumference = 2 * PI_VAL * WHEEL_RADIUS;
float ticksPerUnitLength = ENCODER_PPR / wheelCircumference;


// Calculate motor speed based on distance traveled
int getSpeed(long progress, long remaining) {

  const int MIN_SPEED = 50;
  const int MAX_SPEED = 2000;

  const long RAMP_DISTANCE = 800;

  // Acceleration
  float accelFactor = (float)progress / RAMP_DISTANCE;

  if (accelFactor > 1.0) {
    accelFactor = 1.0;
  }

  // Deceleration
  float decelFactor = (float)remaining / RAMP_DISTANCE;

  if (decelFactor > 1.0) {
    decelFactor = 1.0;
  }

  // Use the smaller factor
  float factor = (accelFactor < decelFactor) ?
                 accelFactor : decelFactor;

  int speed = MIN_SPEED +
              (int)(factor * (MAX_SPEED - MIN_SPEED));

  return speed;
}


// Drive forward or backward a specified distance in cm
void driveDistance(int length) {
  resetPID();
  int dir = (length >= 0) ? 1 : -1;
  long target = abs(length) * ticksPerUnitLength;

  long startL = enc_l.read();
  long startR = enc_r.read();

  Serial.print("Target ticks: ");
  Serial.println(target);

  while (emergencyStop == false) {
    // The while statement makes sure to check if the emergency stop is triggered. It is more for redunenacy if anything.
    // The below if statement takes the bulk of the work. It is what actually triggers the stop.

    // Check for emergency stop
    if (checkEmergencyStop()) {
      break;
    }

    long L = abs(enc_l.read() - startL);
    long R = abs(enc_r.read() - startR);

    long progress = (L + R) / 2;
    long remaining = target - progress;

    if (remaining <= 0) {
      Serial.println("TARGET REACHED!");
      break;
    }

    int speed = getSpeed(progress, remaining);
    updatePID(dir * speed, dir * speed, false);
  }

  motorHalt(LMOTOR);
  motorHalt(RMOTOR);
}


// Rotate the robot by a specified angle
void rotateCCW(int angle) {
  resetPID();
  float wheelBase = 18.25;
  long target = PI_VAL * wheelBase * abs(angle) / 360.0 * ticksPerUnitLength;

  long startL = enc_l.read();
  long startR = enc_r.read();

  int dir = (angle >= 0) ? 1 : -1;

  unsigned long startTime = millis();
  const unsigned long TIMEOUT = 5000;

  while (emergencyStop == false) {
// The while statement makes sure to check if the emergency stop is triggered. It is more for redunenacy if anything.
    // The below if statement takes the bulk of the work. It is what actually triggers the stop.
    
    // Check for emergency stop
    if (checkEmergencyStop()) {
      break;
    }

    if (millis() - startTime >= TIMEOUT) {
      Serial.println("ROTATION TIMEOUT!");
      break;
    }

    long L = abs(enc_l.read() - startL);
    long R = abs(enc_r.read() - startR);

    long progress = (L + R) / 2;
    long remaining = target - progress;

    // Stop when target angle is reached
    if (remaining <= 0) {
      break;
    }

    int speed = getSpeed(progress, remaining);

    updatePID(-dir * speed, dir * speed, false);
  }

  motorHalt(LMOTOR);
  motorHalt(RMOTOR);
}
