
#ifndef _kesemaphor_h_
#define _kesemaphor_h_

#include "PCB.h"
#include "SleepQ.h"

#include "SCHEDULE.h"
#include "Define.h"

typedef unsigned int Time;


class SleepQueue;
class PCB;

class KernelSem {

public:
    volatile int value;
    SleepQueue *blocked;
    KernelSem(int init);
    virtual ~KernelSem();

    void block(Time time);
    void deblock();
    
    int val() const;
    int wait(Time maxTimeToWait);
    int signal(int n = 0);
};


#endif
