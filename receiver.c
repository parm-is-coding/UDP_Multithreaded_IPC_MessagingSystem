#include <pthread.h>
#include <string.h>//strncpy
#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "receiver.h"

#define BUFFERSIZE 100
//static pthread_t thread;
static char* rxPort;
static char* msgToPrint;
static List* pRecieverToPrinterBuffer;


//initialize Receiver static variables and run thread fn
//since local port is data shared between processes it is a critical section

//void receiverThread(){
// 
//
//}




//readBuffer() 
//stores item at the front of the list inside passed char*
//list.removeFront()
//returns the item at the front of the list  
void printerThread(){
    while(pRecieverToPrinterBuffer->numElements == 0);
    //entry_sec()
    {//Critical Section 
        //readBuffer();
    }

}


void Receiver_init(const char* localPort){
    pRecieverToPrinterBuffer = List_create();
    rxPort = (char*)malloc(100);
    msgToPrint = (char*)malloc(500);
    strncpy(rxPort,localPort,100);
    printf("Recieving From Port#%s\n",rxPort);


}
void Receiver_shutDown(void){
    //pthread_join(&thread,NULL);
    free(rxPort);
    free(msgToPrint);
}
