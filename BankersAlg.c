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

void setMatrixSize(int ***arr, int size);
void populateMatrix(int **arr, int size);
void printMatrix(int **arr, int size);
void* processJob(void *arg);
int requestResources(int customerNum, int request[]);
int releaseResources(int customerNum, int release[]);


/* This will dynamically allocate a two dimensional array, with the user defined number of customers (processes) being one dimension, and the statically
 defined number of resources being the other. *** is used because the address of a 2d array is being passed into function, so that a copy of the 2d array is not 
 created instead */
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
/* populates the matrix with random values between 1 and 100 */
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
void* processJob(void *arg){
    //locks the mutex, so that other processes may not have access to the critical section
    pthread_mutex_lock(&lock);

    //code for the thread to execute goes here

    //unlocks the mutex so that another process may operate on the critical section
    pthread_mutex_unlock(&lock);
    pthread_exit(0);
}

int main(int argc, char *argv[]){
    int size;
    //initializing mutex
    pthread_mutex_init(&lock, NULL);

    /* takes in the number of resources as command line arguments and stores them in the available array. 
    only will work for commands that have NUM_RESOURCES numerical arguments */
    for(int i = 0; i < NUM_RESOURCES; i++){
        long convert = strtol(argv[i+1], NULL, 10);
        available[i] = convert;
    }
    printf("Enter the number of customers: ");
    scanf("%d", &size);
    
    //setting the maximum array size and populating with random numbers
    setMatrixSize(&maximum, size);
    populateMatrix(maximum, size);
    
    //destroys the initialized mutex
    pthread_mutex_destroy(&lock);
    free(maximum);
    return 0;
}
