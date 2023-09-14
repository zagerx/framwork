// #include "fsm.h"
#include "key_fsm.h"
#include "global_data.h"

// unsigned char key_databuf[2] = {0};
// unsigned char aht20_databuf[2];
// unsigned short mpu6050_databuf[4] = {0};

typedef struct key_data
{
  unsigned char key_data;
}key_data_t;

typedef struct sensor_data
{
  unsigned char key_data;
}sensor_data_t;
 
sensor_data_t g_sensor_data;

void set_keysensor_data(unsigned char data)
{
  g_sensor_data.key_data = data;
}


unsigned char get_keysensor_data(void)
{
  return g_sensor_data.key_data;
}




