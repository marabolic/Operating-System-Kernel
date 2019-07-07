/*
 * IVTEntry.h
 *
 *  Created on: Jul 7, 2019
 *      Author: OS1
 */

#ifndef IVTENTRY_H_
#define IVTENTRY_H_
#include "KEvent.h"
#include "Event.h"
#include <dos.h>

class IVTEntry {
public:
	IVTNo ivtNo;
	interruptFun oldInt;

	IVTEntry(IVTNo ivtNo, interruptFun newInt);
	~IVTEntry();
	void signal();
	void callOld();
};



#endif /* IVTENTRY_H_ */
