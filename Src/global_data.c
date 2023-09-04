// #include "fsm.h"
#include "key_fsm.h"


float key_databuf[2] = {0};
unsigned char aht20_databuf[2];
unsigned short mpu6050_databuf[4] = {0};
// typedef struct sensor_data
// {
//   float buf1[2];
//   unsigned char buf2[2];
//   unsigned short buf3[4];
// }sensor_data_t;
// sensor_data_t sensor_data; 

typedef struct key_data
{
  float buf[2];
}key_data_t;
typedef struct aht20_data
{
  unsigned char buf[2];
}aht20_data_t;
typedef struct mpu6050_data
{
  unsigned short buf[2];
}mpu6050_data_t;

typedef struct sensor_data
{
  key_data_t buf1[2];
  aht20_data_t buf2[2];
  mpu6050_data_t buf3[4];
}sensor_data_t;
 

typedef struct sensor
{
  struct{
    unsigned short ts;//传感器调用周期
    unsigned int t0;//上次的时间戳
  };
  fsm_cb_t *fsm_cb;
  sensor_data_t sensor_data;
  void* (*get_data)(void);
  fsm_t* opt;
}sensor_t;

extern fsm_cb_t keyfsm_cb;
/*传感器调用周期   传感器控制块      获取传感器数据         传感器方法*/
sensor_t sensor_list[] = {
  {{100,0},     &keyfsm_cb,     {{0},{0},{0}},   keyfsm_updata,       (fsm_t *)keyfsm_process},//传感器1号
  {{20,0},      0,              {{0},{0},{0}},      0,  0},//2号
};


void sensor_init(void)
{
  // sensor_list[0].opt = (fsm_t *)keyfsm_process;
  // sensor_list[0].fsm_cb = &keyfsm_cb;
  // sensor_list[0].get_data = keyfsm_updata;
}

/*调度所有的传感器*/
void sensor_process(void)
{
  float *pf;
  sensor_t *p = &sensor_list[0];
  p->opt(p->fsm_cb);//更新新数据
  pf = (float*)(p->get_data());//获取更新后的数据
}
