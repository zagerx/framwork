#include "contrl_mode.h"
#include "highflow_adjust.h"
#include "stdio.h"
#include "fsm.h"
extern fsm_cb_t hf_adjust_cb;
extern fsm_rt_t hf_adjust_process(fsm_cb_t *me);

static fsm_rt_t capamode_process(fsm_cb_t *me);
static fsm_rt_t hfmode_process(fsm_cb_t *me);
static fsm_rt_t idle_mode_process(fsm_cb_t *me);
fsm_cb_t contrl_mode_cb;
fsm_cb_t* contrl_mode_init(void)
{
  contrl_mode_cb.fsm = (fsm_t*)idle_mode_process;
  contrl_mode_cb.chState = START;
  contrl_mode_cb.sig = 0;
  printf("contrl_mode init\r\n");
  return &contrl_mode_cb;
}
/*������ģʽ*/
static fsm_rt_t hfmode_process(fsm_cb_t *me)
{
    enum {
    IDLE_MODE = USER,
    };
    switch (me->chState)
    {
      case START:
        printf("hf_mode start \r\n");
        me->chState = IDLE_MODE;
			  hf_adjust_init();
        break; 
      case IDLE_MODE:
        /*�Ƿ���Ҫ�л�ģʽ*/
        // if(get_curmode() == CPAP_MODE)
        // {
        //   /*��ʼ��hf_adjust_init*/
//           hf_adjust_init();
//           TRAN_STATE(me,capamode_process);
        // }else{
           hf_adjust_process(&hf_adjust_cb);
        //   /*�¶�״̬��*/
        //   /*����״̬��*/
        // }
        break;           
      case EXIT:
				/**/
			  hf_adjust_cb.chState = EXIT;
			  hf_adjust_process(&hf_adjust_cb);
        printf("hf_mode exit \r\n");
        break;
      default:
        break;
    }
    return fsm_rt_cpl;
}
/*CPAPģʽ*/
static fsm_rt_t capamode_process(fsm_cb_t *me)
{
    enum {
    IDLE_MODE = USER,
    };
    static unsigned int t0 = 0;
    switch (me->chState)
    {
      case START:
        printf("capamode start\r\n");
				hf_adjust_init();
        me->chState = IDLE_MODE;
        break; 
      case IDLE_MODE:
        /*�Ƿ���Ҫ�л�ģʽ*/
        // if(get_curmode() == CPAP_MODE)
        // {
        //   /*��ʼ��hf_adjust_init*/
        //   hf_adjust_init();
         TRAN_STATE(me,hfmode_process);
        // }else{
           hf_adjust_process(&hf_adjust_cb);
        //   /*�¶�״̬��*/
        //   /*����״̬��*/
        // }
        break;           
      case EXIT:
        printf("capamode exit\r\n");
        break;
      default:
        break;
    }
    return fsm_rt_cpl;
}
/*����ģʽ*/
static fsm_rt_t idle_mode_process(fsm_cb_t *me)
{
    enum {
    IDLE_MODE,
    };
    static unsigned int t0 = 0;
    switch (me->chState)
    {
      case START:
        printf("idle mode starr\r\n");
        /*��ȡ��ǰģʽ*/
        // if(get_curmode() == HF_MODE)
        // {
          TRAN_STATE(me,hfmode_process);
        // }
        break;
      case EXIT:
				printf("idle_mode exit\r\n");
        break;
      default:
        break;
    }
    return fsm_rt_cpl;
}

/*ֹͣģʽģʽ*/
fsm_rt_t stop_mode_process(fsm_cb_t *me)
{
    enum {
    IDLE_MODE,
    };
    static unsigned int t0 = 0;
    switch (me->chState)
    {
      case START:
				printf("stop_mode start\r\n");
        break;
      case EXIT:
        break;
      default:
        break;
    }
    return fsm_rt_cpl;
}

