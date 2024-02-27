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
//static pthread_t threadPrinterID;
static pthread_mutex_t receiverToPrinterMutex = PTHREAD_MUTEX_INITIALIZER;




void *receiverThread(void*){
    while (1){
        char messageRx[MSG_MAX_LEN]; //buffer to store Message Recieved
        recvfrom(socketFD,messageRx,MSG_MAX_LEN,0,
        (struct sockaddr *) (struct sockaddr *)&clientAddr,&clientAddressSize); //fills buffer with whatever data comes in on the network
        clientAddressSize = sizeof(clientAddr);
        //Do Something Amazing to the recieved Message!
        printf("%s\n",messageRx);
        //while(pReceiverToPrinterBuffer->numElements > BUFFERSIZE);
        //Critical Section
        //prepend messageRx to List
    }
    printf("Done rx thread!");
    return NULL;

}




//readBuffer() 
//stores item at the front of the list inside passed char*
//list.removeFront()
//returns the item at the front of the list  
// void printerThread(){
//     while(1){
//         while(pReceiverToPrinterBuffer->numElements == 0);
//         //entry_sec()
//         {//Critical Section 
//             //readBuffer();
//         }


//     }

// }


void Receiver_init(const char* localPort){
    //Initialize variables
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


}
void Receiver_shutDown(void){
    pthread_join(threadRecieverID,NULL);
    free(msgToPrint);
    close(socketFD);

}
