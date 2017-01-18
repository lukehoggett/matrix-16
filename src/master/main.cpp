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

ColorUtils colorUtils;

void setup() {
  Wire.begin(1); // join i2c bus (address optional for master)
  // Serial.begin(115200);
  // plasmaSetup(); // plasma setup
}

void loop() { generateData(); }

// update display buffer using x,y,r,g,b format
void display(uint8_t addr, uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b) {
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
      display(addr, xRel, yRel, colorRGB.r, colorRGB.g, colorRGB.b);
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
  colorHSV.h = (uint8_t)((value)*128) & 0xff;
  colorHSV.s = 255;
  colorHSV.v = 255;
  colorUtils.HSVtoRGB(&colorRGB, &colorHSV);
  
  return colorRGB;
}
