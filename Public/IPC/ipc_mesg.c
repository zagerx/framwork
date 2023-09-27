#include "ipc.h"



static mesg_t *mesg_buf[5];
void mesgqueue_init(void)
{
//    mesg_buf;
}

void mesgqueue_write(mesg_t *msg)
{
    mesg_buf[0] = msg;
}

mesg_t * mesgqueue_read(void)
{
    return mesg_buf[0];
}