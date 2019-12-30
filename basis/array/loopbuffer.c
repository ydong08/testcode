#include "stdafx.h"
#include <stdio.h>
#include <math.h>
#include <Windows.h>
 
#define RBUFFER_INIT_LEN 5
#define RBUFFER_INC_LEN 1.0
class rbuffer
{
public:
    rbuffer()
    {
        begin = (unsigned char *) new char[RBUFFER_INIT_LEN];
        end = begin + RBUFFER_INIT_LEN;
        head = begin;
        tail = head;
    }
     
    ~rbuffer()
    {
        delete[] begin;
        begin = head = tail = end = NULL;
    }
 
    int getBufferLen()
    {
        int ret = 0;
        switch (getBufferType())
        {
        case 0:
            break;
        case 1:
        {
            ret = tail - head;
        }
        break;
        case 2:
        {
            ret = tail - head;
        }
        break;
        case 3:
        {
            ret = end - head;
        }
        break;
        case 4:
        {
            ret = end - head + tail - begin;
        }
        break;
        case 5:
        {
            ret = end - head + tail - begin;
        }
        break;
        case 6:
        {
            ret = end - begin;
        }
        break;
        default:
            break;
        }
        return ret;
    }
 
    void write(void *data, int len)
    {
    RE_TRY:
        switch (getBufferType())
        {
        case 0:
        {
            if (end - begin > len)
            {
                memcpy(head, data, len);
                tail += len;
            }
            else
            {
                delete[] begin;
                int newlen = RBUFFER_INIT_LEN + (size_t)(ceil((end - begin) / RBUFFER_INC_LEN) * RBUFFER_INC_LEN);
                begin = (unsigned char *) new char[newlen];
                end = begin + newlen;
                head = tail = begin;
                goto RE_TRY;
            }
        }
        break;
        case 1:
        {
            if (end - tail > len)
            {
                memcpy(tail, data, len);
                tail += len;
            }
            else
            {
                int newlen = RBUFFER_INIT_LEN + (size_t)(ceil((end - begin + len) / RBUFFER_INC_LEN) * RBUFFER_INC_LEN);
                unsigned char *p = (unsigned char *) new char[newlen];
                int datalen = tail - head;
                memcpy(p, begin, datalen);
                delete[] begin;
                begin = p;
                head = begin;
                tail = head + datalen;
                end = begin + newlen;
                goto RE_TRY;
            }
        }
        break;
        case 2:
        {
            if (end - tail > len)
            {
                memcpy(tail, data, len);
                tail += len;
            }
            else if ((end - tail + head - begin) > len)
            {
                memcpy(tail, data, end - tail);
                memcpy(begin, (unsigned char *)data + (size_t)end - (size_t)tail, len - (end - tail));
                tail = begin + len - (end - tail);
            }
            else
            {
                int newlen = RBUFFER_INIT_LEN + (size_t)(ceil((end - begin + len) / RBUFFER_INC_LEN) * RBUFFER_INC_LEN);
                unsigned char *p = (unsigned char *) new char[newlen];
                int datalen = tail - head;
                int headoff = (size_t)head - (size_t)begin;
                memcpy(p + headoff, head, datalen);
                delete[] begin;
                begin = p;
                head = begin + headoff;
                tail = head + datalen;
                end = begin + newlen;
                goto RE_TRY;
            }
        }
        break;
        case 3:
        {
            if (head - tail > len)
            {
                memcpy(begin, data, len);
                tail += len;
            }
            else
            {
                int newlen = RBUFFER_INIT_LEN + (size_t)(ceil((end - begin + len) / RBUFFER_INC_LEN) * RBUFFER_INC_LEN);
                unsigned char *p = (unsigned char *) new char[newlen];
                int datalen = end - head;
                int headoff = (size_t)head - (size_t)begin;
                memcpy(p + headoff, head, datalen);
                delete[] begin;
                begin = p;
                head = begin + headoff;
                tail = head + datalen;
                end = begin + newlen;
                goto RE_TRY;
            }
        }
        break;
        case 4:
        {
            if (head - tail > len)
            {
                memcpy(tail, data, len);
                tail += len;
            }
            else
            {
                int newlen = (end - begin) * 2;
                unsigned char *p = (unsigned char *) new char[newlen];
                int datalen = end - head + tail - begin;
                int headoff = (size_t)head - (size_t)begin;
                memcpy(p + headoff, head, end - head);
                memcpy(p + (size_t)end - (size_t)begin, begin, tail - begin);
                delete[] begin;
                begin = p;
                head = begin + headoff;
                tail = head + datalen;
                end = begin + newlen;
                goto RE_TRY;
            }
        }
        break;
        case 5:
        {
            int newlen = RBUFFER_INIT_LEN + (size_t)(ceil((end - begin + len) / RBUFFER_INC_LEN) * RBUFFER_INC_LEN);
            unsigned char *p = (unsigned char *) new char[newlen];
            int datalen = end - head + tail - begin;
            int headoff = (size_t)head - (size_t)begin;
            memcpy(p + headoff, head, end - head);
            memcpy(p + (size_t)end - (size_t)begin, begin, tail - begin);
            delete[] begin;
            begin = p;
            head = begin + headoff;
            tail = head + datalen;
            end = begin + newlen;
            goto RE_TRY;
        }
        break;
        case 6:
        {
            int newlen = RBUFFER_INIT_LEN + (size_t)(ceil((end - begin) / RBUFFER_INC_LEN) * RBUFFER_INC_LEN);
            unsigned char *p = (unsigned char *) new char[newlen];
            int datalen = end - begin;
            memcpy(p, begin, end - begin);
            delete[] begin;
            begin = p;
            end = begin + newlen;
            head = begin;
            tail = head + datalen;
            goto RE_TRY;
        }
        break;
        default:
            break;
        }
    }
 
