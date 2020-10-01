#include<iostream>
#include<cstdio>
#include<pthread.h>
#include<unistd.h>
#include<semaphore.h>
#include<cstring>
#include <time.h>
#include <stdlib.h> 
#include <chrono>
#include <thread>

using namespace std;

/* defining the number of threads, number of servicemen, sleep duration and capacity of the payment room */
#define num_of_cycle_threads 30
#define num_of_servicemen 15
#define cap_of_payment_room 5



int rand_max = 10;

/* some global counters to keep track of num of serviceman and departurer */
int departure_counter = 0;

/* defining the array of mutexes equal to the number of servicemen and a semaphore
to handle the case of the payment*/
pthread_mutex_t servicemen[num_of_servicemen];
sem_t makepay;

/* mutex to handle departure_counter */
pthread_mutex_t departure_counter_mutex;

/* mutex to handle the entry to the servicemen*/
pthread_mutex_t start_service;

/* mutex to handle the secondary entry to the servicemen*/
pthread_mutex_t second_gate;

void increment_departure(){
    departure_counter++;
}

void decrement_departure(){
    departure_counter--;
}


void make_payment(char* cycle_id){

    sem_wait(&makepay);
    printf("%s started paying the service bill\n",cycle_id);
    //sleep(rand() % rand_max + 1);
    this_thread::sleep_for(chrono::milliseconds((rand() % rand_max + 1)));

}

void perform_servicing(char* cycle_id){

    /* locking over the start service mutex, this will control the lock of first variables */
    pthread_mutex_lock(&start_service);

    /* locking the second gate here, that will be controlled by departure*/
    pthread_mutex_lock(&second_gate);

    /* looping over the mutexes to lock and unlock them */
    for (int i = 0; i < num_of_servicemen; i++){
        
        if (i != 0){

            /* locking the i-th serviceman as cycle is being serviced by him */
            pthread_mutex_lock(&servicemen[i]);
            printf("%s started taking service from serviceman %d\n",cycle_id,i+1);


             /* unlocking the mutex here so that others can get a chance to enter the zone 
                at the start of the code */
            if(i == 1){
                pthread_mutex_unlock(&start_service);
            }
           

            /* unlocking the (i-1)-th serviceman so that the previous i-th cycle can now 
               take the required service */
            pthread_mutex_unlock(&servicemen[i-1]);
            
            /* making the thread sleep for a random duration*/
            //sleep(rand() % rand_max + 1);
            this_thread::sleep_for(chrono::milliseconds((rand() % rand_max + 1)));

            /* unlocking the ith thread so that the cycle can go to the next repairman */
            printf("%s finished taking service from serviceman %d\n",cycle_id,i+1);

            if ( i+1 == num_of_servicemen){
                pthread_mutex_unlock(&servicemen[i]);
            }

        }else{

            /* locking the i-th serviceman as cycle is being serviced by him */
            pthread_mutex_lock(&servicemen[i]);


            /* unlocking the second gate first here */
            pthread_mutex_unlock(&second_gate);

            printf("%s started taking service from serviceman %d\n",cycle_id,i+1);
            
            /* making the thread sleep for a random duration*/
            //sleep(rand() % rand_max + 1);
            this_thread::sleep_for(chrono::milliseconds((rand() % rand_max + 1)));

            printf("%s finished taking service from serviceman %d\n",cycle_id,i+1);

            if ( i+1 == num_of_servicemen){

                pthread_mutex_unlock(&servicemen[i]);

                pthread_mutex_unlock(&start_service);

            }
        }
    }

}

