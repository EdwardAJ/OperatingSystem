char buffer[512];
char path[512];

char runprog[512];
int lwfound;
int result;
void launchShell();

int main () {
	int i;
	int input_switch = 0;
	
	//Initialize path
	launchShell();
}

void copyString(char *src, char *dest){
	int i = 0;
	interrupt(0x21, 0x0, "=-=-=-=\0", 0x0 , 0x0);
	interrupt(0x21, 0x0, "\n", 0x0 , 0x0);
	interrupt(0x21, 0x0, "\r", 0x0 , 0x0);
	interrupt(0x21, 0x0, src, 0x0 , 0x0);
	interrupt(0x21, 0x0, "\n", 0x0 , 0x0);
	interrupt(0x21, 0x0, "\r", 0x0 , 0x0);
	while (src[i] != '\0'){
		dest[i] = src[i];
		i++;
	}

	dest[i] = '\0';
}

void launchShell() {
	int idx;
	int jawal;
	int isEnd = 0;
	char argc = 0;
	int i,j,k;
	int success = -1;
	char *argv[10];
	char buffertemp[10];
	char buffertemp1[10];
	char argvtemp[10][10];
	char curdir;

	//Get current directory
	interrupt(0x21, 0x21, &curdir, 0, 0);
	//interrupt(0x21, (AH << 8) | AL, BX, CX, DX);
	
	interrupt(0x21, 0x0 , "$" , 0x0 , 0x0);
	interrupt(0x21, 0x0 , " " , 0x0 , 0x0);
	interrupt(0x21, 0x1, buffer, 0x0, 0x0);


	for (i = 0 ; i < 512 ; i++) {
		runprog[i] = '\0';
		//argvtemp[i] = '\0';
	}

	for (i = 0 ; i < 10 ; i++) {
		for (k = 0 ; k < 10 ; k++) {
			argvtemp[i][k] = '\0';
		}
	}
	

	for (i = 0 ; i < 10 ; i++) {
		buffertemp[i] = 0;
	}
	
	
	//implementasi dari execute program, sudah bisa memakai parameter
	if (buffer[0] == '.' && buffer[1] == '/') {
		i = 2;
		j = 0;

		//Baca execute program
		while (buffer[i] != '\0' && buffer[i] != ' ') {
			runprog[j] = buffer[i];
			i++;
			j++;
		}

		runprog[j] = '\0';
		
		//Ada parameter
		if (buffer[i] == ' '){
			argc = 1;
			i++;
			idx = 0;
			j = 0;
						
			while (buffer[i] != '\0' && buffer[i] != ' ') {
				buffertemp[j] = buffer[i];
				i++;
				j++;
			}
			buffertemp[j] = '\0';
			jawal = j;

			argv[0] = buffertemp;
			
			for (k = 0 ; k < 10 ; k++) {
				buffertemp1[k] = '\0';
			}

			if(buffer[i] == ' ') {
				argc = 2;
				i++;
				j = 0;

				while (buffer[i] != '\0' && buffer[i] != ' ') {
					buffertemp1[j] = buffer[i];
					i++;
					j++;
					
				}

				buffertemp1[j] = '\0';
				
				k = 0;
				argv[1] = buffertemp1;
			}
				
		}	
			
		interrupt(0x21, 0x20, curdir, argc, argv);
		
		interrupt(0x21, curdir << 8 | 0x6, runprog , 0x2000, &success);
	}



	//implementasi dari cd
	else if (buffer[0] == 'c' && buffer[1] == 'd'){

		i = 3;
		j = 0;
		while (buffer[i] != '\0') {
			runprog[j] = buffer[i];
			i++;
			j++;
		}

		//set argc dan argv

		//execute changeDirectory program
	} 

	//panggil echo
	else if (buffer[0] == 'e' && buffer[1] == 'c' && buffer[2] == 'h' && buffer[3] == 'o') {
		argc = 1;
		i = 5;
		j = 0;
		while (buffer[i] != '\0') {
			argv[0][j] = buffer[i];
			i++;
			j++;
		}
		argv[0][j] = '\0';

		//setArgs
		interrupt(0x21, 0x20, curdir, argc, argv);

		//execute Program
		interrupt(0x21, 0xFF << 8 | 0x6, "echo" , 0x2000, &success);
	}

	//panggil mkdir
	else if (buffer[0] == 'm' && buffer[1] == 'k' && buffer[2] == 'd' && buffer[3] == 'i' && buffer[4] == 'r') {
		argc = 1;
		i = 6;
		j = 0;
	
		while (buffer[i] != '\0') {
			argv[0][j] = buffer[i];
			i++;
			j++;
		}
	
		argv[0][j] = '\0';
		//setArgs
		interrupt(0x21, 0x20, curdir, argc, argv);

		//execute Program
		interrupt(0x21, 0xFF << 8 | 0x6, "mkdir" , 0x2000, &success);

	}

	//panggil ls
	else if (buffer[0] == 'l' && buffer[1] == 's') {
		argc = 0;
		//setArgs
		interrupt(0x21, 0x20, curdir, argc, argv);
		//execute Program
		interrupt(0x21, 0xFF << 8 | 0x6, "ls" , 0x2000, &success);
	}

	//panggil cat
	else if (buffer[0] == 'c' && buffer[1] == 'a' && buffer[2] == 't') {
		//jumlah argumen  = 1 atau 2
		
		i = 4;
		argc = 1;
		j = 0;
	
		while (buffer[i] != '\0' && buffer[i] != ' ') {
			argv[0][j] = buffer[i];
			i++;
			j++;
		}
		argv[0][j] = '\0';


		//Ada argumen kedua
		if (buffer[i] == ' '){
			argc = 2;
			i++;
			j = 0;
			if (argv[0][0] == '-' && argv[0][1] == 'w') {
				while (buffer[i] != '\0') {
					argv[1][j] = buffer[i];
					i++;
					j++;
				}
			}
			argv[1][j] = '\0';
		}
		//interrupt(0x21, 0x00, argv[0], 0x00 ,0x00);
		//interrupt(0x21, 0x00, "\n",0x00, 0x00);
		//interrupt(0x21, 0x00, "\r",0x00, 0x00);
		
		//setArgs
		interrupt(0x21, 0x20, curdir, argc, argv);

		//interrupt(0x21, 0x00, "", 0x00 ,0x00);
		//execute Program
		interrupt(0x21, 0xFF << 8 | 0x6, "cat" , 0x2000, &success);
	}


	for (i = 0 ; i < 512 ; i++) {
		for (j = 0 ; j < 512 ; j++) {
			argv[i][j] = '\0';
		}
	} 

	//Clear argv
	if (success != 0) {
		interrupt(0x21, 0x0 , "Tidak ada" , 0x0 , 0x0);
		interrupt(0x21, 0x0 , "\n" , 0x0 , 0x0);
		interrupt(0x21, 0x0 , "\r" , 0x0 , 0x0);
		//TERMINATE PROGRAM.
		interrupt(0x21, 0x07, &success , 0x0, 0x0 );
	}

}
	
