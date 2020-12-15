/*
  This is the demo firmware for the Haptic Glove 2.0

  This is coded by Teemu Laurila in 01.12.2020 w/ bugs

  TODO:
    - Add averaging for the pressure sensors to reduce the
      jitter.
    - Add tracking?
    - Check if max < min or min > max in sensor calibration?

  Modified on 3.12.2020
   > The communication is now syncronous
      -> You need to send either "f;" or "r;" from unity to
         get the finger values or quaternions
   > The calibration is now done manually so that the
     Unity script doesn't time out. It seems that the lag had something
     to do with the timeouts, and now it's a lot faster.

  Modified on 9.12.2020
   > Fix setCalibrationArray.
   > Add nSensor -variable to store the pressure sensor count
   > So much other stuff that I don't even remember what I've done
*/

/* <<//>><<\\>><<//>><<\\>><<//>> Define things and import libraries <<\\>><<//>><<\\>><<//>><<\\>> */

#include "PressureSensor.h"

// Pressure sensor library
PressureSensor s;

bool triggerIndex = false;
long oldMillis = 0;

// How many pressuresensors does the glove have?
uint16_t nSensor = 10;

/* <<//>><<\\>><<//>><<\\>><<//>><<\\>><<//>>     Setup   <<\\>><<//>><<\\>><<//>><<\\>><<//>><<\\>> */

void setup() {
  Serial.begin(115200);

  // Set the calibration manually
  uint16_t minCalibrationArray[nSensor] = {144,49,215,466,152,251,83,206,71,373};
  uint16_t maxCalibrationArray[nSensor] = {405,330,421,482,446,456,277,525,340,483};


  // Copy the calibration arrays to the PressureSensor object
  s.setMinCalibrationArray(minCalibrationArray, nSensor);
  s.setMaxCalibrationArray(maxCalibrationArray, nSensor);

  // Test
  pinMode(5, OUTPUT);
  
  beginBNO();
}

/* <<//>><<\\>><<//>><<\\>><<//>><<\\>><<//>> Loop ’n’ stuff <<\\>><<//>><<\\>><<//>><<\\>><<//>><<\\>> */

void loop() {

  // If there's data on serial port
  if(Serial.available()>0) {
    // ';' is the termination character
    String getstr = Serial.readStringUntil(';');

    // Finger data
    if(getstr == "f") {
      String temp = "";
      
      for(uint8_t i=0; i<nSensor-1; i++) {
        temp += String(s.readSensor(i));
        temp += ",";
      }
      temp += String(s.readSensor(nSensor-1));
      
      Serial.println(temp);
    }

    // Finger data
    if(getstr == "r") {
      Serial.print(runRot());
      Serial.println(";");
    }

    if(getstr == "raw") {
      String temp = "";
      
      for(uint8_t i=0; i<nSensor-1; i++) {
        temp += String(s.readRawSensor(i));
        temp += ",";
      }
      temp += String(s.readRawSensor(nSensor-1));
      
      Serial.println(temp);
    }

    // TODO: Receive haptic feedback requests
    if(getstr.indexOf('h') == 0) {
      // Split the string
      triggerIndex = true;
      oldMillis = millis();
    }

    // Calibrate the sensor min
    if(getstr == "t8") {
      Serial.println(s.readSensor(8));
    }

    // Calibrate the sensor min
    if(getstr == "cmin") {
      Serial.println("ACK,cmin");
      s.calibrateSensorMin(nSensor);
    }

    // Calibrate the sensor max
    if(getstr == "cmax") {
      Serial.println("ACK,cmax");
      s.calibrateSensorMax(nSensor);
    }

    // Print out calibration data
    if(getstr == "getcal") {
      Serial.println(s.getCalibrationData());
    }
  }

  if(triggerIndex) {
    if(millis() - oldMillis > 100) {
      triggerIndex = false;
    }
  }
  
  digitalWrite(5, triggerIndex);
  
}

/* <<//>><<\\>><<//>><<\\>><<//>><<\\>><<//>> End of da stuff <<\\>><<//>><<\\>><<//>><<\\>><<//>><<\\>> */
