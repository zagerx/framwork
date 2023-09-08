#include "analog_i2c.h"
#include "main.h"
#include "eio_pin.h"

/*定义I2C总线对象*/
const char *i2c_bus_nubmer = "i2c_01";
eio_pin_t i2c_gpio_scl;
eio_pin_t i2c_gpio_sda;
static i2c_bus_t i2c_bus1;

/* i2c1 diver */
static void gpio_set_sda(int8_t state)
{
		eio_pin_set_status(&i2c_gpio_sda,state);
}

static void gpio_set_scl(int8_t state)
{
		eio_pin_set_status(&i2c_gpio_scl,state);
}

static unsigned char gpio_get_sda(void)
{
	return (eio_pin_get_status(&i2c_gpio_sda));
}

static void gpio_set_sda_out(void)
{
	eio_pin_mode(&i2c_gpio_sda,"B.09",PIN_MODE_OUTPUT);
}
static void gpio_set_in(void)
{
 	eio_pin_mode(&i2c_gpio_sda,"B.09",PIN_MODE_INPUT);
}

static void gpio_delayus(unsigned int Time)
{
#if 1  /* 不用系统延时时开启这个！！*/
	int i = 9;
	Time *= i;
	while (Time--);
#else
		Delayus(us);
#endif
}
/*
**	I2C总线初始化
**	
**	返回值:总线对象	
*/
i2c_bus_t* hw_i2c_init(const char *i2c_name)
{
	if(strcmp(i2c_name,i2c_bus_nubmer) != 0)
	{
		return 0;//没有匹配的总线
	}
	eio_pin_init(&i2c_gpio_scl,"A.15",PIN_MODE_OUTPUT);
	eio_pin_init(&i2c_gpio_sda,"B.09",PIN_MODE_OUTPUT);
	eio_pin_set_status(&i2c_gpio_scl,1);
	eio_pin_set_status(&i2c_gpio_sda,1);
	
	i2c_bus1.set_sda = gpio_set_sda;
	i2c_bus1.get_sda = gpio_get_sda;
	i2c_bus1.set_scl = gpio_set_scl;
	i2c_bus1.set_sda_in = gpio_set_in;
	i2c_bus1.set_sda_out = gpio_set_sda_out;
	i2c_bus1.delayus = gpio_delayus;

	return &i2c_bus1;
}






