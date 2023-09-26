#ifndef _FIFO_H
#define _FIFO_H

typedef unsigned char byte_t;
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





#include "plooc.h"
declare_class(byte_queue_t)

def_class(byte_queue_t,
    uint8_t *pchBuffer;
    uint16_t hwSize;
    uint16_t hwHead;
    uint16_t hwTail;
    uint16_t hwCount;
)

end_def_class(byte_queue_t)




// typedef struct byte_queue_t byte_queue_t;

// struct __byte_queue_t {
//     uint8_t *pchBuffer;
//     uint16_t hwSize;
    
//     uint16_t hwHead;
//     uint16_t hwTail;
//     uint16_t hwCount;
// };

// struct byte_queue_t {
//     uint8_t chMask[sizeof(struct __byte_queue_t)] 
//         __attribute__((aligned(
//             __alignof__(struct __byte_queue_t)
//         )));
// };









#endif
