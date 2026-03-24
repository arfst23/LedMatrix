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

  for (int y = 0; y < 64; y++)
  {
    for (int x = 0; x < 43; x++)
      matrix_set(x, y, R, 0, 0);
    for (int x = 43; x < 85; x++)
      matrix_set(x, y, 0, G, 0);
    for (int x = 85; x < 128; x++)
      matrix_set(x, y, 0, 0, B);
  }

  for (int y = 64; y < 128; y++)
  {
    for (int x = 0; x < 43; x++)
      matrix_set(x, y, Y, Y, 0);
    for (int x = 43; x < 85; x++)
      matrix_set(x, y, 0, C, C);
    for (int x = 85; x < 128; x++)
      matrix_set(x, y, M, 0, M);
  }


  matrix_flush();
  delay(20000);

  matrix_free();
  return EXIT_SUCCESS;
}

//*****************************************************************************
