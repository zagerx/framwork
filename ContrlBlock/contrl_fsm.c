#include "fsm.h"
#include "highflow_fsm.h"
#include "capa_fsm.h"
#include "stdio.h"
#include "main.h"
fsm_cb_t contrl_cb;
fsm_cb_t contrl_cb2;

extern fsm_cb_t hf_cb;
extern fsm_cb_t capa_cb;
fsm_rt_t contrl_fsm_process(fsm_cb_t *me);
fsm_cb_t* contrl_fsm_creat(void)
{
  contrl_cb.fsm = (fsm_t*)contrl_fsm_process;
  contrl_cb.chState = START;
  contrl_cb.sig = 0;
  return &contrl_cb;
}


fsm_rt_t contrl_fsm_test_process(fsm_cb_t *me)
{
    enum {
    HF_MODE = USER,
    CAPA_MODE,
    };
    switch (me->chState)
    {
      case START:
        printf("test start\r\n");
    }
    return fsm_rt_cpl;  
}                                  
fsm_rt_t contrl_fsm_idle_process(fsm_cb_t *me)
{
    enum {
    HF_MODE = USER,
    CAPA_MODE,
    };
    switch (me->chState)
    {
      case START:
        printf("idle start\r\n");
        TRAN_STATE(me,contrl_fsm_process);
        break;
      case EXIT:
        printf("idle exit\r\n");
        break;
      default:
        break;
    }
    return fsm_rt_cpl;  
}

fsm_cb_t* contrl_fsm_init(void)
{
  contrl_cb2.fsm = (fsm_t*)contrl_fsm_process;
  contrl_cb2.chState = START;
  contrl_cb2.sig = 0;
  return &contrl_cb2;
}
void control_proess(void)
{
  /*ÔËÐÐ×´Ì¬»ú*/
  DISPATCH_FSM(&contrl_cb2);
}



fsm_rt_t contrl_fsm_process(fsm_cb_t *me)
{
    enum {
    HF_MODE = USER,
    CAPA_MODE,
    IDLE_MODE,
    };
    static unsigned int t0 = 0;
    switch (me->chState)
    {
      case START:
        me->chState = IDLE_MODE;
        t0 = HAL_GetTick();
        break;
      case IDLE_MODE:
        TRAN_STATE(me,contrl_fsm_idle_process);
        break;        
      case HF_MODE:
        hf_fsm_process(&hf_cb);
        break;
      case CAPA_MODE:
        capa_fsm_process(&capa_cb);
        break;      
      case EXIT:
        break;
      default:
        break;
    }
    return fsm_rt_cpl;
}


