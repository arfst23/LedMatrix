//*****************************************************************************
 
#include "matrix.h"
#include "orient.h"

#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <assert.h>

#define DELAY 25

#define ACCEL 0.00005
#define REFLECTION_DAMP 0.8
#define FRICTION_DAMP 0.99
#define MIN_SPEED 0.02

static inline void delay(int ms)
{
  usleep(ms * 1000);
}

//*****************************************************************************

int main()
{
  orient_create();
  matrix_create();

  float x = (float)MATRIX_WIDTH / 2.0 - 0.5;
  float y = (float)MATRIX_HEIGHT / 2.0 - 0.5;
  float u = 0.0;
  float v = 0.0;

  for (;;)
  {
    orient_t orient = orient_get();
    if (orient.z > 200)
      break;
    
    float a = (float)orient.x * ACCEL;
    float b = (float)orient.y * ACCEL;

    u *= FRICTION_DAMP;
    v *= FRICTION_DAMP;

    u += a;
    v += b;

    x += u;
    y += v;

    if (x < 0.0)
    {
      x = -x * REFLECTION_DAMP;
      u = -u * REFLECTION_DAMP;
    }
    if (x > (float)MATRIX_WIDTH)
    {
      x = (float)MATRIX_WIDTH - (x - (float)MATRIX_WIDTH) * REFLECTION_DAMP;
      u = -u * REFLECTION_DAMP;
    }

    if (y < 0.0)
    {
      y = -y * REFLECTION_DAMP;
      v = -v * REFLECTION_DAMP;
    }
    if (y > (float)MATRIX_HEIGHT)
    {
      y = (float)MATRIX_HEIGHT - (y - (float)MATRIX_HEIGHT) * REFLECTION_DAMP;
      v = -v * REFLECTION_DAMP;
    }

    if (u > 4.0)
      u = 4.0;
    if (u < -4.0)
      u = -4.0;

    if (v > 4.0)
      v = 4.0;
    if (v < -4.0)
      v = -4.0;

    float speed = sqrtf(u * u + v * v);

    if (fabsf(speed) < MIN_SPEED)
    {
      u = 0.0;
      v = 0.0;
    }

    speed *= 60.0;
    if (speed >= 96.0)
      speed = 96.0;
    uint8_t h = (uint8_t)speed + 3 * 64;

    uint8_t xx = (uint8_t)x;
    if (xx >= MATRIX_WIDTH)
      xx = MATRIX_WIDTH - 1;

    uint8_t yy = (uint8_t)y;
    if (yy >= MATRIX_HEIGHT)
      yy = MATRIX_HEIGHT - 1;

    matrix_set_hv(xx, yy, h, 255);
    matrix_flush();

    delay(DELAY);
  }

  matrix_free();
  orient_free();
  return EXIT_SUCCESS;
}
 
//*****************************************************************************
