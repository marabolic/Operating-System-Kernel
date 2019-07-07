/*
 * PrepEnt.h
 *
 *  Created on: Jul 7, 2019
 *      Author: OS1
 */

#ifndef PREPENT_H_
#define PREPENT_H_

#include "IVTEntry.h"
typedef unsigned char IVTNo;

#define PREPAREENTRY(ivtNo, old)\
	void interrupt interruptEvent##ivtNo(...);\
	\
	static IVTEntry entry##ivtNo(ivtNo, interruptEvent##ivtNo);\
	\
	void interrupt interruptEvent##ivtNo(...){\
		entry##ivtNo.signal();\
		if(old)\
			entry##ivtNo.callOld();\
		dispatch();\
	}

#endif /* PREPENT_H_ */


