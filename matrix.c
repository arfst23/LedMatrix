//******************************************************************************

#include "matrix.h"

#include "rgbmatrix.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#include <math.h>

//******************************************************************************

static const char *matrix_argv[] =
{
  "",
  "--led-gpio-mapping=adafruit-hat-pwm",
  "--led-cols=64",
  "--led-rows=64",
  "--led-chain=4",
  "--led-pixel-mapper=U-mapper",
  NULL
};

static struct RGBLedMatrixOptions matrix_options;
static struct RGBLedMatrix *matrix = NULL;
static struct LedCanvas *matrix_buffer;

//******************************************************************************

void matrix_create(void)
{
  assert(!matrix);
  memset(&matrix_options, 0, sizeof(matrix_options));
  matrix = led_matrix_create_from_options_const_argv(&matrix_options,
    6, (char**)matrix_argv);
  assert(matrix);
  matrix_buffer = led_matrix_create_offscreen_canvas(matrix);
}

//******************************************************************************

void matrix_set(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b)
{
  assert(matrix);

  if (x < MATRIX_WIDTH / 2)
  {
    x = MATRIX_WIDTH / 2 - 1 - x;
    y = y < MATRIX_HEIGHT / 2 ? MATRIX_HEIGHT / 2 - 1 - y
      : 3 * MATRIX_HEIGHT / 2 - 1 - y;
  }

  // begin color correction
  if (x < MATRIX_WIDTH / 2 && y < MATRIX_HEIGHT / 2)
  {
    float bb = (float)b / 255.0 * 0.93;
    b = (uint8_t)(bb * 255.0 + 0.5);
  }
  else
  {
    uint8_t y = r < g ? r : g;
    float yy = (float)y / 255.0;
    yy *= 0.07;
    y = (uint8_t)(yy * 255.0 / 2.0 + 0.5);
    r -= y;
    g -= y;
  }
  // end color correction

  led_canvas_set_pixel(matrix_buffer,
    y & (MATRIX_HEIGHT - 1),
    MATRIX_WIDTH - 1 - (x & (MATRIX_WIDTH - 1)),
    r, g, b);
}

//******************************************************************************

void matrix_flush(void)
{
  assert(matrix);
  matrix_buffer = led_matrix_swap_on_vsync(matrix, matrix_buffer);
  led_canvas_clear(matrix_buffer);
}

//******************************************************************************

void matrix_free(void)
{
  assert(matrix);
  matrix_flush();
  matrix_flush();
  led_matrix_delete(matrix);
  matrix = NULL;
}

//******************************************************************************
