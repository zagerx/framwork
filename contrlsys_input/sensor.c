#include "usart.h"
#include "as5600.h"
typedef void (*init_t)(void);
typedef char (*open_t)(void);
typedef char (*close_t)(void);
typedef char (*updata_t)(void *);
typedef struct sensor
{
  /* data */
  void *pdata;
  unsigned char len;
  /*opt*/
  init_t init;
  open_t open;
  close_t close;
  updata_t updata;
}sensor_t;

static float as5600_angle; 
sensor_t sensor_as5600;
sensor_t sensor_buf[2];
void sensor_init(void)
{
    sensor_as5600.open = as5600_open;
    sensor_as5600.close = as5600_close;
    sensor_as5600.updata = as5600_updata;
    sensor_as5600.init = as5600_init;
    sensor_as5600.pdata = &as5600_angle;

    sensor_as5600.init();
    sensor_buf[0] = sensor_as5600;
}

void sensor_actor(void)
{
  updata_t pupdata;
  pupdata = (as5600_updata);
  (sensor_as5600.updata)((void *)sensor_as5600.pdata);
//   USER_DEBUG("angle = %f\r\n",*((float *)sensor_as5600.pdata));
}
