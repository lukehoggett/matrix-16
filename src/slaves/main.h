// includes required for conversion to cpp files
#include <Arduino.h>

#include <Colorduino.h> // colorduino library
#include <Wire.h>       // wire library for I2C

#define I2C_DEVICE_ADDRESS 0x05 // I2C address for this device
#define START_OF_DATA 0x10      // data markers
#define END_OF_DATA 0x20        // data markers

// declarations required for conversion to cpp file, should move to .h file
void setWhiteBalance();
void drawDots();
void listener();
void receiveEvent(int numBytes);
