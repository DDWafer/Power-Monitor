#ifndef PARA_H
#define PARA_H

#include <QDataStream>
//图像状态
#define STOP 0
#define INITIAL 1
#define CONTINUE 2
#define INITIAL_FINISH 3
#define RESET 4
#define DOWN_FINISH 5
//本地状态
#define IDLE 0         //空闲
#define LOAD_ALL 5    //这里5张表为全部表
#define BACKUP_ALL 6
#define RESTORE_ALL 7
#define SAVE_ALL 8
#define SAVE 9
#define LOAD 10
#define AUTO 11
//auto状态
#define OFF 0
#define ON 1
#define READY 2
struct Data_Temp            //本地数据存储
{
    int load_valid_row_num; //读取有效行数
    int save_valid_row_num; //保存有效行数
    int down_valid_row_num; //下传有效行数
    float last_time[100];  //电压持续时间=================================
    float volt[100];  //电压值=======================================
    int loop_times[100];    //循环次数
    int node_number;   //节点数
};
typedef struct Node         //单链表定义
{
    int data;
    struct Node *next;
}Node;

typedef struct Node *LinkList;

typedef int ElemType;

LinkList LinkedListInit() {
    Node *L;
    L = (Node *)malloc(sizeof(Node));   //申请结点空间
    if(L == NULL) { //判断是否有足够的内存空间
        printf("申请内存空间失败\n");
    }
    L->next = NULL;                  //将next设置为NULL,初始长度为0的单链表
    return L;
}


//单链表的建立1，头插法建立单链表

LinkList LinkedListCreatH() {
    Node *L;
    L = (Node *)malloc(sizeof(Node));   //申请头结点空间
    L->next = NULL;                      //初始化一个空链表

    ElemType x;                         //x为链表数据域中的数据
    while(scanf("%d",&x) != EOF) {
        Node *p;
        p = (Node *)malloc(sizeof(Node));   //申请新的结点
        p->data = x;                     //结点数据域赋值
        p->next = L->next;                    //将结点插入到表头L-->|2|-->|1|-->NULL
        L->next = p;
    }
    return L;
}


//单链表的建立2，尾插法建立单链表

LinkList LinkedListCreatT() {
    Node *L;
    L = (Node *)malloc(sizeof(Node));   //申请头结点空间
    L->next = NULL;                  //初始化一个空链表
    Node *r;
    r = L;                          //r始终指向终端结点，开始时指向头结点
    ElemType x;                         //x为链表数据域中的数据
    while(scanf("%d",&x) != EOF) {
        Node *p;
        p = (Node *)malloc(sizeof(Node));   //申请新的结点
        p->data = x;                     //结点数据域赋值
        r->next = p;                 //将结点插入到表头L-->|1|-->|2|-->NULL
        r = p;
    }
    r->next = NULL;

    return L;
}


//单链表的插入，在链表的第i个位置插入x的元素

LinkList LinkedListInsert(LinkList L,int i,ElemType x) {
    Node *pre;                      //pre为前驱结点
    pre = L;
    int tempi = 0;
    for (tempi = 1; tempi < i; tempi++) {
        pre = pre->next;                 //查找第i个位置的前驱结点
    }
    Node *p;                                //插入的结点为p
    p = (Node *)malloc(sizeof(Node));
    p->data = x;
    p->next = pre->next;
    pre->next = p;

    return L;
}


//单链表的删除，在链表中删除值为x的元素

LinkList LinkedListDelete(LinkList L,ElemType x)
{
    Node *p,*pre;                   //pre为前驱结点，p为查找的结点。
    p = L->next;
    while(p->data != x) {              //查找值为x的元素
        pre = p;
        p = p->next;
    }
    pre->next = p->next;          //删除操作，将其前驱next指向其后继。
    free(p);
    return L;
}


struct  Data_Acp        //接收数据结构体
{
    int real_last_time[100];   //链表表示实际持续时间
    int real_volt[100];         //链表表示实际电压
    int real_loop_times[100];   //链表表示实际循环次数
    int real_node_number;       //实际节点数
};

struct Graph_Para       //绘图数据结构体
{
  int x_in_max;     //实际图像横坐标最大值
  int y_in_max;     //实际图像纵坐标最大值
  int x_out_max;    //设计图像横坐标最大值
  int y_out_max;    //设计图像纵坐标最大值
  int x_in_interval;    //实际图像横坐标间隔值
  int y_in_interval;    //实际图像纵坐标间隔值
  int x_out_interval;   //设计图像横坐标间隔值
  int y_out_interval;   //设计图像纵坐标间隔值
  int gra_node_num;     //图中节点数
  float final_x;          //最后一点坐标
  int gra_loop_times;  //记录各循环节点剩余次数
  int gra_scan_row;            //图像扫描的行数
  float gra_last_time[100];  //准备显示电压持续时间
  float gra_volt[100];  //准备电压值
  int gra_loop_head_row;   //循环的开头行数
  int state;  //图像刷新状态 0：不刷新，初始状态；1：数据更新完成，第一次刷新全图；2：二次刷新，一次刷一个点 3：第一次刷新完成

};
#endif // PARA_H
