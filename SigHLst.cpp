/*
 * SigHLst.cpp
 *
 *  Created on: Jul 8, 2019
 *      Author: OS1
 */

#include "SigHLst.h"
#include "Define.h"


SignalHList::SignalHList(){
	first = last = NULL;
}
SignalHList::~SignalHList(){
	 Node *t = first;
	 while(first != NULL){
		t = first;
		first = first->next;
		delete t;
	 }
	 first = last = NULL;
}

void SignalHList::add(SignalHandler handler){
	Node *n = new Node();
	n->handler = handler;
	n->next = NULL;
	if (first == NULL){
		first = n;
	}
	else last->next = n;
	last = n;
}

void SignalHList::remove(SignalHandler handler){
	Node * temp = first, *prev = NULL;
	while (temp && temp->handler != handler) {
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

void SignalHList::removeAll(){
	 Node *t = first;
	 while(first != NULL){
		t = first;
		first = first->next;
		delete t;
	 }
	 first = last = NULL;
}

void SignalHList::swap(SignalHandler hand1, SignalHandler hand2){
	Node *temp = first, *prev = NULL, *h1 = NULL, *h2 = NULL, *prev1 = NULL, *prev2 = NULL;
	while (temp != NULL){
		if (temp->handler == hand1){
			h1 = temp;
			prev1 = prev;
		}
		if (temp->handler == hand2){
			h2 = temp;
			prev2 = prev;
		}
		prev = temp;
		temp = temp->next;
	}
	if (h1 == h2) return;
	if (h1 == NULL || h2 == NULL) return;


	if (prev1 != NULL)
		prev1->next = h2;
	else first = h2;
	if (prev2 != NULL)
		prev2->next = h1;
	else first = h1;

	Node *t = h1->next;
	h1->next = h2->next;
	h2->next = t;
	if (h1 == last)
		last = h2;
	else {
		if (h2 == last)
			last = h2;
	}

}

void SignalHList::processHandlers() {
	Node * temp = first;
	while (temp) {
		#ifndef BCC_BLOCK_IGNORE
		asm pushf;
		asm sti;
		#endif
		temp->handler();
		#ifndef BCC_BLOCK_IGNORE
		asm popf;
		#endif
		temp = temp->next;
	}
}

