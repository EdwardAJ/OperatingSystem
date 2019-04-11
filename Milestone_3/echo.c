int main() {
   int i;
   int success = 0;
   int result;
   char buffer[100];
   char curdir;
   char argc;
   char argv[16][32];
   enableInterrupts();

   //Get current directory, jumlah argumen, dan isi argumen
   interrupt(0x21, 0x21, &curdir, 0, 0);
   interrupt(0x21, 0x22, &argc, 0, 0);
   
   for (i = 0 ; i < argc ; i++) {
      interrupt(0x21, 0x23, i, argv[i], 0);
   }
   
   //Print isi argumen
   
   for (i = 0 ; i < argc ; i++) {
      interrupt(0x21, 0x00, argv[i], 0, 0);
   }
   interrupt(0x21, 0x0, "\n", 0x0 , 0x0);
   interrupt(0x21, 0x0, "\r", 0x0 , 0x0);
   
   //Terminate program
   interrupt(0x21, 0x07, &result , 0x0, 0x0);
   //FORMAT INTERRUPT : interrupt(0x21, (AH << 8) | AL, BX, CX, DX);
   //interrupt(0x21, 0x33, 0x2000 , &result, 0x0);
   //resumeProcess (BX,CX);
   return 0;
} 
