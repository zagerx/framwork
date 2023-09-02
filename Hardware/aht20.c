#include "aht20.h"
// #include "systick.h"
// #include "stm32f0xx.h"
#include "./analog_i2c/analog_i2c.h"
#include "stdio.h"

#if 0
#define AHT10_ADDRESS 0x70
#define AHT10_WRITE 0x70
#define AHT10_READ 0x71

#define AHT10_I2C_SCL_PIN      					GPIO_Pin_6
#define AHT10_I2C_SCL_GPIO_PORT   	    GPIOB  
#define AHT10_I2C_SCL_GPIO_RCC      		RCC_AHBPeriph_GPIOB  
 
#define AHT10_I2C_SDA_PIN      					GPIO_Pin_7   
#define AHT10_I2C_SDA_GPIO_PORT   	    GPIOB  
#define AHT10_I2C_SDA_GPIO_RCC      		RCC_AHBPeriph_GPIOB  

/* Private define ------------------------------------------------------------*/
#define AHT10_SCL_H   			GPIO_WriteBit(AHT10_I2C_SCL_GPIO_PORT,AHT10_I2C_SCL_PIN,Bit_SET)   
#define AHT10_SCL_L   			GPIO_WriteBit(AHT10_I2C_SCL_GPIO_PORT,AHT10_I2C_SCL_PIN,Bit_RESET)
   
#define AHT10_SDA_H         GPIO_WriteBit(AHT10_I2C_SDA_GPIO_PORT,AHT10_I2C_SDA_PIN,Bit_SET)
#define AHT10_SDA_L         GPIO_WriteBit(AHT10_I2C_SDA_GPIO_PORT,AHT10_I2C_SDA_PIN,Bit_RESET)

#define AHT10_READ_SDA      GPIO_ReadInputDataBit(AHT10_I2C_SDA_GPIO_PORT,AHT10_I2C_SDA_PIN)
static void delay_us(unsigned char Time)
{
	int i = 9;
	Time *= i;
	while (Time--);
}

#define AHT10_Delay_us(time)				delay_us(time)
#define AHT10_Delay_ms(time)				HAL_Delay(time)

//初始化IIC
static void AHT10_IIC_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;	
	RCC_AHBPeriphClockCmd(AHT10_I2C_SDA_GPIO_RCC, ENABLE);
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//硬件无上拉
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = AHT10_I2C_SDA_PIN;
	GPIO_Init(AHT10_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);
	
	RCC_AHBPeriphClockCmd(AHT10_I2C_SCL_GPIO_RCC, ENABLE);
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//硬件无上拉
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = AHT10_I2C_SCL_PIN;
	GPIO_Init(AHT10_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);
	

	AHT10_SDA_H;
	AHT10_SCL_H;
}

static void AHT10_SDA_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	RCC_AHBPeriphClockCmd(AHT10_I2C_SDA_GPIO_RCC, ENABLE);
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//硬件无上拉
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = AHT10_I2C_SDA_PIN;
	GPIO_Init(AHT10_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);
}

static void AHT10_SDA_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	RCC_AHBPeriphClockCmd(AHT10_I2C_SDA_GPIO_RCC, ENABLE);
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//硬件无上拉
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = AHT10_I2C_SDA_PIN;
	GPIO_Init(AHT10_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);
}










