//*****************************************************************************

#include "matrix.h"
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <assert.h>

#define DELAY 100

static inline void delay(int ms)
{
  usleep(ms * 1000);
}

//*****************************************************************************

int main()
{
  matrix_create();

  for (int x = 0; x < MATRIX_WIDTH; x++)
    for (int y = 0; y < MATRIX_HEIGHT; y++)
      matrix_set(x, y, 0, 0, 0);
  matrix_flush();

  delay(DELAY);

  matrix_free();
  return EXIT_SUCCESS;
}

//*****************************************************************************
