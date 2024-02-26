#include <pthread.h>
#include <string.h>//strncpy
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "list.h"
#include "receiver.h"

#define MSG_MAX_LEN 100
#define BUFFERSIZE 100
//static pthread_t thread;
static int rxPort;
static char* msgToPrint;
static int socketDescriptor;
static List* pReceiverToPrinterBuffer;
static pthread_t threadRecieverID;
//static pthread_t threadPrinterID;
static pthread_mutex_t receiverToPrinterMutex = PTHREAD_MUTEX_INITIALIZER;


//initialize Receiver static variables and run thread fn
//since local port is data shared between processes it is a critical section



void *receiverThread(void*){

     //Initialize Address Internet Interface 
    struct sockaddr_in sin; // _in means internet
    memset(&sin,0, sizeof(sin));
    sin.sin_family = AF_INET;           //Connection may be from network
    sin.sin_addr.s_addr = htonl(INADDR_ANY);  //Host to Network long
    sin.sin_port = htons(rxPort);             //Host to Network short
    
    //Create the socket for UDP
    socketDescriptor = socket(PF_INET,SOCK_DGRAM,0);
    //Bind the socket to the port (PORT) that we specify
    bind(socketDescriptor,(struct sockaddr*) &sin,sizeof(sin));

    while (1){
        //Get the data (blocking)
        //Will change sin (the address) to be the address of the client.
        //Note: sin passes information in and out of call!
        struct sockaddr_in sinRemote;
        unsigned int sin_len = sizeof(sinRemote);
        char messageRx[MSG_MAX_LEN]; //buffer to store Message Recieved
        recvfrom(socketDescriptor,
            messageRx,MSG_MAX_LEN,0,
            (struct sockaddr *) &sinRemote,&sin_len); //fills buffer with whatever data comes in on the network

        //Do Something Amazing to the recieved Message!
        printf("%s\n",messageRx);
        while(pReceiverToPrinterBuffer->numElements > BUFFERSIZE);
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
    pReceiverToPrinterBuffer = List_create();
    rxPort = atoi(localPort);
    msgToPrint = (char*)malloc(500);
    printf("Recieving From Port#%d\n",rxPort);
    pthread_create(&threadRecieverID,NULL,receiverThread,NULL);


}
void Receiver_shutDown(void){
    pthread_join(threadRecieverID,NULL);
    free(msgToPrint);
}
