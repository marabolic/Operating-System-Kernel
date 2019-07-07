/*
 * ThreadList.cpp
 *
 *  Created on: Jul 7, 2019
 *      Author: OS1
 */




#include "ThrdList.h"
#include "PCB.h"
#include "Define.h"

extern void lock();
extern void unlock();

ThreadList::ThreadList(){
	first = last = NULL;
}

ThreadList::~ThreadList(){
	 Node *t = first;
	 while(first != NULL){
		t = first;
		first = first->next;
		delete t;
	 }
	 first = last = NULL;
}

void ThreadList::add(PCB *Thread){
	Node *n = new Node();
	n->thread = Thread;
	n->next = NULL;
	if (first == NULL){
		first = n;
	}
	else last->next = n;
	last = n;
}

void ThreadList::remove(PCB * Thread){
	Node * temp = first, *prev = NULL;
	while (temp && temp->thread != Thread) {
		prev = temp;
		temp = temp->next;
	}
	if (temp == NULL) return;

	if (temp == last)
			last = prev;
	if (prev != NULL){
		prev->next = temp->next;
		Node * old = temp;
		temp = temp->next;
		delete old;
	}
	else{
		Node * old = temp;
		temp = temp->next;
		first = temp;
		delete old;
	}

}

PCB * ThreadList::getPCB(int id){
	lock();
	Node * temp = first;
	while (temp){
		if (temp->thread->getId() == id){
			unlock();
			return temp->thread;
		}
	temp = temp->next;
	}
	unlock();
	return NULL;
}









