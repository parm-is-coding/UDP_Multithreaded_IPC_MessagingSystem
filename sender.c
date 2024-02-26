#include <pthread.h>
#include <string.h>//strncpy
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "sender.h"
static char* senderIp;
static int senderPort;



void Sender_init(const char* remIp,const char* remPort){
    senderIp = (char*)malloc(AF_MAX);
    senderPort = atoi(remPort);
    strncpy(senderIp,remIp,100);
    printf("Sending To Address %s Port# %d\n",senderIp,senderPort);
}
void Sender_shutDown(void){
    free(senderIp);
    
}