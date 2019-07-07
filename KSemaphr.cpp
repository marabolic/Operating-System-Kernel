
#include "KSemaphr.h"
#include "SemList.h"

extern int syncPrintf(const char *format, ...);
extern void lock();
extern void unlock();

KernelSem::KernelSem(int init){
	PCB::klist.add(this);
    value = init;
    blocked = new SleepQueue();
}

KernelSem::~KernelSem(){

	PCB::klist.remove(this); //const mozda
	assert(blocked->size()==0);
	delete blocked;
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




