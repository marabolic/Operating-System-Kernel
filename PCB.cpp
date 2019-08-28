

#include "PCB.h"
#include <iostream.h>

extern int syncPrintf(const char *format, ...);
extern void tick();
extern volatile int check;
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




int PCB::flagMaskGlobal[16];

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

    if (running != main)
    	parent = running;
    else parent = NULL;

    sem = new KernelSem(0);
    slist = new SignalList();
    status = CREATED;
#ifdef SIGNAL

    if (parent != NULL){
    	lock();
		for (int i = 0; i < 16; i ++)
			flagMaskLocal[i] = parent->flagMaskLocal[i];
	    for (int j = 0; j < 16; j++){
	    	handlerList[j] = new SignalHList();
	    	 SignalHList::Node * temp = parent->handlerList[j]->first;
	    		 while (temp) {
	    			 handlerList[j]->add(temp->handler);
	    			 temp = temp->next;
	    		 }
	    }
	    unlock();
    }
    else{
    	for (int i = 0; i < 16; i ++)
    		flagMaskLocal[i] = 0;

        for (int j = 0; j < 16; j++)
        	handlerList[j] = new SignalHList();
    }


    handlerList[0]->add(PCB::signalZero);
#endif
    tlist.add(this);
}


PCB::~PCB() {
	//lock();
	tlist.remove(this);
	delete sem;
	delete []stack;
	parent = NULL;
#ifdef SIGNAL
	for (int j = 0; j < 16; j++)
		delete handlerList[j];
	delete slist;
#endif
	//unlock();
}


void PCB::createProcess(){
	lock();
	stack = new unsigned[stack_size/sizeof(unsigned)];
	stack[stack_size-1] = 0x200;
	#ifndef BCC_BLOCK_IGNORE
	stack[stack_size-2] = FP_SEG(PCB::wrapper);
	stack[stack_size-3] = FP_OFF(PCB::wrapper);
	bp = sp = FP_OFF(stack + stack_size - 12);
	ss = FP_SEG(stack + stack_size - 12);
	#endif
	unlock();
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

		//for (int i = 0; i < 30000; i++) {
			//for (int j = 0; j < 30000; j++);
		//}

		//lock();
		//cout << "idle\n";
		//cout << flush;
		//unlock();
	}
}

//int context_switch_req = 0;

 void PCB::wrapper() {
     PCB::running->my_thread->run();
     lock();
     if (PCB::running->sem->val() < 0)
    	 PCB::running->sem->signal(-PCB::running->sem->val());
#ifdef SIGNAL
     PCB::running->slist->add(2);
     PCB::running->processSignals();
#endif
     // cout << flush;
     // fflush(stdout);
     PCB::running->status = DONE;
#ifdef SIGNAL
    if ( PCB::running->parent != NULL)
    PCB::running->slist->add(1);
#endif
     unlock();
     dispatch();
     assert(0);
 }

void PCB::initIdle(){
	lock();
	idle = new PCB(NULL,1024,1);
	idle->stack = new unsigned[idle->stack_size];
    idle->stack[idle->stack_size-1] = 0x200;
	#ifndef BCC_BLOCK_IGNORE
		idle->stack[idle->stack_size-2] = FP_SEG(PCB::idleFun);
		idle->stack[idle->stack_size-3] = FP_OFF(PCB::idleFun);
		idle->bp = idle->sp = FP_OFF(idle->stack + idle->stack_size - 12);
		idle->ss = FP_SEG(idle->stack + idle->stack_size - 12);
	#endif
	idle->status = IDLE;
	unlock();
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
		 //for(int l = 0; l < 1000; l++)
			 //for(int t = 0; t < 1000; t++)
		 if (PCB::running->time_slice == 0) {
			 assert(0);
			 return;
		 }


		 PCB::running->timeLeft--;
	 }

	 if(PCB::running->timeLeft == 0 || PCB::timerFlag == 0){
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
			if (check == 1){
				//;
				//assert(PCB::running != PCB::main);
				//assert(PCB::running != NULL);
			}

			if(PCB::running == NULL)
				PCB::running = PCB::idle;

			//assert(PCB::running->status == READY || PCB::running->status == IDLE);


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

#ifdef SIGNAL
	lock();
	PCB::running->processSignals();
	unlock();
#endif
 }


 void initTimer(){
 #ifndef BCC_BLOCK_IGNORE
 	oldTimer = getvect(0x08);
 	setvect(0x08, timer);
 #endif
 }

 void PCB::processSignals(){
	 SignalList::Node * temp = slist->first;
	 while (temp) {
		 if (flagMaskLocal[temp->id] == 0 && flagMaskGlobal[temp->id] == 0) {
			 handlerList[temp->id]->processHandlers();
			 slist->remove(temp->id);
		 }
		 temp = temp->next;
	 }
 }

 void PCB::signalZero(){
	 if (running->parent != NULL)
	 if (running->sem->val() < 0)
		 running->sem->signal(-running->sem->val());
	 running->slist->add(2);
	 running->processSignals();
	 running->status = DONE;
	 running->slist->add(1);
 }

 void PCB::signal(SignalId signal){
	 if (signal == 0){

	 }
	 if (signal == 1 || signal == 2 || signal > 15) return;
	 slist->add(signal);
 }

 void PCB::registerHandler(SignalId signal, SignalHandler handler){
	 if (signal == 0 || signal > 15) return;
	 handlerList[signal]->add(handler);
 }

 void PCB::unregisterAllHandlers(SignalId id){
	 handlerList[id]->removeAll();
 }

 void PCB::swap(SignalId id, SignalHandler hand1, SignalHandler hand2){
	 if (id == 0 || id > 15) return;
	 handlerList[id]->swap(hand1, hand2);
 }

 void PCB::blockSignal(SignalId signal){
	 flagMaskLocal[signal] = 1;
 }

 void PCB::blockSignalGlobally(SignalId signal){
	 flagMaskGlobal[signal] = 1;
 }

 void PCB::unblockSignal(SignalId signal){
	 flagMaskLocal[signal] = 0;
 }

 void PCB::unblockSignalGlobally(SignalId signal){
	 flagMaskGlobal[signal] = 0;
 }

 void PCB::dispatch(){

 	PCB::timerFlag = 0;
 	timer();

  }


