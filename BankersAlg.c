#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_RESOURCES 3


int **maximum;
int available[NUM_RESOURCES];
int **allocation;
int **need;

void setArraySize(int ***arr, int size);
void populateArray(int **arr, int size);
void printMatrix(int **arr, int size);

/* This will dynamically allocate a two dimensional array, with the user defined number of customers (processes) being one dimension, and the statically
 defined number of resources being the other. *** is used because the address of a 2d array is being passed into function */
void setArraySize(int ***arr, int size){
    int i;
    *arr = malloc(size * sizeof(int *));
    if(*arr == NULL){
        fprintf(stderr, "no memory\n");
        return;
    }
    for(i = 0; i < size; i++){
        (*arr)[i] = malloc(NUM_RESOURCES * sizeof(int));
    }
    
}

void populateArray(int** arr, int size){
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
int main(int argc, char *argv[]){
    int size;
    int available[NUM_RESOURCES] = {2, 3, 10};
    
    printf("Enter size: ");
    scanf("%d", &size);
    setArraySize(&maximum, size);
    populateArray(maximum, size);
    printMatrix(maximum, size);
    free(maximum);
    return 0;
}
