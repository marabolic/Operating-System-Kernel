#ifndef _scheduler_h_
#define _scheduler_h_

class PCB;

class Scheduler {
public:
    static void put (PCB*);
    static PCB* get ();
};

#endif