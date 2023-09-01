#include "fsm.h"
#include "pid.h"


fsm_cb_t motor_cb;
fsm_rt_t motor_process(fsm_cb_t *me);
static volatile float buf[2];
fsm_cb_t* motor_creat(void)
{
  motor_cb.fsm = (fsm_t*)motor_process;
  motor_cb.state = START;
  motor_cb.sig = 0;
  return &motor_cb;
}

fsm_rt_t motor_process(fsm_cb_t *me)
{
    enum {
    WAIT = USER,
    RUNING,
    };
    
    switch (me->state)
    {
    case START:
    case WAIT:
      /*关闭电机 */
      break;
    case RUNING:
      /*获取目标流量*/
      /*查看当前流量*/
      /*计算如何爬坡*/
      /*爬坡结束*/
      /*给电机设置对应的信号  驱动电机*/
      break;      
    case EXIT:
      break;
    default:
      break;
    }
    return fsm_rt_cpl;
}

void* motor_updata(void)
{
  return (void *)buf;
}


