
#include "KSemaphr.h"
#include "SemList.h"

extern int syncPrintf(const char *format, ...);

extern volatile BOOL lockFlag;

KernelSem::KernelSem(int init) {
    value = init;
    blocked = new SleepQueue();
    PCB::klist.add(this);
}

KernelSem::~KernelSem() {
	lock();
	PCB::klist.remove(this); //const mozda
	assert(blocked->size()==0);
	delete blocked;
	unlock();

}

int KernelSem::val() const {
    return value;
}

void KernelSem::block(Time time) {
	PCB::running->status = BLOCKED;
    blocked->insert(PCB::running, time);
    unlock();
    dispatch();
    lock();

}

void KernelSem::deblock(){
   PCB *pcb = blocked->remove();
   if (pcb!=NULL) {
       pcb->status = READY;
       pcb->signalFlag = 1;
       Scheduler::put(pcb);
    }
}

int KernelSem::wait(Time maxTimeToWait){
	lock();
	PCB::running->signalFlag = 1;
	if (--value < 0){
		 block(maxTimeToWait);
	}
	unlock();
	return PCB::running->signalFlag;
}


int KernelSem::signal(int n){

	lock();
	if (n == 0){
		if (value++ < 0){
			deblock();
		}
		unlock();
		return 0;
	}
	if (n > 0){
		int i = 0;
		if (value < 0){
			while(i < n && blocked->size() > 0){
				deblock();
				i++;
			}
		}
		value += n;
		unlock();
		return i;
	}
	unlock();
    return n;
}




