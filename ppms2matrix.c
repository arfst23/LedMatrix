//******************************************************************************

// ffmpeg -i $f -f image2pipe -vcodec ppm - | ppms2sp -x

#include <ppm.h>

#include "matrix.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <math.h>

#define GAMMA 1.2f
#define SKIP 13

int gamma_(int x)
{
  return (int)(powf((float)x / 255.0f, GAMMA) * 255.0f);
}

uint64_t time_ms(void)
{
  struct timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);
  return (uint64_t)ts.tv_sec * 1000L + (uint64_t)ts.tv_nsec / 1000000L;
}

void delay(int ms)
{
  usleep(ms * 1000);
}

//******************************************************************************

int main(int ac, char *av[])
{
  int gamma__[256];
  for (int x = 0; x < 256; x++)
    gamma__[x] = gamma_(x);

  ppm_init(&ac, av);

  assert(ac == 2);
  int frame_time = atoi(av[1]);
  assert(frame_time);

  matrix_create();

  int cols, rows;
  pixval maxval;
  int format;
  ppm_readppminit(stdin, &cols, &rows, &maxval, &format);

  assert(cols == MATRIX_WIDTH);
  assert(rows == MATRIX_HEIGHT);

  pixel **pixels = ppm_allocarray(cols, rows);
  assert(pixels);

  uint64_t t0 = time_ms();

  while (!feof(stdin))
  {
    for (int row = 0; row < rows; ++row)
      ppm_readppmrow(stdin, pixels[row], cols, maxval, format);

    int this[cols];
    int next[cols];
    memset(next, 0, sizeof(this));

    for (int row = 0; row < rows; ++row)
    {
      memcpy(this, next, sizeof(next));
      memset(next, 0, sizeof(next));

      for (int col = 0; col < cols; ++col)
      {
        matrix_set_raw(col, row,
            gamma__[pixels[row][col].r],
            gamma__[pixels[row][col].g],
            gamma__[pixels[row][col].b]);
      }
    }

    matrix_flush();

    if (SKIP)
    {
      char buf[SKIP];
      int n = fread(buf, 1, SKIP, stdin);
      if (n < SKIP)
        break;
    }

    t0 += frame_time;
    uint64_t t = time_ms();
    int dt = (int)(t0 - t);
    if (dt > 0)
    {
      //printf("delay %d\n", dt);
      delay(dt);
    }
    else
    {
      //printf("skip %d\n", -dt);
      t0 = t;
    }
  }

  ppm_freearray(pixels, rows);

  matrix_free();

  return 0;
}

//******************************************************************************
