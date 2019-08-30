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
	//assert(0);
	return result;
}


volatile BOOL lockFlag = 0;
volatile int flags;

/*

void lock() {
  if (lockFlag)
    return;

 lockFlag = 1;
 int _flags;

#ifndef BCC_BLOCK_IGNORE
  asm pushf;
  asm pop _flags;
  asm cli;

#endif
  flags = _flags;
}



void unlock() {
  if(!lockFlag)
    return;

  int _flags = flags;

#ifndef BCC_BLOCK_IGNORE
  asm push _flags;
  asm popf;

#endif
  lockFlag = 0;
}
*/


void init(){
	PCB::initMainThread();
	PCB::main->status = READY;
	PCB::running = PCB::main;
	PCB::initIdle();
	initTimer();
}


void restore(){
	delete PCB::idle;
	//delete PCB::running;
	restoreTimer();
}



