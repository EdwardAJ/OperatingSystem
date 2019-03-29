int main() {
   int i;
   int result;
   char buffer[100];
   char curdir;
   char argc;
   char argv[10][16];

   //Get current directory, jumlah argumen, dan isi argumen
   interrupt(0x21, 0x21, &curdir, 0, 0);
   interrupt(0x21, 0x22, &argc, 0, 0);
   interrupt(0x21, 0x23, argv[0], 0, 0);

   //Start makedirectory sequence.
   //interrupt(0x21, 0x00, argv[0], 0x00 ,0x00);
   interrupt(0x21, (0xFF << 8) | 0x008, argv[0], &result, 0x00);
   
   //Terminate program
   interrupt(0x21, 0x07, &result , 0x0, 0x0 );
   return 0;
} 
