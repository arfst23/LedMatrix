//******************************************************************************

#include "matrix.h"

#include "stdint.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "time.h"

#define length(x) (sizeof(x) / sizeof(*x))
#define delay(ms) usleep(ms * 1000)
#define rnd(x) (rand() % x)

#define MINRUN 2
#define MAXRUN 8
#define MINOFF 6
#define MAXOFF 20

#define DELAY 120

#define COLS 22
#define ROWS 16

#define BLACK 0
#define DIM 1
#define GREEN 2
#define WHITE 3

uint8_t colors[][3] =
{
  [BLACK] = { 0, 0, 0 },
  [DIM] = { 0, 90, 0 },
  [GREEN] = { 0, 170, 0 },
  [WHITE] = { 130, 130, 130 },
};

//******************************************************************************
// >>> letters[][7]

uint8_t letters[][7] =
{
  { // 0
    0b01111,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b01111,
  },
  { // 1
    0b11111,
    0b00000,
    0b11111,
    0b10000,
    0b10000,
    0b01000,
    0b00100,
  },
  { // 2
    0b11000,
    0b00100,
    0b00010,
    0b00001,
    0b00001,
    0b10001,
    0b11111,
  },
  { // 3
    0b01110,
    0b10001,
    0b10000,
    0b01000,
    0b00110,
    0b00001,
    0b11111,
  },
  { // 4
    0b00111,
    0b01000,
    0b10000,
    0b11111,
    0b10000,
    0b01000,
    0b00111,
  },
  { // 5
    0b00000,
    0b10001,
    0b01010,
    0b11111,
    0b01010,
    0b10001,
    0b00000,
  },
  { // 6
    0b00000,
    0b00000,
    0b01110,
    0b00000,
    0b01110,
    0b00000,
    0b00000,
  },
  { // 7
    0b11111,
    0b00100,
    0b00100,
    0b01110,
    0b10101,
    0b10101,
    0b10101,
  },
  { // 8
    0b00000,
    0b00011,
    0b00100,
    0b11111,
    0b00100,
    0b11000,
    0b00000,
  },
  { // 9
    0b01110,
    0b10001,
    0b10011,
    0b10101,
    0b11001,
    0b10001,
    0b01110,
  },
  { // 10
    0b10000,
    0b01000,
    0b00100,
    0b10010,
    0b00100,
    0b01000,
    0b10000,
  },
  { // 11
    0b00100,
    0b00000,
    0b00100,
    0b00010,
    0b00001,
    0b10001,
    0b01110,
  },
  { // 12
    0b10001,
    0b10001,
    0b01000,
    0b00100,
    0b00010,
    0b10001,
    0b10001,
  },
  { // 13
    0b00001,
    0b00010,
    0b00110,
    0b01010,
    0b11010,
    0b01010,
    0b10001,
  },
  { // 14
    0b10001,
    0b10001,
    0b10001,
    0b11111,
    0b10001,
    0b10001,
    0b01110,
  },
  { // 15
    0b00100,
    0b11111,
    0b00100,
    0b11111,
    0b00100,
    0b11111,
    0b00100,
  },
  { // 16
    0b11111,
    0b10000,
    0b01000,
    0b00100,
    0b00010,
    0b00001,
    0b11111,
  },
  { // 17
    0b01000,
    0b11110,
    0b01001,
    0b01001,
    0b01001,
    0b11110,
    0b01000,
  },
  { // 18
    0b11110,
    0b00001,
    0b00001,
    0b01010,
    0b00100,
    0b00010,
    0b11111,
  },
  { // 19
    0b01110,
    0b10001,
    0b10001,
    0b01110,
    0b10001,
    0b10001,
    0b01110,
  },
  { // 20
    0b11111,
    0b00001,
    0b00001,
    0b01111,
    0b00001,
    0b00001,
    0b11111,
  },
  { // 21
    0b11101,
    0b00000,
    0b00000,
    0b01110,
    0b00000,
    0b00000,
    0b11111,
  },
  { // 22
    0b01111,
    0b01000,
    0b01000,
    0b01000,
    0b01000,
    0b10100,
    0b10100,
  },
  { // 23
    0b10001,
    0b01001,
    0b00101,
    0b00011,
    0b00101,
    0b01001,
    0b10001,
  },
  { // 24
    0b01110,
    0b10001,
    0b10001,
    0b10111,
    0b10111,
    0b10000,
    0b01111,
  },
  { // 25
    0b00001,
    0b00011,
    0b00101,
    0b01001,
    0b11111,
    0b00001,
    0b00001,
  },
  { // 26
    0b01000,
    0b01001,
    0b01010,
    0b00100,
    0b01100,
    0b10010,
    0b00001,
  },
  { // 27
    0b00111,
    0b00000,
    0b00111,
    0b10000,
    0b10000,
    0b01000,
    0b00111,
  },
  { // 28
    0b01111,
    0b10010,
    0b10010,
    0b10010,
    0b01110,
    0b00010,
    0b00111,
  },
  { // 29
    0b00000,
    0b00000,
    0b00100,
    0b01110,
    0b00100,
    0b00000,
    0b00000,
  },
  { // 30
    0b11111,
    0b10001,
    0b00100,
    0b00100,
    0b00100,
    0b10001,
    0b11111,
  },
  { // 31
    0b01111,
    0b10000,
    0b10000,
    0b01110,
    0b00001,
    0b00001,
    0b11110,
  },
};