//产生IIC起始信号
static void AHT10_IIC_Start(void)
{
	AHT10_SDA_OUT();     //sda线输出
	AHT10_SDA_H;	  	  
	AHT10_SCL_H;
	AHT10_Delay_us(4);
 	AHT10_SDA_L;//START:when CLK is high,DATA change form high to low 
	AHT10_Delay_us(4);
	AHT10_SCL_L;//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
static void AHT10_IIC_Stop(void)
{
	AHT10_SDA_OUT();//sda线输出
	AHT10_SCL_L;
	AHT10_SDA_L;//STOP:when CLK is high DATA change form low to high
 	AHT10_Delay_us(4);
	AHT10_SCL_H; 
	AHT10_SDA_H;//发送I2C总线结束信号
	AHT10_Delay_us(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
static uint8_t AHT10_IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	AHT10_SDA_IN();      //SDA设置为输入  
	AHT10_SDA_H;
	AHT10_Delay_us(1);	   
	AHT10_SCL_H;
	AHT10_Delay_us(1);	 
	while(AHT10_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			AHT10_IIC_Stop();
			return 1;
		}
	}
	AHT10_SCL_L;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
static void AHT10_IIC_Ack(void)
{
	AHT10_SCL_L;
	AHT10_SDA_OUT();
	AHT10_SDA_L;
	AHT10_Delay_us(2);
	AHT10_SCL_H;
	AHT10_Delay_us(2);
	AHT10_SCL_L;
}
//不产生ACK应答		    
static void AHT10_IIC_NAck(void)
{
	AHT10_SCL_L;
	AHT10_SDA_OUT();
	AHT10_SDA_H;
	AHT10_Delay_us(2);
	AHT10_SCL_H;
	AHT10_Delay_us(2);
	AHT10_SCL_L;
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
static void AHT10_IIC_Send_Byte(uint8_t txd)
{                        
    uint8_t t;   
	AHT10_SDA_OUT(); 	    
    AHT10_SCL_L;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        //IIC_SDA=(txd&0x80)>>7;
		if((txd&0x80)>>7)
			AHT10_SDA_H;
		else
			AHT10_SDA_L;
		txd<<=1; 	  
		AHT10_Delay_us(2);   //对TEA5767这三个延时都是必须的
		AHT10_SCL_H;
		AHT10_Delay_us(2); 
		AHT10_SCL_L;	
		AHT10_Delay_us(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
static uint8_t AHT10_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	AHT10_SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        AHT10_SCL_L; 
        AHT10_Delay_us(2);
		AHT10_SCL_H;
        receive<<=1;
        if(AHT10_READ_SDA)receive++;   
			AHT10_Delay_us(1); 
    }					 
    if (!ack)
        AHT10_IIC_NAck();//发送nACK
    else
        AHT10_IIC_Ack(); //发送ACK   
    return receive;
}



/**
brief 检查AHT10是否存在
param NONE
return 0存在  1不存在
*/
uint8_t AHT10Check(void)
{
	uint8_t ack=0;
	AHT10_IIC_Start();
	AHT10_IIC_Send_Byte(AHT10_ADDRESS);
	ack=AHT10_IIC_Wait_Ack();
	AHT10_IIC_Stop();	
	return ack;
}

/**
brief AHT10软复位
param NONE
return NONE
*/
void AHT10Reset(void)
{
	AHT10_IIC_Start();
	AHT10_IIC_Send_Byte(AHT10_WRITE);
	AHT10_IIC_Wait_Ack();
	AHT10_IIC_Send_Byte(0xba);
	AHT10_IIC_Wait_Ack();
	AHT10_IIC_Stop();	
}



/**
brief AHT10初始化函数
param NONE
return NONE
*/
void AHT10Init()
{
	AHT10_IIC_Init();
	
	AHT10_IIC_Start();
	AHT10_IIC_Send_Byte(AHT10_ADDRESS);
	AHT10_IIC_Send_Byte(0xbe);	
	AHT10_IIC_Send_Byte(0x08);
	AHT10_IIC_Send_Byte(0x00);
	AHT10_IIC_Stop();	
	AHT10_Delay_ms(40);//延时20ms让传感器稳定
	
}

/**
brief 检查AHT10读温湿度数据
param *temperature：需要读出的温度数据，uint16_t指针类型,精度范围+-0.3C
param *humidity：需要读出的湿度数据，uint16_t指针类型,精度范围+-2RH
return 1 读数据正常 0读数据失败
*/
uint8_t AHT10ReadData(uint16_t *temperature,uint16_t *humidity)
{
	uint8_t ack;
	uint32_t SRH=0,ST=0;
	uint8_t databuff[6];
	AHT10_IIC_Start();
	AHT10_IIC_Send_Byte(AHT10_WRITE);
	AHT10_IIC_Wait_Ack();
	AHT10_IIC_Send_Byte(0xac);
	AHT10_IIC_Wait_Ack();
	AHT10_IIC_Send_Byte(0x33);
	AHT10_IIC_Wait_Ack();
	AHT10_IIC_Send_Byte(0x00);
	AHT10_IIC_Wait_Ack();
	AHT10_IIC_Stop(); 
	AHT10_Delay_ms(80);//延时一会等待数据读出
	AHT10_IIC_Start();
	AHT10_IIC_Send_Byte(AHT10_READ);
	AHT10_IIC_Wait_Ack();
	ack=AHT10_IIC_Read_Byte(1);
	if((ack&0x40)==0)
	{
		databuff[0]=AHT10_IIC_Read_Byte(1);
		databuff[1]=AHT10_IIC_Read_Byte(1);
		databuff[2]=AHT10_IIC_Read_Byte(1);
		databuff[3]=AHT10_IIC_Read_Byte(1);
		databuff[4]=AHT10_IIC_Read_Byte(0);
		AHT10_IIC_Stop();
		SRH=(databuff[0]<<12)+(databuff[1]<<4)+(databuff[2]>>4);
		ST=((databuff[2]&0X0f)<<16)+(databuff[3]<<8)+(databuff[4]);
		*humidity = SRH*1000/1024/1024;  //计算得到湿度值（放大了10倍,如果c1=523，表示现在湿度为52.3%）
	  *temperature = ST *200*10/1024/1024-500;//计算得到温度值（放大了10倍，如果t1=245，表示现在温度为24.5℃）
		//为读取的数据更稳定，还可以使用平均值滤波或者窗口滤波，或者前面读取的值与后面的值相差不能太大。
		return 1;
	}
	AHT10_IIC_Stop();	
	return 0;
}




#else 

static i2c_dev_t aht200;
static void aht20_readmulitbyte(unsigned char *buf,unsigned char len)
{
	i2c_dev_mesg_t msg;
	msg.addr = 0x38;
	msg.flags = I2C_BUS_RD;
	msg.buff = buf;
	msg.size = len;
	i2c_bus_xfer_read(&aht200,&msg);	
}
static void aht20_writemulitbyte(unsigned char *buf,unsigned char len)
{
	i2c_dev_mesg_t msg;
	msg.addr = 0x38;
	msg.flags = I2C_BUS_WR;
	msg.buff = buf;
	msg.size = len;
	i2c_bus_xfer_write(&aht200,&msg);
}
unsigned char aht20_get_statusbyte(void)
{
	i2c_dev_mesg_t msg;
	unsigned char byte;
	msg.addr = 0x38;
	msg.flags = I2C_BUS_RD;
	msg.buff = &byte;
	msg.size = 1;
	i2c_bus_xfer_read(&aht200,&msg);
	byte = msg.buff[0];
	return byte;
}

void aht20_start_ceasure(void)
{
	unsigned char buf[3] = {0};
	i2c_dev_mesg_t msg;
	buf[0] = 0xAC;
	buf[1] = 0x33;
	buf[2] = 0x00;
	msg.addr = 0x38;
	msg.flags = I2C_BUS_WR;
	msg.buff = buf;
	msg.size = 3;
	aht20_writemulitbyte(buf,3);
}
#if 0
unsigned char aht20_get_temp_humi(float *data)
{
	uint8_t statues;
	uint32_t SRH=0,ST=0;
	uint8_t databuff[6];
	uint8_t temp_buf[6];
	uint16_t temperature, humidity;
	
	aht20_start_ceasure();
	HAL_Delay(80);
	aht20_readmulitbyte((unsigned char*)temp_buf,6);
	statues = temp_buf[0];
	/*获取状态字判断是否忙*/
	if(temp_buf[0]&0x80)
	{
		printf("aht20 busy\r\n");
	}else{
		for(unsigned char i = 0;i<5;i++)
		{
			databuff[i] = temp_buf[i+1];
		}
		SRH=(databuff[0]<<12)+(databuff[1]<<4)+(databuff[2]>>4);
		ST=((databuff[2]&0X0f)<<16)+(databuff[3]<<8)+(databuff[4]);
		data[1] = (float)(SRH*1000/1024/1024);  //计算得到湿度值（放大了10倍,如果c1=523，表示现在湿度为52.3%）
		data[0] = (float)(ST *200*10/1024/1024-500);//计算得到温度值（放大了10倍，如果t1=245，表示现在温度为24.5℃）		
	}
	return statues;
}
#else
unsigned char aht20_get_temp_humi(float *data)
{
	uint8_t statues = 0;
	uint32_t SRH=0,ST=0;
	uint8_t databuff[6];
	uint8_t temp_buf[6];
	uint16_t temperature, humidity;	
	aht20_readmulitbyte((unsigned char*)temp_buf,6);
	
	/*获取状态字判断是否忙*/
	statues = temp_buf[0];
	for(unsigned char i = 0;i<5;i++)
	{
		databuff[i] = temp_buf[i+1];
	}
	SRH=(databuff[0]<<12)+(databuff[1]<<4)+(databuff[2]>>4);
	ST=((databuff[2]&0X0f)<<16)+(databuff[3]<<8)+(databuff[4]);
	data[1] = (float)(SRH*1000/1024/1024);  //计算得到湿度值（放大了10倍,如果c1=523，表示现在湿度为52.3%）
	data[0] = (float)(ST *200*10/1024/1024-500);//计算得到温度值（放大了10倍，如果t1=245，表示现在温度为24.5℃）		
	return statues;	
}
#endif
void aht20_test(void)
{
	float buf[2];
	aht20_init();
	while(1)
	{		
		aht20_get_temp_humi(buf);
		printf("%f  %f\r\n",buf[0],buf[1]);
		HAL_Delay(500);			
	}
}
void aht20_init(void)
{
	unsigned char data = 0x25;	
//	HAL_Delay(30);
	aht200.i2c_phy = hw_i2c_init();
	// data = aht20_get_statusbyte();
	// printf("data = %x\r\n",data);
}
#endif

