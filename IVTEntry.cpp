

#include "IVTEntry.h"
#include "Define.h"

extern volatile BOOL lockFlag;
class KernelEv;
KernelEv * evArr[256];


IVTEntry::IVTEntry(IVTNo ivtNo, interruptFun newInt):ivtNo(ivtNo){
	#ifndef BCC_BLOCK_IGNORE
	//asm cli;
	oldInt = getvect(ivtNo);
	setvect(ivtNo, newInt);
	//asm sti;
	#endif
}

IVTEntry::~IVTEntry(){
    #ifndef BCC_BLOCK_IGNORE
	//asm cli;
	setvect(ivtNo, oldInt);
	//asm sti;
	#endif
	callOld();
}

void IVTEntry::signal() {

	if(evArr[ivtNo] != NULL)
		evArr[ivtNo]->signal();


}
void IVTEntry::callOld(){
	(*oldInt)();
}
