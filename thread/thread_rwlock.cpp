
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/select.h>

int data = 100;
void* read1(void* p) {
    pthread_rwlock_t pint = *(pthread_rwlock_t*)p;
    struct timespec ts = {1, 1000000}; //1us
    do 
    {
        pthread_rwlock_rdlock(&pint);
        printf("R1 read: %d\n", data);
        pthread_rwlock_unlock(&pint);
        pselect(0, NULL, NULL, NULL, &ts, NULL); // pselect 不会更新ts的值

    } while(1);
    pthread_exit(NULL);
}

void* read2(void* p) {
    pthread_rwlock_t pint = *(pthread_rwlock_t*)p;
    struct timespec ts = {1, 1000000};
    do 
    {
        pthread_rwlock_rdlock(&pint);
        printf("R2 read: %d\n", data);
        pthread_rwlock_unlock(&pint);
        pselect(0, NULL, NULL, NULL, &ts, NULL);

    } while(1);

    pthread_exit(NULL);

}

void* write1(void* p) {
    pthread_rwlock_t pint = *(pthread_rwlock_t*)p;
    struct timespec ts = {1, 1000000};
    do
    {
        pthread_rwlock_wrlock(&pint);
        ++data;
        printf("W1 write: %d\n", data);
        pthread_rwlock_unlock(&pint);
        pselect(0, NULL, NULL, NULL, &ts, NULL);
    } while(1);
    pthread_exit(NULL);
}

void* write2(void* p) {
    pthread_rwlock_t pint = *(pthread_rwlock_t*)p;
    struct timespec ts = {1, 1000000};
    do 
    {
        pthread_rwlock_wrlock(&pint);
        ++data;
        printf("W2 write: %d\n", data);
        pthread_rwlock_unlock(&pint);
        pselect(0, NULL, NULL, NULL, &ts, NULL);
    } while(1);
    pthread_exit(NULL);
}

int main() {

    int retval;
    pthread_t tidr1, tidr2, tidw1, tidw2;
    pthread_rwlock_t rw;
    pthread_rwlockattr_t rwattr;
    pthread_rwlockattr_init(&rwattr);
    pthread_rwlock_init(&rw, &rwattr);

    retval = pthread_create(&tidr1, NULL, read1, (void*)&rw);
    if (retval) {
        perror("r1 thread create");
        return 1;
    }
    retval = pthread_create(&tidr2, NULL, read2, (void*)&rw);
    if (retval) {
        perror("r2 thread create");
        return 1;
    }

    retval = pthread_create(&tidw1, NULL, write1, (void*)&rw);
    if (retval) {
        perror("w1 thread create");
        return 1;
    }
    retval = pthread_create(&tidw2, NULL, write2, (void*)&rw);
    if (retval) {
        perror("w2 thread create");
        return 1;
    }

    pthread_join(tidr1, NULL);
    pthread_join(tidr2, NULL);
    pthread_join(tidw1, NULL);
    pthread_join(tidw2, NULL);
    pthread_rwlockattr_destroy(&rwattr);
    pthread_rwlock_destroy(&rw);

    return 0;
}