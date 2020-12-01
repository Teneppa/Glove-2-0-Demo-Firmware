#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>

uint16_t BNO055_SAMPLERATE_DELAY_MS = 10; //how often to read data from the board
uint16_t PRINT_DELAY_MS = 250; // how often to print the data
uint16_t printCount = 0; //counter to avoid printing every 10MS sample

// Check I2C device address and correct line below (by default address is 0x29 or 0x28)
//                                   id, address
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);

void beginBNO() {
  bno.begin();
}

String runRot() {
  unsigned long tStart = micros();

  String temp = "";

  imu::Quaternion quat = bno.getQuat();

  /* Display the quat data */
  temp += ",";
  temp += String(quat.w());
  temp += ",";
  temp += String(quat.y());
  temp += ",";
  temp += String(quat.x());
  temp += ",";
  temp += String(quat.z());

  return temp;
}
