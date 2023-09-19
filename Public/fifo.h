#ifndef _FIFO_H
#define _FIFO_H

#define byte_t unsigned char
typedef struct byte_fifo{
    unsigned short head;
    unsigned short tail;
    volatile short remain_byte;
    byte_t *pbuf;
    volatile unsigned short buflen;//»º³åÇø´óÐ¡
}byte_fifo_t;    
void bytefifo_init(byte_fifo_t *pfifo,byte_t *arry,unsigned short len);
char bytefifo_writemulitebyge(byte_fifo_t *pfifo,byte_t *buf,unsigned short len);
char bytefifo_readmulintebyte(byte_fifo_t *pfifo,byte_t *buf,unsigned short len);
#endif
