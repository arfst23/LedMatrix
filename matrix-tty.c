//******************************************************************************

#include "text.h"
#include "matrix.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

#define AMBER 238, 173, 14
#define LEN 32

static inline void delay(int ms)
{
  usleep(ms * 1000);
}

//******************************************************************************

static char screen[ROWS][COLS];
static int cursor_row;
static int cursor_col;

static void clear(void)
{
  memset(screen, ' ', sizeof(screen));
  cursor_row = 0;
  cursor_col = 0;
}

static void scroll()
{
  memmove(&screen[0][0], &screen[1][0], COLS * (ROWS - 1));
  memset(&screen[ROWS - 1][0], ' ', COLS);
}

static void display()
{
  print_frame(AMBER);
  for (int row = 0; row < ROWS; row++)
    for (int col = 0; col < COLS; col++)
      if (cursor_row == row && cursor_col == col)
	print_chr_rev(row, col, screen[row][col], AMBER);
      else
	print_chr(row, col, screen[row][col], AMBER);
  matrix_flush();
}

static void put(char c)
{
  if (cursor_col < COLS)
    screen[cursor_row][cursor_col] = c;
  cursor_col++;
}

//******************************************************************************

int main(int ac, char *av[])
{
  matrix_create();
  clear();
  display();

  for (;;)
  {
    char buffer[LEN];
    int n = read(STDIN_FILENO, buffer, LEN);
    if (n <= 0) // EOF or error
      break;

    for (int i = 0; i < n; i++)
    {
      char c = buffer[i];
      switch (c)
      {
      case '\r':
        cursor_col = 0;
        break;
      case '\n':
        cursor_row++;
        cursor_col = 0;
        if (cursor_row >= ROWS)
        {
	  scroll();
	  cursor_row--;
        }
        break;
      case '\b':
        if (cursor_col > 0)
	  cursor_col--;
        break;
      case '\f':
        clear();
        break;
      default:
        if (c >= ' ' && c <= '~')
          put(c);
      }
    }
    display();
  }

  matrix_free();
  return EXIT_SUCCESS;
}

//******************************************************************************
