#include "KSemaphr.h"
#include "Schedule.h"
#include "Define.h"

KernelSem::KernelSem(int init = 1){
    val = 0;
    lck = 0;
}
KernelSem::~KernelSem(){
    delete myPCB;
}

int KernelSem::value() const {
    return val;
}

void KernelSem::block(){
    
    blocked.insert(PCB::running);
    PCB::running=Scheduler::get();
    
}

void KernelSem::deblock(){
    PCB *pcb = blocked.remove();
    if (pcb!=NULL){
        pcb->status = READY;
        Scheduler::put(pcb);
    }
}

int KernelSem::wait(Time maxTimeToWait){
    lock();

    if (--val < 0){
        block();
    }

    unlock();
}
int KernelSem::signal(int n = 0){
    lock();
    if (val++ > 0){
        deblock();
    }
    
    unlock();
}