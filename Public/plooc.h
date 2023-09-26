#ifndef __PLOOC__H
#define __PLOOC__H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
#define declare_class(__name)     \
    typedef struct __name __name;        

#define def_class(__name, ...)                   \
    struct __##__name {                          \
        __VA_ARGS__                              \
    };                                           \
    struct __ngitame {                              \
        uint8_t chMask[sizeof(struct __##__name)]\
            __attribute__((aligned(              \
                __alignof__(struct __##__name)   \
            )));                                 \
    };
    
/* ��ֻ��һ��Ϊδ��Ԥ�����﷨�� */
#define end_def_class(...)
    
#define class_internal(__obj_ptr, __ptr, __type) \
    struct __##__type * __ptr =                  \
        (struct __##__type *)(__obj_ptr)


#endif
