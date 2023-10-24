#include "analog_i2c.h"
#include "analog_i2c_cfg.h"
/*-----------------------------------------------------------------------
���ļ���Ҫ��ʼ��
I2C���������Ϣ

��Ҫ�ⲿ�����ļ������õ����м���I2C����
���ߵ�IO����

�����ṩ����Ϣ:
i2c_bus_t* i2c_bus_register(char* bus_name,char *io_scl_name,char* io_sda_name)
------------------------------------------------------------------------*/
/*---------------------------------����IO�ڵľ���ʵ��---------------------------------*/
static void gpio_set_sda(gpio_pin_t *gpio,int8_t state)
{
	gpio_set_pinstatus(gpio,state);
}
static void gpio_set_scl(gpio_pin_t *gpio,int8_t state)
{
	gpio_set_pinstatus(gpio,state);
}

static unsigned char gpio_get_sda(gpio_pin_t *gpio)
{
	return (gpio_get_pinstatus(gpio));
}

static void gpio_set_sda_out(gpio_pin_t *gpio)
{
	gpio_set_pinmode(gpio,"B.09",PIN_MODE_OUTPUT);
}
static void gpio_set_in(gpio_pin_t *gpio)
{
 	gpio_set_pinmode(gpio,"B.09",PIN_MODE_INPUT);
}

static void gpio_delayus(unsigned int Time)
{
#if 1  /* ����ϵͳ��ʱʱ�����������*/
	int i = 9;
	Time *= i;
	while (Time--);
#else
		Delayus(us);
#endif
}
/*------------------------------------------------------------------------*/



/*-------------------------I2C���ߵĳ�ʼ��------------------------------------*/
i2c_bus_t* i2c_bus_register(const char* bus_name,const char *io_scl_name,const char* io_sda_name)
{
    i2c_bus_t *i2c_bus;
    i2c_bus = malloc(sizeof(i2c_bus_t));

    gpio_pin_data_t *gpio_scl;
    gpio_pin_data_t *gpio_sda;
    gpio_sda = malloc(sizeof(gpio_pin_data_t));
    gpio_scl = malloc(sizeof(gpio_pin_data_t));

	gpio_pininit(gpio_scl,io_scl_name,PIN_MODE_OUTPUT);
	gpio_pininit(gpio_sda,io_sda_name,PIN_MODE_OUTPUT);
	gpio_set_pinstatus(gpio_scl,1);
	gpio_set_pinstatus(gpio_sda,1);

    i2c_bus->sda = gpio_sda;
    i2c_bus->scl = gpio_scl;
	i2c_bus->set_sda = gpio_set_sda;
	i2c_bus->get_sda = gpio_get_sda;
	i2c_bus->set_scl = gpio_set_scl;
	i2c_bus->set_sda_in = gpio_set_in;
	i2c_bus->set_sda_out = gpio_set_sda_out;
	i2c_bus->delayus = gpio_delayus;
    i2c_bus->name = bus_name;
    return i2c_bus;
}

i2c_bus_t* i2c_bus_init(const char *i2c_name)
{
    i2c_bus_register("i2c_01","A.15","B.09");
}
/*------------------------------------------------------------------------*/






