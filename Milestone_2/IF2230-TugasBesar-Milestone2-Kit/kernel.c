#define MAX_BYTE 256
#define SECTOR_SIZE 512
#define MAX_FILES 16
#define MAX_FILENAME 15
#define MAX_SECTORS 16
#define MAX_DIRECTORY 16
#define DIR_ENTRY_LENGTH 32
#define MAP_SECTOR 0x100
#define DIRS_SECTOR 0x101
#define FILES_SECTOR 0x102
#define SECTORS_SECTOR 0x103
#define TRUE 1
#define FALSE 0
#define INSUFFICIENT_SECTORS 0
#define INSUFFICIENT_ENTRIES -3
#define NOT_FOUND -1
#define INSUFFICIENT_DIR_ENTRIES -1
#define EMPTY 0x00
#define USED 0xFF
#define ARGS_SECTOR 512
#define ALREADY_EXISTS 2
#define SUCCESS 0

void handleInterrupt21 (int AX, int BX, int CX, int DX);
void printString(char *string);
void readString(char *string);
int mod(int a, int b);
int div(int a, int b);
void readSector(char *buffer, int sector);
void writeSector(char *buffer, int sector);
void readFile(char *buffer, char *path, int *result, char parentIndex);
void clear(char *buffer, int length);
void writeFile(char *buffer, char* path, int* sectors, char parentIndex);
void executeProgram(char *path, int segment, int *result, char parentIndex);
void clearScreen(int _lines);
void drawLogo(char *_logo, int _length);
void terminateProgram (int *result);
void makeDirectory(char *path, int *result, char parentIndex);
void deleteFile(char *path, int *result, char parentIndex);
void deleteDirectory(char *path, int *success, char parentIndex);
void putArgs (char curdir, char argc, char **argv);
void getCurdir (char *curdir);
void getArgc (char *argc);
void getArgv (char index, char *argv);
int findIndexDirectory(char *name, int root);
int findIndexFile(char *name, char root);

