#include <Arduino.h>
#include <Wire.h>


/*
 * SensorData_t
 * Example struct to demostrate how to serialize a struct for transmission 
 * movingAverage contains the calculated moving average based up on the data in rawData
 * rawData contains the unmodified data of the last 5 sensor readouts 
 */

typedef struct SensorData {
  long movingAverage;
  long rawData[5];
} SensorData_t;

//declaration of SensorData_t object with hardcoded values
SensorData_t lastSensorData = {3, {1, 2, 3, 4, 5}};

//stored size of sensor data and slave address in constant for maintability and readability
const uint8_t SENSOR_DATA_SZ = sizeof(SensorData_t);
const uint8_t SENSOR_SLAVE_ADDRESS = 8;

//declared byte buffer to write data to i2c data register
//Wire.write doesnt accept struct as parameter
byte buffer[SENSOR_DATA_SZ];

bool handled = true;

//om i2c data request the function sets a flag and write the whole buffer onto the interface
void requestEvent() {
  if (handled) { 
  Wire.write(buffer, SENSOR_DATA_SZ);
  handled = false;
  }
}

void setup() {
  //set logic low levels for SDA and SCL for 3.3V compatibility
  digitalWrite(SDA, LOW);
  digitalWrite(SCL, LOW);

  //copy struct bytes to byte array for transmission
  //more info about memcpy: https://www.tutorialspoint.com/c_standard_library/c_function_memcpy.htm
  memcpy(&buffer, &lastSensorData, SENSOR_DATA_SZ);

  //init Serial interfaces
  Wire.begin(SENSOR_SLAVE_ADDRESS);
  Wire.onRequest(requestEvent);
  Serial.begin(9600);
}

void loop() {
  // simple flag to check if i2c request is triggered
  if (!handled)
  {
    Serial.println("Requested");
    handled = true;
  }
}