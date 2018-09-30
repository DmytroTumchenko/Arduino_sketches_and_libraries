#include <Arduino.h>
#include "A4988.h"

// using a 200-step motor (most common)
// pins used are DIR, STEP, MS1, MS2, MS3 in that order
A4988 stepper(200, A7, A6, NULL, NULL, NULL);

void setup() {
  // Set target motor RPM to 1RPM
  stepper.setRPM(200);
  // Set full speed mode (microstepping also works for smoother hand movement
  stepper.setMicrostep(1);
}

void loop() {
  
  digitalWrite(13, 0);
  // Tell motor to rotate 360 degrees. That's it.
  stepper.rotate(360);
  digitalWrite(13, 1);
  delay(100);
}