int main() {
   int string[256];
   char buffer[MAX_BYTE];
   int success;
   
   /*
   //ASCII Art untuk logo.
   static char logo[528] = {
   ' ' ,' ', '_', '_', '_', '_', ' ', '_', '_', '_', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '_', '_', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '_', '_', ' ', ' ', ' ', ' ', '\n', '|', ' ', ' ', ' ', ' ', '|', ' ', ' ', ' ', '\\', '_', '_', '_', '_', '_', '/', ' ', ' ', '|', '_', ' ', '_', '_', ' ', '_', '_', '|', ' ', ' ', '|', ' ', '_', '_', '\n', '|', ' ', ' ', ' ', ' ', '|', ' ', ' ', ' ', '/', ' ', ' ', ' ', ' ', '\\', ' ', ' ', ' ', '_', '_', '\\', ' ', ' ', '|', ' ', ' ', '\\', ' ', ' ', '|', '/', ' ', '/', '\n', '|', ' ', ' ', ' ', ' ', '|', ' ', ' ', '/', ' ', ' ', ' ', '|', ' ', ' ', '\\', ' ', ' ', '|', ' ', '|', ' ', ' ', '|', ' ', ' ', '/', ' ', ' ', ' ', ' ', '<', ' ', '\n', '|', '_', '_', '_', '_', '_', '_', '/', '|', '_', '_', '_', '|', ' ', ' ', '/', '_', '_', '|', ' ', '|', '_', '_', '_', '_', '/', '|', '_', '_', '|', '_', ' ', '\\', '\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\\', '/', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\\', '/', '\n', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '_', '_', ' ', ' ', ' ', ' ', ' ', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '.', '_', '_', '.', '_', '_', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '.', '_', '.', '\n', '\\', '_', '_', '_', '_', '_', '_', ' ', ' ', ' ', '\\', '_', '_', '_', '_', '_', ' ', ' ', '|', ' ', ' ', '|', ' ', '_', '_', ' ', '\\', '_', '_', '_', '_', '_', '_', ' ', ' ', ' ', '\\', '_', '_', '|', ' ', ' ', '|', ' ', '_', '_', '_', '_', '_', ' ', '|', ' ', '|', '\n', ' ', '|', ' ', ' ', ' ', ' ', ' ', '_', '_', '_', '/', '\\', '_', '_', ' ', ' ', '\\', ' ', '|', ' ', ' ', '|', '/', ' ', '/', ' ', ' ', '|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '_', '/', ' ', ' ', '|', ' ', ' ', '|', ' ', '\\', '_', '_', ' ', ' ', '\\', '|', ' ', '|', '\n', ' ', '|', ' ', ' ', ' ', ' ', '|', ' ', ' ', ' ', ' ', ' ', '/', ' ', '_', '_', ' ', '\\', '|', ' ', ' ', ' ', ' ', '<', ' ', ' ', ' ', '|', ' ', ' ', ' ', ' ', '|', ' ', ' ', ' ', '\\', ' ', ' ', '|', ' ', ' ', '|', '_', '_', '/', ' ', '_', '_', ' ', '\\', '\\', '|', '\n', ' ', '|', '_', '_', '_', '_', '|', ' ', ' ', ' ', ' ', '(', '_', '_', '_', '_', ' ', ' ', '/', '_', '_', '|', '_', ' ', '\\', ' ', ' ', '|', '_', '_', '_', '_', '|', '_', ' ', ' ', '/', '_', '_', '|', '_', '_', '_', '_', '(', '_', '_', '_', '_', ' ', ' ', '/', '_', '\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\\', '/', ' ', ' ', ' ', ' ', ' ', '\\', '/', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\\', '/', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\\', '/', '\\', '/'
   };
   */

   makeInterrupt21();

   
   //drawLogo(logo, 528);
   

   //printString("TEST 1");
   
   //handleInterrupt21(0x6, "keyproc", 0x2000, &success);
   //printString("sukses");
   //interrupt(0x21, 0x0 , "string" , 0x0 , 0x0);

   
   interrupt(0x21, 0xFF << 8 | 0x6, "shell", 0x2000, &success);
   //handleInterrupt21(0x4, buffer, "key.txt", &success);
   //andleInterrupt21(0x0, buffer);
   //handleInterrupt21(0x1,buffer);

   

   while (1);
}

void clearScreen(int _lines){
   //Prosedur membersihkan layar sebanyak _lines relatif terhadap 0x8000.
   int i;
   //Membersihkan layar.
   for (i = 0; i < 80 * _lines; i++){
      putInMemory(0xB000, 0x8000 + (i * 2), ' ');
   }
}

void drawLogo(char *_logo, int _length){
   //Variabel i untuk iterasi.
   //current_line menyimpan indeks baris posisi saat ini.
   //current_char_pos menyimpan posisi karakter saat ini, relatif terhadap 0x8000.
   //needed_chars menyimpan jumlah karakter yang diperlukan untuk menuju baris baru,
   //relatif terhadap 0x8000.
   int i, current_line, current_char_pos, needed_chars;
   current_line = 1;
   current_char_pos = 0;

   clearScreen(12);
   //Iterasi untuk logo.
   for (i = 0; i < _length; i++){
      //Jika ditemukan karakter end of line, lakukan perubahan line.
      if (_logo[i] == '\n'){
         needed_chars = (80 * current_line) - current_char_pos;
         current_char_pos = current_char_pos + needed_chars;
         current_line = current_line + 1;
      }else {
         //Print karakter logo ke layar.
         putInMemory(0xB000, 0x8000 + (current_char_pos * 2), _logo[i]);
         //Sumber warna: https://gist.github.com/Pwootage/1396569
         if (current_line < 7){
            //Warna merah untuk baris < 7.
            putInMemory(0xB000, 0x8001 + (current_char_pos * 2), 0xC);
         }else {
            //Warna biru untuk baris >= 7.
            putInMemory(0xB000, 0x8001 + (current_char_pos * 2), 0x9);
         }
      }

      //Inkrementasi untuk posisi karakter saat ini.
      current_char_pos++;
   }
}

