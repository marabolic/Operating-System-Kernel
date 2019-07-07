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


volatile BOOL lockFlag = 0;

void lock(){
	assert(lockFlag==0);
	lockFlag = 1;
}

void unlock(){
	assert(lockFlag==1);
	lockFlag = 0;
}


void init(){
	//PCB::mainThread = new Thread(0,0);
	PCB::main = new PCB(NULL,0,0);
	PCB::main->status = READY;
	PCB::running = PCB::main;
	PCB::initIdle();
	initTimer();
}


void restore(){

	delete PCB::idle;
	delete PCB::running;
	restoreTimer();

}



