#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_RESOURCES 3

/* Arrays were declared globally so that they could be easily accessed by processes */
int **maximum;
int available[NUM_RESOURCES];
int **allocation;
int **need;

pthread_mutex_t lock;
void setArraySize(int **arr, int size);
void setMatrixSize(int ***arr, int size);
void populateMatrix(int **arr, int size);
void printMatrix(int **arr, int size);
void* customer(void *arg);
int requestResources(int customerNum, int request[]);
int releaseResources(int customerNum, int release[]);
int safety(int number_customers);

struct{
    int number_customers;
}customerArgs;

/* This will dynamically allocate a two dimensional array, with the user defined number of customers (processes) being one dimension, and the statically
 defined number of resources being the other. *** is used because the address of a 2d array is being passed into function, so that a copy of the 2d array is not 
 created instead */

void setArraySize(int **arr, int size){
    int i;
    *arr = malloc(size * sizeof(int));
}
void setMatrixSize(int ***arr, int size){
    int i;
    *arr = malloc(size * sizeof(int *));
    if(*arr == NULL){
        fprintf(stderr, "no memory available\n");
        return;
    }
    for(i = 0; i < size; i++){
        (*arr)[i] = malloc(NUM_RESOURCES * sizeof(int));
    }
    
}
/* populates the 2d array with random values between 1 and 100 */
void populateMatrix(int** arr, int size){
    int i;
    int j;
    srand(time(0));
    for(i = 0; i < size; i++){
        for(j = 0; j < NUM_RESOURCES; j++){
            arr[i][j] = rand() % 100;
        }
    }
}
/* utility function to print any 2d arrays */
void printMatrix(int** arr, int size)
{
    int i;
    int j;
    for(i = 0; i < size; i++){
        for(j = 0; j < NUM_RESOURCES; j++)
        {
            printf("arr[i][j]: %d \n", arr[i][j]);
        }
    }
}

/* The runner function passed into each instance of pthread init */
void* customer(void *arg){
    //locks the mutex, so that other processes may not have access to the critical section
    pthread_mutex_lock(&lock);

    int request[NUM_RESOURCES];
    
    

    //code for the thread to execute goes here

    //unlocks the mutex so that another process may operate on the critical section
    pthread_mutex_unlock(&lock);
    pthread_exit(0);
}

int requestResources(int customerNum, int request[]){
    int i;
    for(i = 0; i < NUM_RESOURCES; i++){
        need[customerNum][i] = need[customerNum][i] - request[i];
        allocation[customerNum][i] += request[i];
        available[i] = available[i] - request[i];
    }
    return 0;
}

int releaseResources(int customerNum, int release[]){
    return 0;
}

int safety(number_customers){
    int m = NUM_RESOURCES;
    int n = number_customers;
    int* finish;
    int* work;
    int i;
    int j;
    int k;
    int flag = 0;
    //since the number of customers not known at run time, must use dynamic array
    setArraySize(&finish, n);
    setArraySize(&work, m);
    //sets all values in finish array initially to false
    for(i = 0; i < n; i++){
        finish[i] = 0;
    }
    for(i = 0; i < m; i++){
        work[i] = available[i];
    }
    for(i = 0; i < n; i++){
        for(j = 0; j < m; j++){
            if(need[i][j] <= work[i] && finish[i] == 0){
                work[i] = work[i] + allocation[i][j];
                finish[i] = 1;
            }
        }
    }
    for(i = 0; i < m; i++){
        printf("Work: %d\n", work[i]);
    }
    for(i = 0; i < n; i++){
        printf("Finish: %d\n", finish[i]);
    }
    //if any processes are false, returns 1 to say that the process isn't safe
    for(i = 0; i < n; i++){
        if(finish[i] == 0){
            free(finish);
            free(work);
            return 1;
        }
    }
    //frees the dynamically allocated finish array
    free(finish);
    free(work);
    return 0;
}

int main(int argc, char *argv[]){
    //the instructions call for the number of customers to vary, so implemented as a user defined value and allocated at runtime
    int number_customers;
    printf("Enter the number of customers: ");
    scanf("%d", &number_customers);

    pthread_t *tid = malloc(sizeof(pthread_t) * number_customers);
    int *pid = malloc(sizeof(int) * number_customers);

    //initializing mutex
    pthread_mutex_init(&lock, NULL);

    /* takes in the number of resources as command line arguments and stores them in the available array. 
    only will work for commands that have NUM_RESOURCES numerical arguments */
    for(int i = 0; i < NUM_RESOURCES; i++){
        long convert = strtol(argv[i+1], NULL, 10);
        available[i] = convert;
    }
    
    
    //setting the maximum matrix size and populating with random numbers 
    setMatrixSize(&need, number_customers);
    populateMatrix(need, number_customers);
    //printMatrix(need, number_customers);
    setMatrixSize(&allocation, number_customers);
    populateMatrix(allocation, number_customers);
    safety(number_customers);
    




    //destroys the initialized mutex
    pthread_mutex_destroy(&lock);


    free(maximum);
    free(allocation);
    free(need);
    return 0;
}
