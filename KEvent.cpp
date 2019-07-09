
#include "KEvent.h"
#include "Define.h"
extern volatile BOOL lockFlag;

extern KernelEv *evArr[256];

KernelEv::KernelEv(IVTNo ivtNo){
	lock();
	sem =  new KernelSem(0);
	owner = PCB::running;
	this->ivtNo = ivtNo;
	evArr[ivtNo]=(KernelEv*)this;
	unlock();
}
	
KernelEv::~KernelEv(){
	delete sem;
	owner = NULL;
	evArr[ivtNo] = NULL;
}


void KernelEv::wait(){
	if (PCB::running == owner){
		sem->wait(0);
	}
}
    
void KernelEv::signal(){
	if (sem->val() == -1){
		sem->signal(0);
	}

}
