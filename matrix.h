//*****************************************************************************

#pragma once

#include <stdint.h>

#define MATRIX_WIDTH 64
#define MATRIX_HEIGHT 64

//*****************************************************************************

void matrix_create();
void matrix_set_rgb(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b);
void matrix_flush();
void matrix_free();

void matrix_set_hv(uint8_t x, uint8_t y, uint8_t h, uint8_t v);
void matrix_set_g(uint8_t x, uint8_t y, uint8_t v);

//*****************************************************************************
