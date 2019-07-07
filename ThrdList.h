/*
 * ThreadList.h
 *
 *  Created on: Jul 7, 2019
 *      Author: OS1
 */

#ifndef THRDLIST_H_
#define THRDLIST_H_
class PCB;


class ThreadList{
public:
	struct Node{
		PCB *thread;
		Node *next;
	};
	Node *first, *last;

	ThreadList();
	~ThreadList();

	void add(PCB *);
	void remove(PCB *);
	PCB * getPCB(int id);
};

#endif /* THRDLIST_H_ */
