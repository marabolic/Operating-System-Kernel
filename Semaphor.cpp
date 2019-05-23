
#include "Semaphor.h"
#include "KSemaphr.h"

Semaphore::Semaphore(int init){
  myImpl = new KernelSem(init);
}
Semaphore::~Semaphore(){
    delete myImpl;
}

int Semaphore::wait(Time maxTimeToWait){
    return myImpl->wait(maxTimeToWait);
}
int Semaphore::signal(int n){
    return myImpl->signal(n);
}
