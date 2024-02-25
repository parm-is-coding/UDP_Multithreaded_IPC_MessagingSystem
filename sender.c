#include <pthread.h>
#include <string.h>//strncpy
#include <stdio.h>
#include <stdlib.h>

#include "sender.h"
static char* senderIp;
static char* senderPort;
void Sender_init(const char* remIp,const char* remPort){
    senderIp = (char*)malloc(100);
    senderPort = (char*)malloc(100);
    strncpy(senderIp,remIp,100);
    strncpy(senderPort,remPort,100);
    printf("Sending To Address %s Port# %s\n",senderIp,senderPort);
}
void Sender_shutDown(void){
    free(senderIp);
    free(senderPort);
}