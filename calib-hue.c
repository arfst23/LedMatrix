//*****************************************************************************

#include "matrix.h"
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#include <stdio.h>
#include <math.h>

#define TARGET 0.4

static inline void delay(int ms)
{
  usleep(ms * 1000);
}

//*****************************************************************************

typedef struct
{
  float r;
  float g;
  float b;
} rgb_t;

rgb_t raw(float h)
{
  rgb_t rgb = { 0.0, 0.0, 0.0 };
  if (h < 1.0)
  {
    rgb.r = 1.0;
    rgb.g = h;
  }
  else if (h < 2.0)
  {
    float hh = h - 1.0;
    rgb.r = 1.0 - hh;
    rgb.g = 1.0;
  }
  else if (h < 2.5)
  {
    float hh = (h - 2.0) * 2.0;
    rgb.g = 1.0;
    rgb.b = hh;
  }
  else if (h < 3.0)
  {
    float hh = (h - 2.5) * 2.0;
    rgb.g = 1.0 - hh;
    rgb.b = 1.0;
  }
  else if (h < 3.5)
  {
    float hh = (h - 3.0) * 2.0;
    rgb.r = hh;
    rgb.b = 1.0;
  }
  else
  {
    float hh = (h - 3.5) * 2.0;
    rgb.b = 1.0 - hh;
    rgb.r = 1.0;
  }
  return rgb;
}

void set_rgb(uint8_t x, uint8_t y, rgb_t rgb)
{
  uint8_t r = (uint8_t)(255.0 * rgb.r + 0.5);
  uint8_t g = (uint8_t)(255.0 * rgb.g + 0.5);
  uint8_t b = (uint8_t)(255.0 * rgb.b + 0.5);
  matrix_set_rgb(x, y, r, g, b);
}

float val(rgb_t rgb)
{
  //return 0.299 * rgb.r + 0.587 * rgb.g + 0.114 * rgb.b;
  return 0.35 * rgb.r + 0.40 * rgb.g + 0.25 * rgb.b;
}

rgb_t scale(rgb_t rgb, float s)
{
  float m = rgb.r > rgb.g ? rgb.r : rgb.g;
  m = rgb.b > m ? rgb.b : m;

  if (m * s <= 1.0)
  {
    rgb.r *= s;
    rgb.g *= s;
    rgb.b *= s;
    return rgb;
  }

  rgb.r /= m; 
  rgb.g /= m; 
  rgb.b /= m; 
  s *= m;

  if (s <= 1.0)
    return rgb;

  float v = val(rgb);
  rgb.r = 1.0 - (1.0 - rgb.r) * 0.99;
  rgb.g = 1.0 - (1.0 - rgb.g) * 0.99;
  rgb.b = 1.0 - (1.0 - rgb.b) * 0.99;
  float w = val(rgb);
 
  s = s * v / w;
  rgb = scale(rgb, s);
  return rgb;
}

void set_h(uint8_t x, uint8_t y, float h)
{
  rgb_t rgb = raw(h);
  float v = val(rgb);
  rgb = scale(rgb, TARGET / v);
  set_rgb(x, y, rgb);
}


int main()
{
  matrix_create();

  for (int i = 0; i < 64; i++)
  {
    float h = 4.0 * (float)i / 64.0;
    set_h(0, i, h);
    set_h(1, i, h);
    set_h(2, i, h);
    set_h(3, i, h);
    set_h(4, i, h);
  };

  matrix_flush();

  printf("static uint8_t rgb[256][3] =\n");
  printf("{\n");
  for (int i = 0; i < 256; i++)
  {
    float h = 4.0 * (float)i / 256.0;
    rgb_t rgb = raw(h);
    float v = val(rgb);
    rgb = scale(rgb, TARGET / v);
    uint8_t r = (uint8_t)(255.0 * rgb.r + 0.5);
    uint8_t g = (uint8_t)(255.0 * rgb.g + 0.5);
    uint8_t b = (uint8_t)(255.0 * rgb.b + 0.5);
    printf("  [%d] = { %d, %d, %d },\n", i, r, g, b);
  }
  printf("};\n");

  delay(10000);

  matrix_free();
  return EXIT_SUCCESS;
}
 
//*****************************************************************************
