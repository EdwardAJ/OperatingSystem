#define MAIN
#include "proc.h"
int main () {
	struct PCB *process = readyHead;
	struct PCB *processtmp;
	int i, j, found, idx_dirs, idx_files, hsl, result, curdir;
   	char processPID[7];
   	char processState[7];
	enableInterrupts();

	for (i = 0 ; i < 7 ; i++) {
		processPID[i] = '\0';
		processState[i] = '\0';
	}

	//Get Curdir
	interrupt(0x21, 0x21, &curdir, 0, 0);
	interrupt(0x21,0x00, "Proses: \n\r",0x00 ,0x00);
	i = 0;
	while (process != NULL){
		processPID[i] = process->index + '0';
		processState[i] = process ->state + '0';
		process = process->next;
		i++;
	}
	processPID[i] = '\0';
	processState[i] = '\0';

	//PRINT SEMUA PROCESS
	interrupt(0x21,0x00, "Indeks Proses: ",0x00 ,0x00);
	interrupt(0x21,0x00, processPID, 0x00 ,0x00);
	interrupt(0x21,0x00, "\n\r\0", 0x00 ,0x00);
	interrupt(0x21,0x00, "State Proses: ",0x00 ,0x00);
	interrupt(0x21,0x00, processState, 0x00, 0x00);
	interrupt(0x21,0x00, "\n\r\0", 0x00 ,0x00);

	//TERMINATE THIS PROGRAM.
	interrupt(0x21, 0x07, &result , 0x0, 0x0 );
   	return 0;

}