void handleInterrupt21 (int AX, int BX, int CX, int DX){
   char AL, AH;
   AL = (char) (AX);
   AH = (char) (AX >> 8);

   switch (AL) {
      case 0x00:
         printString(BX);
         break;
      case 0x01:
         readString(BX);
         break;
      case 0x02:
         readSector(BX, CX);
         break;
      case 0x03:
         writeSector(BX, CX);
         break;
      case 0x04:
         readFile(BX, CX, DX, AH);
         break;
      case 0x05:
         writeFile(BX, CX, DX, AH);
         break;
      case 0x06:
         executeProgram(BX, CX, DX, AH);
         break;
      case 0x07:
         terminateProgram(BX);
         break;
      case 0x08:
         makeDirectory(BX, CX, AH);
         break;
      case 0x09:
         deleteFile(BX, CX, AH);
         break;
      case 0x0A:
         deleteDirectory(BX, CX, AH);
         break;
      case 0x20:
         putArgs(BX, CX);
         break;
      case 0x21:
         getCurdir(BX);
         break;
      case 0x22:
         getArgc(BX);
         break;
      case 0X23:
         getArgv(BX, CX);
         break;
      default:
         printString("Invalid interrupt");
   }
}

//Implementasi printString (TESTED)
void printString(char* string){
   int i = 0;
   //string adalah null-terminated
   while (string[i] != '\0'){
      //0x10 menyimpan posisi karakter terakhir yang di-print
      interrupt(0x10, 0xE00 + string[i], 0, 0, 0);
      i = i + 1;
   }
   //Pindahkan kursor ke baris bawah
   //interrupt(0x10, 0xE00 + '\n', 0, 0, 0);
   //Pindahkan kursor ke awal baris
   //interrupt(0x10, 0xE00 + '\r', 0, 0, 0);

}

//Implementasi readString (TESTED)
void readString(char* string){
   int i = 0;
   int cur_pos = 1;
   //membaca karakter pertama dari keyboard
   char currChar = interrupt(0x16, 0, 0, 0, 0);
   //looping selama currChar != \r
   while (currChar != '\r'){
      //jika currChar == backspace
      if (currChar == '\b'){
         if (cur_pos > 1) {
            printString("a");
            interrupt(0x10, 0xE00 + '\b', 0, 0, 0);
            interrupt(0x10, 0xE00 + '\0', 0, 0, 0);
            interrupt(0x10, 0xE00 + '\b', 0, 0, 0);
            i = i-1;
            cur_pos--;
         }
      }
      //jika currChar yg lain
      else{
         interrupt(0x10, 0xE00 + currChar, 0, 0, 0);
         string[i] = currChar;
         i = i+1;
         cur_pos++;
      }
      //baca currChar lagi
      currChar = interrupt(0x16, 0, 0, 0, 0);
   }
   //tambahkan '\0' ke akhir string
   string[i] = '\0';
   //Pindahkan kursor ke baris bawah
   interrupt(0x10, 0xE00 + '\n', 0, 0, 0);
   //Pindahkan kursor ke awal baris
   interrupt(0x10, 0xE00 + '\r', 0, 0, 0);
}

//Implementasi mod dan div (copas)
int mod(int a, int b) {
   while(a >= b) {
      a = a - b;
   }
   return a;
}

int div(int a, int b) {
   int q = 0;
   while(q*b <= a) {
      q = q+1;
   }
   return q-1;
}


//implementasi readSector (copas)
void readSector(char* buffer, int sector){
   interrupt(0x13, 0x201, buffer, div(sector, 36) * 0x100 + mod(sector, 18) + 1, mod(div(sector, 18), 2) * 0x100);
}

//implementasi writeSector (copas)
void writeSector(char* buffer, int sector){
   interrupt(0x13, 0x301, buffer, div(sector, 36) * 0x100 + mod(sector, 18) + 1, mod(div(sector, 18), 2) * 0x100);
}

