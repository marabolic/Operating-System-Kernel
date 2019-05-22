
#include "PCB.h"
#include "Schedule.h"
#include <dos.h>


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


void PCB::createProcess(PCB *newPCB){
   stack = new unsigned[stack_size];
   stack[stack_size-1] = 0x200;
   stack[stack_size-2] = FP_SEG(my_thread);
   stack[stack_size-3] = FP_OFF(my_thread);
   newPCB->bp = newPCB->sp = FP_OFF(stack + stack_size - 12);
   newPCB->ss = FP_SEG(stack + stack_size - 12);

   status = READY;
   newPCB->completed = 0;
}

void PCB::start() { 
    if (!started){
        createProcess(this);
        Scheduler::put(this);
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