#ifndef __CPAP_ADJUST_H
#define __CPAP_ADJUST_H

#include "fsm.h"
fsm_cb_t* cpap_adj_init(void);
fsm_rt_t cpap_adj_process(fsm_cb_t *me);
#endif
