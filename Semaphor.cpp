#include "Semaphor.h"
#include "KSemaphr.h"

Semaphore::Semaphore(int init = 1){
  myImpl = new KernelSem(init);
}
Semaphore::~Semaphore(){
    delete myImpl;
}

int Semaphore::wait(Time maxTimeToWait){
    return myImpl->wait(maxTimeToWait);
}
int Semaphore::signal(int n = 0){
    return myImpl->signal(n);
}