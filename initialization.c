#include "assembler.h"


extern int countLabel;
extern unsigned int instructionArr[1025]; 
extern unsigned int dataArr[1025]; 

/*The function resets the global variables, releases the allocations and closes the files*/
void initializeAndClose(FILE *file,label *labelTable){
	int i;
	fclose(file);
	countLabel=0;
	for(i=0;i<1025;i++){
		instructionArr[i]=0;
		dataArr[i]=0;
	}
}
