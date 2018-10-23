

#include <pthread.h>


int main() {

    // mutex attr
    pthread_mutexattr_t mattr;
    pthread_mutexattr_init(&mattr);
    pthread_mutexattr_setprioceiling();
    pthread_mutexattr_setprotocol();
    pthread_mutexattr_setpshared();
    pthread_mutexattr_settype();

    pthread_mutexattr_getprioceiling();
    pthread_mutexattr_getprotocol();
    pthread_mutexattr_getpshared();
    pthread_mutexattr_gettype();
    pthread_mutexattr_destroy(&mattr);


    pthread_condattr_t cattr;
    pthread_condattr_init(&cattr);
    pthread_condattr_setclock();
    pthread_condattr_setpshared();
    
    pthread_condattr_getclock();
    pthread_condattr_getpshared();
    pthread_condattr_destroy(&cattr);
    
    return 0;
}