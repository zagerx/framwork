#ifndef __AS5600_FSM__H
#define __AS5600_FSM__H
#include "fsm.h"
int as5600_fsm_init(fsm_cb_t *ptThis);
fsm_rt_t as5600_fsm_process(fsm_cb_t *ptThis);
char as5600_updata(void *pdata);
#endif