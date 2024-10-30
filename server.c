#include<stdio.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<string.h>

#define SHM_SIZE 1024

int main(){
    char *shared_memory;
    //key banaoo
    int key=ftok("shmfile",65);
    //shmid banaoo with the help of key
    int shmid=shmget(key,SHM_SIZE,0666|IPC_CREAT);

    if(shmid==-1){
        perror("shmget failed");
        exit(1);
    }
    //attach the created shared memory
    shared_memory=(char*)shmat(shmid,NULL,0);
    if(shared_memory==(char*)-1){
        perror("shmat failed");
        exit(1);
    }

    //take input from user
    printf("enter a message to shared memory:");
    fgets(shared_memory,SHM_SIZE,stdin);

    printf("message writtenr to shared emory: %s\n",shared_memory);

    //detach from shared memroty
    shmdt(shared_memory);
    return 0;
}
