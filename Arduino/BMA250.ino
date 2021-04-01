// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// BMA250
// This code is designed to work with the BMA250_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Accelorometer?sku=BMA250_I2CS#tabs-0-product_tabset-2

#include <Wire.h>
//Analog port 4 (A4) = SDA (serial data)
//Analog port 5 (A5) = SCL (serial clock)
// BMA250 I2C address is 0x18(24) or 0x19(25)

#define Addr 0x18

void setup()
{
  // Initialise I2C communication as MASTER
  Wire.begin();
  // Initialise Serial Communication, set baud rate = 9600
  Serial.begin(9600);

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select range selection register
  Wire.write(0x0F);
  // Set range +/- 2g
  Wire.write(0x03);
  // Stop I2C Transmission
  Wire.endTransmission();

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select bandwidth register
  Wire.write(0x10);
  // Set bandwidth 7.81 Hz
  Wire.write(0x08);
  // Stop I2C Transmission
  Wire.endTransmission();
  delay(300);
}

void loop()
{
  uint8_t data[6]; //correct type declaration
  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select Data Registers (0x02 − 0x07)
  Wire.write(0x02);
  // Stop I2C Transmission
  Wire.endTransmission();
  
  // Request 6 bytes 
  Wire.requestFrom(Addr, 6);
  
  // Read the six bytes 
  // xAccl lsb, xAccl msb, yAccl lsb, yAccl msb, zAccl lsb, zAccl msb
  if(Wire.available() == 6)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
    data[2] = Wire.read();
    data[3] = Wire.read();
    data[4] = Wire.read();
    data[5] = Wire.read();
  }
  delay(300);
  
  // Convert the data to 10 bits
  float xAccl = ((data[1] * 256.0) + (data[0] & 0xC0)) / 64;
  if (xAccl > 511)
  {
    xAccl -= 1024;
  }
  float yAccl = ((data[3] * 256.0) + (data[2] & 0xC0)) / 64;
  if (yAccl > 511)
  {
    yAccl -= 1024;
  }
  float zAccl = ((data[5] * 256.0) + (data[4] & 0xC0)) / 64;
  if (zAccl > 511)
  {
    zAccl -= 1024;
  }
  
  // Output data to the serial monitor
  Serial.print("Acceleration in X-Axis :");
  Serial.println(xAccl);
  Serial.print("Acceleration in Y-Axis :");
  Serial.println(yAccl);
  Serial.print("Acceleration in Z-Axis :");
  Serial.println(zAccl) ; 
}
