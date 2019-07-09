#include "SemList.h"
#include "KSemaphr.h"


SemList::SemList(){
	first = last = NULL;
}

SemList::~SemList(){
	 Node *t = first;
	 while(first != NULL){
		t = first;
		first = first->next;
		delete t;
	 }
	 first = last = NULL;
}

void SemList::add(KernelSem *sem){
	Node *n = new Node();
	n->sem = sem;
	n->next = NULL;
	if (first == NULL){
		first = n;
	}
	else last->next = n;
	last = n;
}

void SemList::remove(KernelSem * sem){
	Node * temp = first, *prev = NULL;
	while (temp && temp->sem != sem) {
		prev = temp;
		temp = temp->next;
	}
	if (temp == NULL){
		return;
	}

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

void SemList::decrementTime(){
	Node * temp = first;
	while (temp){

		temp->sem->blocked->decTime();

		temp = temp->next;
	}
}









