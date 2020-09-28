#include<iostream>
#include<cstdio>
#include<pthread.h>
#include<unistd.h>
#include<semaphore.h>
#include<cstring>
#include <time.h>

using namespace std;

/* defining the number of threads, number of servicemen, sleep duration and capacity of the payment room */
#define num_of_cycle_threads 5
#define num_of_servicemen 3
#define cap_of_payment_room 2
#define sleep_for 2

/* defining the array of mutexes equal to the number of servicemen and a semaphore
to handle the case of the payment*/
pthread_mutex_t servicemen[num_of_servicemen];
sem_t makepay;


/* method called by each of the cycle threads to take service from the servicemen, make payment
and departure from the store */
void* perform_cycle_repairing(void* arg){

    char *cycle_id;
    cycle_id = (char*) arg;

    /* looping over the mutexes to lock and unlock them */
    for (int i = 0; i < num_of_servicemen; i++){
        
        if (i != 0){

            /* locking the ith service man as cycle is being serviced by him */
            pthread_mutex_lock(&servicemen[i]);
            printf("%s started taking service from serviceman %d\n",cycle_id,i+1);

            /* unlocking the i-1th service man so that the previous cycle can now 
               take the required service */
            pthread_mutex_unlock(&servicemen[i-1]);
            
            /* making the thread sleep for a random duration*/
            sleep(sleep_for);

            /* unlocking the ith thread so that the cycle can go to the next repairman */
            //pthread_mutex_unlock(&servicemen[i]);
            printf("%s finished taking service from serviceman %d\n",cycle_id,i+1);

            if ( i+1 == num_of_servicemen){
                pthread_mutex_unlock(&servicemen[i]);
            }

        }else{

            /* locking the ith service man as cycle is being serviced by him */
            pthread_mutex_lock(&servicemen[i]);
            printf("%s started taking service from serviceman %d\n",cycle_id,i+1);
            
            /* making the thread sleep for a random duration*/
            sleep(sleep_for);

            /* unlocking the ith thread so that the cycle can go to the next repairman */
            //pthread_mutex_unlock(&servicemen[i]);
            printf("%s finished taking service from serviceman %d\n",cycle_id,i+1);
        }
    }

    // sleep(1);
    // printf("Just testing for thread %s\n", cycle_id);
}


/* the main function where the main thread runs */
int main(){
    
    /* a checker to check if the mutexes and semaphores have been created clearly */
    int result;

    /* initializing the array of mutexes and performing the required checks */
    for (int i = 0; i < num_of_servicemen; i++){
        result = pthread_mutex_init(&servicemen[i], NULL);
        if (result != 0){
            cout<<"Failed To Create Mutex "<<endl;
        }
    }

    /* initializing the semaphore for payment and performing the required checks */
    result = sem_init(&makepay, 0, cap_of_payment_room);

    /* creating the required number of threads */
    pthread_t cycles[num_of_cycle_threads];

    for (int i = 0; i < num_of_cycle_threads; i++){

        char *cycle_id = new char[5];
        strcpy(cycle_id,to_string(i+1).c_str());

        result = pthread_create(&cycles[i],NULL,perform_cycle_repairing,(void *)cycle_id);

        if(result != 0){
            cout<<"Failed To Create Thread"<<endl;
        }
    }

    /* joining the required number of threads with the main thread */
    for (int i = 0; i < num_of_cycle_threads; i++){
        void *thread_result;
        pthread_join(cycles[i], &thread_result);
    }

    /* destroying the mutexes */
    for (int i = 0; i < num_of_servicemen ; i++){
        result = pthread_mutex_destroy(&servicemen[i]);

        if (result != 0){
            cout<<"Failed To Destroy Mutex"<<endl;
        }
    }

    /* destroying the semaphores */
    result = sem_destroy(&makepay);

    if (result != 0){
        cout<<"Failed To Destroy Semaphore"<<endl;
    }

    return 0;


}