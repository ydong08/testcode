
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int main() {
    pthread_rwlock_t rwl;  
    pthread_rwlock_rdlock(&rwl);  
    pthread_rwlock_wrlock(&rwl);  
    pthread_rwlock_unlock(&rwl);  
    pthread_rwlock_unlock(&rwl);  
    return -1;  
}


