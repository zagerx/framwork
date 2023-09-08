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
      /*�رյ�� */
      break;
    case RUNING:
      /*��ȡĿ������*/
      /*�鿴��ǰ����*/
      /*�����������*/
      /*���½���*/
      /*��������ö�Ӧ���ź�  �������*/
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


