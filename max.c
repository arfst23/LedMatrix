//*****************************************************************************

#include "matrix.h"
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <assert.h>

#define DELAY 10

static inline void delay(int ms)
{
  usleep(ms * 1000);
}

//*****************************************************************************

int main()
{
  matrix_create();

  for (int v = 0; v < 256; v++)
  {
    for (int x = 0; x < MATRIX_WIDTH; x++)
      for (int y = 0; y < MATRIX_HEIGHT; y++)
        matrix_set_g(x, y, v);
    matrix_flush();
    delay(DELAY);
  }
  delay(3000);

  for (int v = 254; v >= 0; v--)
  {
    for (int x = 0; x < MATRIX_WIDTH; x++)
      for (int y = 0; y < MATRIX_HEIGHT; y++)
        matrix_set_g(x, y, v);
    matrix_flush();
    delay(DELAY);
  }

  matrix_free();
  return EXIT_SUCCESS;
}

//*****************************************************************************
