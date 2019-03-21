#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

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
int isSafe(int **need, int** allocation, int** maximum, int number_customers);
void findNeed(int **need, int **allocation, int **maximum, int number_customers);

struct{
    int number_customers;
}customerArgs;



void setArraySize(int **arr, int size){
    int i;
    *arr = calloc(size, sizeof(int));
}
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
    //if arr[i] is null, clean up what has been allocated and return
    for(i = 0; i < size; i++){
        (*arr)[i] = calloc(NUM_RESOURCES, sizeof(int));
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
void populateMatrixFixed(int** arr, int size){
    int data[5][NUM_RESOURCES] = {
        {7,5,3},
        {3,2,2},
        {9,0,2},
        {2,2,2},
        {4,3,3},
    };
    for(int i = 0; i < 5; i++){
        memcpy(arr[i], data[i], sizeof(int) * NUM_RESOURCES);
    }
}
void populateMatrixAllocation(int** arr, int size){
    int data[5][NUM_RESOURCES] = {
        {0,1,0},
        {2,0,0},
        {3,0,2},
        {2,1,1},
        {0,0,2},
    };
    for(int i = 0; i < 5; i++){
        memcpy(arr[i], data[i], sizeof(int) * NUM_RESOURCES);
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
    for(i = 0; i < NUM_RESOURCES; ++i){
        need[customerNum][i] = need[customerNum][i] - request[i];
        allocation[customerNum][i] += request[i];
        available[i] = available[i] - request[i];
    }
    return 0;
}

int releaseResources(int customerNum, int release[]){
    return 0;
}

/* This function will make sure that a current set of processes are in a safe state, and
finds the sequence of processes that will be safe, if possible */
int isSafe(int** need, int** allocation, int** maximum, int number_customers){
    int m = NUM_RESOURCES;
    int n = number_customers;
    int* finish;
    int* work;
    int* safeSequence;//to store the safe sequence of processes
    int i, j, k;
    int count = 0;
    int flag = 0;
    //all of these must be dynamic arrays
    setArraySize(&finish, n);
    setArraySize(&work, m);
    setArraySize(&safeSequence, n);
    //finds the needed resources for the current processes
    findNeed(need, allocation, maximum, number_customers);
    //sets all values in finish array initially to false (zero)
    bzero(finish, sizeof(int) * n);
    memcpy(work, available, sizeof(int) * m);
    for(i = 0; i < NUM_RESOURCES; i++){
        printf("Available[i]: %d ", work[i]);
    }
    printf("\n");
    while(count < n){
        //iterates through the need and work arrays, as long as the processes are not finished
        for(i = 0; i < n; i++){
            if (finish[i] == 0) {
                for(j = 0; j < m; j++){
                    if(need[i][j] > work[j]){
                        break;
                    }
                }
                if (j == m) {
                    printf("Updating Work for %d\n", i);
                    for(k = 0; k < m; k++){
                        work[k] += allocation[i][k];
                        printf("%d\t", work[k]);
                    }
                    //updates the safesequence to the current value, if it is safe
                    safeSequence[count++] = i;
                    printf("Count: %d\n", count);
                    printf("\n");
                    finish[i] = 1;
                    flag = 1;
                }
            }
        }
    }
    for(i = 0; i < n; i++){
        printf("Safe Sequence: %d\n", safeSequence[i]);
    }
    
    //if any processes are false, returns 1 to say that the state isn't safe
    for(i = 0; i < n; i++){
        if(finish[i] == 0){
            free(finish);
            free(work);
            free(safeSequence);
            return 0;
        }
    }
    //frees the dynamically allocated finish array
    free(finish);
    free(work);
    free(safeSequence);
    return 1;
}

void findNeed(int **need, int **allocation, int** maximum, int number_customers){
    int n = number_customers;
    int m = NUM_RESOURCES;
    int i, j;
    for(i = 0; i < n; i++){
        for(j = 0; j < m; j++){
            need[i][j] = maximum[i][j] - allocation[i][j];
        }
    }
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
    setMatrixSize(&maximum, number_customers);
    setMatrixSize(&need, number_customers);
    setMatrixSize(&allocation, number_customers);

    populateMatrixFixed(maximum, number_customers);
    populateMatrixAllocation(allocation, number_customers);
    //populateMatrix(allocation, number_customers);
    int safe;
    safe = isSafe(need, allocation, maximum, number_customers);
    printf("%s\n", safe?"it is safe": "not safe");





    //destroys the initialized mutex
    pthread_mutex_destroy(&lock);


    free(maximum);
    free(allocation);
    free(need);
    return 0;
}
