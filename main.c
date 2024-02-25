#include <stdio.h>// printf()
#include <stdlib.h>// exit()



int main(int argc,char** args){
    if(argc != 4){
        printf("usage: s-talk <localPort> <remoteIP> <remotePort>");
        exit(1);
    }
    const char* localPort = args[1];
    const char* remoteIp = args[2];
    const char* remotePort = args[3];
    printf("localPort: %s ip: %s remotePort: %s",localPort,remoteIp,remotePort);


    // Receiver_init();
    // Sender_init(ipserv,port);

    // Receiver_shutDown();
    // Sender_shutDown();
    return 0;
}