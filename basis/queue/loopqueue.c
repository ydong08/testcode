#include <stdio.h>  
#include <stdlib.h>  
#include <pthread.h>  
#include <semaphore.h>  
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
 
 
#define FILESIZE    8*1024
#define FILESUM    16
   
 
 
#define MAXSIZE 1024*8
 
typedef char ElemType;  
typedef struct Queue 
{  
    char *base;  
    int wp;//Ð´Ö¸Õë  
    int rp;//¶ÁÖ¸Õë  
    int queueCnt;//±ê¼Ç¶ÓÁÐÊÇ·ñÂú»ò¿Õ  
} CommQueue;  
//¶ÓÁÐ×´Ì¬£ºÂú¡¢¿Õ¡¢ÔËÐÐÕýÈ·¡¢ÔËÐÐ´íÎó  
typedef enum
{  
    QueueFull = 0,  
    QueueEmpty,  
    QueueOK,  
    QueueFail  
} QueueStatus;  
 
static int isFull(CommQueue *queue);
static int isEmpty(CommQueue *queue);  
//³õÊ¼»¯¶ÓÁÐ  
QueueStatus init_queue(CommQueue* queue);  
//Èë¶ÓÁÐ  
QueueStatus inQueue(ElemType val, CommQueue* queue);  
//³ö¶ÓÁÐ  
QueueStatus outQueue(CommQueue* queue, ElemType *val);  
//¶ÁÈ¡»º³åÇøÊý¾Ý  
int readQueue(ElemType *buf, int size, CommQueue *queue);  
//Íù»º³åÇøÐ´ÈëÊý¾Ý  
int writeQueue(const ElemType *buf, int size, CommQueue *queue);  
 
   
QueueStatus init_queue(CommQueue* queue)  
{  
    queue->base =(char *)calloc(sizeof(char) * MAXSIZE, 1);  
    //·ÖÅäÊ§°Ü  
    if (queue->base == NULL)  
    {  
        perror("malloc()");  
        return QueueFail;  
    }  
    //³õÊ¼»¯¶ÁÐ´Ö¸Õë  
    queue->wp = queue->rp = 0;  
    //³õÊ¼»¯¶ÓÁÐÖÐÔªËØ¸öÊý  
    queue->queueCnt = 0; // conuter  
}  
 
 int isFull(CommQueue *queue)  
{  
    return (queue->queueCnt == MAXSIZE - 1) ? 1 : 0;  
}
 
 
 int isEmpty(CommQueue *queue)  
{  
    return (0==queue->queueCnt)? 1 : 0;  
} 
 
QueueStatus inQueue(ElemType val, CommQueue *queue)  
{  
    if (1==isFull(queue))  
    {
//  printf("queue is full .\n");  
        return QueueFull;  
    }  
    else  
    {  
        memcpy((queue->base + queue->wp),&val, sizeof(ElemType));  
        //»Ø×ª  
        if (++(queue->wp) == MAXSIZE)  
        {  
            queue->wp = 0;  
        }  
        //ÔªËØ¸öÊý¼Ó1  
        (queue->queueCnt)++;  
    }     
    return QueueOK;  
}  
 
QueueStatus outQueue(CommQueue *queue, ElemType *val)  
{  
    if (1==isEmpty(queue))  
    { 
      
//  printf("queue is empty .\n");  
        return QueueEmpty;  
    }  
    else {  
        int index = queue->rp;  
        if (++(queue->rp) == MAXSIZE)  
        {  
            queue->rp = 0;  
        }  
        (queue->queueCnt)--;  
        memcpy(val ,(queue->base + index), sizeof(ElemType));  
    }     
    return QueueOK;  
}  
 
int readQueue(ElemType *buf, int len, CommQueue * queue)  
{  
    int i ;  
    ElemType val;  
    for (i=0; i < len; i++)  
    {  
        QueueStatus ret = outQueue(queue, &val);  
        if (ret != QueueEmpty)   
        {  
            memcpy(buf+i, &val, sizeof(ElemType));  
        } else  
        {  
            break;  
        }  
    }  
    return i;  
}  
 
int writeQueue(const ElemType *buf, int len, CommQueue *queue)  
{  
 
 
    int i ;   
    for (i=0; i < len; i++)   
    {  
        QueueStatus ret = inQueue(buf[i], queue);  
        if (ret == QueueFull)   
        {  
            break;  
        }  
    }  
 
 
}
   
 
 
  typedef struct read_write_0
{  
    char ID; 
    char fdname_buffer_0[8*1024]; 
    int real_read_0;
    int real_write_0;  
}BUFFER_0; 
 
BUFFER_0 buffer_0;
 
void *readDataFromFile(CommQueue *queue)  
{  
    int read_fd;
    int ret ;
    char read_buf[8*1024] = {0};
     
    read_fd = open("/home/lzm/lzm/1.c", O_RDWR|O_CREAT);
    if(-1  == read_fd)
    {
        printf("open file err.\n");
        close(read_fd);
        return;
    }
    ret = read(read_fd,read_buf,sizeof(read_buf));
    if(-1 == ret)
    {
        printf("read error.\n");
        close(read_fd);
        return ;
    }
//  printf("in %d\n",ret);
//  printf("read buff %s\n",read_buf);
    writeQueue(read_buf,ret,queue);
    close(read_fd);
    return NULL;
}  
   
   
void *writeDataToFile(CommQueue *queue)  
{
    sleep(1);    
    int write_fd;
    write_fd = open("/home/lzm/lzm/2.c", O_RDWR|O_CREAT);
        if(-1  == write_fd)
        {
                printf("open file err.\n");
                close(write_fd);
                return;
        }
    char write_buf[8*1024] = {0};
    readQueue(write_buf,8*1024,queue);
    write(write_fd,write_buf,strlen(write_buf));  
     if(-1  == write_fd)
         {
                printf("write file err.\n");
                close(write_fd);
                return;
         }
 
    return NULL;
}  
 
 
int main(int argc , char **argv)  
{  
     int ret; 
     CommQueue queue;  
     init_queue(&queue);  
     pthread_t id1,id2;  
 
     ret = pthread_create(&id1, NULL, (void *)writeDataToFile,&queue);  
     ret = pthread_create(&id2, NULL, (void *)readDataFromFile,&queue);  
   
    pthread_join(id1, NULL);  
    pthread_join(id2, NULL);  
       
 
 
 
    return 0;  
}  