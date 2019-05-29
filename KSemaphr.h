
#ifndef _kesemaphor_h_
#define _kesemaphor_h_

#include "PCB.h"
#include "Queue.h"

typedef unsigned int Time;

class Queue;
class PCB;

class KernelSem {

public:
    int val;
    int lck;
    Queue *blocked;
    KernelSem(int init = 1);
    virtual ~KernelSem();

    void block();
    void deblock();
    
    int value() const;
    virtual int wait(Time maxTimeToWait);
    virtual int signal(int n = 0);
    PCB *myPCB;
};


#endif
