#ifndef __HIGHFLOW_FSM__H
#define __HIGHFLOW_FSM__H
#include "fsm.h"

fsm_cb_t* hf_fsm_creat(void);
fsm_rt_t hf_fsm_process(fsm_cb_t *me);

#endif
