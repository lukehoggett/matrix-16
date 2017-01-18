/*
   arduino I2C to Colorduino demo

   based on
   -arduino firmware by michael vogt <michu@neophob.com>
   -blinkm firmware by thingM
   -"daft punk" firmware by Scott C / ThreeFN

   libraries to patch:
   Wire:
    utility/twi.h: #define TWI_FREQ 400000L (was 100000L)
 #define TWI_BUFFER_LENGTH 70 (was 32)
    wire.h: #define BUFFER_LENGTH 70 (was 32)

   This DEMO is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.
   This DEMO is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "main.h"

unsigned char plasma[SCREENSIZE_X][SCREENSIZE_Y];
long paletteShift;

void setup() {
  Wire.begin(1); // join i2c bus (address optional for master)
  // Serial.begin(115200);
  // plasmaSetup(); // plasma setup
}

void loop() { generateData(); }

// update display buffer using x,y,r,g,b format
void display(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b, uint8_t addr) {
  uint8_t p = (y * 8) + x; // convert from x,y to pixel number in array

  displayByte[0][addr][p] = r;
  displayByte[1][addr][p] = g;
  displayByte[2][addr][p] = b;
}

// send display buffer to display
void updateDisplay() {
  uint8_t matrixAddress;
  for (matrixAddress = ADDR_TOP_LEFT; matrixAddress <= ADDR_BOTTOM_RIGHT; matrixAddress++) {
    sendBuffer(matrixAddress, 0, displayByte[0][matrixAddress]);
    sendBuffer(matrixAddress, 1, displayByte[1][matrixAddress]);
    sendBuffer(matrixAddress, 2, displayByte[2][matrixAddress]);
  }
}

// send data via I2C to a client
static uint8_t sendBuffer(uint8_t addr, uint8_t col, uint8_t *disp_data) {
  Wire.beginTransmission(addr);
  Wire.write(START_OF_DATA);
  Wire.write(col);
  Wire.write(disp_data, 64);
  Wire.write(END_OF_DATA);
  return Wire.endTransmission();
}

// plasma convert
// Converts an HSV color to RGB color
void HSVtoRGB(void *vRGB, void *vHSV) {
  float r, g, b, h, s, v; // this function works with floats between 0 and 1
  float f, p, q, t;
  int i;
  ColorRGB *colorRGB = (ColorRGB *)vRGB;
  ColorHSV *colorHSV = (ColorHSV *)vHSV;

  h = (float)(colorHSV->h / 256.0);
  s = (float)(colorHSV->s / 256.0);
  v = (float)(colorHSV->v / 256.0);

  // if saturation is 0, the color is a shade of grey
  if (s == 0.0) {
    b = v;
    g = b;
    r = g;
  }

  // if saturation > 0, more complex calculations are needed
  else {
    h *= 6.0;            // to bring hue to a number between 0 and 6,
                         // better
                         // for the calculations
    i = (int)(floor(h)); // e.g. 2.7 becomes 2 and 3.01 becomes 3 or 4.9999
                         // becomes 4
    f = h - i;           // the fractional part of h

    p = (float)(v * (1.0 - s));
    q = (float)(v * (1.0 - (s * f)));
    t = (float)(v * (1.0 - (s * (1.0 - f))));

    switch (i) {
    case 0:
      r = v;
      g = t;
      b = p;
      break;

    case 1:
      r = q;
      g = v;
      b = p;
      break;

    case 2:
      r = p;
      g = v;
      b = t;
      break;

    case 3:
      r = p;
      g = q;
      b = v;
      break;

    case 4:
      r = t;
      g = p;
      b = v;
      break;

    case 5:
      r = v;
      g = p;
      b = q;
      break;

    default:
      r = g = b = 0;
      break;
    }
  }
  colorRGB->r = (int)(r * 255.0);
  colorRGB->g = (int)(g * 255.0);
  colorRGB->b = (int)(b * 255.0);
}

unsigned int RGBtoINT(void *vRGB) {
  ColorRGB *colorRGB = (ColorRGB *)vRGB;

  return (((unsigned long int)colorRGB->r) << 16) +
         (((unsigned long int)colorRGB->g) << 8) +
         (unsigned long int)colorRGB->b;
}

float dist(float a, float b, float c, float d) {
  return sqrt((c - a) * (c - a) + (d - b) * (d - b));
}

void generateData() {
  uint8_t x, y, xRel, yRel;
  ColorRGB colorRGB;
  uint8_t addr;

  for (x = 0; x < SCREENSIZE_X; x++) {
    for (y = 0; y < SCREENSIZE_Y; y++) {
      // @TODO: can we replace this with a callback or someting to allow us to change the algorithm
      colorRGB = getValue(x, y, colorRGB);
      
      // dtermine which address we wish to send data to
      if (x < MATRIX_X) {
        if (y < MATRIX_Y) {
          // top left
          // std::cout << " top left ";
          addr = ADDR_TOP_LEFT;
          xRel = x;
          yRel = y;
        } else {
          // top left
          // std::cout << " top right ";
          addr = ADDR_TOP_RIGHT;
          xRel = x;
          yRel = y - MATRIX_Y;
        }
      } else {
        if (y < MATRIX_Y) {
          // top left
          // std::cout << " bottom left ";
          addr = ADDR_BOTTOM_LEFT;
          xRel = x - MATRIX_X;
          yRel = y;
        } else {
          // top left
          // std::cout << " bottom right ";
          addr = ADDR_BOTTOM_RIGHT;
          xRel = x - MATRIX_X;
          yRel = y - MATRIX_Y;
        }
      }
      display(xRel, yRel, colorRGB.r, colorRGB.g, colorRGB.b, addr);
    }
  }
  paletteShift++;

  updateDisplay();
}

ColorRGB getValue(uint8_t x, uint8_t y, ColorRGB colorRGB) {
  ColorHSV colorHSV;
  float value;
  value = sin(dist(x + paletteShift, y, 128.0, 128.0) / 8.0) +
          sin(dist(x, y, 64.0, 64.0) / 8.0) +
          sin(dist(x, y + paletteShift / 7, 192.0, 64) / 7.0) +
          sin(dist(x, y, 192.0, 100.0) / 8.0);
  colorHSV.h = (unsigned char)((value)*128) & 0xff;
  colorHSV.s = 255;
  colorHSV.v = 255;
  HSVtoRGB(&colorRGB, &colorHSV);
  
  return colorRGB;
}

// plasma setup - start with morphing plasma, but allow going to color cycling
// if desired.
// void plasmaSetup() {
//   paletteShift = 128000;
//   unsigned char bcolor;
// 
//   for (unsigned char x = 0; x < SCREENSIZE_X; x++)
//     for (unsigned char y = 0; y < SCREENSIZE_Y; y++) {
//       // the plasma buffer is a sum of sines
//       bcolor = (unsigned char)(128.0 + (128.0 * sin(x * 8.0 / 16.0)) + 128.0 +
//                                (128.0 * sin(y * 8.0 / 16.0))) /
//                2;
//       plasma[x][y] = bcolor;
//     }
// }
