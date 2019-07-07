
#include "KEvent.h"
extern KernelEv *evArr[256];

KernelEv::KernelEv(IVTNo ivtNo){
	sem =  new KernelSem(0);
	owner = PCB::running;
	this->ivtNo = ivtNo;
	evArr[ivtNo]=(KernelEv*)this;
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
	if (sem->val() == -1)
		sem->signal(0);
}
