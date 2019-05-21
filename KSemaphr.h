
#ifndef _kesemaphor_h_
#define _kesemaphor_h_

#include "PCB.h"

typedef unsigned int Time;

class KernelSem{

public:
    int val;
    KernelSem(int init = 1);
    virtual ~KernelSem();

    virtual int wait(Time maxTimeToWait);
    virtual int signal(int n = 0);
    PCB *myPCB;
};


#endif