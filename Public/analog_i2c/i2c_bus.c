#include "analog_i2c.h"
#include "main.h"
#include "eio_pin.h"
/*-----------------------------------------------------------------------
该文件主要初始化
I2C总线相关信息

需要外部配置文件来配置到底有几条I2C总线
总线的IO具体

对外提供的信息:
i2c_bus_t* i2c_bus_register(char* bus_name,char *io_scl_name,char* io_sda_name)
------------------------------------------------------------------------*/


/*------------------------------定义I2C总线对象----------------------------*/
const char *i2c_bus_nubmer = "i2c_01";
static i2c_bus_t i2c_bus1;

eio_pin_t i2c_gpio_scl;
eio_pin_t i2c_gpio_sda;


/*---------------------------------总线IO口的具体实现---------------------------------*/
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
/*------------------------------------------------------------------------*/



/*-------------------------I2C总线的初始化------------------------------------*/
static void _gpio_set_scl(const char* bus_name,int8_t state)
{
    // HAL_GPIO_WritePin(me->data.gpio_x,
    //                         me->data.pin,
    //                         status ? GPIO_PIN_SET : GPIO_PIN_RESET);    
	eio_pin_set_status(&i2c_gpio_scl,state);
}
i2c_bus_t* i2c_bus_register(const char* bus_name,const char *io_scl_name,const char* io_sda_name)
{
    i2c_bus_t *i2c_bus;
    i2c_bus = malloc(sizeof(i2c_bus_t));



	i2c_bus->set_sda = gpio_set_sda;
	i2c_bus->get_sda = gpio_get_sda;
	i2c_bus->set_scl = gpio_set_scl;
	i2c_bus->set_sda_in = gpio_set_in;
	i2c_bus->set_sda_out = gpio_set_sda_out;
	i2c_bus->delayus = gpio_delayus;
    i2c_bus->name = bus_name;
    return i2c_bus;
}

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
/*------------------------------------------------------------------------*/






