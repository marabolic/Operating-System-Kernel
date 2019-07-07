
#ifndef SLEEPQ_H_
#define SLEEPQ_H_

#include "PCB.h"
#include "Define.h"

class SleepQueue {

public:
	struct Node{
		PCB * pcb;
		Time sleepTime;
		Node * next;
	};

	Node *first, *last;
	volatile int len;

	SleepQueue();
	~SleepQueue();

	void insert(PCB *, Time);
	PCB * remove();
	void decTime();

	int size();

};

#endif /* SLEEPQ_H_ */
