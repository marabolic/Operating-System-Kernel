

#include <iostream.h>
#include "PCB.h"
#include <dos.h>
#include "Define.h"

void init();
void restore();

extern int userMain(int argc, char* argv[] );

int main(int argc, char* argv[]){

	init();

	int result = userMain(argc, argv);

	restore();

	return result;


}


void init(){

	#ifndef BCC_BLOCK_IGNORE
	asm cli;
	#endif

	PCB::running = new PCB(NULL, 0,0);
	PCB::idle->initIdle();


	#ifndef BCC_BLOCK_IGNORE
	asm sti;
	#endif
}


void restore(){
	#ifndef BCC_BLOCK_IGNORE
	asm cli;
	#endif


	delete PCB::idle;
	delete PCB::running;

	#ifndef BCC_BLOCK_IGNORE
	asm sti;
	#endif

}

