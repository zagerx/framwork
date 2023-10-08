#include "protocol.h"
#include "crc.h"
#include "stdio.h"
#include "usart.h"
#include "stdlib.h"
#include "string.h"

#include "fsm.h"
#undef  NULL
#define NULL 0                  
#undef  this
#define this (*ptThis)
/*发送状态机  ptThis里面的消息为待发送的消息*/
fsm_rt_t protocol_send(fsm_cb_t *ptThis)
{
    enum {
        IDLE = USER,
        SEND_NOW,
        FIRST,
        SECOND,
        THIRD,
        FOURTH,
        ERR,
    };
    static unsigned int t0;
    unsigned int delta_t;
    static unsigned char cnt;
    float data_buf[2] = {6.4f,2.3f};
    static msg_t *p_wait_send_msg;//待发送的消息
    static pro_frame_t *p_wait_send_fram;  
    static unsigned char wait_send_cmd;
    static unsigned char wait_send_cmdtype;

    switch (this.chState)
    {
    case START:
        {          
            /*记录状态机所携带数据*/
            p_wait_send_msg = (msg_t *)ptThis->pdata;//获取状态机机的数据域
            p_wait_send_fram = (pro_frame_t*)p_wait_send_msg->pdata;
            if(p_wait_send_msg == NULL || p_wait_send_fram==NULL)
            {
                return fsm_rt_cpl;
            }
            wait_send_cmd = (unsigned char)(p_wait_send_fram->func_c>>8);
            wait_send_cmdtype = (unsigned char)(p_wait_send_fram->func_c);
            this.chState = FIRST;
        }
    case FIRST:
            if(wait_send_cmdtype == CMD_RESP)
            {
                this.chState = SECOND;
            }else{
                this.chState = SEND_NOW;
            }    
        break;        
    case SEND_NOW:
        /*直接发送*/
        protocol_sendfram((pro_frame_t *)p_wait_send_msg->pdata,p_wait_send_msg->len);
        /*释放状态机携带的数据区域*/
        free(p_wait_send_fram);
        free(p_wait_send_msg);
        /*更新状态机数据区域*/
        this.pdata = NULL;
        this.len = 0;

        this.sig = 0x02;//通知
        this.chState = EXIT;     
        break;
    case SECOND://重传发送
        this.chState = THIRD;
    case THIRD:
        {
            protocol_sendfram((pro_frame_t *)p_wait_send_msg->pdata,p_wait_send_msg->len);
            t0 = HAL_GetTick();
            if (cnt++>5)
            {
                cnt = 0;
                this.chState = ERR;
                break;
            }else{
                this.chState = FOURTH;
            }            
        }
    case FOURTH:
        {
            msg_t *p_reciv_msg;
            pro_frame_t *p_reciv_fram;
            /*获取当前命令接收到的消息*/
            msg_t p_msg;
            p_msg.id = wait_send_cmd;
            p_reciv_msg = (msg_t *)ipc_msgpool_read_val(&p_msg);
            if (p_reciv_msg == NULL)
            {
                delta_t = HAL_GetTick()-t0;
                if(delta_t > 1000)
                {
                    this.chState = THIRD;
                }
                break;
            }
            p_reciv_fram = (pro_frame_t *)p_reciv_msg->pdata;
            if ((unsigned char)(p_reciv_fram->func_c) == CMD_ACK)
            {//成功接收到命令的响应
                USER_DEBUG_RTT("CMD ACK OK\r\n");
                // ipc_msgpool_del(p_reciv_msg);

                /*释放状态机携带的旧的数据区域   */
                free(p_wait_send_fram);
                free(p_wait_send_msg);

                /*更新状态机数据区域   更新新的数据到状态机的数据域*/
                this.pdata = p_reciv_msg;
                this.len = p_reciv_msg->len;

                this.sig = 0x02;//通知线程可以关闭此次通信
                this.chState = EXIT;
            }            
        }
        break;
    case ERR:
        USER_DEBUG_RTT("no respond\r\n");
        this.chState = EXIT;//复位状态机
        break;
    case EXIT:
        break;
    default:
        break;
    }
    return fsm_rt_on_going;   
}



char protocol_sendfram(pro_frame_t *msg,unsigned short len)
{
    /*整理帧数据*/
    unsigned char *pucmsg;
    unsigned char *pdata;
    unsigned short data_len;
    data_len = len-sizeof(pro_frame_t);
    unsigned char buf[2] = {0};

    pucmsg = (unsigned char*)msg;
    memcpy(buf,(unsigned char*)&pucmsg[FRAM_TAIL_OFFSET],2);
    pdata = malloc(data_len);
    memcpy(pdata,(unsigned char*)&pucmsg[sizeof(pro_frame_t)],data_len);
    memcpy((unsigned char*)&pucmsg[FRAM_PDATA_OFFSET],pdata,data_len);    
    free(pdata);
    unsigned short crc_16;
    crc_16 = CRC16_Subsection((unsigned char*)&pucmsg[2],0xFFFF,FRAM_PDATA_OFFSET+data_len-2);
    crc_16 = __SWP16(crc_16);    
    memcpy((unsigned char*)&pucmsg[FRAM_PDATA_OFFSET+data_len],(unsigned char*)&crc_16,sizeof(crc_16));   

    memcpy((unsigned char*)&pucmsg[FRAM_PDATA_OFFSET+data_len+2],buf,2);    
    HAL_UART_Transmit(&huart1,(unsigned char*)msg,sizeof(pro_frame_t)-sizeof(void *)+data_len,0XFFFF);
	return 0;
}






