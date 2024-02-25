#include <pthread.h>
#include <string.h>//strncpy
#include <stdio.h>
#include <stdlib.h>

#include "receiver.h"
//static pthread_t thread;
static char* rxPort;
void Receiver_init(const char* localPort){
    rxPort = (char*)malloc(100);
    strncpy(rxPort,localPort,100);
    printf("Recieving From Port#%s\n",rxPort);

}
void Receiver_shutDown(void){
    //pthread_join(&thread,NULL);
    free(rxPort);
}
