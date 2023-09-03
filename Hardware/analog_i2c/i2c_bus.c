#include "analog_i2c.h"
#include "main.h"

const char *i2c_nubmer = "i2c_01";
eio_pin_t i2c_gpio_scl;
eio_pin_t i2c_gpio_sda;




static const GPIO_TypeDef *gpio_table[] =
{
    GPIOA, GPIOB, GPIOC, GPIOD,
};

static void _translate_pin_name(const char *name, eio_pin_data_t *data)
{
    //elab_assert(data != NULL);

    data->gpio_x = (GPIO_TypeDef *)gpio_table[name[0] - 'A'];
    data->pin = (1 << ((uint8_t)((name[2] - '0') * 10 + (name[3] - '0'))));

    if (name[0] == 'A')
    {
        __HAL_RCC_GPIOA_CLK_ENABLE();
    }
    else if (name[0] == 'B')
    {
        __HAL_RCC_GPIOB_CLK_ENABLE();
    }
    else if (name[0] == 'C')
    {
        __HAL_RCC_GPIOC_CLK_ENABLE();
    }
    else if (name[0] == 'D')
    {
        __HAL_RCC_GPIOD_CLK_ENABLE();
    }
}






/*定义I2C总线对象*/
static i2c_bus_t i2c_bus1;

/* i2c1 diver */
static void gpio_set_sda(int8_t state)
{
		HAL_GPIO_WritePin(i2c_gpio_sda.data.gpio_x,
                            i2c_gpio_sda.data.pin,
                            state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

static void gpio_set_scl(int8_t state)
{
		HAL_GPIO_WritePin(i2c_gpio_scl.data.gpio_x,
                            i2c_gpio_scl.data.pin,
                            state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

static unsigned char gpio_get_sda(void)
{
	GPIO_PinState status = HAL_GPIO_ReadPin(i2c_gpio_sda.data.gpio_x, i2c_gpio_sda.data.pin);
	i2c_gpio_sda.status = (status == GPIO_PIN_SET) ? true : false;
	return i2c_gpio_sda.status;
}

static void gpio_set_sda_out(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = i2c_gpio_sda.data.pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(i2c_gpio_sda.data.gpio_x, &GPIO_InitStruct);
}
static void gpio_set_in(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = i2c_gpio_sda.data.pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(i2c_gpio_sda.data.gpio_x, &GPIO_InitStruct);
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
	if(!strcmp(i2c_name,i2c_nubmer))
	{
			_translate_pin_name("A.15",&i2c_gpio_scl);
			_translate_pin_name("B.09",&i2c_gpio_sda);		
	}

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = i2c_gpio_scl.data.pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(i2c_gpio_scl.data.gpio_x, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = i2c_gpio_sda.data.pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(i2c_gpio_sda.data.gpio_x, &GPIO_InitStruct);  
	
	HAL_GPIO_WritePin(i2c_gpio_scl.data.gpio_x,i2c_gpio_scl.data.pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(i2c_gpio_sda.data.gpio_x,i2c_gpio_sda.data.pin,GPIO_PIN_SET);
	
	i2c_bus1.set_sda = gpio_set_sda;
	i2c_bus1.get_sda = gpio_get_sda;

	i2c_bus1.set_scl = gpio_set_scl;

	i2c_bus1.set_sda_in = gpio_set_in;
	i2c_bus1.set_sda_out = gpio_set_sda_out;
	i2c_bus1.delayus = gpio_delayus;

	return &i2c_bus1;
}






