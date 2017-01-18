#include "main.h"

unsigned char whiteBalanceValue[3] = {20, 63, 63};

void setWhiteBalance() { Colorduino.SetWhiteBal(whiteBalanceValue); }

void listener() {
  // Serial.print("listener() Wire available: ");
  // Serial.println(Wire.available());
  // 1 pixel = 3 bytes B00000000 B00000000 B00000000.
  // We send R then G then B bytes as 3 separate transfers
  // This is because if we make the I2C buffer too large, we run out of SRAM for
  // other things on our master Arduino
  if (Wire.available() > 66) { // when buffer full, process data. 66 =  start
                               // byte + colour + 64 pixel data + end byte
    // error check - make sure our data starts with the right byte
    if (Wire.read() != START_OF_DATA) {
      // else handle error by reading remaining data until end of data marker
      // (if available)
      while (Wire.available() > 0 && Wire.read() != END_OF_DATA) {
      }
      return;
    }

    byte c = Wire.read(); // read our color byte so we know if these are the R,
                          // G or B pixels.

    // Serial.print("color byte: ");
    // Serial.println(c, HEX);
    // depeding on c read pixels in as R G or B
    // read red display data
    if (c == COLOR_BYTE_RED) {
      for (byte x = 0; x < MATRIX_WIDTH; x++) {
        for (byte y = 0; y < MATRIX_HEIGHT; y++) {
          PixelRGB *p = Colorduino.GetPixel(x, y);
          p->r = Wire.read();
        }
      }
    }

    // read green display data 
    if (c == COLOR_BYTE_GREEN) {
      for (byte x = 0; x < MATRIX_WIDTH; x++) {
        for (byte y = 0; y < MATRIX_HEIGHT; y++) {
          PixelRGB *p = Colorduino.GetPixel(x, y);
          p->g = Wire.read();
        }
      }
    }

    // read blue display data
    if (c == COLOR_BYTE_BLUE) {
      for (byte x = 0; x < MATRIX_WIDTH; x++) {
        for (byte y = 0; y < MATRIX_HEIGHT; y++) {
          PixelRGB *p = Colorduino.GetPixel(x, y);
          p->b = Wire.read();
        }
      }
    }

    // read end of data marker
    if (Wire.read() == END_OF_DATA) {
      // if colour is blue, then update display
      if (c == 2)
        Colorduino.FlipPage();
    }
  }
}

// =============HANDLERS======================================

// get data from master - HINT: this is a ISR call!
// HINT2: do not handle stuff here!! this will NOT work
// collect only data here and process it in the main loop!
void receiveEvent(int numBytes) {
  // do nothing here
}

void setup() {
  Colorduino.Init();
  setWhiteBalance();

  // Colorduino.ColorFill(255, 255, 255);
  randomSeed(analogRead(13));

  Colorduino.ColorFill(random(256), random(92), random(128));

  // Serial.begin(115200);
  // Serial.println("setup");
  // Serial.println(TWI_FREQ);
  // Serial.println(TWI_BUFFER_LENGTH);
  // Serial.println(BUFFER_LENGTH);

  // drawDots();

  Wire.begin(I2C_DEVICE_ADDRESS); // join i2c bus as slave
  Wire.onReceive(receiveEvent);
}

void loop() {
  // Serial.println("loop start");
  listener();
}
