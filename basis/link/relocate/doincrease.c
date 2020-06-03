
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int count = 7;
void increase()
{
    count = count + 1;
}

void doincrease(int n)
{
    count += n;
    increase();
}