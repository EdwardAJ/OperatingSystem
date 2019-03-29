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
	int i,j;
	int success;
	//interrupt(0x21, (AH << 8) | AL, BX, CX, DX);
	interrupt(0x21, 0x0 , "$" , 0x0 , 0x0);
	interrupt(0x21, 0x0 , " " , 0x0 , 0x0);
	interrupt(0x21, 0x1, buffer, 0x0, 0x0);

	for (i = 0 ; i < 512 ; i++) {
		runprog[i] == '\0';
	}

	//implementasi dari execute program
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

	//implementasi dari cd
	if (buffer[0] == 'c' && buffer[1] == 'd') {
		i = 3;
	}
		
}
	//readString(buffer);
