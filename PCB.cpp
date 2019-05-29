

#include "PCB.h"
#include "SCHEDULE.h"

#include "Define.h"
#include "Thread.h"


extern void tick();

ID PCB::id =0;
PCB * volatile PCB::running = NULL;
PCB * volatile PCB::idle = NULL;
unsigned tsp;
unsigned tss;
unsigned tbp;

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


void PCB::createProcess(){
   stack = new unsigned[stack_size];
   stack[stack_size-1] = 0x200;
	#ifndef BCC_BLOCK_IGNORE
   stack[stack_size-2] = FP_SEG(&wrapper);
   stack[stack_size-3] = FP_OFF(&wrapper);
   bp = sp = FP_OFF(stack + stack_size - 12);
   ss = FP_SEG(stack + stack_size - 12);
	#endif
   status = READY;
   completed = 0;
}

void PCB::start() { 
    if (!started){
        createProcess();
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

void PCB::idleFun(){
	while(1);
}


 void PCB::wrapper() {
     PCB::running->my_thread->run();
     //...
 }

void PCB::initIdle(){
	idle = new PCB(NULL, 0,0);
	stack = new unsigned[stack_size];
    stack[stack_size-1] = 0x200;
	#ifndef BCC_BLOCK_IGNORE
		stack[stack_size-2] = FP_SEG(&idleFun);
		stack[stack_size-3] = FP_OFF(&idleFun);
		bp = sp = FP_OFF(stack + stack_size - 12);
		ss = FP_SEG(stack + stack_size - 12);
	#endif
	status = IDLE;
}






//TIMER
 void interrupt timer() {


	 tick();
	 PCB::running->timeLeft--;

	 if(PCB::running->timeLeft <= 0){
		#ifndef BCC_BLOCK_IGNORE
		 tsp = _SP;
		 tss = _SS;
		 tbp = _BP;
		#endif
	 	PCB::running->sp = tsp;
	 	PCB::running->ss = tss;
		PCB::running->bp = tbp;

		if(PCB::running != PCB::idle){
		Scheduler::put(PCB::running);
		}
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
		PCB::running->timeLeft = PCB::running->time_slice;
	}
 }


 void PCB::dispatch(){

 	#ifndef BCC_BLOCK_IGNORE
 	asm cli;
 	#endif
 	//zahtevana_promena_konteksta = 1;
 	timer();
 	#ifndef BCC_BLOCK_IGNORE
 	asm sti;
 	#endif
  }


