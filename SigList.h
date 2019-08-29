/*
 * SigList.h
 *
 *  Created on: Jul 8, 2019
 *      Author: OS1
 */

#ifndef SIGLIST_H_
#define SIGLIST_H_
#include "Thread.h"
#include "SigHLst.h"

class SignalList{
public:
	struct Node{
			SignalId id;
			Node *next;
		};
		Node *first, *last;

		SignalList();
		~SignalList();

		void add(SignalId );
		void remove(Node *);
		void removeAll();


};


#endif /* SIGLIST_H_ */
