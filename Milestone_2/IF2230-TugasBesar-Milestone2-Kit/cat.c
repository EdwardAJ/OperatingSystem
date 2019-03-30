int main () {
   int i;
   int result;
   char buffer[100];
   char curdir;
   char argc;
   char argv[10][16];

   //Get current directory, jumlah argumen, dan isi argumen
   interrupt(0x21, 0x21, &curdir, 0x00, 0x00);
   interrupt(0x21, 0x22, &argc, 0x00, 0x00);
   interrupt(0x21, 0x23, 0, argv[0], 0x00);
   

   
   for (i = 0  ; i < 100 ; i++){
   		buffer[i] = '\0';
   }
   
   	
   //cek apakah ada argumen kedua ("-w")
   
   		//Jika tidak ada "-w", maka read dan print isi dari file saja.

   		//Mulai read file
   		
   		//interrupt(0x21, (AH << 8) | AL, BX, CX, DX);
   		//readFile(BX, CX, DX, AH);
   		//readFile(char *buffer, char *path, int *result, char parentIndex);
   		//interrupt(0x21, 0x00, argv[0], 0x00 ,0x00);
   		interrupt(0x21, (0xFF << 8) | 0x04, buffer, argv[0], &result);
   		interrupt(0x21, 0x00, buffer, 0x00 ,0x00);
   

   	//Terminate program
   	interrupt(0x21, 0x07, &result , 0x0, 0x0 );
}