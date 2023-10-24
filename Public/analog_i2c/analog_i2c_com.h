#ifndef __ANALOG_I2C_COM__H
#define __ANALOG_I2C_COM__H
#include "analog_i2c.h"
int i2c_bitops_bus_xfer_read(i2c_bus_t *i2c_bus,i2c_dev_info_t *info);
int i2c_bitops_bus_xfer_write(i2c_bus_t *i2c_bus,i2c_dev_info_t *info);
i2c_bus_t* i2c_bus_register(const char* bus_name,const char *io_scl_name,const char* io_sda_name);
#endif