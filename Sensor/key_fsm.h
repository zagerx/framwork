#ifndef __KEY_FSM__H
#define __KEY_FSM_H
#include "fsm.h"//fsm.h文件暂时放在.h文件内 后续优化
fsm_rt_t keyfsm_process(fsm_cb_t *me);
void* keyfsm_updata(void);
#endif