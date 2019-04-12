#define MAIN
#include "proc.h"
int main () {

	int i, j, found, idx_dirs, idx_files, hsl, result;
   	char buffer[2];
   	char curdir;
	enableInterrupts();
	interrupt(0x21, 0x21, &curdir, 0, 0);
	interrupt(0x21,0x00, "Indeks proses: ",0x00 ,0x00);
	interrupt(0x21,0x00, "\n\r",0x00 ,0x00);
	buffer[0] = running->index;
	buffer[1] = '\0';
	buffer[2] = '\0';
	interrupt(0x21,0x00, buffer, 0x00 ,0x00);
	interrupt(0x21, 0x07, &result , 0x0, 0x0 );
   	return 0;

}