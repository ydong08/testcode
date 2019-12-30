
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <signal.h>

// union sigval {          /* Data passed with notification */
//     int     sival_int;         /* Integer value */
//     void   *sival_ptr;         /* Pointer value */
// };

// struct sigevent {
//     int          sigev_notify; /* Notification method */
//     int          sigev_signo;  /* Notification signal */
//     union sigval sigev_value;  /* Data passed with notification */
//     void         (*sigev_notify_function) (union sigval);
//                  /* Function used for thread notification (SIGEV_THREAD) */
//     void         *sigev_notify_attributes;
//                  /* Attributes for notification thread (SIGEV_THREAD) */
//     pid_t        sigev_notify_thread_id;
//                  /* ID of thread to signal (SIGEV_THREAD_ID) */
// };





int main() {
    timer_t trt;
    struct sigevent sgt;
    memset(&sgt, 0x00, sizeof(sgt));

    int retval = timer_create(CLOCK_MONOTONIC, NULL, &trt);

}



