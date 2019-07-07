
#include "Queue.h"


 Queue::Queue(){
    len = 0;
    first = last = NULL;
 }
 

 Queue::~Queue(){
	 len = 0;
	 Node *t = first;
	 while(t != NULL){
	 	t = first;
	 	first = first->next;
	 	delete t;
	 }
	 first = last = NULL;
 }

void Queue::insert(PCB *pcb){
    Node *n = new Node();
    n->pcb = pcb;
    n->next = NULL;
    if (first == NULL){
        first = n;
    }
    else last->next = n;
    last = n;
    len++;
}

PCB * Queue::remove(){
    if (len == 0){
        return NULL;
    }
    PCB * pcb = first->pcb;
    Node *prev = first;
    first = first->next;
    delete prev;
    len--;
    return pcb;
}

int Queue::size() const{
    return len;
}


