

#ifndef _define_h_
#define _define_h_

#include <assert.h>
#define regSP = _SP
#define regSS = _SS
#define	regBP = _BP
#define NULL 0

typedef unsigned BOOL;

typedef void interrupt(*interruptFun)(...);


enum STATUS{READY, DONE, BLOCKED, SLEEP, CREATED, IDLE};
enum{FALSE = 0, TRUE = 1};

#endif
