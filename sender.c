#include <pthread.h>
#include <string.h>//strncpy
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "sender.h"
#include "list.h"
static char* senderIp;
static int senderPort;
static char* messageTx;
static pthread_t threadSenderID;
static int socketFD;
static struct sockaddr_in serverAddr;
static unsigned int serverAddressSize;
static pthread_t threadKeyboardID; 
static List* pKeyboardToSenderBuffer;
static pthread_mutex_t keyboardToSenderMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t* pEndCond;
static pthread_mutex_t* pListAddOrRemoveMutex;
static pthread_cond_t inputVar = PTHREAD_COND_INITIALIZER;
static pthread_cond_t sentVar = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t* pMainMutex;

void* keyboardThread(void* unused){
    while(1){
        pthread_mutex_lock(&keyboardToSenderMutex);
            printf("Enter a message: ");
            fgets(messageTx, 100, stdin);
            char* message = (char*)malloc(100);
            strcpy(message,messageTx);
            pthread_mutex_lock(pListAddOrRemoveMutex);
                List_prepend(pKeyboardToSenderBuffer,message);
            pthread_mutex_unlock(pListAddOrRemoveMutex);
            pthread_cond_signal(&inputVar);
        pthread_mutex_unlock(&keyboardToSenderMutex);

    }
}
void* senderThread(void* unused){
    printf("Send thread is active\n");
    while(1){
        pthread_mutex_lock(&keyboardToSenderMutex);
        if(List_count(pKeyboardToSenderBuffer)== 0){
            pthread_cond_wait(&inputVar,&keyboardToSenderMutex);

        }
            pthread_mutex_lock(pListAddOrRemoveMutex);
                char* msgtoSend = List_trim(pKeyboardToSenderBuffer);
            pthread_mutex_unlock(pListAddOrRemoveMutex);
        sendto(socketFD,msgtoSend,strlen(msgtoSend),
        0,(struct sockaddr*) &serverAddr,serverAddressSize);
        if(strcmp(messageTx,"!\n") == 0){
                pthread_mutex_lock(pMainMutex);
                    pthread_cond_signal(pEndCond);
                pthread_mutex_unlock(pMainMutex);
        }
        free(msgtoSend);
        pthread_mutex_unlock(&keyboardToSenderMutex);
    }
}
           

void Sender_init(const char* remIp,const char* remPort,pthread_cond_t* pEndConditionVar,pthread_mutex_t* plistmutex,pthread_mutex_t* pMMutex){
    //initialize variables
    pEndCond = pEndConditionVar;
    pMainMutex = pMMutex;
    pListAddOrRemoveMutex = plistmutex;
    pKeyboardToSenderBuffer = List_create();
    messageTx = malloc(100);
    senderIp = (char*)malloc(100);
    senderPort = atoi(remPort);
    strncpy(senderIp,remIp,100);
    socketFD = socket(AF_INET,SOCK_DGRAM,0); //init socket
    memset(&serverAddr, 0, sizeof(serverAddr)); //init serverAddr
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(senderPort);
    serverAddr.sin_addr.s_addr = inet_addr(senderIp);
    serverAddressSize = sizeof(serverAddr);
    bind(socketFD,(struct sockaddr*) &serverAddr,serverAddressSize);
 
    printf("Sending To Address %s Port# %d\n",senderIp,senderPort);
    pthread_create(&threadKeyboardID,NULL,keyboardThread,NULL);
    pthread_create(&threadSenderID,NULL,senderThread,NULL);
}
void Sender_shutDown(void){
    printf("Sender is off");
    free(senderIp);
    free(messageTx);
    pthread_join(threadKeyboardID,NULL);
    pthread_join(threadSenderID,NULL);
    close(socketFD);   
}