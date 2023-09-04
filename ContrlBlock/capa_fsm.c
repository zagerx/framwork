#include "fsm.h"
#include "capa_fsm.h"

fsm_cb_t capa_cb;
fsm_rt_t capa_fsm_process(fsm_cb_t *me);
static volatile float buf[2];
fsm_cb_t* capa_fsm_creat(void)
{
  capa_cb.fsm = (fsm_t*)capa_fsm_process;
  capa_cb.chState = START;
  capa_cb.sig = 0;
  return &capa_cb;
}

fsm_rt_t capa_fsm_process(fsm_cb_t *me)
{
    enum {
    WAIT = USER,
    RUNING,
    };
    switch (me->chState)
    {
    case START:
    case WAIT:
      /*启动电机的事件发生 */
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

void* capa_updata(void)
{
  return (void *)buf;
}


