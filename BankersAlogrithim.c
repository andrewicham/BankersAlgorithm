/*
HMW4 by Corey Fajfer and Andrew Chambers
Bankers Algorithm w/ Safety
CSCI 402
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUMBER_OF_CUSTOMERS 5 //needs to be dynamic number of customers
#define NUMBER_OF_RESOURCES 3

int available[NUMBER_OF_RESOURCES];
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES]; //needs random initialization
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES]; //needs random initialization
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

int requestResource(int customer_num,int request[]);
int releaseResource(int customer_num,int release[]);
void *customer(void* customer);
int greaterThanNeed(int customer_num,int request[]);
int equalRelease(int customer_num,int release[]);
int equalAllocation();
void printResource();
void printAllocation();
void printAvailable();
void printMaximum();
void printNeed();

//globals//
int i, j;
pthread_mutex_t mutex;

int requestResource(int customer_num, int request[]){
    //check resource allocation
    if(equalAllocation(request) == -1){ //failed
        printf("Allocation has FAILED, not enough resources available!\n");
        return -1;
    }
    else if (greaterThanNeed(customer_num, request) == -1){ //failed
        printf("Allocation has FAILED, requested resources is more than needed!\n");
        return -1;
    }
    else{ //success
        for (i = 0; i < NUMBER_OF_RESOURCES; ++i){
            need[customer_num][i] -= request[i];
            allocation[customer_num][i] += request[i];
            available[i] -= request[i];
        }
    }

    //check safety
    if (CheckSafety() == 0){ //safe
        printf("Allocation is SAFE and resources were allocated!\n");
        printf("Resource:\n");
        printResource();
        printf("Allocation Matrix:\n");
        printAllocation();
        printf("Available Matrix:\n");
        printAvailable();
        printf("Maximum Matrix:\n");
        printMaximum();
        printf("Need Matrix:\n");
        printNeed();
        return 0;
    }
    else{           
        printf("Allocation is NOT SAFE and resources were NOT allocated!\n"); //danger
        for (i = 0; i < NUMBER_OF_RESOURCES; ++i){
            need[customer_num][i] += request[i];
            allocation[customer_num][i] -= request[i];
            available[i] += request[i];
        }         
        return -1;
    }
}

int releaseResource(int customer_num, int release[]){          

    if(equalRelease(customer_num,release) == -1){ //failed
        printf("Release has FAILED, not enough resources available!\n");
        return -1;
    }
    else{ //success
        for(i = 0; i < NUMBER_OF_RESOURCES; i++){
            allocation[customer_num][i] -= release[i];
            need[customer_num][i] += release[i];
            available[i] += release[i];
        }
        printf("Resources released!\n");
        printf("Resource:\n");
        printResource();
        printf("Allocation Matrix:\n");
        printAllocation();
        printf("Available Matrix:\n");
        printAvailable();
        printf("Maximum Matrix:\n");
        printMaximum();
        printf("Need Matrix:\n");
        printNeed();
        return 0;
    }
}

void *customer(void* customer){

    int customer_num = *(int*)customer;
    for(j = 0; j < NUMBER_OF_RESOURCES; j++){ //loops 3 times for 3 resources
        //request//
        int request[NUMBER_OF_RESOURCES];              
        pthread_mutex_lock(&mutex); //lock thread

        //need to initialize need vector randomly here//

        printf("Customer %d is requesting resources:\n", customer_num);
        
        //print the request here//

        requestResource(customer_num, request); //process request
        pthread_mutex_unlock(&mutex); //unlock thread

        //release//
        int release[NUMBER_OF_RESOURCES];                
        pthread_mutex_lock(&mutex); //lock thread
        
        //need to initialize need vector randomly here//

        printf("Customer %d is releasing resources:\n", customer_num);
        
        //print the release here//

        releaseResource(customer_num, release);
        pthread_mutex_unlock(&mutex); //unlock thread
    }
}

int main(int argc, char const *argv[]){

    pthread_t *tid = malloc(sizeof(pthread_t) * NUMBER_OF_CUSTOMERS); //thread ID
    int *pid = malloc(sizeof(int) * NUMBER_OF_CUSTOMERS); //process ID
    pthread_mutex_init(&mutex,NULL); //mutex init

    //ResourceVector//
    for(i = 0; i < NUMBER_OF_RESOURCES; i++){
        ResourceVector[i] = atoi(argv[i+1]); //inititialize vector with user given values
    }
    //need matrix//
    for (j = 0; j < NUMBER_OF_CUSTOMERS; j++){
        for (i = 0; i < NUMBER_OF_RESOURCES; i++){
            need[j][i] = maximum[j][i] - allocation[j][i]; //need = (max-allocation)
        }
    }
    for(i = 0; i < NUMBER_OF_CUSTOMERS; i++){ //create threads
        *(pid + i) = i;
        pthread_create((tid+i), customer, (pid+i));
    }
    for(i = 0; i < NUMBER_OF_CUSTOMERS; i++){ //join threads
        pthread_join(*(tid+i),NULL);
    }
    return 0;
}