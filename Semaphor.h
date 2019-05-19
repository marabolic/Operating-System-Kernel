/*
 * Semaphor.h
 *
 *  Created on: May 11, 2019
 *      Author: OS1
 */

#ifndef _semaphor_h_
#define _semaphor_h_

typedef unsigned int Time;

class KernelSem;

class Semaphore{
public:
	Semaphore(int init = 1);
	virtual ~Semaphore();

	virtual int wait(Time maxTimeToWait);
	virtual int signal(int n=0);
private:
	KernelSem* myImpl;
};

#endif /* _semaphor_h_ */
