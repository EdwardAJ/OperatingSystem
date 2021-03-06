#define MAIN
#include "proc.h"
#define MAX_BYTE 256
#define SECTOR_SIZE 512
#define MAX_FILES 16
#define MAX_FILES_ENTRY 32
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
#define INSUFFICIENT_SEGMENTS -2

void handleInterrupt21 (int AX, int BX, int CX, int DX);
void printString(char *string);
void readString(char *string, int disableProcessControl);
int mod(int a, int b);
int div(int a, int b);
void readSector(char *buffer, int sector);
void writeSector(char *buffer, int sector);
void readFile(char *buffer, char *path, int *result, char parentIndex);
void clear(char *buffer, int length);
void writeFile(char *buffer, char* path, int* sectors, char parentIndex);
void executeProgram (char *path, int *result, char parentIndex, int isParallel);
void clearScreen(int _lines);
//void drawLogo(char *_logo, int _length);
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
void changeDirectory(char *path, int *result, char parentIndex);
void remove(char *path, int *result, char parentIndex);
void deleteFilewithIndex(char parentIndex);
void yieldControl(); 
void sleep();
void pauseProcess (int segment, int *result);
void resumeProcess (int segment, int *result);
void killProcess (int segment, int *result);
void move(char *path1, char *path2, char curDir);

void deleteDirectoryWithIndex(char dirIndex);

int isEmpty(char dirIndex);

char findFirstFileInDir(char dirIndex);

char findFirstDirInDir(char dirIndex);

void showProcess();
char convSegmentToPID(int seg);

int isBooting = 1;

int main() {

   int string[256];
   char buffer[MAX_BYTE];
   int success;
   char argc = 0;
   char *argv[32];
   char curdir = 0xFF;
   //FORMAT INTERRUPT : interrupt(0x21, (AH << 8) | AL, BX, CX, DX);
   initializeProcStructures();
   makeInterrupt21();
   makeTimerInterrupt();
   
   interrupt(0x21, 0x20, curdir, argc, argv);
   //FOMRMAT :executeProgram(BX, CX, AH);
   interrupt(0x21, 0xFF << 8 | 0x06, "shell", &success, 0x00);
   //printString("AING CUPU");

   while (1);
}


void clearScreen(int _lines){
   /*
   //Prosedur membersihkan layar sebanyak _lines relatif terhadap 0x8000.
   int i;
   int *myPointer;
   //Membersihkan layar.
   for (i = 0; i < 80 * _lines; i++){
      putInMemory(0xB000, 0x8000 + (i * 2), ' ');
   }
   interrupt(0x10, 0x0200, 0, 0, 0);
   */
}

/*
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
*/

