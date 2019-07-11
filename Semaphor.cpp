
#include "Semaphor.h"
#include "KSemaphr.h"

extern volatile BOOL lockFlag;

Semaphore::Semaphore(int init){
	lock();
	myImpl = new KernelSem(init);
	unlock();
}
Semaphore::~Semaphore(){
    //delete myImpl;
}

int Semaphore::wait(Time maxTimeToWait){
    return myImpl->wait(maxTimeToWait);
}
int Semaphore::signal(int n){
    return myImpl->signal(n);
}
int Semaphore::val () const{
	return myImpl->val();
}
