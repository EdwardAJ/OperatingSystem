#define MAX_BYTE 256
#define SECTOR_SIZE 512
#define MAX_FILES 16
#define MAX_FILENAME 12
#define MAX_SECTORS 20
#define DIR_ENTRY_LENGTH 32
#define MAP_SECTOR 0x100
#define DIRS_SECTOR 0x101
#define FILES_SECTOR 0x102
#define SECTORS_SECTOR 0x103
#define TRUE 1
#define FALSE 0
#define INSUFFICIENT_SECTORS 0
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

int findIndexDirectory(char *name, char *root){
   char dirs[SECTOR_SIZE];
   readSector(dirs, DIRS_SECTOR);

   for (int i = 0; i < MAX_FILES; i++){
      
   }
}

//implementasi readFile (ISSUE)
void readFile(char *buffer, char *path, int *result, char parentIndex){
   //kamus
   char dirs[SECTOR_SIZE];  //array dir akan berisi sektor dir setelah pemanggilan readSector
   int i, j, k;
   int found = 0;          //menandakan apakah filename sudah ketemu

   //algoritma
   *result = 0;
   readSector(dirs, DIRS_SECTOR);
   //printString("MASUK    PAK EKO\n");

   //cek tiap 12 byte untuk filename
   for (i = 0; i < SECTOR_SIZE; i = i + DIR_ENTRY_LENGTH){
      j = 0;
      while (dirs[i + j] == filename[j] && j <=7){
         j = j + 1;
      }

      if (j == 8){
         found = 1;
         break;
      }
   }

   //mulai pengisian buffer
   if (found == 1){
      k = 0;

      //udah di test dir[j] itu yang kebaca 13, 14, 15 (sesuai dengan keyproc)
      for (j = i + MAX_FILENAME;(k < MAX_SECTORS) && (dir[j] != 0); j = j + 1){
         readSector(buffer + (k * SECTOR_SIZE), dir[j]);
         //interrupt(0x10, 0xE00 + dir[j]+48, 0, 0, 0);
         //printString(buffer);

         k = k + 1;
      }
      *success = 1;
      //printString("KELUAR PAK EKO\n");
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
void writeFile(char *buffer, char* filename, int* sectors){
   char map[SECTOR_SIZE];
   char dir[SECTOR_SIZE];
   char sectorBuffer[SECTOR_SIZE];
   int dirIndex;

   readSector(map, MAP_SECTOR);
   readSector(dir, DIR_SECTOR);

   for (dirIndex = 0; dirIndex < MAX_FILES; ++dirIndex) {
      if (dir[dirIndex * DIR_ENTRY_LENGTH] == '\0') {
         break;
      }
   }

   if (dirIndex < MAX_FILES) {
      int i, j, sectorCount;
      for (i = 0, sectorCount = 0; i < MAX_BYTE && sectorCount < *sectors; ++i) {
         if (map[i] == EMPTY) {
            ++sectorCount;
         }
      }

      if (sectorCount < *sectors) {
         *sectors = INSUFFICIENT_SECTORS;
         return;
      } else {
         clear(dir + dirIndex * DIR_ENTRY_LENGTH, DIR_ENTRY_LENGTH);
         for (i = 0; i < MAX_FILENAME; ++i) {
            if (filename[i] != '\0') {
               dir[dirIndex * DIR_ENTRY_LENGTH + i] = filename[i];
            } else {
               break;
            }
         }

         for (i = 0, sectorCount = 0; i < MAX_BYTE && sectorCount < *sectors; ++i) {
            if (map[i] == EMPTY) {
               map[i] = USED;
               dir[dirIndex * DIR_ENTRY_LENGTH + MAX_FILENAME + sectorCount] = i;
               clear(sectorBuffer, SECTOR_SIZE);
               for (j = 0; j < SECTOR_SIZE; ++j) {
                  sectorBuffer[j] = buffer[sectorCount * SECTOR_SIZE + j];
               }
               writeSector(sectorBuffer, i);
               ++sectorCount;
            }
         }
      }
   } else { 
      *sectors = INSUFFICIENT_DIR_ENTRIES;
      return;
   }

   writeSector(map, MAP_SECTOR);
   writeSector(dir, DIR_SECTOR);

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
}


