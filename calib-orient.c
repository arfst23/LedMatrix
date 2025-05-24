//******************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "mpu6050.h"

#define delay(x) usleep(x * 1000)

#define SAMPLES 512
#define DELAY 10

typedef struct
{
  int16_t x;
  int16_t y;
  int16_t z;
} accl_t;

//******************************************************************************

static accl_t sample()
{
  int32_t x = 0;
  int32_t y = 0;
  int32_t z = 0;

  for (int i = 0; i < SAMPLES; i++)
  {
    x += accl_x();
    y += accl_y();
    z += accl_z();
    delay(DELAY);
  }

  accl_t accl;
  accl.x = (int16_t)(x / SAMPLES);
  accl.y = (int16_t)(y / SAMPLES);
  accl.z = (int16_t)(z / SAMPLES);
  return accl;
}

//******************************************************************************

static accl_t average(const accl_t *accl, int n)
{
  int32_t x = 0;
  int32_t y = 0;
  int32_t z = 0;

  for (int i = 0; i < n; i++)
  {
    x += accl[i].x;
    y += accl[i].y;
    z += accl[i].z;
  }
  
  accl_t avg;
  avg.x = (int16_t)(x / n);
  avg.y = (int16_t)(y / n);
  avg.z = (int16_t)(z / n);
  return avg;
}

//******************************************************************************

static void swap(accl_t *a, accl_t *b)
{
  if (a == b)
    return;
  accl_t c = *a;
  *a = *b;
  *b = c;
}

//******************************************************************************

static void sort(accl_t *accl)
{
  {
    int max_x = accl[0].x;
    int max_i = 0;
    for (int i = 1; i < 6; i++)
      if (accl[i].x > max_x)
      {
        max_x = accl[i].x;
        max_i = i;
      }
    swap(accl, accl + max_i);

    int min_x = accl[1].x;
    int min_i = 1;
    for (int i = 2; i < 6; i++)
      if (accl[i].x < min_x)
      {
        min_x = accl[i].x;
        min_i = i;
      }
    swap(accl + 1, accl + min_i);
  }
  {
    int max_y = accl[2].y;
    int max_i = 2;
    for (int i = 3; i < 6; i++)
      if (accl[i].y > max_y)
      {
        max_y = accl[i].y;
        max_i = i;
      }
    swap(accl + 2, accl + max_i);

    int min_y = accl[3].y;
    int min_i = 3;
    for (int i = 4; i < 6; i++)
      if (accl[i].y < min_y)
      {
        min_y = accl[i].y;
        min_i = i;
      }
    swap(accl + 3, accl + min_i);
  }
  {
    int max_z = accl[4].z;
    int max_i = 4;
    if (accl[5].z > max_z)
    {
      max_z = accl[5].z;
      max_i = 5;
    }
    swap(accl + 4, accl + max_i);
  }
}

//******************************************************************************

