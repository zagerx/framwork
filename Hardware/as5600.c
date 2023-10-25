#include "as5600.h"
#include "./analog_i2c/analog_i2c.h"
#include "stdio.h"
#include "usart.h"
#define AS5600_ADDR                     0X36
#define Angle_Hight_Register_Addr       0x0C //寄存器高位地址
/*-----------定义一个I2C设备-------------------*/
static i2c_dev_t as5600;
/*----------定义一个I2C总线-----------------*/
i2c_bus_info_t as5600_i2cbus = {
    "i2c_bus_number01",
    "A.15",
    "B.09"
};

static void _readmulitbyte(unsigned char *buf,unsigned char len)
{
	i2c_dev_info_t msg;
	msg.addr = AS5600_ADDR;
	msg.flags = I2C_BUS_RD;
	msg.buff = buf;
	msg.size = len;
    as5600.dev_msg = &msg;
	i2c_bus_xfer_read(&as5600);	
}
static void _writemulitbyte(unsigned char *buf,unsigned char len)
{
	i2c_dev_info_t msg;
	msg.addr = AS5600_ADDR;
	msg.flags = I2C_BUS_WR;
	msg.buff = buf;
	msg.size = len;
    as5600.dev_msg = &msg;
	i2c_bus_xfer_write(&as5600);
}

static float _getangle(void)
{
    unsigned char tepbuf[2];
    unsigned char regs_addr = Angle_Hight_Register_Addr;
    float data;
    _writemulitbyte(&regs_addr,1);
    _readmulitbyte(tepbuf,2);
    data = (float)((tepbuf[0] & 0x0f) << 8 | tepbuf[1]);
    return data;
}

/*--------------------------接口函数------------------------------------------*/
char as5600_init(void)
{
	as5600.i2c_phy = (i2c_bus_t*)i2c_bus_init(&as5600_i2cbus);
    return 0;
}
void as5600_process(void)
{
    enum{
        START ,
		MEASURE_START,
    };
    static unsigned char Step = START;
    switch (Step) {
      case START:
        Step = MEASURE_START;
        break;
      case MEASURE_START:
        USER_DEBUG_RTT("%3f\r\n",_getangle());
        break;
    }
}