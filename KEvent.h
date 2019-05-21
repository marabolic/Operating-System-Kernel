
#ifndef _kevent_h_
#define _kevent_h_

typedef unsigned char IVTNo;
class KernelEv{
public:

KernelEv(IVTNo ivtNo);
~KernelEv();
void wait();


friend class KernelEv;
void signal(); //can call KernelEv
};

#endif //