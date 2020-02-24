#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include "banker.h"
#include <time.h>
#include <unistd.h>

// Put any other macros or constants here using #define
// May be any values >= 0
#define NUM_CUSTOMERS 5
#define NUM_RESOURCES 3

// Put global environment variables here
// Available amount of each resource
int available[NUM_RESOURCES];

// Maximum demand of each customer
int maximum[NUM_CUSTOMERS][NUM_RESOURCES];

// Amount currently allocated to each customer
int allocation[NUM_CUSTOMERS][NUM_RESOURCES];

// Remaining need of each customer
int need[NUM_CUSTOMERS][NUM_RESOURCES];

pthread_mutex_t mutex; /*mutex id*/
bool finished = false;




//Execute Customer Method 
void *executeCustomer(void *customer_id){
    //Request resource function requires a customer id integer 
    int customer_number = *(int*)customer_id;
    int request[3];
    bool request_granted;
    
    while(finished == false){
        for(int i = 0; i < 3; i++){
        request[i] = rand() % (maximum[customer_number][i] + 1);
        }
        pthread_mutex_lock(&mutex);
        request_granted = request_res(customer_number, request);
        pthread_mutex_unlock(&mutex);
        int counter = 0;

        //CHECK IF ALL THE CUSTOMERS ARE FINISHED EXECUTING
        for(int i = 0; i < 5; i++){
            for(int j = 0; j < 3; j++)
            if(need[i][j] == 0){
                counter++;
                
            }
        }
        
        if(counter == 15){
            printf("SYSTEM IS IN SAFE STATE\n");
            exit(0);
        }

        //CURRENT CUSTOMER REQUESTS RESOURCES
        if(request_granted == true){
            printf("Safe!!\n");
            release_res(customer_number,request);
            sleep(1);
            
        }
        if(request_granted == false){
            printf("Unsafe\n");
            
        }
        
    
    }
    return 0;
}

bool request_res(int n_customer, int request[]){
    printf("Customer %d is requesting resources ",n_customer);
    for(int i = 0; i < 3; i++){
        printf("%d",request[i]);
    }
    printf("\n");
    
    //Print all resources 
    printf("Available Resources\n");
    for(int i = 0; i < 3; i++){
        printf("%d ",available[i]);
    }
    printf("\n");

    printf("Max Resources\n");
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 3; j++){
            printf("%d ",maximum[i][j]);
        }
        printf("\n");
    }

    printf("Allocated Resources\n");
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 3; j++){
            printf("%d ",allocation[i][j]);
        }
        printf("\n");
    }


    printf("Needed Resources\n");
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 3; j++){
            printf("%d ",need[i][j]);
        }
        printf("\n");
    }
    printf("\n");



    //CHECK IF REQUESTS CAN BE GRANTED
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 3; j++){
            if(request[j] > need[n_customer][j]){
                printf("Request denied\n");
                return false;
            }
        }
    }
    //CHECK IF REQUESTS CAN BE GRANTED
    for(int i = 0; i < 3; i++){
        if(request[i] > available[i]){
            printf("Request denied\n");
            return false;
        }
    }
    return true;

    
}


bool release_res(int n_customer, int release[]){
    
    
    for(int i = 0; i < 3; i++){
        need[n_customer][i] = need[n_customer][i] - release[i];
        available[i] = available[i] + allocation[n_customer][i];
    }
}



//Main Function
int main(int argc, char *argv[])
{
    
    //Define Threads Customer id
    pthread_t tid[5];
    srand(time(NULL));
    
    for(int i = 0; i < 5; i++){
        int *cid = malloc(sizeof(*cid));
        *cid = i;
        pthread_create(&tid[0], NULL, executeCustomer, (void *)cid);
    }
    //Get the available resources from the user CLI
    available[0] = atoi(argv[1]);
    available[1] = atoi(argv[2]);
    available[2] = atoi(argv[3]);
    
    //Populate the maximum resource array randomly
    for(int i = 0; i < 3; i ++){
        for(int j = 0; j < 5; j++){
            maximum[j][i] = rand() % (available[i] + 1);
        }
    }

    //Populate the resources allocated array randomly
    for(int i = 0; i < 5; i ++){
        for(int j = 0; j < 3; j++){
            allocation[i][j] = rand() % (maximum[i][j] + 1 - 0) + 0;
        }
    }

    //Populate the resources needed Array randomly
    for(int i = 0; i < 5; i ++){
        for(int j = 0; j < 3; j++){
           need[i][j] = maximum[i][j] - allocation[i][j];
        }
    }

    

    pthread_join(tid[0], NULL); 
    pthread_join(tid[1], NULL); 
    pthread_join(tid[3], NULL); 
    pthread_join(tid[4], NULL); 
    


    return EXIT_SUCCESS;
}
