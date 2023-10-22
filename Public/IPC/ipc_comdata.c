/*-------------------------����������-----------------------------------
���ļ����ݾ������Ŀʵ�֣��������ձ���
---------------------------------------------------------------------*/
#include "ipc.h"
typedef enum{
    START_ADDR,
    END_ADDR,
}E_PUBLIC_DATA;

static float gIpc_datazone[END_ADDR] = {
    0.0f,
};

float ipc_read_data(E_PUBLIC_DATA index)
{
    return gIpc_datazone[index];
}

char ipc_write_data(E_PUBLIC_DATA index,float data)
{
    if ((E_PUBLIC_DATA)index > END_ADDR)
    {
        return 1;
    }
    gIpc_datazone[index] = data;
}