#define LETTERS length(letters)

// <<<
//******************************************************************************

void print_letter(uint8_t row, uint8_t col, uint8_t letter, uint8_t color)
{
  int x = 6 * col - 1;
  int y = 8 * row;

  for (int i = 0; i < 6 - 1; i++)
  {
    if (x + i < 0)
      continue;
    if (x + i >= MATRIX_WIDTH)
      break;
    for (int j = 0; j < 8 - 1; j++)
    {
      uint8_t c = (letters[letter][j] << i) & 0b10000
        ? color : BLACK;
      matrix_set(x + i, y + j, colors[c][0], colors[c][1], colors[c][2]);
    }
  }
}

uint8_t letter[ROWS][COLS];
uint8_t color[ROWS][COLS];

void print_all()
{
  for (int row = 0; row < ROWS; row++)
    for (int col = 0; col < COLS; col++)
      print_letter(row, col, letter[row][col], color[row][col]);
}

//******************************************************************************

int count[COLS];

void step()
{
  for (int row = ROWS - 1; row > 0; row--)
    for (int col = 0; col < COLS; col++)
    {
      color[row][col] = color[row - 1][col];
      if (color[row][col] == WHITE)
	letter[row][col] = rnd(LETTERS);
    }

  for (int col = 0; col < COLS; col++)
    switch (color[0][col])
    {
    case BLACK:
      count[col]--;
      if (count[col] < 0)
      {
	color[0][col] = WHITE;
	letter[0][col] = rnd(LETTERS);
      }
      break;

    case DIM:
      color[0][col] = BLACK;
      count[col] = MINOFF - 1 + rnd(MAXOFF - MINOFF + 1);
      break;

    case GREEN:
      count[col]--;
      if (count[col] < 0)
	color[0][col] = DIM;
      break;

    case WHITE:
      color[0][col] = GREEN;
      count[col] = MINRUN - 1 + rnd(MAXRUN - MINRUN + 1);
      break;
    }
}

//******************************************************************************
void pertube()
{
  int row = rnd(ROWS);
  int col = rnd(COLS);
  if (color[row][col] == GREEN)
    letter[row][col] = rnd(LETTERS);
}

//******************************************************************************

int main()
{
  srand(time(NULL));
  matrix_create();
  memset(letter, 0, sizeof(letter));
  memset(color, 0, sizeof(color));

  for (int col = 0; col < COLS; col++)
  {
    int run = MINRUN + rnd(MAXRUN - MINRUN + 1);
    int off = MINOFF + rnd(MAXOFF - MINOFF + 1);
    count[col] = rnd(run + off + 1);
  }

  for (;;)
  {
    step();
    pertube();
    print_all();
    matrix_flush();
    delay(DELAY);
  }

  matrix_free();
  return EXIT_SUCCESS;
}

//******************************************************************************
