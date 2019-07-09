/*
 * SigHLst.h
 *
 *  Created on: Jul 8, 2019
 *      Author: OS1
 */

#ifndef SIGHLST_H_
#define SIGHLST_H_

#include "Thread.h"

class SignalHList{
public:
	struct Node{
			SignalHandler handler;
			Node *next;
		};
		Node *first, *last;

		SignalHList();
		~SignalHList();

		void add(SignalHandler);
		void remove(SignalHandler);
		void removeAll();
		void swap( SignalHandler hand1, SignalHandler hand2);

		void processHandlers();
};



#endif /* SIGHLST_H_ */