void depart_from_shop(char* cycle_id){

    /* increasing the counter */
    pthread_mutex_lock(&departure_counter_mutex);
    
    increment_departure();

    if (departure_counter == 1){

        /* whenever we have a candidate for departure we try to block all incoming service requests
           of the threads */
        pthread_mutex_lock(&second_gate);


        /* then we try to lock all the mutexes of the servicemen, that is we try to make
           the serviceman store empty of any cycles */
        for(int i = 0 ; i < num_of_servicemen ; i++){
            pthread_mutex_lock(&servicemen[i]);
        }

        /* allowing others to enter the region */
        pthread_mutex_unlock(&departure_counter_mutex);

        
        printf("%s finished paying the service bill\n",cycle_id);
        sem_post(&makepay);

        for(int i = num_of_servicemen - 1 ; i >= 0 ; i--){
            pthread_mutex_unlock(&servicemen[i]);
        }
        
        //sleep(rand() % rand_max + 1);
        this_thread::sleep_for(chrono::milliseconds((rand() % rand_max + 1)));

        printf("%s has departed\n",cycle_id);
        
    }else if(departure_counter > 1){

        printf("%s finished paying the service bill\n",cycle_id);
        sem_post(&makepay);

        pthread_mutex_unlock(&departure_counter_mutex);

        for(int i = num_of_servicemen - 1 ; i >= 0 ; i--){
            pthread_mutex_lock(&servicemen[i]);
            pthread_mutex_unlock(&servicemen[i]);
        }
        
        //sleep(rand() % rand_max + 1);
        this_thread::sleep_for(chrono::milliseconds((rand() % rand_max + 1)));

        printf("%s has departed\n",cycle_id);

    }

    //locking for decreasing value
    pthread_mutex_lock(&departure_counter_mutex);

    decrement_departure();

    if (departure_counter == 0 ) pthread_mutex_unlock(&second_gate);

    pthread_mutex_unlock(&departure_counter_mutex);

}



/* method called by each of the cycle threads to take service from the servicemen, make payment
and departure from the store */
void* perform_cycle_repairing(void* arg){

    char *cycle_id;
    cycle_id = (char*) arg;

    perform_servicing(cycle_id);

    /* now we handle the case of payment */
    make_payment(cycle_id);

    /* we perform  departure from the shop */
    depart_from_shop(cycle_id);

}


/* the main function where the main thread runs */
int main(){

    srand (time(NULL));
    
    /* a checker to check if the mutexes and semaphores have been created clearly */
    int result;

    /* initializing the array of mutexes and performing the required checks */
    for (int i = 0; i < num_of_servicemen; i++){
        result = pthread_mutex_init(&servicemen[i], NULL);
        if (result != 0){
            cout<<"Failed To Create Mutex "<<endl;
        }
    }

    // start --> service mutex
    result = pthread_mutex_init(&start_service, NULL);
        if (result != 0){
            cout<<"Failed To Create Mutex "<<endl;
        }

    // departure --> initialize departure counter mutex
    result = pthread_mutex_init(&departure_counter_mutex, NULL);
        if (result != 0){
            cout<<"Failed To Create Mutex "<<endl;
        }

    result = pthread_mutex_init(&second_gate, NULL);
        if (result != 0){
            cout<<"Failed To Create Mutex "<<endl;
        }


    /* initializing the semaphore for payment and performing the required checks */
    result = sem_init(&makepay, 0, cap_of_payment_room);
    if(result != 0){
        printf("Failed To Create Semaphore\n");
    }

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


    // departure --> 
    result = pthread_mutex_destroy(&departure_counter_mutex);

    if (result != 0){
        cout<<"Failed To Destroy Mutex"<<endl;
    }

    // departure --> 
    result = pthread_mutex_destroy(&start_service);

    if (result != 0){
        cout<<"Failed To Destroy Mutex"<<endl;
    }

    // departure --> 
    result = pthread_mutex_destroy(&second_gate);

    if (result != 0){
        cout<<"Failed To Destroy Mutex"<<endl;
    }


    /* destroying the semaphores */
    result = sem_destroy(&makepay);

    if (result != 0){
        cout<<"Failed To Destroy Semaphore"<<endl;
    }

    return 0;

}