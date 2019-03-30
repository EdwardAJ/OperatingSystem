char buffer[512];
char path[512];
char curdir;
char runprog[512];
int result;
void launchShell();

int main () {
	int i;
	int input_switch = 0;
	curdir = 0xFF;

	//Initialize path
	for (i = 0 ; i < 512 ; i++) {
		path[i] = '\0';
	}
	launchShell();
}

void launchShell() {
	int idx;
	char argc;
	int i,j;
	int success = -1;
	char *argv[512];
	
	//interrupt(0x21, (AH << 8) | AL, BX, CX, DX);
	
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

<<<<<<< HEAD
	//implementasi dari cd, (..) untuk kembali
	if (buffer[0] == 'c' && buffer[1] == 'd'){
=======
	//implementasi dari cd
	else if (buffer[0] == 'c' && buffer[1] == 'd'){
>>>>>>> 592314a48a28114b6b2facb88e2a2d6145cccd88
		i = 3;
		j = 0;
		while (buffer[i] != '\0') {
			runprog[j] = buffer[i];
			i++;
			j++;
		}
<<<<<<< HEAD

		//set argc dan argv

		//execute changeDirectory program

=======
>>>>>>> 592314a48a28114b6b2facb88e2a2d6145cccd88
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
			if (buffer[i] == '-' && buffer[i+1] == 'w'){
				argv[1][0] = '-';
				argv[1][1] = 'w';
				argv[1][2] = '\0';
			}
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


	

	//Clear argv

	for (i = 0 ; i < 512 ; i++) {
		for (j = 0 ; j < 512 ; j++) {
			argv[i][j] = '\0';
		}
	}

	if (success != 0) {
		interrupt(0x21, 0x0 , "Tidak ada" , 0x0 , 0x0);
		interrupt(0x21, 0x0 , "\n" , 0x0 , 0x0);
		interrupt(0x21, 0x0 , "\r" , 0x0 , 0x0);
	}

	interrupt(0x21, 0x07, &success , 0x0, 0x0 );

}
	
