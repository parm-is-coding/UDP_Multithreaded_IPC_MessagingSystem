#include <pthread.h>
#include <string.h>//strncpy
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include "list.h"
#include "receiver.h"

#define BUFFERSIZE 100
//static pthread_t thread;
static int rxPort;
static char* msgToPrint;

static List* pRecieverToPrinterBuffer;
static pthread_t threadReciever;
static pthread_t threadPrinter;


//initialize Receiver static variables and run thread fn
//since local port is data shared between processes it is a critical section



void receiverThread(){

     //Initialize Address Internet Interface 
    struct sockaddr_in sin; // _in means internet
    memset(&sin,0, sizeof(sin));
    sin.sin_family = AF_INET;           //Connection may be from network
    sin.sin_addr.s_addr = htonl(INADDR_ANY);  //Host to Network long
    sin.sin_port = htons(rxPort);             //Host to Network short
    while(1){


    }

}




//readBuffer() 
//stores item at the front of the list inside passed char*
//list.removeFront()
//returns the item at the front of the list  
void printerThread(){
    while(1){
        while(pRecieverToPrinterBuffer->numElements == 0);
        //entry_sec()
        {//Critical Section 
            //readBuffer();
        }


    }

}


void Receiver_init(const char* localPort,pthread_t* pListAddOrRemoveMutex){
    pRecieverToPrinterBuffer = List_create();
    rxPort = atoi(localPort);
    msgToPrint = (char*)malloc(500);
    printf("Recieving From Port#%d\n",rxPort);
    


}
void Receiver_shutDown(void){
    //pthread_join(&thread,NULL);
    free(msgToPrint);
}
