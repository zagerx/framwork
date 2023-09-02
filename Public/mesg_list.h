#ifndef __MESG_LIST__H
#define __MESG_LIST__H
typedef struct mesg
{
    int  id;
    unsigned char fsm;
    void *pdata;
    unsigned short len;
}mesg_t;

typedef struct mesg_node mesg_node_t;

struct  mesg_node{
    /* data */
    mesg_t *pmsg;
    mesg_node_t *pnext;
};

void mesg_list_init(void);
int mesg_list_tailinsert(mesg_node_t *node);
mesg_node_t* mesg_list_getmesg(unsigned char id);
#endif
