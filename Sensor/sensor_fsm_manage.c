#include "fsm.h"

void fsm_manage_creat_list(void)
{
  
}

void fsm_manage_addfsm_list(fsm_cb_t *cb)
{

}

void fsm_manage_delfsm_list(fsm_cb_t *cb)
{
  
}

fsm_cb_t* fsm_manage_getfsm(void)
{
  
}

void fsm_manage_engine(void)
{
  fsm_cb_t *cur_fsm;
  /*get fsm_cb from manage list*/
  cur_fsm = fsm_manage_getfsm();
  /*fsm no NULL*/
  if(cur_fsm != 0)
  {
    cur_fsm->fsm(cur_fsm);
  }
}

