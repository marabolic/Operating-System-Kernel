
#ifndef _pcb_h_
#define _pcb_h_

#include "Queue.h"
#include "Thread.h"
#include "Semaphor.h"
#include "KSemaphr.h"
#include "Define.h"
#include <dos.h>

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
    BOOL completed, started;
    
    BOOL lockFlag;

    KernelSem *sem;
    
    static ID id;
    ID threadId;
    static PCB *volatile running;
    static PCB *volatile idle;

    Time timeLeft;
    Thread *my_thread;

    PCB(Thread *myThread, StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
    ~PCB();
    void start();
	void waitToComplete();

	void idleFun();
	void initIdle();

    void createProcess();
    //PCB * volatile  idle();

    ID getId();
	static ID getRunningId();
	static Thread * getThreadById(ID id);

    static void dispatch();
    void wrapper();
};

#endif
