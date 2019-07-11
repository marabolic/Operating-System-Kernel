

#ifndef _kevent_h_
#define _kevent_h_

#include  "KSemaphr.h"

typedef unsigned char IVTNo;
class KernelEv{
public:
	PCB * owner;
	#ifdef SEM_EVENT
	KernelSem * sem;
	#endif


	IVTNo ivtNo;

	KernelEv(IVTNo ivtNo);
	~KernelEv();
	void wait();

	void signal();
};

#endif //
