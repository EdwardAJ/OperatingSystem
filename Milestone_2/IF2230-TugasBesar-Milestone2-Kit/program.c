#define MAX_BYTE 256
#define SECTOR_SIZE 512
#define MAX_FILES 16
#define MAX_FILENAME 12
#define MAX_SECTORS 20
#define DIR_ENTRY_LENGTH 32
#define MAP_SECTOR 1
#define DIR_SECTOR 2
#define TRUE 1
#define FALSE 0
#define INSUFFICIENT_SECTORS 0
#define NOT_FOUND -1
#define INSUFFICIENT_DIR_ENTRIES -1
#define EMPTY 0x00
#define USED 0xFF

void printString(char *string);
void readString(char *string);
int mod(int a, int b);
int div(int a, int b);
int charToInt(char c);
char* intToChar(int n);

int main() {


   int string[256];
   char buffer[MAX_BYTE];
   int num1,num2,num3;
   int len;
   char op;
   char temp[MAX_BYTE];
   char final[MAX_BYTE];
   int i,a;
   int input_switch;

   input_switch = 0;

   //KELEMAHAN: Tidak mengatasi kasus angka yang dihasilkan adalah float / double
   //Jika input tidak sesuai format maka menghasilkan 0 saja.
   printString("Masukkan dua ANGKA BULAT dan OPERATOR tanpa spasi!\n");
   printString("Contoh: '4+2' atau '10*4' atau '88-10' atau '8%2' atau '4/2'\n");
   readString(buffer);
   input_switch = 1;
   while (buffer[0] > '0' && buffer[0] < '9'){
      num1 = 0; num2 = 0; num3 = 0;


      //DAPATKAN num1 dari buffer
      i = 0;
      while (i < MAX_BYTE && ((buffer[i] != '*') && (buffer[i] != '/') && (buffer[i] != '+') && (buffer[i] != '%') && (buffer[i] != '-'))) {
         num1 = num1*10 + charToInt(buffer[i]);
         i++;
      }

      //DAPATKAN operator dari buffer
      op = buffer[i];
      i = i + 1;

      //DAPATKAN num2 dari buffer
      while (i < MAX_BYTE && ((buffer[i] != '\0'))) {
         num2 = num2*10 + charToInt(buffer[i]);
         i++;
      }

      //Cek conditional yang ada, operasikan num1 dan num2 sesuai dengan operator
      if (op == '+') {
         num3 = num1 + num2;
      } else if (op == '-') {
         num3 = num1 - num2;
      } else if (op == '*') {
         num3 = num1 * num2;
      } else if (op == '/') {
         num3 = div(num1,num2);
      } else if (op == '%') {
         num3 = mod(num1,num2);
      }

      i = 0; a = 0;

      //CEK apakah num3 != 0 karena diberi perlakuan berbeda
      if (num3 != 0) {
         if (num3 < 0) {
            num3 = -1*num3;
            final[a] = '-'; //awal temp diberi -
            a++;
         } 

         //Untuk menyalin isi dari num3 ke temp
         while (num3 > 0) {
            temp[i] = '0' + mod(num3,10);
            num3 = div(num3,10);
            i++;
         }
         //NULL TERMINATED 
         temp[i] = '\0';
      
         for (len = i-1 ; len >= 0 ; len--) { //iterasi dimulai bukan dari len = 1 (karena nanti jadi null '\0')
            final[a] = temp[len]; //salin isi temp ke dalam final.
            a++;
         }
         final[a] = '\0';

      } else if (num3 == 0) { //KALAU num3 = 0 langsung saja dimasukkan ke final.
         final[a] = '0';
         a++;
         final[a] = '\0';
      }
      printString("Hasil:");
      printString(final);
      input_switch = 0;
      
      while (input_switch == 0){
         readString(buffer);
         input_switch = 1;
      }
   }
   printString("End of Calculator");
   while (1);
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

int charToInt(char c) {
   return (c - '0');
}


