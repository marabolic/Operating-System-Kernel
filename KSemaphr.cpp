
#include "KSemaphr.h"
#include "SCHEDULE.h"
#include "PCB.h"
#include "Define.h"
#include "Queue.h"



KernelSem::KernelSem(int init){
    val = 0;
    lck = 0;
    myPCB = NULL;
}
KernelSem::~KernelSem(){
    delete myPCB;
	delete blocked;
}

int KernelSem::value() const {
    return val;
}

void KernelSem::block(){
    
    blocked->insert(PCB::running);
    PCB::running=Scheduler::get();
    
}

void KernelSem::deblock(){
   PCB *pcb = blocked->remove();
   if (pcb!=NULL){
       pcb->status = READY;
        Scheduler::put(pcb);
    }
}

int KernelSem::wait(Time maxTimeToWait){
	#ifndef BCC_BLOCK_IGNORE
	asm cli;
	#endif

    if (--val < 0){
        block();
    }

	#ifndef BCC_BLOCK_IGNORE
	asm sti;
	#endif
    return val;
}


int KernelSem::signal(int n){

	#ifndef BCC_BLOCK_IGNORE
	asm cli;
	#endif

    if (val++ > 0){
        deblock();
        
    }
    
	#ifndef BCC_BLOCK_IGNORE
	asm sti;
	#endif
    return val;
}




