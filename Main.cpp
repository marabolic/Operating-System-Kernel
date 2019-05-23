

#include <iostream.h>
#include <dos.h>
#include "Define.h"

extern int userMain(int argc, char* argv[]);

int main(int argc, char* argv[]){

	//init();

	int result = userMain(argc, argv);

	//restore();

	return result;
}


