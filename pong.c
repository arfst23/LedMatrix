//*****************************************************************************
 
#include "matrix.h"
#include "orient.h"

#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#define DELAY 50

static inline void delay(int ms)
{
  usleep(ms * 1000);
}

//*****************************************************************************

int main()
{
  orient_create();
  matrix_create();

  for (;;)
  {
    orient_t orient = orient_get();
    uint8_t x = (uint8_t)(((int)orient.x + 1000) * (MATRIX_WIDTH - 2) / 2000);
    uint8_t y = (uint8_t)(((int)orient.y + 1000) * (MATRIX_HEIGHT - 2) / 2000);
    uint8_t h = (uint8_t)((-(int)orient.z + 10000) * 255 / 2000);
    matrix_set_hv(x, y, h, 255);
    matrix_set_hv(x + 1, y, h, 255);
    matrix_set_hv(x, y + 1, h, 255);
    matrix_set_hv(x + 1, y + 1, h, 255);
    matrix_flush();
    delay(DELAY);
  }

  matrix_free();
  orient_free();
  return EXIT_SUCCESS;
}
 
//*****************************************************************************
