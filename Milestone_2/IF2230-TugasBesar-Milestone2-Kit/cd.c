#define DIRS_SECTOR 0x101
int main(){
    char curdirIdx;
    char dirs;
    int i;
    char curdir;
    char argc;
    char argv[15];


    //Get current directory, jumlah argumen, dan isi argumen
    interrupt(0x21, 0x21, &curdir, 0, 0);
    interrupt(0x21, 0x22, &argc, 0, 0);
    interrupt(0x21, 0x23, i, argv[0], 0);

    // Nggak tau yang mau ditulis disini ._.
     return 0;
}


void changeDirectory(char *path, int *result, char parentIndex){  //parentIndex adalah indeks current directory
   char dirs[SECTOR_SIZE];
   char buffer[MAX_FILENAME]; //untuk menyimpan nama direktori yang ingin dituju
   int i, j, counter, lastNameLen;
   int found = 0;

   //readSector(dirs, DIRS_SECTOR);
   interrupt(0x21, 0x02, dirs, FILES_SECTOR, 0x00);
   lastNameLen = 0;

   while (1){
      //isi buffer dengan \0
      for (j = 0; j < MAX_FILENAME; ++j){
         buffer[j] = '\0';
      }

      for (i = lastNameLen, j = 0; i < lastNameLen + MAX_FILENAME && path[i] != '\0' && path[i] != '/'; ++i, ++j){
         //isi buffer dengan nama direktori pertama dari path
         buffer[j] = path[i];
      }

      if (path[i] == '/'){
         lastNameLen = i + 1;
      }

      //jika pathnya kosong, cd akan kembali ke root direktory
      if (buffer[0] == '\0'){
         parentIndex = 0xFF;
      }
      //Jika pathnya merupakan ".." (perintah back)
      else if (buffer[0] == '.' && buffer[0] == '.'){
         //Ambil indeks parent direktori sekarang dan ubah parentIndex menjadi indeks parent direktori sekarang
         parentIndex = dirs[(parentIndex*MAX_DIRECTORY) + 1];
      }else{
         //Cek apakah nama direktori di buffer ada di dirs dengan indeks parentnya == parentIndex
         for (j = 1; j < SECTOR_SIZE; j + MAX_FILENAME){
            counter = 0;

            while (dirs[j + counter] == buffer[counter] && buffer[counter] != '\0'){
               ++counter;
            }

            if (buffer[counter] == '\0'){
               //jika ketemu, parentIndex diubah jadi indeks nama direktori di buffer
               if (dirs[j - 1] == parentIndex){
                  parentIndex = div(j-1, MAX_DIRECTORY);
                  break;
               }
               //Jika tidak ketemu lanjutkan pencarian
            }
         }

         //Nama direktori nggak ketemu
         if (j >= SECTOR_SIZE){
            *result = NOT_FOUND;
            break;
         }
      }

      //jika sudah sampai akhir path, berhenti
      if (path[lastNameLen] == '\0'){
         found = 1;
         *result = parentIndex;
         break;
      }

   }

   if (found){
      //Ganti current direktory dengan nama direktori parentIndex



   }   
   

}
   