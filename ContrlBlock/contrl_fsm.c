#include "fsm.h"
#include "highflow_fsm.h"
#include "capa_fsm.h"
fsm_cb_t contrl_cb;
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

fsm_rt_t contrl_fsm_process(fsm_cb_t *me)
{
    enum {
    HF_MODE = USER,
    CAPA_MODE,
    };
    switch (me->chState)
    {
      case START:
        hf_fsm_creat();
        capa_fsm_creat();
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


