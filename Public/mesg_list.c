#include "mesg_list.h"




static mesg_node_t *phead = 0;
void mesg_list_init(void)
{
    phead = 0;
}

/**/
int mesg_list_tailinsert(mesg_node_t *node)
{
    mesg_node_t *ptemp,*ptail;
    /*��ȡlist���*/
    ptemp = phead;
    /*����list�������һ���ڵ�*/
    while(ptemp->pnext != 0)
    {
        ptail = ptemp->pnext;//
        ptemp = ptemp->pnext;
    }
    /*��ȡ��β�ڵ�*/
    ptail->pnext = node;//���ڵ��β������
    return 0;
}
/*��������ȥ��*/
static int mesg_list_deltnode(mesg_node_t *node)
{
    mesg_node_t *ptemp,*pnext,*priv;
    /*��ȡlist���*/
    ptemp = phead;

    while (ptemp->pnext != node)
    {
        /* code */
        priv = ptemp;//��¼ƥ�䵽�ڵ�֮��ǰһ�����
        ptemp = ptemp->pnext;
        if(ptemp == 0)//��ʾ��ǰ����û�иýڵ�
        {
            return 1;//ȥ���ڵ�ʧ��
        }
    }
    /*�ҵ�ƥ��Ľڵ�*/
    pnext = ptemp->pnext;//��¼��һ���ڵ�λ��
    *priv->pnext = *pnext;
    return 0;
}
static int mesg_list_lookforlist(mesg_node_t *node)
{

}
mesg_node_t* mesg_list_getmesg(unsigned char id)
{
    mesg_node_t *ptemp,*ptail;
    /*�жϵ�һ���ڵ��Ƿ�Ϊ��*/
    if (phead->pnext = 0)//��ǰ����û�нڵ�
    {
        /* code */
        return 0;
    }
    /*��ȡlist���*/
    ptemp = phead->pnext;
    while (ptemp->pmsg->id != id)
    {
        /* ���½ڵ� */
        ptemp = ptemp->pnext;
        if (ptemp == 0)
        {
            /*�Ѿ�ȫ����������*/
            return 0;
        }
    }
    mesg_list_deltnode(ptemp);//���ýڵ��Ƴ�
    return ptemp;
} 







