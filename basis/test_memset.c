
#include <stdio.h>

#define MAX 1000
int ndata[ MAX ][ MAX ];
 
int main()
{
	int i , j;
	int nTemp = 10000;
	i = nTemp, j = nTemp;	
	double BegTime , EndTime;
    BegTime = clock();
    while( i-- ) 
    {
#ifdef BZERO
        bzero(ndata, sizeof(int) * MAX * MAX );  
#else           
        memset(ndata, 0, sizeof(int) * MAX * MAX );
#endif        
    }
    EndTime = clock();
    printf(" %g s \n ", (EndTime - BegTime ) / 1000/1000);

    BegTime = clock();
    while( nTemp-- ) 
    {
        for ( i = 0; i < MAX; i++)
        for ( j = 0; j < MAX; j++)
            ndata[i][j] = 0;
    }

    EndTime = clock();
    printf(" %g s \n ", (EndTime - BegTime ) / 1000/1000);
    return 0;
}