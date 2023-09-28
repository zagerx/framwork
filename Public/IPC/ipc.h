#ifndef __IPC__H
#define __IPC__H

extern unsigned int g_protocol_event;






#define GET_IPC_EVENT(event)      (g_protocol_event >> event) & 1
                                    
#define SET_IPC_EVENT(event)      g_protocol_event |= (1<<event) 
                                        
#define CLEAR_IPC_EVENT(event)    g_protocol_event &= ~(1<<event)




typedef struct mesg mesg_t;
//#pragma pack(push,1)
struct mesg{
    mesg_t *next;
    unsigned short id;
    unsigned short len;
    void *pdata;
};

typedef struct msg_list{
    mesg_t *head;
    mesg_t *next;
    mesg_t *pdata;
    unsigned short node_numb;
}msg_list_t;
typedef struct mesg_equene{
    unsigned short head;
    unsigned short tail;
    unsigned short remain_byte;
    mesg_t  *pbuf;//¶ÓÁÐ»º³åÇø
    unsigned short buflen;
}msg_queue_t;



mesg_t* ipc_mesg_packet(unsigned short id,unsigned short len);
void ipc_msgpool_init(void);
void ipc_msgpool_write(mesg_t *msg);
mesg_t* ipc_msgpool_read(void);
void ipc_msgpool_del(mesg_t *msg);

#endif
