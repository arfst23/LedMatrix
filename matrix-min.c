//*****************************************************************************

#include "matrix.h"
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <assert.h>

#define R 146
#define G 134
#define B 250

#define Y (R * G / 255)
#define C (G * B / 255)
#define M (R * B / 255)

static inline void delay(int ms)
{
  usleep(ms * 1000);
}

//*****************************************************************************

int main()
{
  matrix_create();

  matrix_set(0, 0, 255, 255, 255);

  for (int i = 1; i < 128; i++)
  {
    matrix_set(i, 0, R, 0, 0);
    matrix_set(0, i, 0, G, 0);
    matrix_set(i, i, 0, 0, B);
  }

  matrix_flush();
  delay(20000);

  matrix_free();
  return EXIT_SUCCESS;
}

//*****************************************************************************
