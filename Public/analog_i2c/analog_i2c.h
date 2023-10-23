#ifndef __ANALOG_I2C__H
#define __ANALOG_I2C__H
#include "stdint.h"
#include "stdbool.h"
#include "main.h"
#define I2C_BUS_WR            0x0000
#define I2C_BUS_RD			  0x0001
typedef struct i2c_dev_mesg
{
    unsigned short  addr;
    unsigned short  flags;
    unsigned short  size;
    unsigned char   *buff;
    unsigned char   retries;
}i2c_dev_mesg_t;


/*-----------------------I2C总线-------------------------------------------------*/
typedef struct i2c_bus
{
	/*总线操作方法*/
	void (*set_sda)(int8_t state);
    void (*set_scl)(int8_t state);
	unsigned char (*get_sda)(void);
	void (*delayus)(uint32_t us);
	void (*set_sda_out)(void);
	void (*set_sda_in)(void);

	/*总线名字*/
	const char *name;//哪个I2C总线
}i2c_bus_t;
/*------------------------------------------------------------------------------*/


typedef struct i2c_dev{
	i2c_bus_t *i2c_phy;
}i2c_dev_t;

int i2c_bus_xfer_read(i2c_dev_t *i2c_dev,i2c_dev_mesg_t *msgs);
int i2c_bus_xfer_write(i2c_dev_t *i2c_dev,i2c_dev_mesg_t *msgs);

int i2c_bitops_bus_xfer_read(i2c_bus_t *i2c_bus,i2c_dev_mesg_t *msgs);
int i2c_bitops_bus_xfer_write(i2c_bus_t *i2c_bus,i2c_dev_mesg_t *msgs);

extern i2c_bus_t* hw_i2c_init(const char *i2c_name);


#endif
