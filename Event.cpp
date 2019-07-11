
#include "Event.h"
#include "KEvent.h"

extern volatile BOOL lockFlag;

Event::Event(IVTNo ivtNo){
	//lock();
    myImpl = new KernelEv(ivtNo);
    //unlock();
}
	
Event::~Event(){
    delete myImpl;
}


void Event::wait(){
    myImpl->wait();
}

void Event::signal(){
    myImpl->signal();
}


