#include "Queue.h"


 Queue::Queue(){
     len = 0;

 }

 void Queue::sort() {
    for (Node *temp1 = first; temp1->next != 0; temp1 = temp1->next){
        for (Node *temp2 = first->next; temp2 != 0; temp2 = temp2->next){
            if (temp1->val < temp2->val){

            }
        }
    }
 }

void Queue::insert(PCB *pcb){
    Node *n = new Node();
    n->val = pcb;
    if (first == 0){
        first = n;
    }
    else last->next = n;
    last = n;
    len++;
}

PCB * Queue::remove(){
    if (len == 0){
        return 0;
    }
    PCB * pcb = first->val;
    Node *prev = first;
    first = first->next;
    delete prev;
    len--;
}

int Queue::size() const{
 


    return len;
}

