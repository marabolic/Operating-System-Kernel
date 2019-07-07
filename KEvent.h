

#ifndef _kevent_h_
#define _kevent_h_

#include  "KSemaphr.h"

typedef unsigned char IVTNo;
class KernelEv{
public:
	PCB * owner;
	KernelSem * sem;
	IVTNo ivtNo;

	KernelEv(IVTNo ivtNo);
	~KernelEv();
	void wait();

	void signal();
};

#endif //
