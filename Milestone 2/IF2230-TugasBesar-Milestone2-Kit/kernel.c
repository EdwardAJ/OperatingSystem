#define MAX_BYTE 256
#define SECTOR_SIZE 512
#define MAX_FILES 32
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

void handleInterrupt21 (int AX, int BX, int CX, int DX);
void printString(char *string);
void readString(char *string);
int mod(int a, int b);
int div(int a, int b);
void readSector(char *buffer, int sector);
void writeSector(char *buffer, int sector);
void readFile(char *buffer, char *path, int *result, char parentIndex)
void clear(char *buffer, int length);
void writeFile(char *buffer, char *filename, int *sectors);
void executeProgram(char *filename, int segment, int *success);
void clearScreen(int _lines);
void drawLogo(char *_logo, int _length);

int main() {
   int string[256];
   char buffer[MAX_BYTE];
   int success;

   //ASCII Art untuk logo.
   static char logo[528] = {
	' ' ,' ', '_', '_', '_', '_', ' ', '_', '_', '_', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '_', '_', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '_', '_', ' ', ' ', ' ', ' ', '\n', '|', ' ', ' ', ' ', ' ', '|', ' ', ' ', ' ', '\\', '_', '_', '_', '_', '_', '/', ' ', ' ', '|', '_', ' ', '_', '_', ' ', '_', '_', '|', ' ', ' ', '|', ' ', '_', '_', '\n', '|', ' ', ' ', ' ', ' ', '|', ' ', ' ', ' ', '/', ' ', ' ', ' ', ' ', '\\', ' ', ' ', ' ', '_', '_', '\\', ' ', ' ', '|', ' ', ' ', '\\', ' ', ' ', '|', '/', ' ', '/', '\n', '|', ' ', ' ', ' ', ' ', '|', ' ', ' ', '/', ' ', ' ', ' ', '|', ' ', ' ', '\\', ' ', ' ', '|', ' ', '|', ' ', ' ', '|', ' ', ' ', '/', ' ', ' ', ' ', ' ', '<', ' ', '\n', '|', '_', '_', '_', '_', '_', '_', '/', '|', '_', '_', '_', '|', ' ', ' ', '/', '_', '_', '|', ' ', '|', '_', '_', '_', '_', '/', '|', '_', '_', '|', '_', ' ', '\\', '\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\\', '/', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\\', '/', '\n', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '_', '_', ' ', ' ', ' ', ' ', ' ', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '.', '_', '_', '.', '_', '_', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '.', '_', '.', '\n', '\\', '_', '_', '_', '_', '_', '_', ' ', ' ', ' ', '\\', '_', '_', '_', '_', '_', ' ', ' ', '|', ' ', ' ', '|', ' ', '_', '_', ' ', '\\', '_', '_', '_', '_', '_', '_', ' ', ' ', ' ', '\\', '_', '_', '|', ' ', ' ', '|', ' ', '_', '_', '_', '_', '_', ' ', '|', ' ', '|', '\n', ' ', '|', ' ', ' ', ' ', ' ', ' ', '_', '_', '_', '/', '\\', '_', '_', ' ', ' ', '\\', ' ', '|', ' ', ' ', '|', '/', ' ', '/', ' ', ' ', '|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '_', '/', ' ', ' ', '|', ' ', ' ', '|', ' ', '\\', '_', '_', ' ', ' ', '\\', '|', ' ', '|', '\n', ' ', '|', ' ', ' ', ' ', ' ', '|', ' ', ' ', ' ', ' ', ' ', '/', ' ', '_', '_', ' ', '\\', '|', ' ', ' ', ' ', ' ', '<', ' ', ' ', ' ', '|', ' ', ' ', ' ', ' ', '|', ' ', ' ', ' ', '\\', ' ', ' ', '|', ' ', ' ', '|', '_', '_', '/', ' ', '_', '_', ' ', '\\', '\\', '|', '\n', ' ', '|', '_', '_', '_', '_', '|', ' ', ' ', ' ', ' ', '(', '_', '_', '_', '_', ' ', ' ', '/', '_', '_', '|', '_', ' ', '\\', ' ', ' ', '|', '_', '_', '_', '_', '|', '_', ' ', ' ', '/', '_', '_', '|', '_', '_', '_', '_', '(', '_', '_', '_', '_', ' ', ' ', '/', '_', '\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\\', '/', ' ', ' ', ' ', ' ', ' ', '\\', '/', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\\', '/', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\\', '/', '\\', '/'
   };

   drawLogo(logo, 528);


   makeInterrupt21();
   //handleInterrupt21(0x6, "keyproc", 0x2000, &success);
   //printString("Sukses");
   handleInterrupt21(0x4, buffer, "key.txt", &success);
   handleInterrupt21(0x0, buffer);
   handleInterrupt21(0x1,buffer);

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
   interrupt(0x10, 0xE00 + '\n', 0, 0, 0);
   //Pindahkan kursor ke awal baris
   interrupt(0x10, 0xE00 + '\r', 0, 0, 0);

}

