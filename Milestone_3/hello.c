int main () {
	char buffer[512];
	interrupt(0x21, 0x0 , "Hello World!" , 0x0 , 0x0);
	//interrupt(0x21, 0x0 , "\n" , 0x0 , 0x0);
	//interrupt(0x21, 0x0 , "\r" , 0x0 , 0x0);
	//interrupt(0x21, 0x1, buffer, 0x0, 0x0);
	//interrupt(0x21, 0x0 , buffer , 0x0 , 0x0);


}