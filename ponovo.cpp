
/*
 * ponovo.cpp
 *
 *  Created on: May 23, 2019
 *      Author: OS1
 */

/*

//pretpostavljeni memorijski model: huge

#include <iostream.h>
#include <dos.h>


// Zabranjuje prekide
#define lock asm cli

// Dozvoljava prekide
#define unlock asm sti

struct PCB1{
	unsigned sp;
	unsigned ss;
	unsigned zavrsio;
	int kvant;
};

PCB1 *p[3];
volatile PCB1* running1;
volatile unsigned int lockFlag = 1;

volatile int nextThread = 2;
PCB1* getNextPCBToExecute(){
	if (nextThread == 1)
		nextThread = 2;
	else nextThread = 1;
	if (p[nextThread]->zavrsio){
		if (nextThread == 1)
			nextThread = 2;
		else nextThread = 1;
		if (p[nextThread]->zavrsio)
			nextThread = 0;
	}
	return p[nextThread];
}


unsigned tsp;
unsigned tss;

volatile int brojac = 20;
volatile int zahtevana_promena_konteksta = 0;

void interrupt timer1(){
	if (!zahtevana_promena_konteksta) brojac--;
	if (brojac == 0 || zahtevana_promena_konteksta) {


		if (lockFlag){
			zahtevana_promena_konteksta = 0;
			#ifndef BCC_BLOCK_IGNORE
			asm {
				mov tsp, sp
				mov tss, ss
			}
			#endif

			running1->sp = tsp;
			running1->ss = tss;

			lockFlag=0;
			cout<<"Promena konteksta! Brojac= "<<brojac<<endl;

			#ifndef BCC_BLOCK_IGNORE
			asm cli;
			#endif

			lockFlag=1;

			running1 = getNextPCBToExecute();	// Scheduler

			tsp = running1->sp;
			tss = running1->ss;

			brojac = running1->kvant;
			#ifndef BCC_BLOCK_IGNORE
			asm {
				mov sp, tsp   // restore spg
				mov ss, tss
			}
			#endif
		}
		else zahtevana_promena_konteksta = 1;

	}

	#ifndef BCC_BLOCK_IGNORE
	if(!zahtevana_promena_konteksta)
			asm int 60h;

	#endif

	//zahtevana_promena_konteksta = 0;
}



void dispatch1(){
	#ifndef BCC_BLOCK_IGNORE
	asm cli;
	#endif
	zahtevana_promena_konteksta = 1;
	timer1();
	#ifndef BCC_BLOCK_IGNORE
	asm sti;
	#endif
}

unsigned oldTimerOFF, oldTimerSEG;



void inic(){
#ifndef BCC_BLOCK_IGNORE
	asm{
		cli
		push es
		push ax

		mov ax,0
		mov es,ax

		mov ax, word ptr es:0022h
		mov word ptr oldTimerSEG, ax
		mov ax, word ptr es:0020h
		mov word ptr oldTimerOFF, ax

		mov word ptr es:0022h, seg timer1
		mov word ptr es:0020h, offset timer1
		mov ax, oldTimerSEG
		mov word ptr es:0182h, ax
		mov ax, oldTimerOFF
		mov word ptr es:0180h, ax

		pop ax
		pop es
		sti
	}
#endif
}


void restore(){
#ifndef BCC_BLOCK_IGNORE
	asm {
		cli
		push es
		push ax

		mov ax,0
		mov es,ax


		mov ax, word ptr oldTimerSEG
		mov word ptr es:0022h, ax
		mov ax, word ptr oldTimerOFF
		mov word ptr es:0020h, ax

		pop ax
		pop es
		sti
	}
#endif
}

void exitThread(){
	running1->zavrsio = 1;
	dispatch1();
}

void a(){
	for (int i =0; i < 30; ++i) {
		lockFlag=0;
		cout<<"u a() i = "<<i<<endl;
		lockFlag=1;
		if (zahtevana_promena_konteksta) {
			dispatch1();
		}
		for (int k = 0; k<10000; ++k)
			for (int j = 0; j <30000; ++j);
	}
	exitThread();
}

void b(){
	for (int i =0; i < 30; ++i) {
		lockFlag=0;
		cout<<"u b() i = "<<i<<endl;
		lockFlag=1;
		lockFlag=1;
		if (zahtevana_promena_konteksta) {
			dispatch1();
		}
		for (int k = 0; k<10000; ++k)
			for (int j = 0; j <30000; ++j);
	}
	exitThread();
}


/*void createProcess(PCB1 *newPCB, void (*body)()){
	unsigned* st1 = new unsigned[1024];

	st1[1023] = 0x200;

#ifndef BCC_BLOCK_IGNORE
	st1[1022] = FP_SEG(body);
	st1[1021] = FP_OFF(body);

newPCB->sp = FP_OFF(st1+1012);

	newPCB->ss = FP_SEG(st1+1012);
#endif
	newPCB->zavrsio = 0;
}



void doSomething(){
	lockFlag=0;
	p[1] = new PCB1();
	//createProcess(p[1],a);
	cout<<"napravio a"<<endl;
	p[1]->kvant = 40;

	p[2] = new PCB1();
	//createProcess(p[2],b);
	cout<<"napravio b"<<endl;
	p[2]->kvant = 20;

	p[0] = new PCB1();

	running1 = p[0];
	lockFlag=1;


	for (int i=0; i < 30; ++i) {
		lockFlag=0;
  		cout<<"main "<<i<<endl;
		lockFlag=1;

		for (int j=0; j< 30000; ++j) {
			for (int k = 0; k < 30000; ++k);
		}
	}
	cout<<"Happy End"<<endl;
}

*/

/*

int main(int argc, char* argv){

  inic();

  doSomething();

  restore();

  return 0;
}
*/

