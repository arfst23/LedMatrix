//*****************************************************************************

#include "matrix.h"
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <math.h>

#define FRAMES 100
#define DELAY 30

static inline void delay(int ms)
{
  usleep(ms * 1000);
}

//*****************************************************************************

int main()
{
  matrix_create();
  
  for (;;)
    for (int i = 0; i < FRAMES; i++)
    {
      float rotation = 2 * M_PI * i / (float)FRAMES;
      float sin_rot = sinf(rotation);
      float cos_rot = cosf(rotation);
      for (int x = 0; x < MATRIX_WIDTH; x++)
        for (int y = 0; y < MATRIX_HEIGHT; y++)
        {
          int xx = (x - MATRIX_WIDTH / 2) * cos_rot - (y - MATRIX_HEIGHT / 2) * sin_rot + 0.5;
          int yy = (x - MATRIX_WIDTH / 2) * sin_rot + (y - MATRIX_HEIGHT / 2) * cos_rot + 0.5;
          int xxx = xx + MATRIX_WIDTH / 2;
          int yyy = yy + MATRIX_HEIGHT / 2;
          if (xxx < 10 || xxx > MATRIX_WIDTH - 10
              || yyy < 10 || yyy > MATRIX_HEIGHT - 10)
            matrix_set_g(x, y, 0);
          else
          {
            int h = atan2f((float)yy, (float)xx) * 256.0f / (2.0f * M_PI);
            h += 32;
            h %= 256;
            int v = (abs(xx) > abs(yy) ? abs(xx) : abs(yy)) * 127.0f / 22.0f + 128;
            matrix_set_hv(x, y, h, v);
          }
        }
      matrix_flush();
      delay(DELAY);
    }

  matrix_free();
  return EXIT_SUCCESS;
}

//*****************************************************************************