//*****************************************************************************

#include "matrix.h"
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <assert.h>

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
          float xx = (x - MATRIX_WIDTH / 2) * cos_rot - (y - MATRIX_HEIGHT / 2) * sin_rot;
          float yy = (x - MATRIX_WIDTH / 2) * sin_rot + (y - MATRIX_HEIGHT / 2) * cos_rot;
          float xxx = xx + MATRIX_WIDTH / 2;
          float yyy = yy + MATRIX_HEIGHT / 2;
          if (xxx <= 9.0 || xxx >= MATRIX_WIDTH - 9.0
              || yyy <= 9.0 || yyy >= MATRIX_HEIGHT - 9.0)
            matrix_set_g(x, y, 0);
          else
          {
            int h = atan2f(yy, xx) * 256.0f / (2.0f * M_PI);
            h += 32;
            h %= 256;
	    if (xxx < 10.0 || xxx > MATRIX_WIDTH - 10.0
	      || yyy < 10.0 || yyy > MATRIX_HEIGHT - 10.0)
	    {
	      float v = xxx < 10.0 ? xxx - 9.0
		  : xxx > MATRIX_WIDTH - 10.0 ? MATRIX_WIDTH - 9 - xxx
		  : yyy < 10.0 ? yyy - 9.0 
		  : yyy > MATRIX_HEIGHT - 10.0 ? MATRIX_HEIGHT - 9 - yyy
		  : 0.0;
	      int vv = 255.9 * v;
	      matrix_set_hv(x, y, h, vv);
	    }
	    else
	    {
	      int v = (fabsf(xx) > fabsf(yy) ? fabsf(xx) : fabsf(yy)) * 127.0f / 22.0f + 128;
	      matrix_set_hv(x, y, h, v);
	    }
          }
        }
      matrix_flush();
      delay(DELAY);
    }

  matrix_free();
  return EXIT_SUCCESS;
}

//*****************************************************************************
