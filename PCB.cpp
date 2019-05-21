
#include "PCB.h"



PCB::PCB(Thread *myThread, StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice) {
    threadId = ++id;
    my_thread = myThread;
    stack_size = stackSize;
    time_slice = timeSlice;
    stack = NULL;
    sem = new KernelSem(0);
    completed = FALSE;
    started = FALSE;
}

void PCB::start() { 
    if (!started){
        //...
        started = TRUE;
    }
}
void PCB::waitToComplete() {
    if (!completed)
        sem->wait(time_slice);
}

ID PCB::getId() {
    return threadId;
}
	
ID PCB::getRunningId() {
    return PCB::running->threadId;
}

Thread * PCB::getThreadById(ID id){
    for (Queue::Node *temp = queue.first; temp != queue.last; temp = temp->next) {
        if (temp->val->threadId == id){
            return temp->val->my_thread;
        }
    }
}

 void PCB::wrapper() {
     PCB::running->my_thread->run();
     //...
 }