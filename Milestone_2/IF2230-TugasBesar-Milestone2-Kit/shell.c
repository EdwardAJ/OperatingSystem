char buffer[512];
int result;
void launchShell();

int main () {
	launchShell();
	while (1);
}

void launchShell() {
	//interrupt(0x21, (AH << 8) | AL, BX, CX, DX);
	interrupt(0x21, 0xFF << 8 | 0x00, "$", 0x00, 0x00);
	interrupt(0x21, 0xFF << 8 | 0x00, "Nama saya Edward Alexander", 0x00, 0x00);
	//printString("$");
	//readString(buffer);
}