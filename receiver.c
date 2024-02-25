#include <pthread.h>
#include <string.h>//strncpy
#include "receiver.h"
#include <stdio.h>
#include <stdlib.h>

//static pthread_t thread;
static char* rxPort;
void Receiver_init(const char* localPort){
    rxPort = (char*)malloc(100);
    strncpy(rxPort,localPort,100);
    printf("\n%s",rxPort);
}
void Receiver_shutDown(void){
    //pthread_join(&thread,NULL);
    free(rxPort);
}
