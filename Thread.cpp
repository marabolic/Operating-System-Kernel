#include "PCB.h"
#include "Thread.h"

Thread::Thread(StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice){
    myPCB = new PCB(this, stackSize, timeSlice);
}

Thread::~Thread(){ //pazi ovde
    waitToComplete();
    delete myPCB;
}

void Thread::start(){
    myPCB->start();
}

void Thread::waitToComplete() {
    myPCB->waitToComplete();
}

ID Thread::getId(){
    return myPCB->getId();
}

ID Thread::getRunningId(){
    return PCB::getRunningId();
}
	
Thread * Thread::getThreadById(ID id){
    return PCB::getThreadById(id);
}


void dispatch() {
   PCB::dispatch();
}