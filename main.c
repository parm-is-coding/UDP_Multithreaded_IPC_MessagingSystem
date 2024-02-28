#include <stdio.h>// printf()
#include <stdlib.h>// exit()
#include <pthread.h>

#include "receiver.h"
#include "sender.h"






//Note: printerthread encapsulated inside receiver.h
//Note: keyboardthread encapsulated inside sender.h 
static pthread_mutex_t listAddOrRemoveMutex;
static pthread_mutex_t mainMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t EndCondVar;

int main(int argc,char** args){
    if(argc != 4){
        printf("usage: s-talk <localPort> <remoteIP> <remotePort>\n");
        exit(1);
    }

    const char* localPort = args[1];
    const char* remoteIp = args[2];
    const char* remotePort = args[3];
    printf("localPort: %s ip: %s remotePort: %s\n",localPort,remoteIp,remotePort);

    pthread_mutex_init(&listAddOrRemoveMutex,NULL);
    pthread_cond_init(&EndCondVar,NULL);
    Receiver_init(localPort,&listAddOrRemoveMutex,&EndCondVar);
    Sender_init(remoteIp,remotePort,&EndCondVar,&listAddOrRemoveMutex);
    pthread_mutex_lock(&mainMutex);
    pthread_cond_wait(&EndCondVar,&mainMutex);
    
    Receiver_shutDown();
    Sender_shutDown();
    pthread_mutex_destroy(&mainMutex);
    pthread_mutex_destroy(&listAddOrRemoveMutex);
    pthread_cond_destroy(&EndCondVar);
    printf("program is done :)");
    return 0;
}