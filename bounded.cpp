//===========================================================================//
//   Project: Projekat iz Operativnih sistema 1
//   File:    bounded.cpp
//   Date:    Jun 2019
//===========================================================================//
#include "bounded.h"
#include <ASSERT.H>
#include <stdlib.h>
#include "PCB.h"

#include "intLock.h"

BoundedBuffer::BoundedBuffer (unsigned size) : Size(size),
	mutexa(1), mutext(1), spaceAvailable(size), itemAvailable(0),
	head(0), tail(0) {
		buffer = new char[size];
		if (!buffer) exit(1);
	}
	
BoundedBuffer::~BoundedBuffer(){
	intLock
	delete [] buffer;
	intUnlock
}

int BoundedBuffer::append (char d) {
	spaceAvailable.wait(0);
	mutexa.wait(0);
		buffer[tail] = d;
		tail = (tail+1)%Size;
	mutexa.signal();
	itemAvailable.signal();
	return 0;
}

char BoundedBuffer::take () {
	assert(PCB::running->status >= 0 && PCB::running->status < 6);
	//assert(PCB::running->status != READY);
	assert(PCB::running->status != CREATED);
	//assert(PCB::running->status != BLOCKED);
	assert(PCB::running->status != IDLE);
	assert(PCB::running->status != SLEEP);
	assert(PCB::running->status != DONE);
	itemAvailable.wait(0);
	mutext.wait(0);
	assert(itemAvailable.val() >= 0);
		char d = buffer[head];
		head = (head+1)%Size;
	mutext.signal();
	spaceAvailable.signal();
	return d;
}
