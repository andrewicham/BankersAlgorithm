#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define NUMBER_OF_RESOURCES 3

//available, maximum, allocation, need
int available[NUMBER_OF_RESOURCES];
int **maximum = NULL;
int **allocation = NULL;
int **need = NULL;

int requestResource(int customer_num, int request[]);
int releaseResource(int customer_num, int release[]);
void *customer(void* customer);
int CheckSafety();
int greaterThanNeed(int customer_num, int request[]);
int greaterThanAllocation(int customer_num, int release[]);
int greaterThanAvailable(int request[]);
void printMatrix();
void printVector();
void setMatrixSize(int **array, int size);
void setArraySize(int **array, int size);
void populateMatrix(int **array, int size);


//globals
int i, j, k;
int number_of_customers;
pthread_mutex_t mutex;

int requestResource(int customer_num, int request[]){
    //check resource allocation
    if(greaterThanAvailable(request) == -1){ //failed
        printf("Allocation has FAILED, not enough resources available!\n");
        return -1;
    }
    else if (greaterThanNeed(customer_num, request) == -1){ //failed
        printf("Allocation has FAILED, requested resources is more than needed!\n");
        return -1;
    }
    else{ //success
        for (i = 0; i < NUMBER_OF_RESOURCES; i++){
            available[i] -= request[i];
            need[customer_num][i] -= request[i];
            allocation[customer_num][i] += request[i];
        }
    }

    //check safety
    if (CheckSafety(number_of_customers) == 0){ //safe
        printf("Allocation is SAFE and resources were allocated!\n");
        printf("Request:\n");
        printVector(request);
        printf("Allocation Matrix:\n");
        printMatrix(allocation);
        printf("Available Matrix:\n");
        printVector(available);
        printf("Maximum Matrix:\n");
        printMatrix(maximum);
        printf("Need Matrix:\n");
        printMatrix(need);
        return 0;
    }
    else{           
        printf("Allocation is NOT SAFE and resources were NOT allocated!\n"); //danger
        for (i = 0; i < NUMBER_OF_RESOURCES; i++){ //failed, revert matrices to before allocation
            available[i] += request[i];
            need[customer_num][i] += request[i];
            allocation[customer_num][i] -= request[i];
        }         
        return -1;
    }
}

int releaseResource(int customer_num, int release[]){          

    if(greaterThanAllocation(customer_num,release) == -1){ //failed
        printf("Release has FAILED, not enough resources available!\n");
        return -1;
    }
    else{ //success
        for(i = 0; i < NUMBER_OF_RESOURCES; i++){
            available[i] += release[i];
            need[customer_num][i] += release[i];
            allocation[customer_num][i] -= release[i];
        }
        printf("Resources released!\n");
        printf("Release:\n");
        printVector(release);
        printf("Allocation Matrix:\n");
        printMatrix(allocation);
        printf("Available Matrix:\n");
        printVector(available);
        printf("Maximum Matrix:\n");
        printMatrix(maximum);
        printf("Need Matrix:\n");
        printMatrix(need);
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

        printf("Customer #%d is requesting resources:\n", customer_num);
        
        //print the request here//

        requestResource(customer_num, request); //process request
        pthread_mutex_unlock(&mutex); //unlock thread

        //release//
        int release[NUMBER_OF_RESOURCES];                
        pthread_mutex_lock(&mutex); //lock thread
        
        //need to initialize need vector randomly here//

        printf("Customer #%d is releasing resources:\n", customer_num);
        
        //print the release here//

        releaseResource(customer_num, release);
        pthread_mutex_unlock(&mutex); //unlock thread
    }
}

