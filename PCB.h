
#ifndef _pcb_h_
#define _pcb_h_

#include "Queue.h"
#include "Thread.h"
#include "Semaphor.h"
#include "KSemaphr.h"
#include "ThrdList.h"
#include "Define.h"
#include <dos.h>
#include "SigHLst.h"
#include "SCHEDULE.h"
#include <stdio.h>
#include <assert.h>
#include "SemList.h"
#include "SigList.h"
#include "ThrdList.h"

class KernelSem;

class PCB {
public:
    unsigned sp;
    unsigned ss;
    unsigned bp;


    unsigned *stack;
    StackSize stack_size;
    Time time_slice;

    STATUS status;


    BOOL signalFlag;
    
    static volatile int timerFlag;

    static SemList klist;
    static ThreadList tlist;

    SignalList *slist;
    SignalHList *handlerList[16];

    int flagMaskLocal[16];
    static int flagMaskGlobal[16];


    KernelSem *sem;
    
    static ID id;
    ID threadId;
    static PCB *volatile running;
    static PCB *volatile idle;
    static PCB *main;
    static Thread * mainThread;

    Time timeLeft;
    Thread *my_thread;

    PCB * parent;

    PCB(Thread *myThread, StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
    ~PCB();
    void start();
	void waitToComplete();

	static void idleFun();
	static void initIdle();

    void createProcess();
    //PCB * volatile  idle();

    ID getId();
	static ID getRunningId();
	static Thread * getThreadById(ID id);

	void processSignals();
	static void signalZero();

	void signal(SignalId signal);
	void registerHandler(SignalId signal, SignalHandler handler);
	void unregisterAllHandlers(SignalId id);
	void swap(SignalId id, SignalHandler hand1, SignalHandler hand2);
	void blockSignal(SignalId signal);
	static void blockSignalGlobally(SignalId signal);
	void unblockSignal(SignalId signal);
	static void unblockSignalGlobally(SignalId signal);

    static void dispatch();
    static void wrapper();


};


void initTimer();
void restoreTimer();

#endif
