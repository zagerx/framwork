#include "as5600.h"
#include "./analog_i2c/analog_i2c.h"
#include "stdio.h"

#include "fsm.h"
#undef NULL
#define NULL 0                  
#undef this
#define this (*ptThis)
#define AS5600_RESET_FSM()               \
        do { this.chState = START; } while(0)
static fsm_cb_t as5600_cb;

#define AS5600_ADDR                     0X36
#define Angle_Hight_Register_Addr       0x0C //寄存器高位地址
static i2c_dev_t as5600;
const char *i2c_name = "i2c_01";
static void as5600_readmulitbyte(unsigned char *buf,unsigned char len)
{
	i2c_dev_mesg_t msg;
	msg.addr = AS5600_ADDR;
	msg.flags = I2C_BUS_RD;
	msg.buff = buf;
	msg.size = len;
    as5600.dev_msg = &msg;
	i2c_bus_xfer_read(&as5600,&msg);	
}
static void as5600_writemulitbyte(unsigned char *buf,unsigned char len)
{
	i2c_dev_mesg_t msg;
	msg.addr = AS5600_ADDR;
	msg.flags = I2C_BUS_WR;
	msg.buff = buf;
	msg.size = len;
    as5600.dev_msg = &msg;
	i2c_bus_xfer_write(&as5600,&msg);
}

static float as5600_getangle(void)
{
    unsigned char tepbuf[2];
    unsigned char regs_addr = Angle_Hight_Register_Addr;
    float data;
    as5600_writemulitbyte(&regs_addr,1);
    as5600_readmulitbyte(tepbuf,2);
    data = (float)((tepbuf[0] & 0x0f) << 8 | tepbuf[1]);
    return data;
}
static char as5600fsm_init(fsm_cb_t *ptThis)
{
    if(ptThis == NULL) {
        return -1; 
    }
    this.sig = 0;
    this.chState = START;
    return 0;
}
fsm_rt_t as5600fsm_process(fsm_cb_t *ptThis)
{
    enum {
    START ,
		MEASURE_START = USER,
    };
    if(ptThis == NULL) {
        return fsm_rt_err; 
    }    
    switch (this.chState) {
      case START:
        this.chState = MEASURE_START;
        break;
      case MEASURE_START:
        *(float *)(ptThis->pdata) = as5600_getangle();;
    }
    return fsm_rt_on_going;
}

char as5600_init(void)
{
	as5600.i2c_phy = i2c_bus_init(i2c_name);
    as5600fsm_init(&as5600_cb);
    return 0;
}
char as5600_open(void)
{
  return 0;
}
char as5600_close(void)
{
  return 0;
}
char as5600_updata(void *pdata)
{
  as5600_cb.pdata = pdata;
  as5600fsm_process(&as5600_cb);
  return 0;
}
