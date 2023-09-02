#include "analog_i2c.h"
#include "main.h"
// #include "stm32f0xx.h"
#define	I2C1_SDA_PORT	GPIOB
#define	I2C1_SDA_PIN	GPIO_PIN_9
#define I2C1_SDA_RCC	RCC_AHBPeriph_GPIOB

#define	I2C1_SCL_PORT	GPIOA
#define	I2C1_SCL_PIN	GPIO_PIN_15
#define I2C1_SCL_RCC	RCC_AHBPeriph_GPIOA

/*定义I2C总线对象*/
static i2c_bus_t i2c_bus1;

/* i2c1 diver */
static void gpio_set_sda(int8_t state)
{
	if (state)
	{	
		HAL_GPIO_WritePin(I2C1_SDA_PORT,I2C1_SDA_PIN,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(I2C1_SDA_PORT,I2C1_SDA_PIN,GPIO_PIN_RESET);
	}
}

static void gpio_set_scl(int8_t state)
{
	if (state)
	{	
		HAL_GPIO_WritePin(I2C1_SCL_PORT,I2C1_SCL_PIN,GPIO_PIN_SET);
	}
	else
	{	
		HAL_GPIO_WritePin(I2C1_SCL_PORT,I2C1_SCL_PIN,GPIO_PIN_RESET);
	}
}

static unsigned char gpio_get_sda(void)
{
	return HAL_GPIO_ReadPin(I2C1_SDA_PORT,I2C1_SDA_PIN);
}

static void gpio_set_sda_out(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = I2C1_SDA_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(I2C1_SDA_PORT, &GPIO_InitStruct);
}
static void gpio_set_in(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = I2C1_SDA_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(I2C1_SDA_PORT, &GPIO_InitStruct);
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
i2c_bus_t* hw_i2c_init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = I2C1_SCL_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(I2C1_SCL_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = I2C1_SDA_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(I2C1_SDA_PORT, &GPIO_InitStruct);  
	
	HAL_GPIO_WritePin(I2C1_SDA_PORT,I2C1_SDA_PIN,GPIO_PIN_SET);
	HAL_GPIO_WritePin(I2C1_SDA_PORT,I2C1_SDA_PIN,GPIO_PIN_SET);
	
	i2c_bus1.set_sda = gpio_set_sda;
	i2c_bus1.get_sda = gpio_get_sda;
	i2c_bus1.set_scl = gpio_set_scl;
	i2c_bus1.set_sda_in = gpio_set_in;
	i2c_bus1.set_sda_out = gpio_set_sda_out;
	i2c_bus1.delayus = gpio_delayus;
	
	return &i2c_bus1;
}






