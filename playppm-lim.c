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

//        MAX = 102000
#define LIMITER  25000

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

    // limiter
    int r = 0;
    int g = 0;
    int b = 0;
    for (int row = 0; row < rows; ++row)
      for (int col = 0; col < cols; ++col)
      {
	r += gamma[pixels[row][col].r];
        g += gamma[pixels[row][col].g];
	b += gamma[pixels[row][col].b];
      }
    // max 255 * 128 * 128 < 2^22

    int bright = 35 * r + 40 * g + 25 * b;
    // max 255 * 128 * 128 * 100 < 2^29
    bright >>= 12;
    // max 102000 < 2^17
 
    if (bright <= LIMITER)
    {
      for (int row = 0; row < rows; ++row)
	for (int col = 0; col < cols; ++col)
	{
	  uint8_t r = pixels[row][col].r;
	  uint8_t g = pixels[row][col].g;
	  uint8_t b = pixels[row][col].b;
	  uint8_t m = r < g ? r : g;
          m = m < b ? m : b;
	  m >>= 3;
	  r -= m;
	  g -= m;
	  b -= m;

	  matrix_set(col, row, gamma[r], gamma[g], gamma[b]);
	}
    }
    else
    {
      int q = 1024 * LIMITER / bright;
      for (int row = 0; row < rows; ++row)
	for (int col = 0; col < cols; ++col)
	{
	  uint8_t r = pixels[row][col].r;
	  uint8_t g = pixels[row][col].g;
	  uint8_t b = pixels[row][col].b;
	  uint8_t m = r < g ? r : g;
          m = m < b ? m : b;
	  m >>= 3;
	  r -= m;
	  g -= m;
	  b -= m;

	  matrix_set(col, row,
	    (gamma[r] * q) >> 10,
	    (gamma[g] * q) >> 10,
	    (gamma[b] * q) >> 10);
	}
    }
 
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
