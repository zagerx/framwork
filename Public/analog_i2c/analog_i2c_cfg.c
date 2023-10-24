
#include "analog_i2c.h"
typedef struct xxx 
{
    const char *i2c_name;
    const char *i2c_scl;
    const char *i2c_sda;
}xxx_t;

static xxx_t* i2c_bus_arry[I2C_BUS_MAX] = {
                                            {"i2c_bus_01","B.09","B.10"}     \
                                        };


