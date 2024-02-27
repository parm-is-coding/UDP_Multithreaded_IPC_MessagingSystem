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
static unsigned int sin_len;
//static pthread_t threadKeyboardID; 
//static List* pKeyboardToSenderBuffer;
//static pthread_mutex_t keyboardToSenderMutex = PTHREAD_MUTEX_INITIALIZER

void* senderThread(void* unused){
    printf("Send thread is active\n");
    while(1){
        printf("Enter a message: ");
        fgets(messageTx, 100, stdin); // Read a line from the console
        sendto(socketFD,messageTx,strlen(messageTx),
        0,(struct sockaddr*) &serverAddr,sin_len);
    
    }
}

void Sender_init(const char* remIp,const char* remPort){
    //initialize variables
    messageTx = malloc(100);
    senderIp = (char*)malloc(100);
    senderPort = atoi(remPort);
    strncpy(senderIp,remIp,100);
    socketFD = socket(AF_INET,SOCK_DGRAM,0); //init socket
    memset(&serverAddr, 0, sizeof(serverAddr)); //init serverAddr
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(senderPort);
    serverAddr.sin_addr.s_addr = inet_addr(senderIp);
    sin_len = sizeof(serverAddr);
    bind(socketFD,(struct sockaddr*) &serverAddr,sin_len);
 
    printf("Sending To Address %s Port# %d\n",senderIp,senderPort);
    pthread_create(&threadSenderID,NULL,senderThread,NULL);
}
void Sender_shutDown(void){
    free(senderIp);
    free(messageTx);
    pthread_join(threadSenderID,NULL);
    close(socketFD);   
}