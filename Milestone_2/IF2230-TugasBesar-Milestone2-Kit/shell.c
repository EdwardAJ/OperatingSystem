char buffer[512];
char runprog[512];
int result;
void launchShell();

int main () {
	int input_switch = 0;
	while (input_switch == 0) {
		launchShell();
	}
}

void launchShell() {
	int idx;
	char curdir;
	char argc;
	int i,j;
	int success;
	char *argv[512];
	

	curdir = 0xFF;
	//interrupt(0x21, (AH << 8) | AL, BX, CX, DX);
	/*
	for (i = 0 ; i < 512 ; i++) {
		buffer[i] = '\0';
	}
	*/
	interrupt(0x21, 0x0 , "$" , 0x0 , 0x0);
	interrupt(0x21, 0x0 , " " , 0x0 , 0x0);
	interrupt(0x21, 0x1, buffer, 0x0, 0x0);


	for (i = 0 ; i < 512 ; i++) {
		runprog[i] = '\0';
	}

	//implementasi dari execute program (belum memakai argumen)
	if (buffer[0] == '.' && buffer[1] == '/') {
		i = 2;
		j = 0;
		idx = -1;
		//Baca execute program
		while (buffer[i] != '\0') {
			while (buffer[i] != '\0' && buffer[i] != ' ') {
				if (idx == -1) {
					//interrupt(0x21, 0x0,"testing",0x0,0x0);
					runprog[j] = buffer[i];
				} else {
					argv[idx][j] = buffer[i];
				}
				i++;
				j++;
			}
			if (idx != -1) {
				argv[idx][j] = '\0';
 			}
			//buang semua space
			while(buffer[i] == ' ') {
				i++;
			}
			idx++;
			j = 0;
		}
		argc = idx+1;

		//setArgs
		interrupt(0x21, 0x20, curdir, argc, argv);
		interrupt(0x21, 0xFF << 8 | 0x6, runprog , 0x2000, &success);
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


	if (success != 0) {
		interrupt(0x21, 0x0 , "Tidak ada" , 0x0 , 0x0);
		interrupt(0x21, 0x0 , "\n" , 0x0 , 0x0);
		interrupt(0x21, 0x0 , "\r" , 0x0 , 0x0);
	}

}
	
