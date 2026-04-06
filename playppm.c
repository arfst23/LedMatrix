//******************************************************************************

// ffmpeg -i $f -f image2pipe -vcodec ppm - | ppms2matrix

#include "matrix.h"

#include <stdint.h>
#include "gamma.h"

#include <ppm.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <assert.h>

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
  ppm_init(&ac, av);

  assert(ac == 2);
  int frame_time = atoi(av[1]);
  assert(frame_time);
 
  matrix_create();
 
  pixel **pixels = ppm_allocarray(MATRIX_WIDTH, MATRIX_HEIGHT);
  assert(pixels);

  uint64_t t0 = time_ms();
  while (!feof(stdin))
  {
    int c = getchar();
    if (c == EOF)
      break;
    ungetc(c, stdin);

    int cols, rows;
    pixval maxval;
    int format;
    ppm_readppminit(stdin, &cols, &rows, &maxval, &format);
    assert(cols == MATRIX_WIDTH);
    assert(rows == MATRIX_HEIGHT);
 
    for (int row = 0; row < rows; ++row)
      ppm_readppmrow(stdin, pixels[row], cols, maxval, format);
 
    for (int row = 0; row < rows; ++row)
      for (int col = 0; col < cols; ++col)
        matrix_set(col, row,
          gamma[pixels[row][col].r],
          gamma[pixels[row][col].g],
          gamma[pixels[row][col].b]);
 
    t0 += frame_time;
    uint64_t t = time_ms();
    int dt = (int)(t0 - t);
    if (dt > 0)
      delay(dt);
    else
      t0 = t;

    matrix_flush();
  }
 
  ppm_freearray(pixels, MATRIX_HEIGHT);
  matrix_free();
 
  return 0;
}

//******************************************************************************
