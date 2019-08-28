
#include "KEvent.h"
#include "Define.h"

extern volatile BOOL lockFlag;

extern KernelEv *evArr[256];

KernelEv::KernelEv(IVTNo ivtNo){
	lock();
#ifdef SEM_EVENT
	sem =  new KernelSem(0);
#endif
	owner = PCB::running;
	this->ivtNo = ivtNo;
	evArr[ivtNo] = (KernelEv*)this;
	unlock();
}
	
KernelEv::~KernelEv(){
	//lock();
#ifdef SEM_EVENT
	delete sem;
#endif
	owner = NULL;
	evArr[ivtNo] = NULL;
	//unlock();
}


void KernelEv::wait(){
#ifdef SEM_EVENT
	//asm pushf;
	//asm cli;
	if (PCB::running == owner){
		sem->wait(0);
	}
	//asm popf;
#else
	if (PCB::running == owner){
		owner->status = BLOCKED;
		dispatch();
	}
#endif
}
    
void KernelEv::signal(){
#ifdef SEM_EVENT
	//asm pushf;
	//asm cli;
	if (sem->val() == -1){
		sem->signal(0);
	}

	//asm popf;
#else
	lock();
	if (owner->status == BLOCKED){
		owner->status = READY;
		Scheduler::put(owner);
	}
	unlock();
#endif
}
