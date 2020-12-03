/*
  This is the demo firmware for the Haptic Glove 2.0

  This is coded by Teemu Laurila in 01.12.2020 w/ bugs
*/

#include "PressureSensor.h"

// Pressure sensor library
PressureSensor s;

void setup() {
  Serial.begin(115200);

  // Set the calibration manually
  uint16_t minCalibrationArray[3] = {401,95,109};
  uint16_t maxCalibrationArray[3] = {780,823,614};

  // Copy the calibration arrays to the PressureSensor object
  s.setMinCalibrationArray(minCalibrationArray, 3);
  s.setMaxCalibrationArray(maxCalibrationArray, 3);
  
  beginBNO();
}

void loop() {

  // If there's data on serial port
  if(Serial.available()>0) {
    // ';' is the termination character
    String getstr = Serial.readStringUntil(';');

    // Finger data
    if(getstr == "f") {
      String temp = "";
      temp += String(s.readSensor(0));
      temp += ",";
      temp += String(s.readSensor(1));
      temp += ",";
      temp += String(s.readSensor(2));
      Serial.println(temp);
    }

    // Finger data
    if(getstr == "r") {
      Serial.print(runRot());
      Serial.println(";");
    }
  }
  
}
