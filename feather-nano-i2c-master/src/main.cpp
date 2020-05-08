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

//declare SensorData_t object to store data received on i2c interface
SensorData_t lastSensorData;

//stored size of sensor data and slave address in constant for maintability and readability
const uint8_t SENSOR_DATA_SZ = sizeof(SensorData_t);
const uint8_t SENSOR_SLAVE_ADDRESS = 8;

void setup() {
  //init serial interface
  Wire.begin();
  Serial.begin(9600);
}

void loop() {
  //request data from i2c slave and block code until expected bytes are received
  Wire.requestFrom(SENSOR_SLAVE_ADDRESS, SENSOR_DATA_SZ);
  while (Wire.available() < SENSOR_DATA_SZ);

  //read and store the expected bytes in a byte array
  byte buffer[SENSOR_DATA_SZ];
  for (size_t i = 0; i < SENSOR_DATA_SZ; i++)
  {
    buffer[i] = Wire.read();
    Serial.println(buffer[i]);
  }

  //copy the byte array to struct bytes
  //more info about memcpy: https://www.tutorialspoint.com/c_standard_library/c_function_memcpy.htm
  memcpy(&lastSensorData, &buffer, SENSOR_DATA_SZ);

  //print received data for debugging purposes
  Serial.print("MA: ");Serial.println(lastSensorData.movingAverage);
  Serial.print("RD: ");Serial.println(lastSensorData.rawData[0]);
  Serial.print("RD: ");Serial.println(lastSensorData.rawData[1]);
  Serial.print("RD: ");Serial.println(lastSensorData.rawData[2]);
  Serial.print("RD: ");Serial.println(lastSensorData.rawData[3]);
  Serial.print("RD: ");Serial.println(lastSensorData.rawData[4]);
  
  delay(500);
}
