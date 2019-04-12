int main() {
   int i;
   int result;
   char buffer[100];
   char curdir;
   char argc;
   char argv[32][32];

   //Get current directory, jumlah argumen, dan isi argumen
   interrupt(0x21, 0x21, &curdir, 0x00, 0x00);
   interrupt(0x21, 0x22, &argc, 0x00, 0x00);
   interrupt(0x21, 0x23, 0, argv[0], 0x00);

   //Start makedirectory sequence.
   //interrupt(0x21, 0x00, argv[0], 0x00 ,0x00);

   interrupt(0x21, (curdir << 8) | 0x08, argv[0], &result, 0x00);
   if (result != 0) {
      interrupt(0x21, 0x00, "ERROR" , 0x00 ,0x00);
      interrupt(0x21, 0x00, "\n" , 0x00 ,0x00);
      interrupt(0x21, 0x00, "\r" , 0x00 ,0x00);
   }
   //Terminate program
   interrupt(0x21, 0x07, &result , 0x0, 0x0 );
   return 0;
} 
