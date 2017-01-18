#ifndef COLORUTILS_H
#define COLORUTILS_H

// includes required for conversion to cpp files
#include <Arduino.h>

// setup for plasma
typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
} ColorRGB;

// a color with 3 components: h, s and v
typedef struct {
  uint8_t h;
  uint8_t s;
  uint8_t v;
} ColorHSV;

class ColorUtils {
  public:
    ColorUtils();
    ~ColorUtils();
    void HSVtoRGB(void *vRGB, void *vHSV);
    unsigned int RGBtoINT(void *vRGB);
};

#endif
