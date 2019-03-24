char buffer[512];
int result;
void launchShell();

int main () {
	launchShell();
	//while (1);
}

void launchShell() {
	//interrupt(0x21, (AH << 8) | AL, BX, CX, DX);
	interrupt(0x21, 0x0 , "Halo nama saya Edward" , 0x0 , 0x0);
	//printString("$");
	//readString(buffer);
}