#ifndef __TYPES_H__
#define __TYPES_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Define to prevent recursive inclusion 
-------------------------------------*/
#include <stdio.h>     // 标准输入输出定义
#include <stdlib.h>     // 标准函数库定义
#include <string.h>             // memset
#include <unistd.h>     // Unix标准函数定义，read,write...
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> // 文件控制定义
#include <termios.h>     // POSIX中断控制定义
#include <errno.h>     // 错误号定义
#include <pthread.h>        // pthread_t,pthread_create...
#include "error.h"
#include "debug.h"

/* 类型定义 */
typedef signed        char        int8_t;
typedef unsigned     char         uint8_t;

typedef signed        short         int16_t;
typedef unsigned     short         uint16_t;

typedef signed        int            int32_t;
typedef unsigned     int         uint32_t;

typedef signed        long long     int64_t;
typedef unsigned long long         uint64_t;


#define    BUFFER_SIZE                256

/* 1，COM，串口相关*/
#define    COM_TYPE_UPPER_DEVICE    1
#define    COM_TYPE_LOWER_DEVICE    2

#define    COM_BUFFER_SIZE            (BUFFER_SIZE)


/* 2，pools，池相关 */



/* 3，命令相关*/
#define    CMD_DATA_LEN_MAX        (BUFFER_SIZE)




#ifdef __cplusplus
}
#endif

#endif