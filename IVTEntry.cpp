

#include "IVTEntry.h"


class KernelEv;
KernelEv * evArr[256];


IVTEntry::IVTEntry(IVTNo ivtNo, interruptFun newInt):ivtNo(ivtNo){
	#ifndef BCC_BLOCK_IGNORE
	oldInt = getvect(ivtNo);
	setvect(ivtNo, newInt);
	#endif
}

IVTEntry::~IVTEntry(){
    #ifndef BCC_BLOCK_IGNORE
	setvect(ivtNo, oldInt);
	#endif

	//callold
}

void IVTEntry::signal(){
	if(evArr[ivtNo] != NULL)
		evArr[ivtNo]->signal();
}
void IVTEntry::callOld(){
	(*oldInt)();
}
