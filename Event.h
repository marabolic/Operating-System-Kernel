/*
 * Event.h
 *
 *  Created on: May 11, 2019
 *      Author: OS1
 */

#ifndef _event_h_
#define _event_h_

#include "PrepEnt.h"

class KernelEv;

class Event {
public:
	Event(IVTNo ivtNo);
	~Event();
	void wait();

protected:
	friend class KernelEv;
	void signal(); //can call KernelEv

private:
	KernelEv *myImpl;
};


#endif
