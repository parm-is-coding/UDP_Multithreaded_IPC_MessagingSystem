# S-Talk

## Notes

./s-talk ClientIPAddress RemoteIPAddress PortNumber 
list is going to be shared between two modules each running its own thread
each list is shared between your modules. share a mutex between two modules.
you could pass a mutex in if you wanted to. example
void Receiever_int(char* rxMessage, pthread_mutex_t sharedMutexWithOtherThread);