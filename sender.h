#ifndef _SENDER_H
#define _SENDER_H
#include <pthread.h>
void Sender_init(const char* remoteIp,const char* remotePort,pthread_cond_t* EndCondVar,pthread_mutex_t* pListMutex,pthread_mutex_t* pMainMutex);
void Sender_shutDown(void);


#endif