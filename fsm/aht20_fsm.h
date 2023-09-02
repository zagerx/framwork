#ifndef __AHT20_FSM__H
#define __AHT20_FSM__H
#include "fsm.h"

extern fsm_cb_t aht20_fsm_ctrlblock;

int aht20_get_init(fsm_cb_t *ptThis);

fsm_rt_t aht20_get_process(fsm_cb_t *ptThis);
#endif
