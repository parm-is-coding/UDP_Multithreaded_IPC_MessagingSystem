#include <stdio.h>// printf()
#include <stdlib.h>// exit()

#include "receiver.h"


int main(int argc,char** args){
    if(argc != 4){
        printf("usage: s-talk <localPort> <remoteIP> <remotePort>\n");
        exit(1);
    }
    const char* localPort = args[1];
    const char* remoteIp = args[2];
    const char* remotePort = args[3];
    printf("localPort: %s ip: %s remotePort: %s\n",localPort,remoteIp,remotePort);


    Receiver_init(localPort);
    //Sender_init(remoteIp,remotePort);

    Receiver_shutDown();
    //Sender_shutDown();
    return 0;
}