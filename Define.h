

#ifndef _define_h_
#define _define_h_

#define lock() {asm{cli}}
#define unlock() {asm{sti}}
#define regSP = _SP
#define regSS = _SS
#define	regBP = _BP


typedef unsigned BOOL;


enum STATUS{READY, DONE, BLOCKED, SLEEP};
enum{FALSE = 0, TRUE = 1};

#endif