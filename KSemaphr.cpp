#include "KSemaphr.h"

KernelSem::KernelSem(int init = 1){
    val = 0;
}
KernelSem::~KernelSem(){
    
}

int KernelSem::wait(Time maxTimeToWait){
    //lock
    val--;
    if (val < 0){
        
        //block
    }

    //unlock
}
int KernelSem::signal(int n = 0){
     //lock
    val++;
    if (val > 0){

    }
    
    //unlock
}