// includes required for conversion to cpp files
#include <Arduino.h>
// wire lib for I2C
#include <Wire.h>


#define START_OF_DATA 0x10       // data markers
#define END_OF_DATA 0x20         // data markers
#define DEST_I2C_ADDR 5          // set destination I2C address (must match firmware in Colorduino module)

#define DEST_I2C_ADDR_TOP_LEFT 5
#define DEST_I2C_ADDR_TOP_RIGHT 6
#define DEST_I2C_ADDR_BOTTOM_LEFT 7
#define DEST_I2C_ADDR_BOTTOM_RIGHT 8

#define SCREENSIZEX 8            // num of LEDs across
#define SCREENSIZEY 16            // num of LEDs down

uint8_t display_byte[3][64];        // display array - 64 bytes x 3 colours

// setup for plasma
typedef struct
{
        unsigned char r;
        unsigned char g;
        unsigned char b;
} ColorRGB;

// a color with 3 components: h, s and v
typedef struct
{
        unsigned char h;
        unsigned char s;
        unsigned char v;
} ColorHSV;

void display(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b);
void update_display(uint8_t addr);
static uint8_t BlinkM_sendBuffer(uint8_t addr, uint8_t col, uint8_t* disp_data);
void HSVtoRGB(void *vRGB, void *vHSV);
unsigned int RGBtoINT(void *vRGB);
float dist(float a, float b, float c, float d);
void plasma_morph();
void plasma_setup();
