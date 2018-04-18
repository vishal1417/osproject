#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#define MAX_N 100000
#define MAX_THREADS 25


int threadNumber;
int largestNumber;
int isPrime;
int nthreads,  
    prime[MAX_N + 1],
    n,  
    nextbase;  


pthread_mutex_t nextbaselock = PTHREAD_MUTEX_INITIALIZER;

void crossout() {
    int i, j, check;
    for (i = 0; i <=MAX_N; i++)
        prime[i] = 1;

    for (i = 2; i*i <=MAX_N;i++)
        if (prime[i])
            for (j = i*2; j < MAX_N; j+=i)
                prime[j] = 0;
}

int checkPrime(int a) {
    int i,isPrime;
        if (prime[a])
            isPrime = 2;
        else
            isPrime = 1;
        return isPrime;
}

void*  workerThread(void* t) {

    int lim=largestNumber, base;
    long i, j;
    long tid;

    tid = (long)t;
    printf("\nThread %ld starting", tid);

    while (1)  {


        pthread_mutex_lock(&nextbaselock);
        base = nextbase;
        nextbase++;
        pthread_mutex_unlock(&nextbaselock);
        if (checkPrime(base) == 2)
           { printf("\nThread %ld done. Prime = %d\n", tid, base);
            	pthread_exit((void*) t); 
	   }
        
    }

    return NULL;
}


int main(int argc, char** argv) {

    printf("\nEnter the number of threads=");
    scanf("%d",&threadNumber);
    printf("\nEnter the largest number=");
    scanf("%d",&largestNumber);


    int i;
    crossout();
    pthread_t thread[threadNumber];
    int rc;
    long t;
    void* status;

    for (t = 0; t < threadNumber; t++) {
        printf("Main: creating thread %ld\n", t);
        rc = pthread_create(&thread[t], NULL, workerThread, (void*)t);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    for (t = 0; t < threadNumber; t++) {
        rc = pthread_join(thread[t], (void*)&t);
        if (rc) {
            printf("ERROR; return code from pthread_join() is %d\n", rc);
            exit(-1);
        }
        printf("completed join with thread %ld \n", t);
    }

}



