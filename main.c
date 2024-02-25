#include <stdio.h>// printf()
#include <stdlib.h>// exit()


int main(int argc,char** args){
    if(argc != 3){
        printf("usage: s-talk ipAddressToConnectTo PORT");
        exit(1);
    }
    const char* ipserv = args[1];
    const char* port = args[2];
    printf("ip: %s port: %s",ipserv,port);
    return 0;
}