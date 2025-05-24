//******************************************************************************

#include "orient.h"
#include "mpu6050.h"

//******************************************************************************

void orient_create()
{
  accel_gyro_create(ACCEL_RANGE_4, GYRO_RANGE_500);
}

void orient_free()
{
  accel_gyro_free();
}

orient_t orient_get()
{
  int16_t x = accl_x();
  int16_t y = accl_y();
  int16_t z = accl_z();

  x -= -2953;
  y -= 464;
  z -= -4994;

  int16_t xx = (int16_t)((int32_t)x - (int32_t)y * 55 / 16443 - (int32_t)z * 829 / 16810);
  int16_t yy = (int16_t)((int32_t)y - (int32_t)x * -101 / 16435 - (int32_t)z * 57 / 16810);
  int16_t zz = (int16_t)((int32_t)z - (int32_t)x * -813 / 16435 - (int32_t)y * -744 / 16443);

  xx = (int16_t)((int32_t)x * 2048 / 16435);
  yy = (int16_t)((int32_t)y * 2048 / 16443);
  zz = (int16_t)((int32_t)z * 2048 / 16810);

  zz -= -5;

  if (xx > 1000)
    xx = 1000;
  if (xx < -1000)
    xx = -1000;

  if (yy > 1000)
    yy = 1000;
  if (yy < -1000)
    yy = -1000;

  if (zz > 1000)
    zz = 1000;
  if (zz < -1000)
    zz = -1000;

  orient_t orient = { -xx, -yy, zz };
  return orient;
}

//******************************************************************************
