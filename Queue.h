
#ifndef _queue_h_
#define _queue_h_



#include "PCB.h"

#define NULL 0

class PCB;

class Queue {

private:
    struct Node{
        PCB *val;
        Node *next = 0;
    };

    Node *first, *last;
    int len;
public:

    Queue();
    ~Queue();

    void sort();

    void insert(PCB *pcb);
    PCB * remove();

    int size() const;
};


#endif