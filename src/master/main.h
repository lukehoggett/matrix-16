// includes required for conversion to cpp files
#include <Arduino.h>

// wire lib for I2C
#include <Wire.h>

#define START_OF_DATA 0x10 // data markers
#define END_OF_DATA 0x20   // data markers
#define DEST_I2C_ADDR 5    // set destination I2C address (must match firmware
// in Colorduino module)

#define ADDR_TOP_LEFT 0x05
#define ADDR_TOP_RIGHT 0x06
#define ADDR_BOTTOM_LEFT 0x07
#define ADDR_BOTTOM_RIGHT 0x08

#define SCREENSIZE_X 16  // num of LEDs across all matrices
#define SCREENSIZE_Y 16 // num of LEDs down all matrices
#define MATRIX_X 8 // num LED across a matix
#define MATRIX_Y 8 // num LED down a matix
#define DISPLAY_X 2 // number of matrices across
#define DISPLAY_Y 2 // number of matrices down

uint8_t displayByte[3][4][64]; // display array - 64 bytes x 3 colours

// setup for plasma
typedef struct {
  unsigned char r;
  unsigned char g;
  unsigned char b;
} ColorRGB;

// a color with 3 components: h, s and v
typedef struct {
  unsigned char h;
  unsigned char s;
  unsigned char v;
} ColorHSV;

void display(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b, uint8_t addr);
void updateDisplay(uint8_t addr);
static uint8_t sendBuffer(uint8_t addr, uint8_t col, uint8_t *disp_data);
void HSVtoRGB(void *vRGB, void *vHSV);
unsigned int RGBtoINT(void *vRGB);
float dist(float a, float b, float c, float d);
void generateData();
ColorRGB getValue(uint8_t x, uint8_t y, ColorRGB colorRGB);

// void plasmaSetup();
