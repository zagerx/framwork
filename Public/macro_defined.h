#ifndef __MACRO_DEFINE__H
#define __MACRO_DEFINE__H


/*字节交换*/
#define __SWP16(A)   (( ((unsigned short)(A) & 0xff00) >> 8)    | \
                                        (( (unsigned short)(A) & 0x00ff) << 8))  

#define __SWP32(A)   ((( (unsigned int)(A) & 0xff000000) >> 24) | \
                                        (( (unsigned int)(A) & 0x00ff0000) >> 8)   | \
                                        (( (unsigned int)(A) & 0x0000ff00) << 8)   | \
                                        (( (unsigned int)(A) & 0x000000ff) << 24))

/*计算数组的元素个数*/
#ifndef dimof
#define   dimof(__array)     (sizeof(__array)/sizeof(__array[0]))
#endif


#endif
