
#ifndef _pcb_h_
#define _pcb_h_

#include "Queue.h"
#include "Thread.h"

#define lock asm cli
#define unlock asm sti

class PCB {
public:
    unsigned sp;
    unsigned ss;
    unsigned bp;
    unsigned finished;
    unsigned *stack;
    StackSize stack_size;
    Time time_slice;
    
    static Queue queue;
    

    ID threadId;
    static PCB *running;
    Thread *my_thread;

    PCB(Thread *myThread, StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
    ~PCB();
    void start();
	void waitToComplete();

    ID getId();
	static ID getRunningId();
	static Thread * getThreadById(ID id);

    static void dispatch();
    void wrapper();
};

#endif