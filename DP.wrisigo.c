#include <stdio.h>
#include <ntsid.h>
#include <zconf.h>
#include <pthread.h>


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
#define NUM_PHILOSOPHERS 5
#define RUNTIME 10
#define EAT_TIME 2
#define THINK_TIME 1

pthread_mutex_t forks[NUM_PHILOSOPHERS];



void *philosopher(void *param){

    int *myID;
    int leftIdx;
    int rightIdx;

    myID = (int *) param;
    leftIdx = *myID;
    rightIdx = (*myID + 1) % NUM_PHILOSOPHERS;
    printf("P %d start\n", *myID);

    while ( 1 ) {
        pthread_mutex_lock(&(forks[leftIdx]));
        usleep(10);
        pthread_mutex_lock(&(forks[rightIdx]));
        printf("P %d, eating for %d secs\n", *myID, EAT_TIME);
        sleep(EAT_TIME);
        pthread_mutex_unlock(&(forks[leftIdx]));
        pthread_mutex_unlock(&(forks[rightIdx]));
        printf("P %d, thinking for %d secs\n", *myID, THINK_TIME);
        sleep(THINK_TIME);
    }
}


void *philosopher_good(void *param) {
    int *myID;
    int myForkIdx;
    int aboveForkIdx;
    myID = (int *) param;

    myForkIdx = *myID;
    aboveForkIdx = (*myID + 1) % NUM_PHILOSOPHERS;;
    printf("P %d start\n", *myID);

 while(1) {
     int x = 0;
         pthread_mutex_lock(&(forks[myForkIdx]));
         usleep(1000);
         int failed = pthread_mutex_trylock(&(forks[aboveForkIdx]));
         if (!failed) {
             printf("P %d, eating for %d secs\n", *myID, EAT_TIME);
             sleep(EAT_TIME);
             pthread_mutex_unlock(&(forks[aboveForkIdx]));
             x=1;
         }
         pthread_mutex_unlock(&(forks[myForkIdx]));
         if(x) {
             printf("P %d, thinking for %d secs\n", *myID, THINK_TIME);
             sleep(THINK_TIME);
         }


 }
}


int main() {


    int rtnval;
    int id[NUM_PHILOSOPHERS];
    pthread_t tid[NUM_PHILOSOPHERS];
    pthread_attr_t attr; /* thread attributes */

    pthread_attr_init(&attr);


    for (int i=0; i<NUM_PHILOSOPHERS; ++i) {
        rtnval = pthread_mutex_init(&(forks[i]), NULL);
        if (rtnval != 0) {
            printf("error initializing mutex\n");
            return(8);
        }
    }


    for (int i=0; i<NUM_PHILOSOPHERS; ++i) {
        id[i] = i;
        pthread_create(&(tid[i]), &attr, philosopher_good, &(id[i]));
    }

    sleep(10);

    for (int i=0; i<NUM_PHILOSOPHERS; ++i) {
        pthread_kill(tid[i], 0);
    }


    return 0;
}
#pragma clang diagnostic pop