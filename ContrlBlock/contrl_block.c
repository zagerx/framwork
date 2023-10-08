#include "fsm.h"
#include "contrl_mode.h"
#include "highflow_adjust.h"
#include "cpap_adjust.h"
#include "stdio.h"
#include "global_data.h"
unsigned char get_keystate()
{
  unsigned char data;
  data = get_keysensor_data();
  set_keysensor_data(E_KEY_EVENT_IDLE);
  return data;
}



extern fsm_cb_t contrl_mode_cb;
fsm_cb_t contrl_sys_cb;
static fsm_rt_t sys_state_process(fsm_cb_t *me);
void control_proess(void)
{
  /*����״̬��*/
  DISPATCH_FSM(&contrl_sys_cb);
}
fsm_cb_t* sys_state_init(void)
{
  contrl_sys_cb.fsm = (fsm_t*)sys_state_process;
  contrl_sys_cb.chState = START;
  contrl_sys_cb.sig = 0;
  return &contrl_sys_cb;
}
extern fsm_rt_t stop_mode_process(fsm_cb_t *me);
static fsm_rt_t sys_state_process(fsm_cb_t *me)
{
    enum {
    SELF_CHECK_MODE = USER,
    CONTROL_MODE,
    IDLE_MODE,
    FALUT_MODE
    };
    static unsigned int t0 = 0;
    switch (me->chState)
    {
      case START:
        _calib_mode_init();
        me->chState = SELF_CHECK_MODE;
        break;
      case SELF_CHECK_MODE:
        // printf("self_check start\r\n");  // ԭ�ӿ�
        _calib_mode_proess();
        // if(start_selfcheck())
        // {
        //   me->chState = IDLE_MODE;
        // }else{
        //   me->chState = FALUT_MODE;
        // }
        break;        
      case IDLE_MODE:
        /*��������*/
         if(get_keystate() == E_KEY_EVENT_HOLD)
         {
          me->chState = CONTROL_MODE;
          contrl_mode_init();
         }
        break;
      case CONTROL_MODE:
         if(get_keystate() == E_KEY_EVENT_HOLD)/*������������£���������״̬*/
         {
					printf("exit contorl mode!!!!!!!!\r\n");
          me->chState = IDLE_MODE;
					/*Ҫ��֤�ܹ����е�ǰģʽ��ɨβ����  Ҫ����һ��״̬Ǩ��*/
					TRAN_STATE(&contrl_mode_cb,stop_mode_process);
         }else{
					/*��ʼ�����ĸ�ģʽ״̬��*/
					DISPATCH_FSM(&contrl_mode_cb);	 
				 }
        break;      
      case EXIT:
        break;
      default:
        break;
    }
    return fsm_rt_cpl;
}
