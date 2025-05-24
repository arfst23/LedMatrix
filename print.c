//******************************************************************************

#include "matrix.h"

#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#define FONT_MISC 10

static inline void delay(int ms)
{
  usleep(ms * 1000);
}

//******************************************************************************

static uint8_t font[11][5] =
{
  { // 0
    0b010,
    0b101,
    0b101,
    0b101,
    0b010,
  },
  { // 1
    0b010,
    0b110,
    0b010,
    0b010,
    0b010,
  },
  { // 2
    0b010,
    0b101,
    0b001,
    0b010,
    0b111,
  },
  { // 3
    0b110,
    0b001,
    0b010,
    0b001,
    0b110,
  },
  { // 4
    0b100,
    0b101,
    0b111,
    0b001,
    0b001,
  },
  { // 5
    0b111,
    0b100,
    0b110,
    0b001,
    0b110,
  },
  { // 6
    0b010,
    0b100,
    0b110,
    0b101,
    0b010,
  },
  { // 7
    0b111,
    0b001,
    0b001,
    0b010,
    0b010,
  },
  { // 8
    0b010,
    0b101,
    0b010,
    0b101,
    0b010,
  },
  { // 9
    0b010,
    0b101,
    0b011,
    0b001,
    0b010,
  },
  { // x
    0b000,
    0b010,
    0b101,
    0b010,
    0b000,
  },
};

//******************************************************************************

void print_char(int i, int x, int y, int h)
{
  for (int r = 0; r < 5; r++)
    for (int c = 0; c < 3; c++)
      if ((font[i][r] << c) & 0b100)
        matrix_set_hv(x + c, y + r, h, 200);
}

int main(int ac, char *av[])
{
  assert(ac == 2);
  const char *s = av[1];

  matrix_create();
  int x = 0;
  while (*s && x < MATRIX_WIDTH)
  {
    int i = *s >= '0' && *s <= '9' ? *s - '0' : FONT_MISC;
    int h = i == FONT_MISC ? 0 : 128;
    print_char(i, x, 0, h);
    x += 4;
    s++;
  }

  matrix_flush();
  delay(10000);
  matrix_free();
  return EXIT_SUCCESS;
}

//******************************************************************************
