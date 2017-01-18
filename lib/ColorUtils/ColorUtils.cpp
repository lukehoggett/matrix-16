#include "ColorUtils.h"

//<<constructor>>
ColorUtils::ColorUtils() {/* nothing to construct */}
 
//<<destructor>>
ColorUtils::~ColorUtils() {/* nothing to destruct */}

// Converts an HSV color to RGB color
void ColorUtils::HSVtoRGB(void *vRGB, void *vHSV) {
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

unsigned int ColorUtils::RGBtoINT(void *vRGB) {
  ColorRGB *colorRGB = (ColorRGB *)vRGB;

  return (((unsigned long int)colorRGB->r) << 16) +
         (((unsigned long int)colorRGB->g) << 8) +
         (unsigned long int)colorRGB->b;
}
