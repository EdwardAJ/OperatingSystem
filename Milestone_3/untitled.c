void remove(char *path, int *result, char parentIndex){ //ParentIndex adalah indeks dari currDir
   int i, j, k, lastNameLen, isFile, aborting, lenQueue, indeksFile;
   char dirs[SECTOR_SIZE];
   char files[SECTOR_SIZE];
   char map[SECTOR_SIZE];
   char sectors[SECTOR_SIZE];
   char buffer[MAX_FILENAME];
   int queueDeletedFolder[DIR_ENTRY_LENGTH];
  

   //baca sektor dirs dan files dan map
   readSector(dirs, DIRS_SECTOR);
   readSector(files, FILES_SECTOR);
   readSector(map, MAP_SECTOR);
   readSector(sectors, SECTORS_SECTOR);

   lastNameLen = 0;
   isFile = 0;
   aborting = 0;
   

   while (1){
      //set buffer dengan '\0' semua
      for (i = 0; i < MAX_FILENAME; ++i){
         buffer[i] = '\0';
      }

      //isi buffer dengan nama direktori atau file pertama di path
      for (i = lastNameLen, j = 0; i < MAX_FILENAME && path[i] != '\0' && path[i] != '/'; ++i, ++j){
         buffer[j] = path[i];
      }
      
      lastNameLen = i + 1;

      //hanya akan meremove jika buffer tidak kosong
      if (buffer[0] != '\0'){
         //Cek apakah path valid (direktori atau file ada di system)
         for (i = 1; i < SECTOR_SIZE; i + MAX_DIRECTORY){
            j = 0;

            while(dirs[j + i] == buffer[j]){
               ++j;
            }

            //jika namanya ketemu yang sama, cek apakah indeks parentnya == parentIndex
            if (j == MAX_FILENAME && dirs[i - 1] == parentIndex){
               //jika nama direktorinya betul maka parentIndex menjadi indeks direktory diatas
               parentIndex = div(i-1, MAX_DIRECTORY);
               break;
            }
         }

         //kalau direktorinya nggak ketemu, cari di files
         if (i >= SECTOR_SIZE){
            for (i = 1; i < SECTOR_SIZE; i + MAX_DIRECTORY){
               j = 0;
               while (files[j + i] == buffer[j]){
                  ++j;
               }

               //jika ketemu di files, parentIndex menjadi index file tsb
               if (j == MAX_FILENAME && files[i -1] == parentIndex){
                  parentIndex = div(i-1, MAX_DIRECTORY);
                  isFile == 1;
                  break;
               }
            }
         }

         //jika yang mau di hapus adalah sebuah file
         if (isFile){
            break;
         //jika direktori nggak ketemu samsek
         }else{
            *result = NOT_FOUND;
            aborting = 1;
            break;
         }

      }else{
         *result = NOT_FOUND;
         aborting = 1;
         break;
      }

      //jika pathnya belum habis tapi buffer merupakan nama file, hentikan program
      if (path[lastNameLen] != '\0' && isFile){
         aborting = 1;
         break;
      }

      //jika pathnya sudah habis
      if (path[lastNameLen] == '\0'){
         break;
      }
   }

   //jika path tidak valid (nama file diikuti oleh '/')
   if (!aborting){
      //hapus file aja
      if (isFile){
         //ganti baris dari file yang ingin dihapus jadi '\0'
         for (i = 0; i < MAX_DIRECTORY; ++i){
            files[parentIndex*MAX_DIRECTORY + i] = '\0';
         }
         //ganti map dari sektor-sektor dari file tsb menjadi EMPTY
         for (i = 0; i < MAX_DIRECTORY; ++i){
            map[sectors[parentIndex*MAX_DIRECTORY + i]] = EMPTY;
            sectors[parentIndex*MAX_DIRECTORY + i] = EMPTY; 
         }

         writeSector(files, FILES_SECTOR);
         writeSector(map, MAP_SECTOR);
         writeSector(sectors, SECTORS_SECTOR);
      //hapus folder dan file2 di dalamnya
      }else{
         lenQueue = 0;
         k = 0;

         while (k < lenQueue || k == 0){
            //hapus file-file dulu
            for (i = 0; i < DIR_ENTRY_LENGTH; i + MAX_DIRECTORY){
               //jika indeks parent filenya == indeks folder yang ingin dihapus
               if (files[i] == parentIndex){
                  indeksFile = div(i, MAX_DIRECTORY);

                  //ganti baris dari file yang ingin dihapus jadi '\0'
                  for (j = 0; j < MAX_DIRECTORY; ++j){
                     files[indeksFile*MAX_DIRECTORY + j] = '\0';
                  }
                  //ganti map dari sektor-sektor dari file tsb menjadi EMPTY
                  for (j = 0; j < MAX_DIRECTORY; ++j){
                     map[sectors[indeksFile*MAX_DIRECTORY + j]] = EMPTY;
                     sectors[indeksFile*MAX_DIRECTORY + j] = EMPTY;
                  }
               }
            }

            //masukkan seluruh indeks sub-folder yang ada di folder yang mau dihapus
            for (i = 0; i < DIR_ENTRY_LENGTH; i + MAX_DIRECTORY){
               if (dirs[i] == parentIndex){
                  queueDeletedFolder[lenQueue] = div(i, MAX_DIRECTORY);
                  lenQueue++;
               }
            }

            //ganti parentIndex menjadi nilai terdepan queue (masuk ke sub-folder)
            parentIndex = queueDeletedFolder[k];
            k++;
         }

         //hapus nama direktori2 dengan indeks yang ada di queueDeletedFolder
         for (i = 0; i < lenQueue; ++i){
            for (j = 0; j < MAX_DIRECTORY; ++j){
               dirs[queueDeletedFolder[i]*MAX_DIRECTORY + j] = '\0'; 
            }
         }

         writeSector(files, FILES_SECTOR);
         writeSector(map, MAP_SECTOR);
         writeSector(dirs, DIRS_SECTOR);
         writeSector(sectors, SECTORS_SECTOR);
      }
   }

}