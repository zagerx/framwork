/** Modified by:			zager
** Modified date:		2023-08-17
** Version:
** Descriptions:	Э�����
**
********************************************************************************************************/
#include "protocol_comment.h"
#include "protocol_cfg.h"



/*********************************************************************************************************
** Function name(��������):						protocol_parse()
**
** Descriptions��������:							Э�����
**
** input parameters�����������:			None
** Returned value������ֵ��:					None
**         
** Used global variables��ȫ�ֱ�����:	None
** Calling modules������ģ�飩:				fifo.c/h
**
** Created by�������ˣ�:							zager
** Created Date���������ڣ�:					2023-08-17
**
********************************************************************************************************/
void protocol_parse(void)
{
    unsigned char frame_buf[PRO_FRAME_MAX_SIZE];
    unsigned short frame_len;
	/*��ȡ1֡����*/
	if(_get_framedata_fromfifo(&frame_buf[0],&frame_len) != 0)
	{
		return;
	}
    pro_frame_t * p_r_fram;
    p_r_fram = _unpack_proframe((unsigned char*)frame_buf,frame_len);
    if (!p_r_fram)
    {
        return;
    }
    unsigned short cmd = __SWP16(p_r_fram->func_c);
    unsigned char cmd_fun = (unsigned char)cmd;
    unsigned char cmd_type = (unsigned char)(cmd>>8);
    heap_free(p_r_fram);

	switch (cmd_fun)
	{                 
		case 0x06:
            {
                USER_DEBUG("cmd 06\r\n");
                float buf[4] = {0.0f};
                protocol_transmit(0x03,0x02,buf,16);//������
            }            
			break; 
 		case 0x0A:
            {
                unsigned char event;
                float buf[4] = {0.0f};
                event =  forch_keymap_enevt(0x0A);
                IPC_SET_EVENT(g_protocol_event,event);
                // protocol_nowtransmit(0x03,0x02,buf,16); 
                // 
            }
			break;  
 		case 1:
            {
                float buf[4] = {0.0f};
                protocol_nowtransmit(0x03,0x02,buf,16); 
                // protocol_transmit(0x03,0x02,buf,16);//������
            }
			break;              	
		default:
			break;
	}

}





