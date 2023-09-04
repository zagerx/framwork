#ifndef __CMDRESP_FSM__H
#define __CMDRESP_FSM__H
// #include "./app_cfg.h"
#include "fsm.h"
typedef enum{
  NO_CMD_SEND_SIG = 0,
  CMD_SEND_SIG,
}dri_sig_t;

/*状态机控制块*/
typedef struct cmd_resp_t {
    unsigned char chState;     //!< 状态变量
    dri_sig_t sig;
}cmd_resp_t;

extern cmd_resp_t cmd_resp_task1;
int cmd_resp_init(cmd_resp_t *ptThis);
fsm_rt_t cmd_resp(cmd_resp_t *ptThis);


/*触发信号*/
#define CMD_RESP_SET_SIG(_sig)  \
                    do {cmd_resp_task1.sig = _sig;} while(0)

										
#endif
