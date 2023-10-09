#include "calibration_mode.h"
#include "fsm.h"
#include "usart.h"

#include "ipc.h"

#undef  NULL
#define NULL 0                  
#undef  this
#define this (*ptThis)

fsm_cb_t calib_mode_cb;
static fsm_rt_t calib_mode_process(fsm_cb_t *ptThis);
static fsm_rt_t calib_mode_flow(fsm_cb_t *ptThis);		

#include "protocol.h"
#include "stdlib.h"
#include "string.h"
/*===============================�ⲿ����========================================*/
// #include "ctrlb_global_data.h"
#include "protocol.h"//
#include "string.h"
#include "stdlib.h"

static fsm_rt_t calib_mode_process(fsm_cb_t *ptThis)
{
    enum {
        CALIB_IDLE = USER,
        CALIB_FLOW,
        CALIB_PRESS,
        CALIB_XXX,
    };
    static unsigned int t0 = 0;
    switch (this.chState)
    {
    case START:
        this.chState = CALIB_IDLE;
    case CALIB_IDLE:
        TRAN_STATE(ptThis,calib_mode_flow);
        break;
    case EXIT:
        break;
    default:
        break;
    }
    return fsm_rt_cpl;
}

/*����У׼*/				
static fsm_rt_t calib_mode_flow(fsm_cb_t *ptThis)
{

	unsigned char buf[PRO_FRAME_MAX_SIZE];
	unsigned char cmd;
    unsigned char len;
    unsigned char cmd_type = 0;
    float data_buf[2] = {6.4f,2.3f};
    unsigned char data_len;    
    enum {
        CALIB_DATA_READY = USER,
        CALIB_RUN
    };
    
    static msg_t *p_msg02;
    static unsigned int t0 = 0;
    
    switch (this.chState)
    {
    case START:
        {
            p_msg02 = pro_send_cmd_data(0xFFFE,CMD_RESP,PRO_FUNC_C_PF300,data_buf,sizeof(data_buf));
            this.chState = CALIB_DATA_READY;
        }
        break;
    case CALIB_DATA_READY:
        {
            //p_msg02����״̬������Ϣ���
            fsm_cb_t *cmd_fsm;
            cmd_fsm = (fsm_cb_t *)p_msg02->pdata;//״̬�����
            unsigned char sig;
            sig = cmd_fsm->sig;
            if(sig == 0x02)//����״̬�� �Ѿ���ɷ��Ͳ��ҳɹ����յ�Ӧ��,���������Ѹ�����״̬����������
            {
                /*�ͷŽ��յ��Ļظ���Ϣ*/
                msg_t *pmsg;
                pro_frame_t *pfram;  
                pmsg = (msg_t*)cmd_fsm->pdata;
                pfram =  pmsg->pdata;
                
                if(!pmsg)
                {
                    pfram = NULL;
                }
                
                /****��������******/
                float *a,*p;
                float buf[2];
                a = pfram->pdata;
                p = buf;
                p = (float *)pfram->pdata;
//                USER_DEBUG_RTT("a = %3f\r\n",*a);
                USER_DEBUG_RTT("buf = %3f  %3f\r\n",p[0],p[1]);
                /****�������*****/                
                
                ipc_msgpool_del(pmsg);                
                free(pmsg);
                
                fsm_destructor(cmd_fsm);                
                free(pfram);//(MDK�汾��)��ʱ�������״̬������������֮��(��������ٴ�����1��2�����ڻ����hardfault������Ӧ��ӡ��hardfault֮ǰ��PCָ�����)������
                
                ipc_msgpool_del(p_msg02);
                free(p_msg02);                
                
                this.chState = START;
            }

            if(sig == 0x03)//����״̬�� �Ѿ���ɷ���  û�н��յ�Ӧ���ź�
            {
                /*�ͷŽ��յ��Ļظ���Ϣ*/
                msg_t *pmsg;
                pro_frame_t *pfram;  
                pmsg = (msg_t*)cmd_fsm->pdata;
                
                if(!pmsg)
                {
                    pfram = NULL;
                }else{
                    pfram =  pmsg->pdata;
                }                

                free(pfram);
                ipc_msgpool_del(pmsg);
                free(pmsg);

                fsm_destructor(cmd_fsm);
                ipc_msgpool_del(p_msg02);
                free(p_msg02);                                              
                
                this.chState = CALIB_RUN;
            }
        }
        break;
    case CALIB_RUN:
        break;
    case EXIT:
        USER_DEBUG_RTT("flow calib stop\r\n");
        break;
    default:
        break;
    }
    return fsm_rt_cpl;

}




/*ֹͣģʽģʽ    Ϊ���˳�ɨβ��������*/
static fsm_rt_t calibstop_mode_proess(fsm_cb_t *me)
{
    enum {
    IDLE_MODE = USER,
    };
    static unsigned int t0 = 0;
    switch (me->chState)
    {
      case START:
		break;
      case EXIT:
        break;
      default:
        break;
    }
    return fsm_rt_cpl;
}
void _calib_mode_init(void)
{
    fsm_init(&calib_mode_cb,(fsm_t*)calib_mode_process);
}

void _calib_mode_proess(void)
{
    DISPATCH_FSM(&calib_mode_cb);	
}

void _calibmode_tran_stopstate(void)
{
    TRAN_STATE(&calib_mode_cb,calibstop_mode_proess);  
}


