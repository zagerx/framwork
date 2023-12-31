/** Modified by:			zager
** Modified date:		2023-08-17
** Version:
** Descriptions:	协议解析
**
********************************************************************************************************/
#include "protocol_comment.h"
#include "protocol_cfg.h"



/*********************************************************************************************************
** Function name(函数名称):						protocol_parse()
**
** Descriptions（描述）:							协议解析
**
** input parameters（输入参数）:			None
** Returned value（返回值）:					None
**         
** Used global variables（全局变量）:	None
** Calling modules（调用模块）:				fifo.c/h
**
** Created by（创建人）:							zager
** Created Date（创建日期）:					2023-08-17
**
********************************************************************************************************/
void protocol_parse(void)
{
    unsigned char frame_buf[PRO_FRAME_MAX_SIZE];
    unsigned short frame_len;
	/*读取1帧数据*/
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
    free(p_r_fram);

    // search_msgmap(cmd_fun,pf1,data_len);
    // return;

	switch (cmd_fun)
	{                 
		case 0x06:
            /*添加数据到链表并打印*/
            ipc_msg_printf();
			break; 
 		case 0x0A:
            {
                float buf[4] = {0.0f};
                // protocol_nowtransmit(0x03,0x02,buf,16); 
                protocol_transmit(0x03,0x02,buf,16);//测试用
            }
			break;   	
		default:
			break;
	}

}





