
#ifndef _queue_h_
#define _queue_h_



#include "PCB.h"

#define NULL 0

class PCB;

class Queue {

public:
    struct Node{
        PCB *pcb;
        Node *next;
    };

    Node *first, *last;
    int len;
public:

    Queue();
    ~Queue();
    
    void insert(PCB *pcb);
    PCB * remove();

    int size() const;
};


#endif