//Implementasi readString (TESTED)
void readString(char* string){
   int i = 0;

   //membaca karakter pertama dari keyboard
   char currChar = interrupt(0x16, 0, 0, 0, 0);
   //looping selama currChar != \r
   while (currChar != '\r'){
      //jika currChar == backspace
      if (currChar == '\b'){
         interrupt(0x10, 0xE00 + '\b', 0, 0, 0);
         interrupt(0x10, 0xE00 + '\0', 0, 0, 0);
         interrupt(0x10, 0xE00 + '\b', 0, 0, 0);
         i = i-1;
      }
      //jika currChar yg lain
      else{
         interrupt(0x10, 0xE00 + currChar, 0, 0, 0);
         string[i] = currChar;
         i = i+1;
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
   int i, j;
   char dirs[SECTOR_SIZE];
   char elemen;
   readSector(dirs, DIRS_SECTOR);

   for (i = 0; i < MAX_DIRECTORY; i++){
      for (j = 0; j < 16; j++){
         elemen = dirs[(i * 16) + j];
         if (j == 0 && elemen != root){
            break;
         }else {
            if (elemen != name[j - 1]){
               break;
            }
         }
      }

      if (j == 16){
         return i;
      }
   }

   return -1;
}

int findIndexFile(char *name, int root){
   int i, j;
   char files[SECTOR_SIZE];
   char elemen;
   readSector(files, FILES_SECTOR);

   for (i = 0; i < MAX_DIRECTORY; i++){
      for (j = 0; j < 16; j++){
         elemen = files[(i * 16) + j];
         if (j == 0 && elemen != root){
            break;
         }else {
            if (elemen != name[j - 1]){
               break;
            }
         }
      }

      if (j == 16){
         return i;
      }
   }

   return -1;
}

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

//implementasi readFile (ISSUE)
void readFile(char *buffer, char *path, int *result, char parentIndex){
   int i, j;
   char name[16];
   int isFile = 0;
   char currentRoot = parentIndex;
   char currentDirIndex = INSUFFICIENT_DIR_ENTRIES;
   char fileIndex;

   i = 0;
   while (!isFile){
      for (j = 0; j < 16; j++){
         name[j] = '\0';
      }

      j = 0;
      if (path[i] = '\\'){
         i++;
      }
      for (; path[i] != '\0' || path[i] != '\\'; i++){
         name[j] = path[i];
         j++;
      }

      if (name[i] == '\\'){
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
   *result = INSUFFICIENT_DIR_ENTRIES;
   if (fileIndex != -1){
      *result = 0;
      readSector(buffer, fileIndex);
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
   int i, j, countName, sectorCount, idxFileKosong, idxParentFile;
   char dirName[MAX_FILENAME];
   char map[SECTOR_SIZE];
   char dirs[SECTOR_SIZE];
   char files[SECTOR_SIZE];
   char NamaFile[15];
   int daftarSektorKosong[16];

   readSector(files, MAP_SECTOR);
   readSector(dirs, DIR_SECTOR);
   readSector(map, DIR_SECTOR);

   //Dapatkan nama direktori awal
   for (i = 0; path[i] != '/'; ++i){
      dirName[i] = path[i];
   }

   //Dapatkan nama file
   for (i = 0; path[i] != '\0'; ++i){
      if (path[i] == '/'){
         j = 0;
      }else{
         NamaFile[j] = path[i];
         ++j;
      }
   }

   for (;j < 15; ++j){
      NamaFile[j] = '\0';
   }

   //Jika mau write ke root
   if (i == 0){
      continue;
   }

   //Cek apakah jumlah sektor kosong cukup untuk write file
   for (i = 0, sectorCount = 0; i < SECTOR_SIZE && sectorCount < *sectors; ++i){
      if (map[i] == EMPTY){
         ++sectorCount;
      }else{
         sectorCount = 0;
      }
   }

   //Set daftarSektor ke -1 semua
   for (i = 0; i < 16; ++i){
      daftarSektorKosong[i] = -1;
   }

   //Jika sectorCount == sector yang diperlukan
   if (sectorCount == *sectors){
      //Cek apakah masih tersisa entri kosong pada sektor files
      for (i = 1; i < SECTOR_SIZE; i + 16){
         if (files[i] == '\0'){
            idxFileKosong = mod(i-1, 16);
            break;
         }
      }
      if (i < SECTOR_SIZE){
         //Cari index file
         j = findFile(path, &idxParentFile);
         //Tulis nama file ke files
         if (j != -1){
            files[idxFileKosong*16] = idxParentFile;
            for (i = 1; i < 16; i++){
               files[idxFileKosong*16 + i] =  NamaFile[i-1];
            }
         }else{
            *sectors = -2;
         }
      }else{
         *sectors = -3;
      }
      //Clear semua sektor yang tercatat "kosong" di daftarSektorKosong
      for (i = 0; daftarSektroKosong[i] != -1; ++i){
         clear(daftarSektorKosong*SECTOR_SIZE, SECTOR_SIZE);
         writeSector();
      }
      
   }else{
      *sectors = 0;
   }
}

}

//implementasi executeProgram (NOT TESTED)
void executeProgram(char* filename, int segment, int* success){
   //kamus
   char buffer[MAX_SECTORS*SECTOR_SIZE + MAX_BYTE];   //diambahin buat jaga-jaga nggak semua sektor buffer ke copy
   int i;

   //algoritma
   //buka file
   readFile(buffer, filename, success);

   //jika pembacaan file sukses
   if (*success == 1){
      //traversal setiap byte
      for (i = 0; i < MAX_SECTORS*SECTOR_SIZE + MAX_BYTE; i = i + 1){
         putInMemory(segment, i, buffer[i]);
      }
      //menjalankan program yang di memori
      launchProgram(segment);
      printString("Selesai");                //tidak tereksekusi, kemungkinan masalahnya di dalam launchProgram
      readFile(buffer, "key.txt", success);  //tidak tereksekusi, kemungkinan masalahnya di dalam launchProgram
      printString(buffer);                   //tidak tereksekusi, kemungkinan masalahnya di dalam launchProgram
   }

   void makeDirectory(char *path, int *result, char parentIndex) {
      char buffer[MAX_SECTORS*SECTOR_SIZE + MAX_BYTE];
      int i;


   }
}