int CheckSafety(int size){

    int *finish;
    setArraySize(&finish, size);
    //initialize Work=Available
    int work[NUMBER_OF_RESOURCES];

    for(i = 0; i < NUMBER_OF_RESOURCES; i++){
        work[i] = available[i];
    }
    for(i = 0; i < number_of_customers; i++){
        if(finish[i] == 0){
            for(j = 0; j < NUMBER_OF_RESOURCES; j++){
                if(need[i][j] <= work[j] && finish[i] == 0){
                    if(j == (NUMBER_OF_RESOURCES - 1)){
                        for (k = 0; k < NUMBER_OF_RESOURCES; k++){
                            work[k] += allocation[i][k];                                                                  
                        }
                        finish[i] = 1;                                                             
                        i = -1;
                    }
                }
            }
        }
    }
            
    for(i = 0; i < number_of_customers; i++){ //if any tests fail, return -1 (not safe)
        if (finish[i] == 0){
            return -1;
        }
    }

    return 0; //success
}

int greaterThanAllocation(int customer_num, int release[]){

    for (i = 0; i < NUMBER_OF_RESOURCES; i++){
        if (release[i] > allocation[customer_num][i]){
            return -1;
        }
    }
    return 0;
}

int greaterThanNeed(int customer_num, int request[]){

    for (i = 0; i < NUMBER_OF_RESOURCES; i++){
        if (request[i] > need[customer_num][i]){
            return -1;
        }
    }
    return 0;
}

int greaterThanAvailable(int request[]){

    for (i = 0; i < NUMBER_OF_RESOURCES; i++){
        if (request[i] > available[i]){
            return -1;
        }
    }
    return 0;
}

void printMatrix(int tempMatrix[][NUMBER_OF_RESOURCES]){
            
    for (i = 0; i < number_of_customers; i++){
        printf("%d: [ ", i+1);
        for (j = 0; j < NUMBER_OF_RESOURCES; j++){
            printf("%d, ", tempMatrix[i][j]);
        }
        printf("]\n");
    }
    return;
}

void printVector(int tempVector[]){
    printf("[ ");
    for (i = 0; i < NUMBER_OF_RESOURCES; i++){
        printf("%d, ", tempVector[i]);
    }
    printf("]\n");
    return;
}

void setMatrixSize(int **array, int size){
    array = (int **)malloc(sizeof(int *) * size);
    for(i = 0; i < size; i++){
	array[i] = (int *)malloc(sizeof(int) * NUMBER_OF_RESOURCES);
    }
}
void setArraySize(int **array, int size){
    *array = calloc(size, sizeof(int));
}
void populateMatrix(int **array, int size){ /*SEGMENTATION FAULT */
    srand(time(0));
    for(i = 0; i < size; i++){
	for(j = 0; j < NUMBER_OF_RESOURCES; j++){
	    array[i][j] = rand() / 10;
	    printf("%d \n", array[i][j]);
	}
    }
}
//(rand() / 10)
int main(int argc, char const *argv[]){

    printf("Enter the number of customers: ");
    scanf("%d", &number_of_customers);

    pthread_t *tid = malloc(sizeof(pthread_t) * number_of_customers); //thread ID
    int *pid = malloc(sizeof(int) * number_of_customers); //process ID
    pthread_mutex_init(&mutex, NULL); //mutex init
    
    setMatrixSize(maximum, number_of_customers);
    setMatrixSize(allocation, number_of_customers);
    setMatrixSize(need, number_of_customers);
    printf("I made it here #1\n");
    populateMatrix(maximum, number_of_customers);
    populateMatrix(allocation, number_of_customers);
    printf("I made it here #2\n");
    //Setup available vector//
    for(i = 0; i < NUMBER_OF_RESOURCES; i++){
        available[i] = atoi(argv[i + 1]); //inititialize vector with user given values
    }
    //Setup need matrix//
    for (i = 0; i < number_of_customers; i++){
        for (j = 0; j < NUMBER_OF_RESOURCES; j++){
            need[i][j] = maximum[i][j] - allocation[i][j]; //need = (max-allocation)
        }
    }

    

    //Setup threads//
    for(i = 0; i < number_of_customers; i++){ //create threads
        *(pid + i) = i;
        pthread_create((tid + i), NULL, customer, (pid + i));
    }
    for(i = 0; i < number_of_customers; i++){ //join threads
        pthread_join(*(tid + i),NULL);
    }

    //clean up
    pthread_mutex_destroy(&mutex);

    return 0;
}
