#ifndef PressureSensor_h
#define PressureSensor_h

#include "Arduino.h"

class PressureSensor{
  public:
    PressureSensor();
    uint16_t readSensor(uint8_t index);
    uint16_t readRawSensor(uint8_t index);

    // Sensor calibration
    void calibrateSensorMin(uint16_t sensorAmount);
    void calibrateSensorMax(uint16_t sensorAmount);

    bool calibrationStatus = false;
    bool minCalibrated = false;
    bool maxCalibrated = false;
  private:
    void selectMUXLine(uint8_t index);

    float mapFloat(float x, float in_min, float in_max, float out_min, float out_max);
  
    uint8_t selectPinArray[4] = {4, 7, 8, 10}; // {S0, S1, S2, S3}
    uint8_t enablePin = 2;  // EN
    uint8_t arraySize = 0;

    uint16_t * minValArray;    // Array to store all of min values for the sensors
    uint16_t * maxValArray;    // Array to store all of the max values for the sensors

    uint8_t convertedSensorMinValue = 0;
    uint8_t convertedSensorMaxValue = 100;
};

#endif
