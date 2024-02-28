# S-Talk

## Notes
credit 
Brian Fraser Receiverthread


./s-talk ClientIPAddress RemoteIPAddress PortNumber 
list is going to be shared between two modules each running its own thread
each list is shared between your modules. share a mutex between two modules.
you could pass a mutex in if you wanted to. example
void Receiever_int(char* rxMessage, pthread_mutex_t sharedMutexWithOtherThread);

Critical Sections (All Threads)
case(1): adding from any list amongst any thread
case(2): removing from any list amongst any thread

solution -> ListAddOrRemoveMutex

Critical Sections receiverThread
case(1) adding the recieved message to the list;

Critical Sections printerThread
case(2)

Critical Sections scannerThread
case(1)

Critical Sections senderThread
case(2)

Current Issues
thread ordering on startup is random, therefore program will only receive if "receive thread is active" 
is more recent than "send thread is active"

This happens randomly one in eight startups but the program will work if this is acheived


