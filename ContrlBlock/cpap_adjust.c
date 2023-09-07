#include "fsm.h"
#include "cpap_adjust.h"

fsm_cb_t cpap_cb;
fsm_rt_t cpap_adj_process(fsm_cb_t *me);
fsm_cb_t* cpap_adj_init(void)
{
  cpap_cb.fsm = (fsm_t*)0;
  cpap_cb.chState = START;
  cpap_cb.sig = 0;
  return &cpap_cb;
}

fsm_rt_t cpap_adj_process(fsm_cb_t *me)
{
    enum {
    WAIT = USER,
    RUNING,
    };
    switch (me->chState)
    {
    case START:
    case WAIT:
      break;
    case RUNING:
      break;      
    case EXIT:
      break;
    default:
      break;
    }
    return fsm_rt_cpl;
}

