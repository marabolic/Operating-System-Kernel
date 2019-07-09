/*
 * SleepQ.cpp
 *
 *  Created on: Jul 2, 2019
 *      Author: OS1
 */

#include "SleepQ.h"

extern int syncPrintf (const char* format, ... );

SleepQueue::SleepQueue(){
	first = last = NULL;
	len = 0;
}


SleepQueue::~SleepQueue(){
	 Node *t = first;
	 assert(first == NULL);
	 while(first != NULL){
		t = first;
		first = first->next;
		delete t;
	 }
	 last = first = NULL;
	 len = 0;
}


void SleepQueue::insert(PCB * pcb, Time time){

	Node *n = new Node();
	n->pcb = pcb;
	n->next = NULL;
	n->sleepTime = time;

	if (len == 0){
		first = n;
	}
	else last->next = n;
	last = n;
	len++;
	//syncPrintf("len insert: %d \n", len);
	//if(pcb == PCB::main)
	//	assert(0);
}


PCB * SleepQueue::remove(){

	if (len <= 0){
		return NULL;
	}
	PCB * pcb = first->pcb;
	Node *prev = first;
	first = first->next;
	delete prev;
	if (first == NULL)
		last = NULL;
	len--;
	//syncPrintf("len remove: %d \n", len);

	return pcb;
}

void SleepQueue::decTime(){
	Node * temp = first, *prev = NULL;
	while (temp) {
		if (temp->sleepTime != 0){
			temp->sleepTime--;
			if (temp->sleepTime == 0){
				if (temp == last)
						last = prev;
				if (prev != NULL){
					prev->next = temp->next;
					Node * old = temp;
					temp = temp->next;
					old->pcb->signalFlag=0;
					Scheduler::put(old->pcb);
					delete old;
					len--;
					continue;
				}
				else{
					Node * old = temp;
					temp = temp->next;
					first = temp;
					old->pcb->signalFlag=0;
					Scheduler::put(old->pcb);
					delete old;
					len--;
					continue;
				}
			}
		}
		prev = temp;
		temp = temp->next;
	}
}

int SleepQueue::size(){
	return len;
}
