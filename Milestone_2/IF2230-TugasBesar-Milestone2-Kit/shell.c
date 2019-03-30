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
	char curdir;
	char argc;
	char *argv[512];
	int i,j;
	int success;

	curdir = 0xFF;
	//interrupt(0x21, (AH << 8) | AL, BX, CX, DX);
	interrupt(0x21, 0x0 , "$" , 0x0 , 0x0);
	interrupt(0x21, 0x0 , " " , 0x0 , 0x0);
	interrupt(0x21, 0x1, buffer, 0x0, 0x0);

	for (i = 0 ; i < 512 ; i++) {
		runprog[i] == '\0';
	}

	//implementasi dari execute program (belum memakai argumen)
	if (buffer[0] == '.' && buffer[1] == '/') {
		i = 2;
		j = 0;
		while (buffer[i] != '\0') {
			runprog[j] = buffer[i];
			i++;
			j++;
		}
		//interrupt(0x21, 0x0, runprog, 0x0, 0x0);
		interrupt(0x21, 0xFF << 8 | 0x6, runprog , 0x2000, &success);
		if (success != 0) {
			interrupt(0x21, 0x0 , "\n" , 0x0 , 0x0);
			interrupt(0x21, 0x0 , "Tidak ada" , 0x0 , 0x0);
			interrupt(0x21, 0x0 , "\n" , 0x0 , 0x0);
			interrupt(0x21, 0x0 , "\r" , 0x0 , 0x0);
		}
	}

	//implementasi dari cd, (..) untuk kembali
	if (buffer[0] == 'c' && buffer[1] == 'd'){
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
	if (buffer[0] == 'e' && buffer[1] == 'c' && buffer[2] == 'h' && buffer[3] == 'o') {
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

		//interrupt(0x21, 0x0, "ABC", 0x0, 0x0);
		//execute Program
		interrupt(0x21, 0xFF << 8 | 0x6, "echo" , 0x2000, &success);
	}

	//panggil mkdir
	if (buffer[0] == 'm' && buffer[1] == 'k' && buffer[2] == 'd' && buffer[3] == 'i' && buffer[4] == 'r') {
		argc = 1;
		i = 6;
		j = 0;
	}

	while (buffer[i] != '\0') {
		argv[0][j] = buffer[i];
		i++;
		j++;
	}
	
	argv[0][j] = '\0';

	//setArgs
	interrupt(0x21, 0x20, curdir, argc, argv);

	//interrupt(0x21, 0x0, "ABC", 0x0, 0x0);
	//execute Program
	interrupt(0x21, 0xFF << 8 | 0x6, "mkdir" , 0x2000, &success);


}
	
