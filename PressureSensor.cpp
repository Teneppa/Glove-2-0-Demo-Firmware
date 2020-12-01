/*
   This is a stupid simple library for reading the haptic glove's
   pressure sensors through the HP4067 MUX.

   This is coded by Teemu Laurila in 26.11.2020
*/


#include "Arduino.h"
#include "PressureSensor.h"

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

// Read the sensor value
uint16_t PressureSensor::readRawSensor(uint8_t index) {
  // Select the line from the MUX
  selectMUXLine(index);

  // Return the value from ADC
  return analogRead(0);
}

// Read the converted sensor value
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

  } else {
    // Raw value from ADC
    sensorValue = analogRead(0);
  }

  return sensorValue;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
// If calibration is requested

void PressureSensor::calibrateSensorMin(uint16_t sensorAmount) {

  // Allocate memory for the minimum value array
  minValArray = new uint16_t[sensorAmount];

  // Get the min values
  for (uint8_t i = 0; i < sensorAmount; i++) {
    minValArray[i] = readSensor(i);
  }

  minCalibrated = true;

  if(minCalibrated && maxCalibrated) {
    calibrationStatus = true;
  }
}

void PressureSensor::calibrateSensorMax(uint16_t sensorAmount) {

  // Allocate memory for the maximum value array
  maxValArray = new uint16_t[sensorAmount];

  // Get the max values
  for (uint8_t i = 0; i < sensorAmount; i++) {
    maxValArray[i] = readSensor(i);
  }

  maxCalibrated = true;

  if(minCalibrated && maxCalibrated) {
    calibrationStatus = true;
  }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

// Map function but for floating points
float PressureSensor::mapFloat(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