int findIndexDirectory(char *name, int root){
   int i, j, hsl;
   char dirs[SECTOR_SIZE];
   char elemen;
   readSector(dirs, DIRS_SECTOR);
   hsl = -1;

   for (i = 0; i < DIR_ENTRY_LENGTH; i++){
      for (j = 0; j < MAX_DIRECTORY; j++){
         elemen = dirs[(i * MAX_DIRECTORY) + j];
         if (j == 0) {
            if (elemen != root){
               break;
            }
         } else {
            if (elemen != name[j - 1]){
               break;
            }
         }
      }

      if (j == 16){
         hsl = i;
         break;
      }
   }

   return hsl;
}

int findIndexFile(char *name, char root){
   int i, j, found, idx, hsl;
   char files[SECTOR_SIZE];
   char elemen;

   readSector(files, FILES_SECTOR);
   //printString(files);
   hsl = -1;

   for (i = 0; i < 32; i++){
      for (j = 0; j < MAX_FILES; j++){
         elemen = files[(i*16)+ j];
         if (j == 0){
            if (elemen != root) {
               break;
            }
         }else {
            if (elemen != name[j - 1]){
               break;
            }
         }
      }

      if (j == 16){
         hsl = i;
         break;
      }
   }

   return hsl;
}

/*

int findFile(char *path, char *currentRootFound){
   int i, j;
   char name[16];
   char currentRoot = 255;
   char currentDirIndex = INSUFFICIENT_DIR_ENTRIES;
   int isFile = 0;
   int fileIndex;

   i = 0;
   while (!isFile){
      for (j = 0; j < 16; j++){
         name[j] = '\0';
      }

      j = 0;
      if (path[i] = '/'){
         i++;
      }
      for (; path[i] != '\0' || path[i] != '/'; i++){
         name[j] = path[i];
         j++;
      }

      if (name[i] == '/'){
         currentDirIndex = findIndexDirectory(name, currentRoot);
         if (currentDirIndex == INSUFFICIENT_DIR_ENTRIES){
            return -1;
         }else {
            currentRoot = currentDirIndex;
         }
      }else {
         isFile = 1;
      }
   }

   *currentRootFound = currentRoot;
   fileIndex = findIndexFile(name, currentRoot);
   return fileIndex;
}
*/

//implementasi readFile (ISSUE)
void readFile(char *buffer, char *path, int *result, char parentIndex){
   int i, j, i_kol;
   char elemen;
   int check = -1;
   char k;
   char name[16];
   char sectors[SECTOR_SIZE];
   char files[SECTOR_SIZE];
   int isFile = 0;
   int currentRoot = parentIndex;
   int currentDirIndex = INSUFFICIENT_DIR_ENTRIES;
   int fileIndex;

   readSector(sectors, SECTORS_SECTOR);

   i = 0;
   //Looping through directories UNTIL it's time to read file. (not directory anymore)
   while (!isFile){
      //MAX_DIRECTORY = 16
      for (j = 0; j < MAX_DIRECTORY; j++){
         name[j] = '\0';
      }
      j = 0;

      if (path[i] == '/'){
         i++;
      }

      //Get directory name.
      for (; path[i] != '\0' && path[i] != '/'; i = i + 1){
         name[j] = path[i];
         j = j + 1;
         
      }

      //If read = '/' and not '\0', it means: read another entry in directory!
      if (name[i] == '/'){
         //call findIndexDirectory (search for DIRECTORY_INDEX)
         currentDirIndex = findIndexDirectory(name, currentRoot);
         if (currentDirIndex == INSUFFICIENT_DIR_ENTRIES){
            *result = INSUFFICIENT_DIR_ENTRIES;
            return;
         }else {
            //Update "parent"
            currentRoot = currentDirIndex;
         }
      }else {
         //It's time to read file!
         isFile = 1;
      }
   }
   //call findIndexFile (search for FILE_INDEX)
   fileIndex = findIndexFile(name, currentRoot);
   *result = INSUFFICIENT_DIR_ENTRIES;

   //IF fileIndex is not error.
   if (fileIndex != -1){
      *result = 0;
      //Start reading buffer from sector
      j = 0;
      
      for (i = 0 ; j < MAX_SECTORS && (sectors[fileIndex*MAX_FILES + i] != EMPTY) ; i++ ) {
            //read another sector in that INDEX (if exists)
            readSector(buffer + (j*SECTOR_SIZE), sectors[fileIndex*MAX_FILES + i]);
            j++;
      }
      //printString(buffer);
   }
}

