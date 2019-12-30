

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

static volatile int snum = 100;
void* proc(void* p) {
    pthread_spinlock_t* pint = (pthread_spinlock_t*)p;
    for (int i = 0; i < 1000; ++i) {
        pthread_spin_lock(pint);
        //pthread_spin_trylock(&pint);
        ++snum;
        pthread_spin_unlock(pint);
    }

    pthread_exit(NULL);
}


int main() {

    pthread_t tid, tid2;
    pthread_spinlock_t pint;
    pthread_spin_init(&pint, PTHREAD_PROCESS_PRIVATE);

    int retval = pthread_create(&tid, NULL, proc, (void*)&pint);
    if (retval) {
        perror("thread create");
        return 1;
    }
    
    retval = pthread_create(&tid2, NULL, proc, (void*)&pint);
    if (retval) {
        perror("thread create");
        return 1;
    }
    printf("num: %d\n", snum);
    pthread_join(tid, NULL);
    pthread_join(tid2, NULL);
    printf("num: %d\n", snum);
    pthread_spin_destroy(&pint);

    return 0;

}