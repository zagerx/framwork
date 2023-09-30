#ifndef __IPC__H
#define __IPC__H

extern unsigned int g_protocol_event;






#define GET_IPC_EVENT(event)      (g_protocol_event >> event) & 1
                                    
#define SET_IPC_EVENT(event)      g_protocol_event |= (1<<event) 
                                        
#define CLEAR_IPC_EVENT(event)    g_protocol_event &= ~(1<<event)




typedef struct mesg mesg_t;
//#pragma pack(push,1)
struct mesg{
    unsigned short id;
    unsigned short len;
    void *pdata;
};

typedef struct _list{
    mesg_t *head;
    mesg_t *tail;
    unsigned short node_numb;
}_list_t;


typedef struct _node
{
    /* data */
    mesg_t *next;
    mesg_t *msg;
}_node_t;


mesg_t* ipc_mesg_packet(unsigned short id,unsigned short len);
void ipc_msgpool_init(void);
void ipc_msgpool_write(mesg_t *msg);
mesg_t* ipc_msgpool_read(void);
void ipc_msgpool_del(mesg_t *msg);

#endif
