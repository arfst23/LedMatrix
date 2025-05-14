//******************************************************************************

#include "mpu6050.h"

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <linux/i2c-dev.h>
#include <i2c/smbus.h>
#include <assert.h>

// sudo apt install i2c-tools
// i2cdetect -y 1
// i2cset -y 1 0x68 0x6b 0x00
// watch -n 0.5 'i2cget -y 1 0x68 0x44'

// https://github.com/grizzlei/mpu6050/blob/master/src/mpu6050.c

#define ACCEL_GYRO_ADDR 0x68
#define ACCEL_GYRO_DEV "/dev/i2c-1"

static int accel_gyro_fh = -1;

//******************************************************************************

void accel_gyro_create(int accel_range, int gyro_range)
{
  assert(accel_gyro_fh < 0);
  accel_gyro_fh = open(ACCEL_GYRO_DEV, O_RDWR);
  assert(accel_gyro_fh >= 0);

  int rc = ioctl(accel_gyro_fh, I2C_SLAVE, ACCEL_GYRO_ADDR);
  assert(rc == 0);

  // Take MPU6050 out of sleep mode
  i2c_smbus_write_byte_data(accel_gyro_fh, 0x6b, 0b00000000);

  // Set DLPF (low pass filter) to 44Hz
  // so no noise above 44Hz will pass through
  i2c_smbus_write_byte_data(accel_gyro_fh, 0x1a, 0b00000011);

  // Set sample rate divider (to 200Hz)
  i2c_smbus_write_byte_data(accel_gyro_fh, 0x19, 0b00000100);

  // Configure accelerometer settings
  i2c_smbus_write_byte_data(accel_gyro_fh, 0x1c, accel_range);

  // Configure gyroscope settings
  i2c_smbus_write_byte_data(accel_gyro_fh, 0x1b, gyro_range);

  // Set offsets to zero
  i2c_smbus_write_byte_data(accel_gyro_fh, 0x06, 0b00000000);
  i2c_smbus_write_byte_data(accel_gyro_fh, 0x07, 0b00000000);
  i2c_smbus_write_byte_data(accel_gyro_fh, 0x08, 0b00000000);
  i2c_smbus_write_byte_data(accel_gyro_fh, 0x09, 0b00000000);
  i2c_smbus_write_byte_data(accel_gyro_fh, 0x0A, 0b00000000);
  i2c_smbus_write_byte_data(accel_gyro_fh, 0x0B, 0b00000000);
  i2c_smbus_write_byte_data(accel_gyro_fh, 0x00, 0b10000001);
  i2c_smbus_write_byte_data(accel_gyro_fh, 0x01, 0b00000001);
  i2c_smbus_write_byte_data(accel_gyro_fh, 0x02, 0b10000001);
}

void accel_gyro_free()
{
  assert(accel_gyro_fh >= 0);
  close(accel_gyro_fh);
  accel_gyro_fh = -1;
}

//******************************************************************************

static int16_t accel_gyro_read(uint8_t addr)
{
  assert(accel_gyro_fh >= 0);
  return i2c_smbus_read_byte_data(accel_gyro_fh, addr) << 8
      | i2c_smbus_read_byte_data(accel_gyro_fh, addr + 1);
}

int16_t accl_x()
{
  return accel_gyro_read(0x3b);
}

int16_t accl_y()
{
  return accel_gyro_read(0x3d);
}

int16_t accl_z()
{
  return accel_gyro_read(0x3f);
}

int16_t gyro_x()
{
  return accel_gyro_read(0x43);
}

int16_t gyro_y()
{
  return accel_gyro_read(0x45);
}

int16_t gyro_z()
{
  return accel_gyro_read(0x47);
}

//******************************************************************************
