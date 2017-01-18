// includes required for conversion to cpp files
#include <Arduino.h>

#include <Colorduino.h> // colorduino library
#include <Wire.h>       // wire library for I2C

#define I2C_DEVICE_ADDRESS 0x05 // I2C address for this device
#define START_OF_DATA 0x10      // data markers
#define END_OF_DATA 0x20        // data markers

#define MATRIX_WIDTH 8
#define MATRIX_HEIGHT 8

#define COLOR_BYTE_RED 0
#define COLOR_BYTE_GREEN 1
#define COLOR_BYTE_BLUE 2

// declarations required for conversion to cpp file, should move to .h file
void setWhiteBalance();
void listener();
void receiveEvent(int numBytes);
