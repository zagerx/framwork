#ifndef __AHT20_FSM__H
#define __AHT20_FSM__H
#include "fsm.h"


/*״̬�����ƿ�*/
typedef struct aht20_get {
    unsigned char chState;     //!< ״̬����
    unsigned char sig;
}aht20_get_t;

extern aht20_get_t aht20_fsm_ctrlblock;

int aht20_get_init(aht20_get_t *ptThis);

fsm_rt_t aht20_get_process(aht20_get_t *ptThis);
#endif