void handleInterrupt21 (int AX, int BX, int CX, int DX){
   char AL, AH;
   AL = (char) (AX);
   AH = (char) (AX >> 8);

   switch (AL) {
      case 0x00:
         printString(BX);
         break;
      case 0x01:
         readString(BX, CX);
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
         executeProgram(BX, CX, AH, DX);
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
         putArgs(BX, CX, DX);
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
      case 0x24:
         clearScreen(25);
         break;
      case 0x25:
         move(BX, CX, DX);
         break;
      case 0x30:
         //Tidak menerima parameter.
         yieldControl();
         break;
      case 0x31:
         sleep();
         break;
      case 0x32:
         pauseProcess(BX,CX);
         break;
      case 0x33:
         resumeProcess (BX,CX);
         break;
      case 0x34:
         killProcess(BX,CX);
         break;
      case 0x35:
         showProcess();
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
void readString(char* string, int disableProcessControls){
   int i = 0;
   int res = 0;
   int parentSegment = 0;
   int cur_pos = 1;
   //membaca karakter pertama dari keyboard
   char currChar = interrupt(0x16, 0, 0, 0, 0);
   //looping selama currChar != \r
   while (currChar != '\r'){
      if(!disableProcessControls) {
         if(currChar == 0x03) {
            terminateProgram(&res);
         } else if (currChar == 0x1A){
            //Pause program sekarang
            sleep();
            //Resume Process yang terdahulu (0x2000 itu punya shell, kan?)
            resumeProcess(0x2000,&res);
         }
      }
      if (currChar == '\b'){ //jika currChar == backspace
         if (cur_pos > 1) {
            //printString("a");
            interrupt(0x10, 0xE00 + '\b', 0, 0, 0);
            interrupt(0x10, 0xE00 + '\0', 0, 0, 0);
            interrupt(0x10, 0xE00 + '\b', 0, 0, 0);
            i = i-1;
            cur_pos--;
         }
      } else{
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


//implementasi readFile (ISSUE)
void readFile(char *buffer, char *path, int *result, char parentIndex){
   int i, j, i_kol;
   char elemen;
   int check = -1;
   char k;
   char name[16];
   char sectors[SECTOR_SIZE];
   char dirs[SECTOR_SIZE];
   char files[SECTOR_SIZE];
   int isFile = 0;
   int currentRoot = parentIndex;
   int currentDirIndex = INSUFFICIENT_DIR_ENTRIES;
   int fileIndex;
   readSector(dirs, DIRS_SECTOR);
   readSector(sectors, SECTORS_SECTOR);

   i = 0;
   //Looping through directories UNTIL it's time to read file. (not directory anymore)
   while (!isFile){
      //MAX_DIRECTORY = 16
      for (j = 0; j < MAX_DIRECTORY; j++){
         name[j] = '\0';
      }
      j = 0;

      //Get directory name.
      for (; path[i] != '\0' && path[i] != '/'; i = i + 1){
         name[j] = path[i];
         j = j + 1;
         
      }
      //If read = '/' and not '\0', it means: read another entry in directory!
      if (path[i] == '/'){
         //call findIndexDirectory (search for DIRECTORY_INDEX)
         currentDirIndex = findIndexDirectory(name, currentRoot);
         if (currentDirIndex == INSUFFICIENT_DIR_ENTRIES){
            *result = INSUFFICIENT_DIR_ENTRIES;
            return;
         }else {
            //Update "parent"
            currentRoot = currentDirIndex;//dirs[currentDirIndex*16];
         }
      }else {
         //It's time to read file!
         isFile = 1;
      }

      if (path[i] == '/'){
         i++;
      }
   }
   //call findIndexFile (search for FILE_INDEX)
   fileIndex = findIndexFile(name, currentRoot);
   *result = INSUFFICIENT_DIR_ENTRIES;
   //IF fileIndex is not error.
   if (fileIndex != -1){
      //Mengembalikan Indeks File
      *result = fileIndex;
      //Start reading buffer from sector
      j = 0;
      for (i = 0 ; j < MAX_SECTORS && (sectors[fileIndex*MAX_FILES + i] != EMPTY) ; i++ ) {
         //read another sector in that INDEX (if exists)
         readSector(buffer + (j*SECTOR_SIZE), sectors[fileIndex*MAX_FILES + i]);
         j++;
      }
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
   char sectors[SECTOR_SIZE];
   char map[SECTOR_SIZE];
   char dirs[SECTOR_SIZE];
   char files[SECTOR_SIZE];
   char sectorBuffer[SECTOR_SIZE* 16];
   int fileIndex;
   int currentRoot = parentIndex;
   int currentDirIndex = INSUFFICIENT_DIR_ENTRIES;
  
   readSector(map, MAP_SECTOR);
   readSector(dirs, DIRS_SECTOR);
   readSector(files, FILES_SECTOR);
   readSector(sectors, SECTORS_SECTOR);

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

      for (i = 0; (i*MAX_FILES < SECTOR_SIZE) && found!= 1; i++){
         if (files[i*MAX_FILES+1] == '\0'){
            found = 1;
            idxFileKosong = i;
         }
      }

      //Kalau masih terdapat entri yang kosong
      if (found == 1){
         isDir = 0;
         i = 0;
         //Mulai pencarian directory
         while (!isDir) {
         //Inisialisasi dengan NULL terrminated.
            for (j = 0 ; j < 16 ; j++) {
               dirName[j] = '\0';
            }
      
            j = 0;
           

            //Dapatkan nama direktori awal
            for (; path[i] != '/' && path[i] != '\0'; ++i){
               dirName[j] = path[i];
               j = j + 1;
            }

            //Kalau belum saatnya membaca file
            if (path[i] == '/') {
               currentDirIndex = findIndexDirectory(dirName, currentRoot);
               if (currentDirIndex == INSUFFICIENT_DIR_ENTRIES) {
                  *sectors = INSUFFICIENT_DIR_ENTRIES; //Not Found (Step 4,5)
                  return;
                  break;
               } else {
                  currentRoot = currentDirIndex;//dirs[currentDirIndex*MAX_DIRECTORY];
               }
            //Kalau sudah saatnya membaca file.
            } else {
               isDir = 1;
            }

            if (path[i] == '/') {
               //Increment untuk membaca nama (bukan dari '/')
               i++;
            }
         }

         //dirName disini seharusnya menjadi fileName
         fileIndex = findIndexFile(dirName, currentRoot);
         //Kalau tidak ditemukan, Tulis nama file ke files
         if (fileIndex == -1){
            files[idxFileKosong*MAX_FILES] = currentRoot; //idxParentFile;
            //assign dengan nama
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

                  //Tulis sector apa saja yang dipakai pada SECTORS_SECTOR
                  sectors[idxFileKosong*MAX_SECTORS+sectorCount] = i;
                  writeSector(sectors,SECTORS_SECTOR);

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

void executeProgram (char *path, int *result, char parentIndex, int isParallel){
   struct PCB* pcb;
   int segment;
   int i, fileIndex;
   char buffer[MAX_SECTORS * SECTOR_SIZE];
   readFile(buffer, path, result, parentIndex);

   if (*result != NOT_FOUND) {
      setKernelDataSegment();
      segment = getFreeMemorySegment();
      restoreDataSegment();

      fileIndex = *result;
      if (segment != NO_FREE_SEGMENTS) {
         setKernelDataSegment();

         pcb = getFreePCB();
         pcb->index = fileIndex;
         pcb->state = STARTING;
         pcb->segment = segment;
         pcb->stackPointer = 0xFF00;
         pcb->parentSegment = running->segment;
         pcb->isParallel = isParallel;
         addToReady(pcb);

         restoreDataSegment();
         for (i = 0; i < SECTOR_SIZE * MAX_SECTORS; i++) {
            putInMemory(segment, i, buffer[i]);
         }
         initializeProgram(segment);
         if (!isParallel){
            interrupt(0x21, 0x0, "Not Parallel\0", 0, 0);
            sleep();
         }else {
            interrupt(0x21, 0x0, "Parallel\0", 0, 0);
            yieldControl();
         }
      } else {
         *result = INSUFFICIENT_SEGMENTS;
      }
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
   for (i = 0; (i*MAX_FILES < SECTOR_SIZE) && found!= 1; i++){
      if (dirs[i*MAX_FILES+1] == '\0'){
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
         
        
         //Traversal dari i hingga ditemukan '/0' atau '/'.
         j = 0;
         for (; path[i] != '\0' && path[i] != '/'; i++){
            name[j] = path[i];
            j++;
         }
         name[j] ='\0';

         //Jika path masih berupa dir, belum file.
         if (path[i] == '/'){
            currentDirIndex = findIndexDirectory(name, currentRoot);
            if (currentDirIndex == INSUFFICIENT_DIR_ENTRIES){
               *result = INSUFFICIENT_DIR_ENTRIES;
               return;
               break;
            } 
            else {
               currentRoot = currentDirIndex;//dirs[currentDirIndex*MAX_DIRECTORY];
            }

         //Jika sudah harus membaca file.
         }  else {
            isDir = 1;
         }

          //Jika variabel i sudah mencapai '/'.
         if (path[i] == '/'){
            i++;
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
  int parentSegment;
  setKernelDataSegment();

  parentSegment = running->parentSegment;
  releaseMemorySegment(running->segment);
  releasePCB(running);

  restoreDataSegment();
  if (parentSegment != NO_PARENT) {
      
      setKernelDataSegment();
      if (!running->isParallel){
         restoreDataSegment();
         resumeProcess(parentSegment, result);
      }else {
         restoreDataSegment();
      }
      
      //resumeProcess(parentSegment, result);
  }
  yieldControl();
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
   char dirs[SECTOR_SIZE];
   char daftarSector[SECTOR_SIZE];

   //Baca map
   readSector(map, MAP_SECTOR);
   readSector(dirs, DIRS_SECTOR);
   //Baca sectors
   readSector(daftarSector, SECTORS_SECTOR);
   readSector(files, FILES_SECTOR);


   i = 0;
   while (!isFile){
      for (j = 0; j < 16; j++){
         name[j] = '\0';
      }

      j = 0;
      
      for (; path[i] != '\0' && path[i] != '/'; i++){
         name[j] = path[i];
         j = j + 1;
      }

      if (path[i] == '/'){
         currentDirIndex = findIndexDirectory(name, currentRoot);
         if (currentDirIndex == INSUFFICIENT_DIR_ENTRIES){
            *result = INSUFFICIENT_DIR_ENTRIES;
            return;
         }else {
            currentRoot = currentDirIndex;
         }

      } else {
         isFile = 1;
      }

      if (path[i] == '/'){
         i++;
      }
   }

   fileIndex = findIndexFile(name, currentRoot);
   *result = INSUFFICIENT_DIR_ENTRIES; //(NOT FOUND)

   //Kalau ditemukan
   if (fileIndex != -1){      
      *result = 0;
      j = 1;

      //Ubah SEMUA byte nama file menjadi '\0'.
      for (j = 1 ; j < 16 ; j++)
         files[fileIndex*MAX_FILES + j] = 0x00;

      j = 0;
      for (i = 0; i < SECTOR_SIZE; i++){
         for (j = 0; j < MAX_SECTORS; j++){
            if (i == daftarSector[fileIndex*MAX_SECTORS + j]){
               map[i] = 0x00;
            }   
          }
      }

      writeSector(files, FILES_SECTOR);
      writeSector(map, MAP_SECTOR);
      
   }  
}

void deleteDirectory(char *path, int *success, char parentIndex){
   /*
   
   int i, j, k;
   char name[16];
   char isDir = 0;
   char currentRoot = parentIndex;
   int currentDirIndex = INSUFFICIENT_DIR_ENTRIES;
   char fileIndex;
   char dirIndex;
   char files[SECTOR_SIZE];
   char map [SECTOR_SIZE];
   char dirs[SECTOR_SIZE];
   char daftarSector[SECTOR_SIZE];


   //Cari path secara traversal
   i = 0;
   while (!isDir){
      for (j = 0; j < 16; j++){
         name[j] = '\0';
      }

      j = 0;
      
      for (; path[i] != '\0' && path[i] != '/'; i++){
         name[j] = path[i];
         j = j + 1;
      }
      
      if (path[i] == '/'){
         currentDirIndex = findIndexDirectory(name, currentRoot);
         if (currentDirIndex == INSUFFICIENT_DIR_ENTRIES){
            *success = INSUFFICIENT_DIR_ENTRIES;
            return;
         }else {
            currentRoot = currentDirIndex;
         }
      
      } else {
         isDir = 1;
      }


      if (path[i] == '/'){
         i++;
      }
      
      //i disimpan sebagai indeks terakhir
      
   }


   //Baca map
   readSector(map, MAP_SECTOR);
   readSector(dirs, DIRS_SECTOR);
   //Baca sectors
   readSector(daftarSector, SECTORS_SECTOR);
   readSector(files, FILES_SECTOR);
   
   currentDirIndex = findIndexDirectory(name, currentRoot);
   if (currentDirIndex == -1){
      *success = NOT_FOUND;
   } else {
      deleteDirectoryWithIndex(currentDirIndex);
   }
   */  
}

void deleteDirectoryWithIndex(char dirIndex){
   /*
   int i;
   char fileIndex;
   char currentDirIndex = dirIndex;
   char dirs[SECTOR_SIZE];

   //interrupt(0x21, 0x00, "CCC\n\r", 0, 0);

   readSector(dirs, DIRS_SECTOR);
   
   
   fileIndex = findFirstFileInDir(currentDirIndex);
   while (fileIndex != 127){
      deleteFilewithIndex(fileIndex);
      fileIndex = findFirstFileInDir(currentDirIndex);
   }

   
   dirIndex = findFirstDirInDir(currentDirIndex);
   while (dirIndex != 127){ 
      //Go recursively
      deleteDirectoryWithIndex(dirIndex);
      dirIndex = findFirstDirInDir(currentDirIndex);
   }
   
   
   i = 0;
   dirs[currentDirIndex * MAX_DIRECTORY + i] = 32;
   for (i = 1; i < DIR_ENTRY_LENGTH; i++){
      dirs[currentDirIndex * MAX_DIRECTORY + i] = 0x00;
   }
   
   writeSector(dirs, DIRS_SECTOR);
   */
}

int isEmpty(char dirIndex){
   int i;
   char files[SECTOR_SIZE];
   char dirs[SECTOR_SIZE];

   readSector(files, FILES_SECTOR);
   readSector(dirs, DIRS_SECTOR);

   for (i = 0; i < MAX_FILES_ENTRY; i++){
      if (files[i * MAX_FILES] == dirIndex){
         return 0;
      }
   }

   for (i = 0; i < DIR_ENTRY_LENGTH; i++){
      if (dirs[i * MAX_DIRECTORY] == dirIndex){
         return 0;
      }
   }

   return 1;
}

char findFirstFileInDir(char dirIndex){
   int i;
   char files[SECTOR_SIZE];

   readSector(files, FILES_SECTOR);

   for (i = 0; i < MAX_FILES_ENTRY; i++){
      if (files[i * MAX_FILES] == dirIndex){
         return i;
      }
   }

   return 127;
}

char findFirstDirInDir(char dirIndex){
   int i;
   char dirs[SECTOR_SIZE];

   readSector(dirs, DIRS_SECTOR);

   for (i = 0; i < MAX_FILES_ENTRY; i++){
      if (dirs[i * MAX_FILES] == dirIndex){
         return i;
      }
   }

   return 127;
}

void deleteFilewithIndex(char fileIndex){
   /*
   
   int i, j;
   char files[SECTOR_SIZE];
   char map [SECTOR_SIZE];
   char daftarSector[SECTOR_SIZE];
   
   //Baca map
   readSector(map, MAP_SECTOR);
   //Baca sectors
   readSector(daftarSector, SECTORS_SECTOR);
   //Baca files
   readSector(files, FILES_SECTOR);

   j = 0;
   files[fileIndex*MAX_FILES + j] = 32;
   j++;
   //Ubah SEMUA byte nama file menjadi '\0'.
   for (j = 1 ; j < 16 ; j++)
      files[fileIndex*MAX_FILES + j] = 0x00;
   

   for (i = 0; i < SECTOR_SIZE; i++){
      for (j = 0; j < MAX_SECTORS; j++){
         if (i == daftarSector[fileIndex*MAX_SECTORS + j]){
            map[i] = 0x00;
         }   
      }
   }
   
   writeSector(map, MAP_SECTOR);
   writeSector(files, FILES_SECTOR);
   */
}

void putArgs (char curdir, char argc, char **argv) {
   char args[SECTOR_SIZE];
   int i, j, p;
   clear(args, SECTOR_SIZE);

   args[0] = curdir;
   args[1] = argc; 
   i = 0;
   j = 0;
   for (p = 2; p < ARGS_SECTOR && i < argc; ++p) {
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

void move(char *path1, char *path2, char curDir){
   /*
   int id1, id2, i, j, k;
   int isFolder1 = 0;
   int isFile2 = 0;
   int isToRoot = 0;
   int loop = 1;
   char files[512];
   char dirs[512];
   char namaBaru[MAX_FILENAME];

   readSector(files, FILES_SECTOR);
   readSector(dirs, DIRS_SECTOR);

   id1 = findIndexFile(path1, curDir);
   if (id1 == -1) {
      id1 = findIndexDirectory(path1, curDir);
      isFolder1 = 1;
   }

   if (path2[0] == '.' && path2[1] == '.'){
      id2 = findIndexFile(path1, curDir);
      id2 = dirs[files[id2 * 16] * 16];
      isToRoot = 1;
   }

   if (!isToRoot){
      id2 = findIndexDirectory(path2, curDir);
      if (id2 == -1){
         id2 = findIndexFile(path2, curDir);
         isFile2 = 1;
      }
   }

   if (id1 != -1 && id2 != -1){
      if (!isFolder1 && !isFile2){
         files [id1 * 16] = id2;
      }else if (isFolder1 && !isFile2) {
         dirs [id1 * 16] = id2;
      }
   }else if (!isFolder1 && isFile2 && id2 == -1 && id1 != -1){
      for (i = 1; i < 16; ++i){
         files[id1*16 + i] = path2[i - 1];
      }
   }

   writeSector(files, FILES_SECTOR);
   writeSector(dirs, DIRS_SECTOR);
   */
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
   for (p = 2; p < ARGS_SECTOR; ++p) {
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

void handleTimerInterrupt(int segment, int stackPointer) {
   struct PCB *currPCB;
   struct PCB *nextPCB;

   setKernelDataSegment();

   currPCB = getPCBOfSegment(segment);
   currPCB->stackPointer = stackPointer;

   if (currPCB->state != PAUSED) {
      currPCB->state = READY;
      addToReady(currPCB);
   }

   do {
      nextPCB = removeFromReady();
   }
   while ((nextPCB != NULL && (nextPCB->state == DEFUNCT || nextPCB->state == PAUSED)));

   if (nextPCB != NULL) {
      nextPCB->state = RUNNING;
      segment = nextPCB->segment;
      stackPointer = nextPCB->stackPointer;
      running = nextPCB;
   } else {
      running = &idleProc;
   }

   /*
   if (!currPCB->isReceivingInput){
      interrupt(0x21, 0x00, "NOT ISREC", 0, 0);
   }
   else {
      interrupt(0x21, 0x00, "ISREC", 0, 0);
   }*/
   restoreDataSegment();
   returnFromTimer(segment, stackPointer);
} 

void yieldControl () {
  interrupt(0x08, 0, 0, 0, 0);
}


void sleep () {
  setKernelDataSegment();
  running->state = PAUSED;
  restoreDataSegment();
  yieldControl();
}


void pauseProcess (int segment, int *result) {
  struct PCB *pcb;
  int res;

  setKernelDataSegment();

  pcb = getPCBOfSegment(segment);
  if (pcb != NULL && pcb->state != PAUSED) {
    pcb->state = PAUSED; res = SUCCESS;
  } else {
    res = NOT_FOUND;
  }

  restoreDataSegment();
  *result = res;
} 

void resumeProcess (int segment, int *result) {
  struct PCB *pcb;
  int res;
  setKernelDataSegment();

  pcb = getPCBOfSegment(segment);
  if (pcb != NULL && pcb->state == PAUSED) {
    pcb->state = READY;
    addToReady(pcb);
    res = SUCCESS;
  } else {
    res = NOT_FOUND;
  }

  restoreDataSegment();
  *result = res;
}

void killProcess (int segment, int *result) {
  struct PCB *pcb;
  int res;
  setKernelDataSegment();

  pcb = getPCBOfSegment(segment);
  if (pcb != NULL) {
    releaseMemorySegment(pcb->segment);
    releasePCB(pcb);
    res = SUCCESS;
  } else {
    res = NOT_FOUND;
  }

  restoreDataSegment();
  *result = res;
}

void showProcess() {
   int i;
   char processPID[9];
   char processState[9];
   char enter[3];
   struct PCB *process0, *process1, *process2, *process3;
   struct PCB *process4, *process5, *process6, *process7;
   for (i = 0 ; i < 9 ; i++) {
      processPID[i] = '\0';
      processState[i] = '\0';
   }


   i = 0;
   setKernelDataSegment();
   process0 = pcbPool + 0;
   process1 = pcbPool + 1;
   process2 = pcbPool + 2;
   process3 = pcbPool + 3;
   process4 = pcbPool + 4;
   process5 = pcbPool + 5;
   process6 = pcbPool + 6;
   process7 = pcbPool + 7;

   processPID[0] = convSegmentToPID(process0->segment);
   processState[0] = process0->state + '0';
   processPID[1] = convSegmentToPID(process1->segment);
   processState[1] = process1->state + '0';
   processPID[2] = convSegmentToPID(process2->segment);
   processState[2] = process2->state + '0';
   processPID[3] = convSegmentToPID(process3->segment);
   processState[3] = process3->state + '0';
   processPID[4] = convSegmentToPID(process4->segment);
   processState[4] = process4->state + '0';
   processPID[5] = convSegmentToPID(process5->segment);
   processState[5] = process5->state + '0';
   processPID[6] = convSegmentToPID(process6->segment);
   processState[6] = process6->state + '0';
   processPID[7] = convSegmentToPID(process7->segment);
   processState[7] = process7->state + '0';
   restoreDataSegment();

   enter[0] = '\n';
   enter[1] = '\r';
   enter[2] = '\0';

   interrupt(0x21,0x00, processPID, 0x00 ,0x00);
   interrupt(0x21,0x00, enter, 0x00 ,0x00);
   interrupt(0x21,0x00, processState, 0x00, 0x00);
}

char convSegmentToPID(int seg){
   if (seg == 0x2000)
      return '0';
   else if (seg == 0x3000)
      return '1';
   else if (seg == 0x4000)
      return '2';
   else if (seg == 0x5000)
      return '3';
   else if (seg == 0x6000)
      return '4';
   else if (seg == 0x7000)
      return '5';
   else if (seg == 0x8000)
      return '6';
   else if (seg == 0x9000)
      return '7';
   else if (seg == 0x00)
      return '9';
}



