
char as5600_open(void)
{
  return 0;
}
char as5600_close(void)
{
  return 0;
}
char as5600_getdata(float data)
{
  return 0;
}
char as5600_process()
{
  
}






typedef char (*open_t)(void);
typedef char (*close_t)(void);

typedef struct sensor
{
  /* data */
  void *pdata;
  /*opt*/
  open_t *open;
  close_t *close;
}sensor_t;

sensor_t sensor_as5600;
sensor_t sensor_buf[2];
void sensor_init(void)
{
  sensor_as5600.open = as5600_open;
  sensor_as5600.close = as5600_close;
  sensor_buf[0] = sensor_as5600;
}


