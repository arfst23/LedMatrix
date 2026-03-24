//*****************************************************************************

#pragma once

#include "matrix.h"
#include <stdint.h>

#define SIZE 8
#define ROWS (MATRIX_HEIGHT / SIZE)
#define COLS (MATRIX_WIDTH / SIZE)

//*****************************************************************************

void print_chr(int row, int col, char c, uint8_t r, uint8_t g, uint8_t b);
void print_str(int row, const char *str, uint8_t r, uint8_t g, uint8_t b);

//*****************************************************************************
