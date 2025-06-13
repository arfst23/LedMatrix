//*****************************************************************************

#include "matrix.h"
#include "orient.h"

#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <stdbool.h>

#define SAND 1024
#define ACCEL 0.00001
#define DELAY 50

// Add collision grid
static bool occupied[MATRIX_WIDTH][MATRIX_HEIGHT];

static inline void delay(int ms)
{
  usleep(ms * 1000);
}

typedef struct
{
  float x;
  float y;
  float u;
  float v;
} sand_t;

static sand_t sand[SAND];

//*****************************************************************************

static void sand_create()
{
  for (int i = 0; i < SAND; i++)
  {
    sand[i].x = (float)rand() * (float)MATRIX_WIDTH / (float)(RAND_MAX);
    sand[i].y = (float)rand() * (float)MATRIX_HEIGHT / (float)(RAND_MAX);
    sand[i].u = 0.0;
    sand[i].v = 0.0;
  }
}

//*****************************************************************************

static void sand_update(orient_t accel)
{
  for (int i = 0; i < SAND; i++)
  {
    sand[i].u += accel.x * ACCEL;
    sand[i].v += accel.y * ACCEL;
    if (accel.z > 0.0)
    {
      float friction = 1.0 - (float)accel.z * 0.0001;
      sand[i].u *= friction;
      sand[i].v *= friction;
    }
    sand[i].x += sand[i].u;
    sand[i].y += sand[i].v;
    if (sand[i].x < 0.0)
    {
      sand[i].x = 0.0;
      sand[i].u = 0.0;
    }
    if (sand[i].x > MATRIX_WIDTH)
    {
      sand[i].x = MATRIX_WIDTH;
      sand[i].u = 0.0;
    }
    if (sand[i].y < 0.0)
    {
      sand[i].y = 0.0;
      sand[i].v = 0.0;
    }
    if (sand[i].y > MATRIX_HEIGHT)
    {
      sand[i].y = MATRIX_HEIGHT;
      sand[i].v = 0.0;
    }
  }
}

//*****************************************************************************

static void sand_show()
{
  for (int i = 0; i < SAND; i++)
  {
    int x = (int)sand[i].x;
    if (x == MATRIX_WIDTH)
      x = MATRIX_WIDTH - 1;
    assert(x >= 0);
    assert(x < MATRIX_WIDTH);

    int y = (int)sand[i].y;
    if (y == MATRIX_HEIGHT)
      y = MATRIX_HEIGHT - 1;
    assert(y >= 0);
    assert(y < MATRIX_HEIGHT);

    matrix_set_hv(x, y, 128, 255);
  }
}

//*****************************************************************************

int main()
{
  orient_create();
  matrix_create();
  sand_create();

  for (;;)
  {
    orient_t accel = orient_get();
    sand_update(accel);
    sand_show();

    matrix_flush();
    delay(DELAY);
  }

  matrix_free();
  orient_free();
  return EXIT_SUCCESS;
}

//*****************************************************************************