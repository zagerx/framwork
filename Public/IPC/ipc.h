#ifndef __IPC__H
#define __IPC__H

extern unsigned int g_protocol_event;



#define GET_IPC_EVENT(event)      (g_protocol_event >> event) & 1
                                    
#define SET_IPC_EVENT(event)      g_protocol_event |= (1<<event) 
                                        
#define CLEAR_IPC_EVENT(event)    g_protocol_event &= ~(1<<event)


typedef struct mesg msg_t;
struct mesg{
    unsigned short id;
    unsigned short len;
    void *pdata;
};

typedef struct _node _node_t;

struct _node
{
    /* data */
    _node_t *next;
    msg_t *msg;
};

typedef struct _list{
    _node_t *head;
    _node_t *tail;
    unsigned short node_numb;
}_list_t;










msg_t* ipc_mesg_packet_02(unsigned short id,unsigned short len,void *pbuf);
msg_t* ipc_mesg_packet(unsigned short id,unsigned short len);
void ipc_msgpool_init(void);
void ipc_msgpool_write(msg_t *msg);
msg_t* ipc_msgpool_read(void);
msg_t* ipc_msgpool_read_val(msg_t* msg);
void ipc_msgpool_del(msg_t *msg);
void ipc_msg_printf(void);
void ipc_msg_printf_number(void);

#endif