//implementasi clear (copas)
void clear(char *buffer, int length) {
   int i;
   for(i = 0; i < length; ++i) {
      buffer[i] = EMPTY;
   }
}

//implementasi writeFile (copas)
void writeFile(char *buffer, char* path, int* sectors, char parentIndex){
   //Milestone 2 variables
   int i, j, countName, sectorCount, idxFileKosong, idxParentFile, idxDirKosong;
   int found, isDir;
   char dirName[MAX_FILENAME];
   char map[SECTOR_SIZE];
   char dirs[SECTOR_SIZE];
   char files[SECTOR_SIZE];
   char sectorBuffer[SECTOR_SIZE];
   int fileIndex;
   char currentRoot = parentIndex;
   int currentDirIndex = INSUFFICIENT_DIR_ENTRIES;
  
   readSector(map, DIRS_SECTOR);

   //Cek apakah jumlah sektor kosong pada MAP cukup untuk write file (Step 2)
   sectorCount = 0;
   for (i = 0 ; i < SECTOR_SIZE && sectorCount < *sectors; ++i){
      if (map[i] == EMPTY){
         ++sectorCount;
      }
   }

   //Kalau jumlah sektor cukup
   if (sectorCount == *sectors){
      //Cek apakah masih tersisa entri kosong pada sektor files (Step 3)
      for (i = 1; i < SECTOR_SIZE; i + MAX_FILES){
         if (files[i] == '\0'){
            idxFileKosong = mod(i-1, MAX_FILES);
            break;
         }
      }

      //Kalau masih terdapat entri yang kosong
      if (i < SECTOR_SIZE){
         isDir = 0;
         i = 0;
         //Mulai pencarian directory
         while (!isDir) {
         //Inisialisasi dengan NULL terrminated.
            for (i = 0 ; i < 16 ; i++) {
               dirName[i] = '\0';
            }
      
            j = 0;
            if (path[i] == '/') {
               //Increment untuk membaca nama (bukan dari '/')
               i++;
            }

            //Dapatkan nama direktori awal
            for (; path[i] != '/' && path[i] != '\0'; ++i){
               dirName[j] = path[i];
               j = j + 1;
            }

            //Kalau belum saatnya membaca file
            if (dirName[i] == '/') {
               currentDirIndex = findIndexDirectory(dirName, currentRoot);
               if (currentDirIndex == INSUFFICIENT_DIR_ENTRIES) {
                  *sectors = INSUFFICIENT_DIR_ENTRIES; //Not Found (Step 4,5)
                  return;
               } else {
                  currentRoot = currentDirIndex;
               }
            //Kalau sudah saatnya membaca file.
            } else {
               isDir = 1;
            }
         }

         fileIndex = findIndexFile(dirName, currentRoot);
         //Kalau tidak ditemukan, Tulis nama file ke files
         if (fileIndex == -1){
            files[idxFileKosong*MAX_FILES] = idxParentFile;
            for (i = 1; i < 16; i++){
               files[idxFileKosong*MAX_FILES + i] =  dirName[i-1];
            }


            for (i = 0, sectorCount = 0; i < MAX_BYTE && sectorCount < *sectors; ++i){
               if (map[i] == EMPTY) {
                  //Ganti elemen map yang kosong menjadi USED. (step 8)
                  map[i] = USED;
                  //Clear sectorBuffer yang akan diisi dengan buffer.
                  clear(sectorBuffer, SECTOR_SIZE);
                  for (j = 0; j < SECTOR_SIZE; ++j) {
                     //Isi sectorBuffer dengan buffer.
                     sectorBuffer[j] = buffer[sectorCount * SECTOR_SIZE + j];
                  }
                  //Tulis sectorBuffer di sektor ke-i (yang kosong)
                  writeSector(sectorBuffer,i);
                  ++sectorCount;
               }
            }

            //Update sector di map.
            writeSector(map,MAP_SECTOR);
            //Tulis file indeks parent dan nama file pada sector files
            writeSector(files, FILES_SECTOR);

         } else {
            //Kalau file sudah ada.
            *sectors = ALREADY_EXISTS;
         }
      
      } else{
         //Kalau tidak ada entry kosong di files.
         *sectors = INSUFFICIENT_ENTRIES; 
      }

   } else{
      *sectors = INSUFFICIENT_SECTORS; 
   }
}


