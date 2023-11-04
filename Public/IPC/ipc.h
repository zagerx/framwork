#ifndef __IPC__H
#define __IPC__H

#include "list.h"
#include "macro_defined.h"
#define IPC_SET_EVENT(__me,event)        USER_SET_BIT(__me,event)
#define IPC_GET_EVENT(__me,event)        USER_GET_BIT(__me,event)
#define IPC_CLEAR_EVENT(__me,event)      USER_CLEAR_BIT(__me,event)

node_item_t* ipc_mesg_packet_02(unsigned short id,unsigned short len,void *pbuf);
node_item_t* ipc_mesg_packet(unsigned short id,unsigned short len);
void ipc_msgpool_init(void);
void ipc_msgpool_write(node_item_t *msg);
node_item_t* ipc_msgpool_read(void);
node_item_t* ipc_msgpool_read_val(node_item_t* msg);
void ipc_msgpool_del(node_item_t *msg);
void ipc_msg_printf(void);
void ipc_msg_printf_number(void);

#endif
