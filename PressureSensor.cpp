/*
   This is a stupid simple library for reading the haptic glove's
   pressure sensors through the HP4067 MUX.

   This is coded by Teemu Laurila in 26.11.2020

   Modified on 03.12.2020
    + --> Added the ability to specify the calibration arrays by hand
      + setMinCalibrationArray
      + setMaxCalibrationArray
    + --> Added a function to retrieve the calibration arrays as a string
      + getCalibrationData
*/


#include "Arduino.h"
#include "PressureSensor.h"

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

// When the object is created
PressureSensor::PressureSensor() {
  // Get the amount of control pins
  arraySize = sizeof(selectPinArray) / sizeof(selectPinArray[0]);

  // Set the Sx pins to OUTPUT
  for (uint8_t i = 0; i < arraySize; i++) {
    pinMode(selectPinArray[i], OUTPUT);
  }

  // Set the ENABLE pin to OUTPUT
  pinMode(2, OUTPUT);   // EN

  // Enable the multiplexer
  digitalWrite(2, 0);   // Enable is inverted
}

// Select specific mux line
void PressureSensor::selectMUXLine(uint8_t index) {

  // Write the control values to the HP4067
  for (uint8_t i = 0; i < arraySize; i++) {
    digitalWrite(selectPinArray[i], bitRead(index, i));   // 0,0,0,0 = Channel 0
  }

}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

// Read the raw sensor value from the DAC
uint16_t PressureSensor::readRawSensor(uint8_t index) {
  // Select the line from the MUX
  selectMUXLine(index);

  // Return the value from ADC
  return analogRead(0);
}

// Read the converted sensor value
//
// NOTE: If the calibration is incomplete, the function will return convertedSensorMinValue
uint16_t PressureSensor::readSensor(uint8_t index) {
  
  // Select the line from the MUX
  selectMUXLine(index);

  uint16_t sensorValue = 0;

  if (calibrationStatus == true) {
    uint16_t rawValue = analogRead(0);

    // Limit the value to the maximum
    if (rawValue > maxValArray[index]) {
      rawValue = maxValArray[index];
    }

    // Limit the value to the minimum
    if (rawValue < minValArray[index]) {
      rawValue = minValArray[index];
    }

    // Map the raw value
    sensorValue = mapFloat(rawValue, minValArray[index], maxValArray[index], convertedSensorMinValue, convertedSensorMaxValue);
  }else{
    sensorValue = convertedSensorMinValue;
  }

  return sensorValue;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

// If calibration is requested
void PressureSensor::calibrateSensorMin(uint16_t sensorAmount) {

  calibrationArraySize = sensorAmount;

  // Deallocate memory
  delete minValArray;

  // Allocate memory for the minimum value array
  minValArray = new uint16_t[sensorAmount];

  // Get the min values
  for (uint8_t i = 0; i < sensorAmount; i++) {
    minValArray[i] = readRawSensor(i);
  }

  // Update the calibration status
  minCalibrated = true;

  if(minCalibrated && maxCalibrated) {
    calibrationStatus = true;
  }
}

// If calibration is requested
void PressureSensor::calibrateSensorMax(uint16_t sensorAmount) {

  calibrationArraySize = sensorAmount;

  // Deallocate memory
  delete maxValArray;

  // Allocate memory for the maximum value array
  maxValArray = new uint16_t[sensorAmount];

  // Get the max values
  for (uint8_t i = 0; i < sensorAmount; i++) {
    maxValArray[i] = readRawSensor(i);
  }

  // Update the calibration status
  maxCalibrated = true;

  if(minCalibrated && maxCalibrated) {
    calibrationStatus = true;
  }
}

/* USAGE:

// This is an example for the usage of setMinCalibrationArray and setMaxCalibrationArray
PressureSensor s;

// This is the calibration array for the glove. You can get it with s.getCalibrationData()
// NOTE: getCalibrationData() returns a pretty dirty output, so you need to modify it a bit.
uint16_t minCalibrationArray[3] = {401,95,109};
uint16_t maxCalibrationArray[3] = {780,823,614};

// These copy the arrays declared above to the PressureSensor object. SensorAmount should be the
// same as your array size.
s.setMinCalibrationArray(minCalibrationArray, 3);
s.setMaxCalibrationArray(maxCalibrationArray, 3);
*/

/* If you want to set the min calibration array manually */
void PressureSensor::setMinCalibrationArray(uint16_t * src, uint16_t sensorAmount) {

  // Save the array size
  calibrationArraySize = sensorAmount;

  
  
  delete minValArray;
  minValArray = new uint16_t[sensorAmount];
  memcpy(minValArray, src, sizeof(src[0])*sensorAmount);

  minCalibrated = true;

  if(minCalibrated && maxCalibrated) {
    calibrationStatus = true;
  }
}

/* If you want to set the max calibration array manually */
void PressureSensor::setMaxCalibrationArray(uint16_t * src, uint16_t sensorAmount) {
  
  calibrationArraySize = sensorAmount;

  // Deallocate memory
  delete maxValArray;

  // Allocate memory for the maximum value array
  maxValArray = new uint16_t[sensorAmount];
  memcpy(maxValArray, src, sizeof(src[0])*sensorAmount);

  maxCalibrated = true;

  if(minCalibrated && maxCalibrated) {
    calibrationStatus = true;
  }
}

String PressureSensor::getCalibrationData() {

  uint16_t sensorAmount = calibrationArraySize;
  
  String temp = "min = {";

  // Get the min values
  for (uint8_t i = 0; i < sensorAmount; i++) {
    temp += String(minValArray[i]);
    temp += ",";
  }
  temp += "};\n max = {";

  // Get the max values
  for (uint8_t i = 0; i < sensorAmount; i++) {
    temp += String(maxValArray[i]);
    temp += ",";
  }
  temp += "};\n";

  return temp;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

// Map function but for floating points
float PressureSensor::mapFloat(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