    int read(void *data, int len)
    {
        int ret = 0;
        switch (getBufferType())
        {
        case 0:
        {
            ret = 0;
        }
        break;
        case 1:
        {
            ret = ((tail - head) > len ? len : (tail - head));
            memcpy(data, head, ret);
            head += ret;
            if (head == tail)
                head = tail = begin;
        }
        break;
        case 2:
        {
            ret = ((tail - head) > len ? len : (tail - head));
            memcpy(data, head, ret);
            head += ret;
            if (head == tail)
                head = tail = begin;
        }
        break;
        case 3:
        {
            ret = ((end - head) > len ? len : (end - head));
            memcpy(data, head, ret);
            head += ret;
            if (head == tail)
                head = tail = begin;
        }
        break;
        case 4:
        {
            int datalen = end - head + tail - begin;
            ret = (datalen > len ? len : datalen);
            if (ret <= end - head)
            {
                memcpy(data, head, ret);
                head += ret;
            }
            else
            {
                memcpy(data, head, end - head);
                memcpy((unsigned char *)data + (size_t)end - (size_t)head, begin, ret - (end - head));
                head = begin + ret - (end - head);
            }
            if(head == tail)
                head = tail = begin;
        }
        break;
        case 5:
        {
            int datalen = end - head + tail - begin;
            ret = (datalen > len ? len : datalen);
            if (ret <= end - head)
            {
                memcpy(data, head, ret);
                head += ret;
            }
            else
            {
                memcpy(data, head, end - head);
                memcpy((unsigned char *)data + (size_t)end - (size_t)head, begin, ret - (end - head));
                head = begin + ret - (end - head);
            }
            if (head == tail)
                head = tail = begin;
        }
        break;
        case 6:
        {
            ret = (end - begin > len ? len : (end - begin));
            memcpy(data, begin, ret);
            head += ret;
            if (head == tail)
                head = tail = begin;
        }
        break;
        default:
            break;
        }
        return ret;
    }
 
//private:
    int getBufferType()
    {
        if (head == tail && head == begin) return 0;
        if (tail == end && head == begin) return 6;
        if (tail == end) tail = begin;
        if (head == end) head = begin;
        if (head == begin && tail < end) return 1;
        if (head > begin && head < tail && tail < end) return 2;
        if (head > begin && head < end && tail == begin) return 3;
        if (head > begin && head < end && tail > begin && tail < head) return 4;
        if (head == tail && head < end && tail > begin) return 5;
 
        throw  "unexcept ~!!\n";
    }
private:
    unsigned char *begin;
    unsigned char *end;
    unsigned char *head;
    unsigned char *tail;
};
 
#include <random>
 
int main()
{
    std::default_random_engine generator;
    generator.seed(GetTickCount());
    std::uniform_int_distribution<int> distribution(0, 9);
    std::uniform_int_distribution<int> xx(1, 2);
 
    int types[7] = { 0,0,0,0,0,0, 0};
    char buff[100];
    rbuffer b;
 
    for (size_t i = 0; i < 0xFF; i++)
    {
        //char c = ;
        b.write(&i, 1);
    }
 
    for (size_t a = 0; a < 1; a++)
    {
         
        //Sleep(1);
        for (size_t i = 0; i < 0xFF; i++)
        {
            int c;
            b.read(&c, 1);
            b.write(&c, 1);
        }
        types[b.getBufferType()] ++;
         
    }
 
    printf("len: %d type: %d  avg: 0:%d 1:%d 2:%d 3:%d 4:%d 5:%d 6:%d\n", b.getBufferLen(), b.getBufferType(), types[0], types[1], types[2], types[3], types[4], types[5], types[6]);
    return 0;
}