#define DIRS_SECTOR 0x101
#define FILES_SECTOR 0x102
int main() {
   int i, j, found, idx_dirs, idx_files, hsl, result;
   char files[512];
   char dirs[512];
   char dirs_store[32][16];
   char files_store[32][16];
   char elemen;
   char buffer[100];
   char curdir;
   char argc;
   char argv[4][16];

   //interrupt(0x21, (AH << 8) | AL, BX, CX, DX);
   //Get current directory, jumlah argumen, dan isi argumen
   interrupt(0x21, 0x21, &curdir, 0, 0);
   interrupt(0x21, 0x22, &argc, 0, 0);
   interrupt(0x21, 0x23, 0, argv[0], 0);


   for (i = 0 ; i < 32 ; i++) {
      for (j = 0 ; j < 16 ; j++) {
         dirs_store[i][j] = '\0';
         files_store[i][j] = '\0';
      }
   }
   
   //Start reading current directory

   interrupt(0x21, 0x00 , "Berikut adalah direktori pada direktori ini:", 0x00 , 0x00);
   interrupt(0x21, 0x00 , "\n", 0x00 , 0x00);
   interrupt(0x21, 0x00 , "\r", 0x00 , 0x00);
   idx_dirs = 0;
   interrupt(0x21, 0x02, dirs, DIRS_SECTOR, 0x00);
   for(i = 0 ; i < 32; i++) {
      for (j = 0 ; j < 16 ; j++){
         elemen = dirs[(i*16)+j];
         if (j == 0) {
            //cari setiap directory yang memiliki parent directory curDir
            if (elemen != curdir) {
               //interrupt(0x21, 0x00, "test", 0x00, 0x00);
               break;
            }
         } else {
            dirs_store[idx_dirs][j-1]= dirs[(i*16)+j];
         }
      }

      if (j==16) {
         interrupt(0x21, 0x00 , dirs_store[idx_dirs], 0x00 , 0x00);
         interrupt(0x21, 0x00 , "\n", 0x00 , 0x00);
         interrupt(0x21, 0x00 , "\r", 0x00 , 0x00);
         idx_dirs++;
      }
   } 
   

   //Start reading files

   interrupt(0x21, 0x00 , "Berikut adalah files pada direktori ini:", 0x00 , 0x00);
   interrupt(0x21, 0x00 , "\n", 0x00 , 0x00);
   interrupt(0x21, 0x00 , "\r", 0x00 , 0x00);
   idx_files = 0;
   interrupt(0x21, 0x02, files, FILES_SECTOR, 0x00);
   for(i = 0 ; i < 32; i++) {
      for (j = 0 ; j < 16 ; j++){
         elemen = files[(i*16)+j];
         if (j == 0) {
            //cari setiap directory yang memiliki parent directory curDir
            if (elemen != curdir) {
               break;
            }
         } else {
            files_store[idx_files][j-1]= files[(i*16)+j];
         }
         //interrupt(0x21, 0x00 , files_store[idx_files], 0x00 , 0x00);
      }
      //interrupt(0x21, 0x00 , files_store[idx_files], 0x00 , 0x00);
      if (j == 16) {
         interrupt(0x21, 0x00 , files_store[idx_files], 0x00 , 0x00);
         interrupt(0x21, 0x00 , "\n", 0x00 , 0x00);
         interrupt(0x21, 0x00 , "\r", 0x00 , 0x00);
         idx_files++;
      }
   } 

   //Terminate program
   interrupt(0x21, 0x07, &result , 0x0, 0x0 );
   return 0;
} 
