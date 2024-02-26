#include <pthread.h>
#include <string.h>//strncpy
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "sender.h"
#include "list.h"
static char* senderIp;
static int senderPort;

static pthread_t threadSenderID;
//static pthread_t threadKeyboardID; 
//static List* pKeyboardToSenderBuffer;
//static pthread_mutex_t keyboardToSenderMutex = PTHREAD_MUTEX_INITIALIZER

void* senderThread(void* unused){
    
    printf("Send thread is active");
    while(1){

    }
}

void Sender_init(const char* remIp,const char* remPort){
    senderIp = (char*)malloc(100);
    senderPort = atoi(remPort);
    strncpy(senderIp,remIp,100);
    printf("Sending To Address %s Port# %d\n",senderIp,senderPort);
    pthread_create(&threadSenderID,NULL,senderThread,NULL);
}
void Sender_shutDown(void){
    free(senderIp);
    pthread_join(threadSenderID,NULL);
    
}