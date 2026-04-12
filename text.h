//*****************************************************************************

#pragma once

#include "matrix.h"
#include <stdint.h>

#define ROWS 12
#define COLS 17

//*****************************************************************************

void print_frame(uint8_t r, uint8_t g, uint8_t b);
void print_chr(int row, int col, char c, uint8_t r, uint8_t g, uint8_t b);
void print_chr_rev(int row, int col, char c, uint8_t r, uint8_t g, uint8_t b);
void print_str(int row, const char *str, uint8_t r, uint8_t g, uint8_t b);

//*****************************************************************************
