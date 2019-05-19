
#ifndef _pcb_h_
#define _pcb_h_


class PCB {
public:
    unsigned sp;
    unsigned ss;
    unsigned bp;
    unsigned finished;
    unsigned *stack;

    
    void wrapper();
};

#endif