//*****************************************************************************
 
#include "matrix.h"

#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <assert.h>
 
#define FRAMES 150
#define DELAY 30

#define delay(x) usleep(x * 1000)
 
//*****************************************************************************

#define SIZE (UINT8_MAX + 1)

static uint8_t red[SIZE];
static uint8_t green[SIZE];
static uint8_t blue[SIZE];

void hue_create()
{
  for (int hue = 0; hue < SIZE; hue++)
  {
    int h = hue;
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;

    switch (h / 32)
    {
    case 0: //   0 ..  63 = r .. y
    case 1:
      h -= 0;
      h *= 4;

      r = 255;
      g = h;
      break;

    case 2: //  64 .. 127 = y .. g
    case 3:
      h -= 64;
      h *= 4;

      r = 255 - h;
      g = 255;
      break;

    case 4: // 128 .. 159 = g .. c
      h -= 128;
      h *= 8;
 
      g = 255;
      b = h;
      break;
 
    case 5: // 160 .. 191 = c .. b
      h -= 160;
      h *= 8;
 
      g = 255 - h;
      b = 255;
      break;

    case 6: // 192 .. 223 = b .. m
      h -= 192;
      h *= 8;
 
      r = h;
      b = 255;
      break;
 
    case 7: // 224 .. 255 = m .. r
      h -= 224;
      h *= 8;
 
      r = 255;
      b = 255 - h;
      break;
    }
 
    int p = 35 * r + 40 * g + 25 * b;
    int q = 25 * 255;
    if (p >= q)
    {
      r = r * q / p;
      g = g * q / p;
      b = b * q / p;
    }
    else
    {
      abort();
      
      r = 255 - r;
      g = 255 - g;
      b = 255 - b;
 
      int p = 35 * r + 40 * g + 25 * b;
      int q = 2 * 8500;
      r = r *q / p;
      g = g *q / p;
      b = b *q / p;
 
      r = 255 - r;
      g = 255 - g;
      b = 255 - b;
    }
 
    red[hue] = r;
    green[hue] = g;
    blue[hue] = b;
  }
}

void matrix_hue(uint8_t x, uint8_t y, uint8_t h, uint8_t z)
{
  int r = red[h];
  int g = green[h];
  int b = blue[h];

  if (z < 255)
  {
    r = (r * z) >> 8;
    g = (g * z) >> 8;
    b = (b * z) >> 8;
  }

  matrix_set(x, y, r, g, b);
}

//*****************************************************************************

int main()
{
  hue_create();
  matrix_create();
  
  float size = MATRIX_WIDTH / 3 + 1.0;
  for (;;)
    for (int i = 0; i < FRAMES; i++)
    {
      float rotation = 2 * M_PI * i / (float)FRAMES;
      float sin_rot = sinf(rotation);
      float cos_rot = cosf(rotation);
      for (int x = 0; x < MATRIX_WIDTH; x++)
        for (int y = 0; y < MATRIX_HEIGHT; y++)
        {
          float xx = (float)(x - MATRIX_WIDTH / 2) * cos_rot
              - (float)(y - MATRIX_HEIGHT / 2) * sin_rot;
          float yy = (float)(x - MATRIX_WIDTH / 2) * sin_rot
              + (float)(y - MATRIX_HEIGHT / 2) * cos_rot;

          float xxx = fabs(xx);
          float yyy = fabs(yy);
          float max = xxx > yyy ? xxx : yyy;

          if (max > size + 1.0)
            matrix_set(x, y, 0, 0, 0);
          else
          {
            int h = atan2f(yy, xx) * 256.0f / (2.0f * M_PI);
            h += 32;
            h %= 256;
            
            if (max > size)
              matrix_hue(x, y, h, (int)((1.0 - max + size) * 255.0));
            else
              matrix_hue(x, y, h, 155 + (int)((max / size) * 100.0));
          }
        }
      matrix_flush();
      delay(DELAY);
    }
 
  matrix_free();
  return EXIT_SUCCESS;
}
 
//*****************************************************************************
