#include <pthread.h>
#include <string.h>//strncpy
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "list.h"
#include "receiver.h"
#include "arpa/inet.h"

#define MSG_MAX_LEN 100
#define BUFFERSIZE 100
//static pthread_t thread;
static int rxPort;
static char* msgToPrint;
static int socketFD;
static socklen_t serverAddressSize;
static socklen_t clientAddressSize;
static struct sockaddr_in serverAddr; 
static struct sockaddr_in clientAddr; 
static List* pReceiverToPrinterBuffer;
static pthread_t threadRecieverID;
static pthread_t threadPrinterID;
static pthread_mutex_t rxtoptMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t received = PTHREAD_COND_INITIALIZER;
static pthread_cond_t printed = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t* pListAddRemoveMutex;
static pthread_cond_t* pEndCond;


void *printerThread(void*){
    while(1){
        pthread_mutex_lock(&rxtoptMutex);
            pthread_cond_wait(&received,&rxtoptMutex);
            pthread_mutex_lock(pListAddRemoveMutex);
                char* messageToPrint = (char*)List_trim(pReceiverToPrinterBuffer);
            pthread_mutex_unlock(pListAddRemoveMutex);
            printf("%s",messageToPrint);
            free(messageToPrint);
        pthread_mutex_unlock(&rxtoptMutex);
        pthread_cond_signal(&printed);
    }
    printf("Done printer thread");
    return NULL;
}


void *receiverThread(void*){
    printf("Receive thread active\n");
    while (1){
        pthread_mutex_lock(&rxtoptMutex);
            char messageRx[MSG_MAX_LEN]; //buffer to store Message Recieved
            recvfrom(socketFD,messageRx,MSG_MAX_LEN,0,
            (struct sockaddr *) (struct sockaddr *)&clientAddr,&clientAddressSize); //fills buffer with whatever data comes in on the network
            clientAddressSize = sizeof(clientAddr);
            if(strcmp(messageRx,"!")==0){
                pthread_cond_signal(pEndCond);
            }



            pthread_mutex_lock(pListAddRemoveMutex);
            char* messageToAdd = malloc(100);
            strcpy(messageToAdd,messageRx);
            List_prepend(pReceiverToPrinterBuffer,messageToAdd);
            messageToAdd = NULL;
            pthread_mutex_unlock(pListAddRemoveMutex);
        pthread_mutex_unlock(&rxtoptMutex);
        pthread_cond_signal(&received);
        pthread_mutex_lock(&rxtoptMutex);
            pthread_cond_wait(&printed,&rxtoptMutex);
        pthread_mutex_unlock(&rxtoptMutex);
    }
    printf("Done rx thread!");
    return NULL;

}


void Receiver_init(const char* localPort,pthread_mutex_t* pListAddRmMutex,pthread_cond_t* pEndCondition){
    //Initialize variables
    pListAddRemoveMutex = pListAddRmMutex;
    pEndCond = pEndCondition;
    pReceiverToPrinterBuffer = List_create();
    rxPort = atoi(localPort);
    msgToPrint = (char*)malloc(500);
    socketFD = socket(AF_INET,SOCK_DGRAM,0);
    //Initialize Server address 
    memset(&serverAddr,0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;           //Connection may be from network
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    serverAddr.sin_port = htons(rxPort);             //Host to Network short
    serverAddressSize = sizeof(serverAddr);
    //Bind server address to localPort
    bind(socketFD,(struct sockaddr*) &serverAddr,serverAddressSize);
    printf("Recieving From Port#%d\n",rxPort);
    //Initialize Client address
    memset(&clientAddr,0, sizeof(clientAddr));
    clientAddressSize = sizeof(clientAddr);
   
   
    pthread_create(&threadRecieverID,NULL,receiverThread,NULL);
    pthread_create(&threadPrinterID,NULL,printerThread,NULL);

}
void Receiver_shutDown(void){
    printf("Receiver is off");
    pthread_join(threadRecieverID,NULL);
    pthread_join(threadPrinterID,NULL);
    free(msgToPrint);
    close(socketFD);

}


