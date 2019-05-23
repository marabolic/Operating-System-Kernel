

#include "PCB.h"
#include "SCHEDULE.h"

#include "Define.h"
#include "Thread.h"
#include <dos.h>


ID PCB::id =0;
PCB * volatile PCB::running = NULL;

PCB::PCB(Thread *myThread, StackSize stackSize, Time timeSlice) {
    threadId = ++id;
    my_thread = myThread;
    stack_size = stackSize;
    time_slice = timeSlice;
    stack = NULL;
    sem = new KernelSem(0);
    status = CREATED;
    completed = FALSE;
    started = FALSE;
}

PCB::~PCB(){
	delete sem;
	delete my_thread;
	delete []stack;
}


void PCB::createProcess(PCB *newPCB){
   stack = new unsigned[stack_size];
   stack[stack_size-1] = 0x200;
	#ifndef BCC_BLOCK_IGNORE
   stack[stack_size-2] = FP_SEG(my_thread);
   stack[stack_size-3] = FP_OFF(my_thread);
   newPCB->bp = newPCB->sp = FP_OFF(stack + stack_size - 12);
   newPCB->ss = FP_SEG(stack + stack_size - 12);
	#endif
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
    
    PCB *temp = Scheduler::get();
    while (temp->my_thread->getId() != id){
       Scheduler::put(temp);
       temp = Scheduler::get();
    }
    return temp->my_thread;
}

 void PCB::wrapper() {
     PCB::running->my_thread->run();
     //...
 }


 void PCB::dispatch(){

 }
