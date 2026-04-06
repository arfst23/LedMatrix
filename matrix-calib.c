//*****************************************************************************

#include "matrix.h"
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <assert.h>

static inline void delay(int ms)
{
  usleep(ms * 1000);
}

//*****************************************************************************

int main()
{
  matrix_create();

  for (int y = 0; y < MATRIX_HEIGHT; y++)
    for (int x = 0; x < MATRIX_WIDTH; x++)
      matrix_set(x, y, 150, 150, 150);

  matrix_flush();
  delay(50000);

  matrix_free();
  return EXIT_SUCCESS;
}

//*****************************************************************************
