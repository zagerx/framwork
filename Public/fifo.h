#ifndef _FIFO_H
#define _FIFO_H
//#include "./app_cfg.h"

extern void fifo_init(void);
extern signed char fifo_write(unsigned char *pdata,unsigned short len);
extern signed char fifo_read(unsigned char *pdata,unsigned short len);


#define byte_t unsigned char
void bytefifo_init(void);
char bytefifo_writemulitebyge(byte_t *buf,unsigned short len);
char bytefifo_readmulintebyte(byte_t *buf,unsigned short len);
int queue_print(void);
#endif
