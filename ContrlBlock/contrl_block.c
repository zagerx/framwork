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
  /*调度状态机*/
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
        // printf("self_check start\r\n");  // 原接口
        _calib_mode_proess();
        // if(start_selfcheck())
        // {
        //   me->chState = IDLE_MODE;
        // }else{
        //   me->chState = FALUT_MODE;
        // }
        break;        
      case IDLE_MODE:
        /*按键按下*/
         if(get_keystate() == E_KEY_EVENT_HOLD)
         {
          me->chState = CONTROL_MODE;
          contrl_mode_init();
         }
        break;
      case CONTROL_MODE:
         if(get_keystate() == E_KEY_EVENT_HOLD)/*如果按键被按下，则进入空闲状态*/
         {
					printf("exit contorl mode!!!!!!!!\r\n");
          me->chState = IDLE_MODE;
					/*要保证能够进行当前模式的扫尾工作  要进行一次状态迁移*/
					TRAN_STATE(&contrl_mode_cb,stop_mode_process);
         }else{
					/*开始调度四个模式状态机*/
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
