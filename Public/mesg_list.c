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
    /*获取list起点*/
    ptemp = phead;
    /*遍历list查找最后一个节点*/
    while(ptemp->pnext != 0)
    {
        ptail = ptemp->pnext;//
        ptemp = ptemp->pnext;
    }
    /*获取到尾节点*/
    ptail->pnext = node;//将节点从尾部插入
    return 0;
}
/*从链表上去除*/
static int mesg_list_deltnode(mesg_node_t *node)
{
    mesg_node_t *ptemp,*pnext,*priv;
    /*获取list起点*/
    ptemp = phead;

    while (ptemp->pnext != node)
    {
        /* code */
        priv = ptemp;//记录匹配到节点之后，前一个结点
        ptemp = ptemp->pnext;
        if(ptemp == 0)//表示当前链表没有该节点
        {
            return 1;//去除节点失败
        }
    }
    /*找到匹配的节点*/
    pnext = ptemp->pnext;//记录下一个节点位置
    *priv->pnext = *pnext;
    return 0;
}
static int mesg_list_lookforlist(mesg_node_t *node)
{

}
mesg_node_t* mesg_list_getmesg(unsigned char id)
{
    mesg_node_t *ptemp,*ptail;
    /*判断第一个节点是否为空*/
    if (phead->pnext = 0)//当前链表没有节点
    {
        /* code */
        return 0;
    }
    /*获取list起点*/
    ptemp = phead->pnext;
    while (ptemp->pmsg->id != id)
    {
        /* 更新节点 */
        ptemp = ptemp->pnext;
        if (ptemp == 0)
        {
            /*已经全部遍历结束*/
            return 0;
        }
    }
    mesg_list_deltnode(ptemp);//将该节点移除
    return ptemp;
} 







