#include "strstr.h"
#include "strtok.h"
#include "test_file.h"
#include "test_syscall.h"
#include "testhtonll.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{

    str_test();

    strtok_test();

    file_test();

    pid_test();

    htonl_test();
    
    return 0;
}

