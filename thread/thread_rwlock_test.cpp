
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int main() {
    pthread_rwlock_t rwl;  
    pthread_rwlock_init(&rwl, NULL);

#if 0
    // point1  blocked at *wrlock api 
    printf("enter 1 rdlock\n");
    pthread_rwlock_rdlock(&rwl);  
    printf("enter 2 wrlock\n");
    pthread_rwlock_wrlock(&rwl);  
    
#endif

#if 0
    // point2  non-block
    printf("enter 1 wrlock\n");
    pthread_rwlock_wrlock(&rwl);  
    printf("enter 2 rdlock\n");
    pthread_rwlock_rdlock(&rwl); 
#endif

#if 0
    // point3  non-block
    printf("enter 1 rdlock\n");
    pthread_rwlock_rdlock(&rwl);  
    printf("enter 2 rdlock\n");
    pthread_rwlock_rdlock(&rwl);  
#endif

#if 1
    // point4  non-block
    printf("enter 1 wrlock\n");
    pthread_rwlock_wrlock(&rwl); 
    sleep(2); 
    printf("enter 2 wrlock\n");
    pthread_rwlock_wrlock(&rwl);  
#endif

    printf("enter unlock\n");
    pthread_rwlock_unlock(&rwl);  
    pthread_rwlock_unlock(&rwl); 

    pthread_rwlock_destroy(&rwl);

    return -1;  
}


