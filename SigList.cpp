/*
 * SigList.cpp
 *
 *  Created on: Jul 8, 2019
 *      Author: OS1
 */


#include "SigList.h"
#include "Define.h"


SignalList::SignalList(){
	first = last = NULL;
}
SignalList::~SignalList(){
	 Node *t = first;
	 while(first != NULL){
		t = first;
		first = first->next;
		delete t;
	 }
	 first = last = NULL;
}

void SignalList::add(SignalId id){
	Node *n = new Node();
	n->id = id;
	n->next = NULL;
	if (first == NULL){
		first = n;
	}
	else last->next = n;
	last = n;
}

void SignalList::remove(SignalList::Node * x){
	Node * temp = first, *prev = NULL;
	while (temp && temp != x) {
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

void SignalList::removeAll(){
	 Node *t = first;
	 while(first != NULL){
		t = first;
		first = first->next;
		delete t;
	 }
	 first = last = NULL;
}




