#include <stdio.h>// printf()
#include <stdlib.h>// exit()
#include <pthread.h>

#include "receiver.h"
#include "sender.h"
#include "list.h"

//#include  "scanner.h"
//#include "printer.h"



//Note: printerthread encapsulated inside receiver.h
//Note: keyboardthread encapsulated inside sender.h 
pthread_mutex_t listAddOrRemoveMutex = PTHREAD_MUTEX_INITIALIZER;
int main(int argc,char** args){
    if(argc != 4){
        printf("usage: s-talk <localPort> <remoteIP> <remotePort>\n");
        exit(1);
    }
    List* pReceiverToPrinterBuffer = List_create();
    List* pScannerToSenderBuffer = List_create();
    const char* localPort = args[1];
    const char* remoteIp = args[2];
    const char* remotePort = args[3];
    printf("localPort: %s ip: %s remotePort: %s\n",localPort,remoteIp,remotePort);

    
    Receiver_init(localPort,&listAddOrRemoveMutex);
    //Printer_int(pReceiverToPrinterBuffer);
    Sender_init(remoteIp,remotePort);

    Receiver_shutDown();
    Sender_shutDown();
    return 0;
}