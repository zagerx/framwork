#include "as5600.h"
#include "./analog_i2c/analog_i2c.h"
#include "stdio.h"

#define AS5600_ADDR 0X36
#define Angle_Hight_Register_Addr 0x0C //寄存器高位地址
static i2c_dev_t as5600;
const char *as5600_scl = "A.15";
const char *as5600_sda = "B.09";
const char *i2c_name = "i2c_01";
static void as5600_readmulitbyte(unsigned char *buf,unsigned char len)
{
	i2c_dev_mesg_t msg;
	msg.addr = AS5600_ADDR;
	msg.flags = I2C_BUS_RD;
	msg.buff = buf;
	msg.size = len;
	i2c_bus_xfer_read(&as5600,&msg);	
}
static void as5600_writemulitbyte(unsigned char *buf,unsigned char len)
{
	i2c_dev_mesg_t msg;
	msg.addr = AS5600_ADDR;
	msg.flags = I2C_BUS_WR;
	msg.buff = buf;
	msg.size = len;
	i2c_bus_xfer_write(&as5600,&msg);
}
void as5600_init(void)
{
	as5600.i2c_phy = hw_i2c_init(i2c_name);
}

float as5600_getangle(void)
{
    unsigned char tepbuf[2];
    unsigned char regs_addr = 0x0C;
    float data;
    as5600_writemulitbyte(&regs_addr,1);
    as5600_readmulitbyte(tepbuf,2);
    data = (float)((tepbuf[0] & 0x0f) << 8 | tepbuf[1]);
    return data;
}


