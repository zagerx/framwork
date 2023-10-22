#ifndef __IPC__H
#define __IPC__H

extern unsigned int g_protocol_event;

// #define IPC_SET_EVENT(event) SET_BIT(g_protocol_event,1)

#define GET_IPC_EVENT(event)      (g_protocol_event >> event) & 1
                                    
#define SET_IPC_EVENT(event)      g_protocol_event |= (1<<event) 
                                        
#define CLEAR_IPC_EVENT(event)    g_protocol_event &= ~(1<<event)











#include "list.h"





_lsit_item_t* ipc_mesg_packet_02(unsigned short id,unsigned short len,void *pbuf);
_lsit_item_t* ipc_mesg_packet(unsigned short id,unsigned short len);
void ipc_msgpool_init(void);
void ipc_msgpool_write(_lsit_item_t *msg);
_lsit_item_t* ipc_msgpool_read(void);
_lsit_item_t* ipc_msgpool_read_val(_lsit_item_t* msg);
void ipc_msgpool_del(_lsit_item_t *msg);
void ipc_msg_printf(void);
void ipc_msg_printf_number(void);

#endif
