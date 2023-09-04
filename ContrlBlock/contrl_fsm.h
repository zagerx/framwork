#ifndef __CONTRL_FSM__H
#define __CONTRL_FSM__H
#include "fsm.h"
fsm_cb_t* contrl_fsm_creat(void);
fsm_rt_t contrl_fsm_process(fsm_cb_t *me);
#endif