//implementasi executeProgram (NOT TESTED)
void executeProgram(char *path, int segment, int *result, char parentIndex) {
   //kamus
   char buffer[MAX_SECTORS*SECTOR_SIZE];   //diambahin buat jaga-jaga nggak semua sektor buffer ke copy
   int i;
   
   readFile(buffer, path, result, parentIndex);

   //jika pembacaan file sukses
   if (*result == SUCCESS){
      for (i = 0; i < MAX_SECTORS*SECTOR_SIZE; i++){
         putInMemory(segment, i, buffer[i]);
      }
      //menjalankan program yang di memori
      launchProgram(segment);
      //printString("Selesai");                //tidak tereksekusi, kemungkinan masalahnya di dalam launchProgram
      //readFile(buffer, "key.txt", success);  //tidak tereksekusi, kemungkinan masalahnya di dalam launchProgram
      //printString(buffer);                   //tidak tereksekusi, kemungkinan masalahnya di dalam launchProgram
   }
}



void makeDirectory(char *path, int *result, char parentIndex) {
   int i, j, idxDirKosong;
   int found; //cek ketemu atau tidak.
   char name[16];
   char dirs[SECTOR_SIZE];
   int isDir;
   int currentRoot = parentIndex;
   int currentDirIndex = INSUFFICIENT_DIR_ENTRIES;
   int fileIndex;

   //Cek apakah terdapat entry kosong atau tidak pada sektor dirs.
   isDir = 0;
   found = 0;
   idxDirKosong = 0;
   readSector(dirs, DIRS_SECTOR);

   for (i = 0; i < SECTOR_SIZE && found!= 1; i = i + MAX_DIRECTORY){
      if (dirs[i+1] == '\0'){
         found = 1;
         idxDirKosong = i;
      }
   }

   //Jika tidak menemukan entry yang kosong
   if (!found) {
      *result = INSUFFICIENT_ENTRIES;
      return;
   }
   //Kalau ditemukan entry yang kosong.
   else {

      i = 0;
      while (!isDir){

         //Inisialisasi name dengan null terminated sepanjang MAX_DIRECTORY (16).
         for (j = 0; j < MAX_DIRECTORY; j++){
            name[j] = '\0';
         }
         
         //Jika variabel i sudah mencapai '/'.
         if (path[i] == '/'){
            i++;
         }

         //Traversal dari i hingga ditemukan '/0' atau '/'.
         j = 0;
         for (; path[i] != '\0' && path[i] != '/'; i++){
            name[j] = path[i];
            j++;
         }

         //printString(name);

         //Jika path masih berupa dir, belum file.
         if (name[i] == '/'){
            currentDirIndex = findIndexDirectory(name, currentRoot);
            if (currentDirIndex == INSUFFICIENT_DIR_ENTRIES){
               *result = INSUFFICIENT_DIR_ENTRIES;
               return;
            } 
            else {
               currentRoot = currentDirIndex;
            }

         //Jika sudah harus membaca file.
         }  else {
            isDir = 1;
         }
      }
      //Cari file.

      fileIndex = findIndexDirectory(name, currentRoot);
      *result = INSUFFICIENT_DIR_ENTRIES;


      //Kalau file sudah ada. Maka make directory tidak jadi dilaksanakan
      if (fileIndex != -1){
         *result = ALREADY_EXISTS;
         return;
      //Kalau belum ada file.
      } else {

         //Tulis indeks dari direktori pada byte indeks parent (indeks 0)
         dirs[idxDirKosong*16] = currentRoot;
         //Tulis isi dari name pada byte setelah indeks parent pada dirs
         j = 0;
         for (i = 1; name[j] != '\0' && i < 16 ;i++) {
            dirs[idxDirKosong*16 + i] = name[j];
            j++;
         } 
         writeSector(dirs,DIRS_SECTOR);
         *result = 0;
      }
   }
}