int main()
{
  accl_t accl[6];

  accel_gyro_create(ACCEL_RANGE_4, GYRO_RANGE_500);
  for (int i = 0; i < 6; i++)
  {
    printf("[%d]", i);
    getchar();
    printf("sampling ...\n");
    accl[i] = sample();
    printf("done\n");
    printf("\n");
  }
  accel_gyro_free();

  accl_t avg = average(accl, 6);
  printf("x -= %d;\n", avg.x);
  printf("y -= %d;\n", avg.y);
  printf("z -= %d;\n", avg.z);

  for (int i = 0; i < 6; i++)
  {
    accl[i].x -= avg.x;
    accl[i].y -= avg.y;
    accl[i].z -= avg.z;
  }

  sort(accl);

  for (int i = 0; i < 6; i++)
    printf("[%d]  %4d  %4d  %4d\n", i, accl[i].x, accl[i].y, accl[i].z);

  int32_t delta_x = accl[0].x - accl[1].x;
  int32_t delta_y = accl[2].y - accl[3].y;
  int32_t delta_z = accl[4].z - accl[5].z;

  int32_t delta_x_y = accl[2].x - accl[3].x;
  int32_t delta_x_z = accl[4].x - accl[5].x;
  int32_t delta_y_x = accl[0].y - accl[1].y;
  int32_t delta_y_z = accl[4].y - accl[5].y;
  int32_t delta_z_x = accl[0].z - accl[1].z;
  int32_t delta_z_y = accl[2].z - accl[3].z;

  printf("int16_t xx = (int16_t)((int32_t)x - (int32_t)y * %d / %d - (int32_t)z * %d / %d);\n",
      delta_x_y, delta_y, delta_x_z, delta_z);
  printf("int16_t yy = (int16_t)((int32_t)y - (int32_t)x * %d / %d - (int32_t)z * %d / %d);\n",
      delta_y_x, delta_x, delta_y_z, delta_z);
  printf("int16_t zz = (int16_t)((int32_t)z - (int32_t)x * %d / %d - (int32_t)y * %d / %d);\n",
      delta_z_x, delta_x, delta_z_y, delta_y);

  accl_t norm[6];
  for (int i = 0; i < 6; i++)
  {
    norm[i].x = (int16_t)((int32_t)accl[i].x - (int32_t)accl[i].y * delta_x_y / delta_y
        - (int32_t)accl[i].z * delta_x_z / delta_z);
    norm[i].y = (int16_t)((int32_t)accl[i].y - (int32_t)accl[i].x * delta_y_x / delta_x
        - (int32_t)accl[i].z * delta_y_z / delta_z);
    norm[i].z = (int16_t)((int32_t)accl[i].z - (int32_t)accl[i].x * delta_z_x / delta_x
        - (int32_t)accl[i].y * delta_z_y / delta_y);
  }

  for (int i = 0; i < 6; i++)
    printf("[%d]  %4d  %4d  %4d\n", i, norm[i].x, norm[i].y, norm[i].z);

  printf("xx = (int16_t)((int32_t)x * 2048 / %d);\n", delta_x);
  printf("yy = (int16_t)((int32_t)y * 2048 / %d);\n", delta_y);
  printf("zz = (int16_t)((int32_t)z * 2048 / %d);\n", delta_z);

  for (int i = 0; i < 6; i++)
  {
    norm[i].x = (int16_t)((int32_t)norm[i].x * 2048 / delta_x);
    norm[i].y = (int16_t)((int32_t)norm[i].y * 2048 / delta_y);
    norm[i].z = (int16_t)((int32_t)norm[i].z * 2048 / delta_z);
  }

  for (int i = 0; i < 6; i++)
    printf("[%d]  %4d  %4d  %4d\n", i, norm[i].x, norm[i].y, norm[i].z);

  int16_t xxx = (int16_t)(((int32_t)norm[2].x + (int32_t)norm[3].x
      + (int32_t)norm[4].x + (int32_t)norm[5].x) / 4);
  int16_t yyy = (int16_t)(((int32_t)norm[0].y + (int32_t)norm[1].y
      + (int32_t)norm[4].y + (int32_t)norm[5].y) / 4);
  int16_t zzz = (int16_t)(((int32_t)norm[0].z + (int32_t)norm[1].z
      + (int32_t)norm[2].z + (int32_t)norm[3].z) / 4);
  
  printf("xx -= %d;\n", xxx);
  printf("yy -= %d;\n", yyy);
  printf("zz -= %d;\n", zzz);

  for (int i = 0; i < 6; i++)
  {
    norm[i].x -= xxx;
    norm[i].y -= yyy;
    norm[i].z -= zzz;
  }

  for (int i = 0; i < 6; i++)
    printf("[%d]  %4d  %4d  %4d\n", i, norm[i].x, norm[i].y, norm[i].z);

  return EXIT_SUCCESS;
}

//******************************************************************************
