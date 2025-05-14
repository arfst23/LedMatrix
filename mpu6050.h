//******************************************************************************

#pragma once

#include <stdint.h>

// accel range @ g
#define ACCEL_RANGE_2 0b00000000
#define ACCEL_RANGE_4 0b00001000
#define ACCEL_RANGE_8 0b00010000
#define ACCEL_RANGE_16 0b00011000

// gyro range @ dps2
#define GYRO_RANGE_250 0b00000000 
#define GYRO_RANGE_500 0b00001000
#define GYRO_RANGE_1000 0b00010000
#define GYRO_RANGE_2000 0b00011000

//******************************************************************************

void accel_gyro_create(int accel_range, int gyro_range);
void accel_gyro_free();

int16_t accl_x();
int16_t accl_y();
int16_t accl_z();
int16_t gyro_x();
int16_t gyro_y();
int16_t gyro_z();

//******************************************************************************
