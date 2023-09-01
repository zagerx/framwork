#include "fsm.h"
#include "pid.h"


fsm_cb_t hf_cb;
fsm_rt_t hf_process(fsm_cb_t *me);
static volatile float buf[2];
fsm_cb_t* hf_creat(void)
{
  hf_cb.fsm = (fsm_t*)hf_process;
  hf_cb.state = START;
  hf_cb.sig = 0;
  return &hf_cb;
}

fsm_rt_t hf_process(fsm_cb_t *me)
{
    enum {
    WAIT = USER,
    RUNING,
    };
    switch (me->state)
    {
    case START:
    case WAIT:
      /*����������¼����� */
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

void* hf_updata(void)
{
  return (void *)buf;
}


