

#include "PCB.h"
#include <iostream.h>

extern int syncPrintf(const char *format, ...);
extern void tick();
extern void lock();
extern void unlock();
extern volatile BOOL lockFlag;

void interrupt timer(...);

ID PCB::id =0;
PCB * volatile PCB::running = NULL;
PCB * volatile PCB::idle = NULL;
PCB * PCB::main = NULL;
Thread * PCB::mainThread = NULL;

void interrupt (*oldTimer)(...);
SemList PCB::klist;
ThreadList PCB::tlist;


unsigned tsp;
unsigned tss;
unsigned tbp;

volatile int PCB::timerFlag = 1;

PCB::PCB(Thread *myThread, StackSize stackSize, Time timeSlice) {
    threadId = ++id;
    my_thread = myThread;
    stack_size = stackSize;
    time_slice = timeSlice;
    stack = NULL;
    sem = new KernelSem(0);
    status = CREATED;
    tlist.add(this);
}


PCB::~PCB() {
	tlist.remove(this);
	delete sem;
	delete []stack;
}


void PCB::createProcess(){

   stack = new unsigned[stack_size/sizeof(unsigned)];
   stack[stack_size-1] = 0x200;
	#ifndef BCC_BLOCK_IGNORE
   stack[stack_size-2] = FP_SEG(PCB::wrapper);
   stack[stack_size-3] = FP_OFF(PCB::wrapper);
   bp = sp = FP_OFF(stack + stack_size - 12);
   ss = FP_SEG(stack + stack_size - 12);
	#endif
}

void PCB::start() {
    if (status == CREATED){
        createProcess();
        status = READY;
        Scheduler::put(this);
    }
}
void PCB::waitToComplete() {
	if(status == DONE){
		 return;
	}
	sem->wait(0);
	//assert(0);
	//assert(status == DONE);
}

ID PCB::getId() {
    return threadId;
}

ID PCB::getRunningId() {
    return PCB::running->threadId;
}

Thread * PCB::getThreadById(ID id){
	return tlist.getPCB(id)->my_thread;
}

void PCB::idleFun(){

	while(1){
		//syncPrintf("idle ");
	}
}


 void PCB::wrapper() {
     PCB::running->my_thread->run();
     if (PCB::running->sem->val() < 0)
    	 PCB::running->sem->signal(-PCB::running->sem->val());
     PCB::running->status = DONE;
     dispatch();
     assert(0);
 }

void PCB::initIdle(){


	idle = new PCB(NULL,128,1);
	idle->stack = new unsigned[idle->stack_size];
    idle->stack[idle->stack_size-1] = 0x200;
	#ifndef BCC_BLOCK_IGNORE
		idle->stack[idle->stack_size-2] = FP_SEG(PCB::idleFun);
		idle->stack[idle->stack_size-3] = FP_OFF(PCB::idleFun);
		idle->bp = idle->sp = FP_OFF(idle->stack + idle->stack_size - 12);
		idle->ss = FP_SEG(idle->stack + idle->stack_size - 12);
	#endif
	idle->status = IDLE;
}





void restoreTimer(){
#ifndef BCC_BLOCK_IGNORE
	setvect(0x08, oldTimer);
#endif
}

//TIMER
 void interrupt timer(...) {

	 if (PCB::timerFlag == 1){

		 tick();
		 (*oldTimer)();

		 PCB::klist.decrementTime();

		 if (PCB::running->time_slice == 0)
			 return;
		 PCB::running->timeLeft--;

	 }

	 if(PCB::running->timeLeft <= 0 || PCB::timerFlag == 0){
		 if (!lockFlag) {

			#ifndef BCC_BLOCK_IGNORE
			 tsp = _SP;
			 tss = _SS;
			 tbp = _BP;
			#endif
			PCB::running->sp = tsp;
			PCB::running->ss = tss;
			PCB::running->bp = tbp;


			if(PCB::running->status == READY){
				//syncPrintf("put \n");
				Scheduler::put(PCB::running);
			}
			//syncPrintf("get \n");
			PCB::running = Scheduler::get();

			if(PCB::running == NULL)
				PCB::running = PCB::idle;


			tsp = PCB::running->sp;
			tss = PCB::running->ss;
			tbp = PCB::running->bp;


			#ifndef BCC_BLOCK_IGNORE
			_SP = tsp;
			_SS = tss;
			_BP = tbp;
			#endif
		 }

		PCB::running->timeLeft = PCB::running->time_slice;
	}

	PCB::timerFlag = 1;
 }

 void initTimer(){
 #ifndef BCC_BLOCK_IGNORE
 	oldTimer = getvect(0x08);
 	setvect(0x08, timer);
 #endif
 }


 void PCB::dispatch(){

 	PCB::timerFlag = 0;
 	timer();

  }


