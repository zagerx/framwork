// #include "fsm.h"
#include "key_fsm.h"
#include "global_data.h"

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