void terminateProgram (int *result) {
   char shell[6];
   shell[0] = 's';
   shell[1] = 'h';
   shell[2] = 'e';
   shell[3] = 'l';
   shell[4] = 'l';
   shell[5] = '\0';
   executeProgram(shell, 0x2000, result, 0xFF);
}


void deleteFile(char *path, int *result, char parentIndex) {
   int i, j;
   char name[16];
   int isFile = 0;
   char currentRoot = parentIndex;
   int currentDirIndex = INSUFFICIENT_DIR_ENTRIES;
   int fileIndex;
   char files[SECTOR_SIZE];
   char map [SECTOR_SIZE];
   char daftarSector[SECTOR_SIZE];

   //Baca map
   readSector(map, DIRS_SECTOR);

   //Baca sectors
   readSector(daftarSector, SECTORS_SECTOR);


   i = 0;
   while (!isFile){
      for (j = 0; j < 16; j++){
         name[j] = '\0';
      }

      j = 0;
      if (path[i] = '/'){
         i++;
      }
      for (; path[i] != '\0' && path[i] != '/'; i++){
         name[j] = path[i];
         j++;
      }

      if (name[i] == '/'){
         currentDirIndex = findIndexDirectory(name, currentRoot);
         if (currentDirIndex == INSUFFICIENT_DIR_ENTRIES){
            *result = INSUFFICIENT_DIR_ENTRIES;
            return;
         }else {
            currentRoot = currentDirIndex;
         }
      }else {
         isFile = 1;
      }
   }

   fileIndex = findIndexFile(name, currentRoot);
   *result = INSUFFICIENT_DIR_ENTRIES; //(NOT FOUND)

   //Kalau ditemukan
   if (fileIndex != -1){
      //Ubah SEMUA byte nama file menjadi '\0'.
      for (i = 1 ; i < 16 ; i++)
         files[fileIndex*MAX_FILES + i] = '\0';

      j = 0;
      for (i = 0 ; i < SECTOR_SIZE && j < MAX_SECTORS ; i++) {
         //Cek sektor pada daftarSektor yang sama dengan isi dari map.
         if (map[i] == daftarSector[fileIndex*MAX_SECTORS+j]) {
            map[i] = EMPTY;
            daftarSector[fileIndex*MAX_SECTORS+j] = EMPTY;
            j++;
         }
      }

      writeSector(map, MAP_SECTOR);
      writeSector(files, FILES_SECTOR);
      writeSector(daftarSector, SECTORS_SECTOR);
      
      *result = 0;
   }
}

void deleteDirectory(char *path, int *success, char parentIndex){

}

void putArgs (char curdir, char argc, char **argv) {
   char args[SECTOR_SIZE];
   int i, j, p;
   clear(args, SECTOR_SIZE);

   args[0] = curdir;
   args[1] = argc;
   i = 0;
   j = 0;
   for (p = 1; p < ARGS_SECTOR && i < argc; ++p) {
      args[p] = argv[i][j];
      if (argv[i][j] == '\0') {
         ++i;
         j = 0;
      }
      else {
         ++j;
      }
   }

   writeSector(args, ARGS_SECTOR);
}

void getCurdir (char *curdir) {
   char args[SECTOR_SIZE];
   readSector(args, ARGS_SECTOR);
   *curdir = args[0];
}

void getArgc (char *argc) {
   char args[SECTOR_SIZE];
   readSector(args, ARGS_SECTOR);
   *argc = args[1];
}

void getArgv (char index, char *argv) {
   char args[SECTOR_SIZE];
   int i, j, p;
   readSector(args, ARGS_SECTOR);

   i = 0;
   j = 0;
   for (p = 1; p < ARGS_SECTOR; ++p) {
      if (i == index) {
         argv[j] = args[p];
         ++j;
      }

      if (args[p] == '\0') {
         if (i == index) {
            break;
         }
         else {
         ++i;
         }
      }
   }
}




