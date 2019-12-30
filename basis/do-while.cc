#include <stdio.h>
#include <stdlib.h>

int print(int index)
{
    int arr[10] = {1,2,3,4,5,0,6,7,8,9,};

    return arr[index];

}

int main()
{
    int nRet = 0;
    int index = 0;
    do
    {
        ++index;
        nRet = print(index);
        printf("%d-nRet:%d\n",index, nRet);
    } while(nRet);

    return 0;
}