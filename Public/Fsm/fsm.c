#include "fsm.h"
#include "stdlib.h"

#undef NULL
#define NULL 0

void fsm_init(fsm_cb_t *pThis,fsm_t *machine)
{
    pThis->fsm = (fsm_t *)machine;
    pThis->chState = START;
    return;
}

fsm_cb_t *fsm_creat(fsm_t *initstat,unsigned short len,void *pdata)
{
    fsm_cb_t *pthis;
    pthis = (fsm_cb_t *)malloc(sizeof(fsm_cb_t));
    if(pthis==NULL)
    {
        return NULL;
    }
    fsm_init(pthis,initstat);
      
    return pthis;
}

char fsm_destructor(fsm_cb_t *pthis)
{
    if (pthis!=NULL)
    {
        free(pthis);
    }
    return 0;
}


