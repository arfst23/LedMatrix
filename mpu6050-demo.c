//******************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "mpu6050.h"

#define delay(x) usleep(x * 1000)

//******************************************************************************

int main()
{
  accel_gyro_create(ACCEL_RANGE_4, GYRO_RANGE_500);

  while (1)
  {
    printf("accel: %d %d %d  gyro %d %d %d\n",
        accl_x(), accl_y(), accl_z(),
        gyro_x(), gyro_y(), gyro_z());
    delay(200);
  }
  return EXIT_SUCCESS;
}

//******************************************************************************
