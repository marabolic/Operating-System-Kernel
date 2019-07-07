

#ifndef SEMLIST_H_
#define SEMLIST_H_
#include "Define.h"

class KernelSem;

class SemList{
public:
	struct Node{
		KernelSem *sem;
		Node *next;
	};
	Node *first, *last;

	SemList();
	~SemList();

	void add(KernelSem *);
	void remove(KernelSem *);
	void decrementTime();
};


#endif /* SEMLIST_H_ */
