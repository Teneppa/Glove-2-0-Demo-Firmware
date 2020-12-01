/*
  This is the demo firmware for the Haptic Glove 2.0

  This is coded by Teemu Laurila in 01.12.2020 w/ bugs
*/

#include "PressureSensor.h"

// Pressure sensor library
PressureSensor s;

void setup() {
  Serial.begin(115200);

  // Calibrate the sensors
  Serial.println("Press to calibrate min");
  while(s.readSensor(15) < 600) delay(100);
  Serial.println("Calibrating...");
  s.calibrateSensorMin(15);
  delay(600);

  Serial.println("Press to calibrate max");
  while(s.readSensor(15) < 600) delay(100);
  Serial.println("Calibrating...");
  s.calibrateSensorMax(15);
  
  beginBNO();
}

void loop() {
  Serial.print(s.readSensor(0));
  Serial.print(",");
  Serial.print(s.readSensor(1));
  Serial.print(",");
  Serial.print(s.readSensor(2));
  Serial.print(runRot());

  Serial.println();
  delay(10);
}
