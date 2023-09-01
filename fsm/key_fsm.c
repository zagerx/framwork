#include "fsm.h"

fsm_cb_t keyfsm_cb;

fsm_rt_t keyfsm_process(fsm_cb_t *me);
static volatile float buf[2];
fsm_cb_t* keyfsm_creat(void)
{
  keyfsm_cb.fsm = (fsm_t*)keyfsm_process;
  keyfsm_cb.state = START;
  keyfsm_cb.sig = 0;
  return &keyfsm_cb;
}

fsm_rt_t keyfsm_process(fsm_cb_t *me)
{
    enum {
    WAIT = USER,
    };
    switch (me->state)
    {
    case START:
    case WAIT:
      if (me->sig == 1)//����������
      {
        me->state = EXIT;
        buf[0] = 0.78f;
        buf[1] = 0.89f;
        // me->pdata = (void *)buf;
      }
      break;
    case EXIT:
      break;
    default:
      break;
    }
    return fsm_rt_cpl;
}

void* keyfsm_updata(void)
{
  return (void *)buf;
}