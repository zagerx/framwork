#ifndef __CAPA_FSM__H
#define __CAPA_FSM__H

#include "fsm.h"
fsm_cb_t* capa_fsm_creat(void);
fsm_rt_t capa_fsm_process(fsm_cb_t *me);
#endif