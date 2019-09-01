

#include "PCB.h"
#include <iostream.h>
#define SIGNAL
#define SIGNAL_COUNT 16

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

    if (PCB::main != NULL)
    	parent = running;
    else{
    	parent = NULL;
    	PCB::main = this;
    }

    sem = new KernelSem(0);
    slist = new SignalList();
    status = CREATED;

    if (parent != NULL){
    	lock();
		for (int i = 0; i < SIGNAL_COUNT; i ++)
			flagMaskLocal[i] = parent->flagMaskLocal[i];
	    for (int j = 0; j < SIGNAL_COUNT; j++){
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
    	for (int i = 0; i < SIGNAL_COUNT; i ++)
    		flagMaskLocal[i] = 0;

        for (int j = 0; j < SIGNAL_COUNT; j++)
        	handlerList[j] = new SignalHList();
        handlerList[0]->add(PCB::signalZero);
    }



    tlist.add(this);
}


PCB::~PCB() {
	lock();
	tlist.remove(this);
	delete sem;
	parent = NULL;
	for (int j = 0; j < SIGNAL_COUNT; j++)
		delete handlerList[j];
	delete slist;
	unlock();

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
     //PCB::running->processSignals();
     //TODO
#endif
     // cout << flush;
     // fflush(stdout);
     PCB::running->status = DONE;
#ifdef SIGNAL
    if ( PCB::running->parent != NULL)
    	PCB::running->parent->slist->add(1);
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
			 //assert(0);
			 return;
		 }

		 PCB::running->timeLeft--;
	 }

	 if(PCB::running->timeLeft == 0 || PCB::timerFlag == 0){
		 if (PCB::running->status == DONE){
			 lock();
			 PCB::processSignals();
			 unlock();
		 }

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

	if(!lockFlag) {
		lock();
		PCB::processSignals();
		unlock();

		while (PCB::running->status == DONE){
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
				lock();
				PCB::processSignals();
				unlock();
		}
	}
 }


 void initTimer(){
 #ifndef BCC_BLOCK_IGNORE
 	oldTimer = getvect(0x08);
 	setvect(0x08, timer);
 #endif
 }


 void PCB::processSignals(){
	 SignalList::Node * temp = running->slist->first;
	 while (temp) {
		 if (running->flagMaskLocal[temp->id] == 0 && flagMaskGlobal[temp->id] == 0) {
			 running->handlerList[temp->id]->processHandlers();
			 SignalList::Node* old = temp;
			 temp = temp->next;
			 running->slist->remove(old);
			 //if (PCB::running->status == DONE){
				// return;
			 //}
		 }
		 else {
			 temp = temp->next;
		 }
	 }
 }

 void PCB::signalZero(){
	 if (running == PCB::main) return;
	 if (running->sem->val() < 0)
		 running->sem->signal(-running->sem->val());
	 running->slist->add(2);
	 //running->processSignals();
	 //TODO
	 running->parent->slist->add(1);
	 running->status = DONE;

 }

 void PCB::signal(SignalId signal){
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

 void PCB::initMainThread(){
	 PCB::mainThread = new Thread(0,0);
 }

