#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//can be any values >= 0
#define NUM_RESOURCES 3

int available[NUM_RESOURCES];
int** maximum = NULL;
int* allocation = NULL;
int* need = NULL;

void setArraySize(int **arr, unsigned int size);
void populateArray(int **arr, unsigned int size);
void freeArray(int *arr);

/* This will dynamically allocate a two dimensional array, with the user defined number of customers being one dimension, and the statically 
defined number of resources being the other */
void setArraySize(int **arr, unsigned int size){
    arr = malloc(sizeof(int[size][NUM_RESOURCES]));
    printf("Size: %u\n", size);
}

void populateArray(int **arr, unsigned int size){
    int i;
    int j;
    
    //current time used as the seed for the random number generator
    srand(time(0));
    
    for(i = 0; i < size; i++){
        for (j = 0; j < NUM_RESOURCES; j++)
        {   
            
        }
    }
    
}

void printArray(int** arr, unsigned int size)
{
    int i;
    int j;
    for(i = 0; i < NUM_RESOURCES; i++){
        for(j = 0; j < size; j++)
        {
            printf("Array Value: %d\n", *arr[j]);
        }
    }
    
}
int main(){
    unsigned int size;
    printf("Enter size: ");
    scanf("%u", &size);
    setArraySize(maximum, size);
    populateArray(maximum, size);
    //printArray(maximum, size);
    free(maximum);
    return 0;
}