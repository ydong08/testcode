/*
  https://blog.csdn.net/yannanxiu/article/details/52199178 
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
    char  *buf;             //指向队列数组的指针
    unsigned int length;    //长度
    unsigned int head;      //队头
    unsigned int tail;      //队尾
    int fill_cnt;           //队列计数
} ringbuf_t;

int ringbuf_init(ringbuf_t* r, char array[], size_t len)
{
    if (len <2 || array==NULL){
        return false;
    }

    r->buf = array;
    r->length = len;
    r->fill_cnt = 0;
    r->head = r->tail = 0;
    return true;
}

int ringbuf_put(ringbuf_t* r, char data)
{
    //当tail+1等于head时，说明队列已满
    if (r->fill_cnt >= r->length) {
        printf("buf full!\n");
        return false;                  // 如果缓冲区满了，则返回错误
    }

    r->buf[r->tail] = data;
    r->tail++;
    r->fill_cnt++;
    //计算tail是否超出数组范围，如果超出则自动切换到0
    r->tail = r->tail % r->length;
    return true;
}

int ringbuf_get(ringbuf_t* r, char *c, unsigned int length)
{
    //当tail等于head时，说明队列空
    if (r->fill_cnt<=0) {
        printf("buf empty!\n");
        return false;
    }

    //最多只能读取r->length长度数据
    if (length > r->length) {
        length = r->length;
    }

    for (int i = 0; i<length; i++)
    {
        r->fill_cnt--;
        *c = r->buf[r->head++];                 // 返回数据给*c
        *c++;
        //计算head自加后的下标是否超出数组范围，如果超出则自动切换到0
        r->head = r->head % r->length;
    }
    return true;
}



#define BUF_LEN 5
ringbuf_t stbuf;
char buf[BUF_LEN];

int main()
{
    ringbuf_init(&stbuf, buf, sizeof(buf));

    printf("1、逐个读取数据测试\r\n");
    int length = 5;
    for (int i = 0; i < length; i++) {
        ringbuf_put(&stbuf, i);
    }

    char data;
    length = 5;
    for (int i = 0; i < length; i++) {
        ringbuf_get(&stbuf, &data, 1); //从BUFF读取1个字节
        printf("每次读取1个字节：buf pop : %d \r\n", data);  //打印该字节
    }

    printf("\n2、一次性读取测试\r\n");
    length = 5;
    for (int i = 0; i < length; i++) {
        ringbuf_put(&stbuf, '1' + i);
    }
    char data2[11] = { 0 };
    ringbuf_get(&stbuf, data2, 5);
    printf("一次性读取5个字节：buf pop : %s \r\n", data2);  //打印该字节

    printf("\n3、放入超过缓冲区长度（BUF_LEN+1）数据测试：\r\n");
    length = BUF_LEN + 1;
    for (int i = 0; i < length; i++){
        ringbuf_put(&stbuf, '1'+i);
    }

    char data3[BUF_LEN+1] = { 0 };
    ringbuf_get(&stbuf, data3, BUF_LEN + 1);
    printf("一次性读取（BUF_LEN+1）个字节测试：buf pop : %s \r\n", data3);  //打印该字节

    //4、测试读取空缓冲区
    printf("\n4、读取空缓冲区测试：\r\n");
    ringbuf_get(&stbuf, data3, 2); //从BUFF读取2个字节

    return 0;
}


