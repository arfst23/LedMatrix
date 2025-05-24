//*****************************************************************************

#include "matrix.h"
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#include <stdio.h>
#include <math.h>

#define GAMMA 1.6f

static inline void delay(int ms)
{
  usleep(ms * 1000);
}

//*****************************************************************************

float gamma_(float x)
{
  return powf(x, GAMMA);
}


int main()
{
  matrix_create();

  for (int i = 0; i < 64; i++)
  {
    float x = (float)i / 63.0f; 
    x = gamma_(x);
    int k = (int)(255.0 * x + 0.5);

    matrix_set_raw(0, i, k, k, k);
    matrix_set_raw(1, i, k, k, k);

    matrix_set_raw(4, i, k, 0, 0);
    matrix_set_raw(5, i, k, 0, 0);

    matrix_set_raw(8, i, k, k, 0);
    matrix_set_raw(9, i, k, k, 0);

    matrix_set_raw(12, i, 0, k, 0);
    matrix_set_raw(13, i, 0, k, 0);

    matrix_set_raw(16, i, 0, k, k);
    matrix_set_raw(17, i, 0, k, k);

    matrix_set_raw(20, i, 0, 0, k);
    matrix_set_raw(21, i, 0, 0, k);

    matrix_set_raw(24, i, k, 0, k);
    matrix_set_raw(25, i, k, 0, k);

    k = i << 2;

    matrix_set_rgb(36, i, k, k, k);
    matrix_set_rgb(37, i, k, k, k);

    matrix_set_rgb(40, i, k, 0, 0);
    matrix_set_rgb(41, i, k, 0, 0);

    matrix_set_rgb(44, i, k, k, 0);
    matrix_set_rgb(45, i, k, k, 0);

    matrix_set_rgb(48, i, 0, k, 0);
    matrix_set_rgb(49, i, 0, k, 0);

    matrix_set_rgb(52, i, 0, k, k);
    matrix_set_rgb(53, i, 0, k, k);

    matrix_set_rgb(56, i, 0, 0, k);
    matrix_set_rgb(57, i, 0, 0, k);

    matrix_set_rgb(60, i, k, 0, k);
    matrix_set_rgb(61, i, k, 0, k);
  };

  matrix_flush();

  printf("static uint8_t gamma[256] =\n");
  printf("{\n");
  for (int i = 0; i < 256; i++)
  {
    float x = (float)i / 255.0f;
    x = gamma_(x);
    int k = (int)(255.0 * x + 0.5);

    printf("  [%d] = %d,\n", i, k);
  }
  printf("};\n");

  delay(10000);

  matrix_free();
  return EXIT_SUCCESS;
}
 
//*****************************************************************************
