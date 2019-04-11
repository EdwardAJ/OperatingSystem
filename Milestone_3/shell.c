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

char buffer[512];
char path[512];

char runprog[512];
int lwfound;
int result;
void launchShell();
void changeDirectory(char *path, int *result, char parentIndex, int *curdir);
int div(int a, int b);
void copyString(char *src, char *dest);
int findIndexDirectory(char *name, int root);

int main () {
	int i;
	int input_switch = 0;
	int success = 0;
	//enableInterrupts();
	//Initialize path
	launchShell();
	//interrupt(0x21, 0x07, &success , 0x0, 0x0 );
}

void copyString(char *src, char *dest){
	int i = 0;
	interrupt(0x21, 0x0, "=-=-=-=\0", 0x0 , 0x0);
	interrupt(0x21, 0x0, "\n", 0x0 , 0x0);
	interrupt(0x21, 0x0, "\r", 0x0 , 0x0);
	interrupt(0x21, 0x0, src, 0x0 , 0x0);
	interrupt(0x21, 0x0, "\n", 0x0 , 0x0);
	interrupt(0x21, 0x0, "\r", 0x0 , 0x0);
	while (src[i] != '\0'){
		dest[i] = src[i];
		i++;
	}

	dest[i] = '\0';
}

void launchShell() {
	char PID;
	int idx;
	int jawal;
	int isEnd = 0;
	int result;
	char argc = 0;
	int i,j,k;
	int success = -1;
	char *argv[32];
	char buffertemp[32];
	char buffertemp1[32];
	char argvtemp[32][32];
	char curdir;

	//Get current directory
	interrupt(0x21, 0x21, &curdir, 0, 0);
	//interrupt(0x21, (AH << 8) | AL, BX, CX, DX);
	
	interrupt(0x21, 0x0 , "$" , 0x0 , 0x0);
	interrupt(0x21, 0x0 , " " , 0x0 , 0x0);
	//ReadString
	//Format : readString(BX, CX);
	//FORMAT INTERRUPT : interrupt(0x21, (AH << 8) | AL, BX, CX, DX);
	//readString(char* string, int disableProcessControls)
	interrupt(0x21, 0x1, buffer, 1, 0x0);
	//interrupt(0x21, 0x0 , buffer , 0x0 , 0x0);

	for (i = 0 ; i < 512 ; i++) {
		runprog[i] = '\0';
		//argvtemp[i] = '\0';
	}

	for (i = 0 ; i < 10 ; i++) {
		for (k = 0 ; k < 10 ; k++) {
			argvtemp[i][k] = '\0';
		}
	}
	

	for (i = 0 ; i < 32 ; i++) {
		buffertemp[i] = 0;
	}
	
	
	//implementasi dari execute program, sudah bisa memakai parameter
	if (buffer[0] == '.' && buffer[1] == '/') {
		i = 2;
		j = 0;

		//Baca execute program
		while (buffer[i] != '\0' && buffer[i] != ' ') {
			runprog[j] = buffer[i];
			i++;
			j++;
		}

		runprog[j] = '\0';
		
		//interrupt(0x21, 0x0 , "AKU CUPU" , 0x0 , 0x0);
		//Ada parameter
		if (buffer[i] == ' '){
			argc = 1;
			i++;
			idx = 0;
			j = 0;
						
			while (buffer[i] != '\0' && buffer[i] != ' ') {
				buffertemp[j] = buffer[i];
				i++;
				j++;
			}
			buffertemp[j] = '\0';
			jawal = j;

			argv[0] = buffertemp;
			
			for (k = 0 ; k < 32 ; k++) {
				buffertemp1[k] = '\0';
			}

			if(buffer[i] == ' ') {
				argc = 2;
				i++;
				j = 0;

				while (buffer[i] != '\0' && buffer[i] != ' ') {
					buffertemp1[j] = buffer[i];
					i++;
					j++;
					
				}

				buffertemp1[j] = '\0';
				
				k = 0;
				argv[1] = buffertemp1;
			}
				
		}	
		interrupt(0x21, 0x20, curdir, argc, argv);
		//interrupt(0x21, 0xFF << 8 | 0x06, "shell", &success, 0x00);
		interrupt(0x21, curdir << 8 | 0x6, runprog , &success, 0x00);
		
	}



	//implementasi dari cd
	else if (buffer[0] == 'c' && buffer[1] == 'd'){
		i = 3;
		j = 0;
		while (buffer[i] != '\0') {
			runprog[j] = buffer[i];
			i++;
			j++;
		}
		//interrupt(0x21,0x00, runprog, 0x00 ,0x00);
		//set curdir

		if (buffer[2] == '\0') {
			curdir = 0xFF;
			interrupt(0x21, 0x20, curdir, argc, argv);
		} 
		else {
			changeDirectory(runprog, &result, curdir, &curdir);
			interrupt(0x21, 0x20, curdir, argc, argv);
			if (result != 0) {
				interrupt(0x21,0x00, "Cannot find directory", 0x00 ,0x00);
				interrupt(0x21,0x00, "\n", 0x00 ,0x00);
				interrupt(0x21,0x00, "\r", 0x00 ,0x00);
			} else if (result == 0) {
				interrupt(0x21, 0x20, curdir, argc, argv);
			}
		}
		
		interrupt(0x21, 0x07, &result , 0x0, 0x0 );

		//execute changeDirectory program
	} 

	//panggil echo
	else if (buffer[0] == 'e' && buffer[1] == 'c' && buffer[2] == 'h' && buffer[3] == 'o') {
		argc = 1;
		i = 5;
		j = 0;
		while (buffer[i] != '\0') {
			argv[0][j] = buffer[i];
			i++;
			j++;
		}
		argv[0][j] = '\0';

		//setArgs
		interrupt(0x21, 0x20, curdir, argc, argv);

		//execute Program
		//interrupt(0x21, 0x31, 0x00, 0x00, 0x00);
		interrupt(0x21, 0xFF << 8 | 0x6, "echo" , &success, 0x00);


	}

	//panggil mkdir
	else if (buffer[0] == 'm' && buffer[1] == 'k' && buffer[2] == 'd' && buffer[3] == 'i' && buffer[4] == 'r') {
		argc = 1;
		i = 6;
		j = 0;
	
		while (buffer[i] != '\0') {
			argv[0][j] = buffer[i];
			i++;
			j++;
		}
	
		argv[0][j] = '\0';
		//setArgs
		interrupt(0x21, 0x20, curdir, argc, argv);

		//execute Program
		interrupt(0x21, 0xFF << 8 | 0x6, "mkdir" , 0x2000, &success);

	}

	//panggil ls
	else if (buffer[0] == 'l' && buffer[1] == 's') {
		argc = 0;
		//setArgs
		interrupt(0x21, 0x20, curdir, argc, argv);
		//execute Program
		interrupt(0x21, 0xFF << 8 | 0x6, "ls" , 0x2000, &success);
	}

	//panggil cat
	else if (buffer[0] == 'c' && buffer[1] == 'a' && buffer[2] == 't') {
		//jumlah argumen  = 1 atau 2
		
		i = 4;
		argc = 1;
		j = 0;
	
		while (buffer[i] != '\0' && buffer[i] != ' ') {
			buffertemp[j] = buffer[i];
			i++;
			j++;
		}
		buffertemp[j] = '\0';
		//buffertemp1[j] = '\0';
		argv[0] = buffertemp;

		//Ada argumen kedua
		if (buffer[i] == ' '){
			i++;
			j = 0;

			for (k = 0 ; k < 10 ; k++) {
				buffertemp1[k] = '\0';
			}

			
				argc = 2;
				while (buffer[i] != '\0') {
					buffertemp1[j] = buffer[i];
					i++;
					j++;
				}
				buffertemp1[j] = '\0';

			if (buffertemp1[0] == '-' && buffertemp1[1] == 'w') {
				argv[1] = buffertemp1;
			}
		}
		//interrupt(0x21, 0x00, argv[0], 0x00 ,0x00);
		//interrupt(0x21, 0x00, "\n",0x00, 0x00);
		//interrupt(0x21, 0x00, "\r",0x00, 0x00);
		
		//setArgs
		interrupt(0x21, 0x20, curdir, argc, argv);

		//interrupt(0x21, 0x00, "", 0x00 ,0x00);
		//execute Program
		interrupt(0x21, 0xFF << 8 | 0x6, "cat" , 0x2000, &success);

	} else if (buffer[0] == 'r' && buffer[1] == 'm') {
		argc = 1;
		i = 3;

		for (k = 0 ; k < 32 ; k++) {
			buffertemp[k] = '\0';
		}
		if (buffer[i] == '-' && buffer[i + 1] == 'r'){
			j = 0;
			i = 6;

			for (k = 0 ; k < 32 ; k++) {
				buffertemp[k] = '\0';
			}

			while (buffer[i] != '\0') {
				buffertemp[j] = buffer[i];
				i++;
				j++;
			}
			buffertemp[j] = '\0';
			//interrupt(0x21, 0x00, buffertemp, 0x00 , 0x00);
			//argv[0][j] = '\0';
			interrupt(0x21, (curdir << 8) | 0x0A, buffertemp, &result, 0x00);
		}else {
			j = 0;

			while (buffer[i] != '\0') {
				buffertemp[j] = buffer[i];
				i++;
				j++;
			}
			buffertemp[j] = '\0';

			interrupt(0x21, (curdir << 8) | 0x09, buffertemp, &result, 0x00);
		}
		if (result != 0) {
			//interrupt(0x21, 0x07, &result , 0x0, 0x0 );
   			interrupt(0x21, 0x00, "Not found" , 0x00 ,0x00);
      		interrupt(0x21, 0x00, "\n" , 0x00 ,0x00);
      		interrupt(0x21, 0x00, "\r" , 0x00 ,0x00);
   		}

   		//TERMINATE PROGRAM
		interrupt(0x21, 0x07, &result , 0x0, 0x0 );
		//interrupt(0x21, 0x20, curdir, argc, argv);
		//interrupt(0x21, 0xFF << 8 | 0x6, "rm" , 0x2000, &success);
	}else if (buffer[0] == 'c' && buffer[1] == 'l' && buffer[2] == 'r'){
		interrupt(0x21, 0x24, 0x0 , 0x0, 0x0);
		interrupt(0x21, 0x07, &success , 0x0, 0x0);
	}else if (buffer[0] == 'm' && buffer[1] == 'v'){
		i = 3;
		j = 0;

		while (buffer[i] != '\0' && buffer[i] != ' '){
			buffertemp[j] = buffer[i];
			i++;
			j++;
		}

		j = 0;
		i++;

		while (buffer[i] != '\0' && buffer[i] != ' '){
			buffertemp1[j] = buffer[i];
			i++;
			j++;
		}

		interrupt(0x21, 0x25, buffertemp, buffertemp1, curdir);
		interrupt(0x21, 0x07, &success , 0x0, 0x0);
	
	//PAUSE
	} else if (buffer[0] == 'p' && buffer[1] == 'a' && buffer[2] == 'u' && buffer[3] == 's' && buffer[4] == 'e') {
		argc = 1;
		i = 6;
		j = 0;
		while (buffer[i] != '\0') {
			argv[0][j] = buffer[i];
			PID = buffer[i];
			i++;
			j++;
		}
		argv[0][j] = '\0';

		//setArgs
		interrupt(0x21, 0x20, curdir, argc, argv);

		//FORMAT INTERRUPT : interrupt(0x21, (AH << 8) | AL, BX, CX, DX);
		interrupt(0x21, 0x32, convPIDtoSegment(PID), &success, 0x00);

	//RESUME
	}else if (buffer[0] == 'r' && buffer[1] == 'e' && buffer[2] == 's' && buffer[3] == 'u' && buffer[4] == 'm' && buffer[5] == 'e') {
		argc = 1;
		i = 7;
		j = 0;
		while (buffer[i] != '\0') {
			argv[0][j] = buffer[i];
			PID = buffer[i];
			i++;
			j++;
		}
		argv[0][j] = '\0';

		//setArgs
		interrupt(0x21, 0x20, curdir, argc, argv);

		//FORMAT INTERRUPT : interrupt(0x21, (AH << 8) | AL, BX, CX, DX);
		interrupt(0x21, 0x33, convPIDtoSegment(PID), &success, 0x00);

	//KILL
	} else if (buffer[0] == 'k' && buffer[1] == 'i' && buffer[2] == 'l' && buffer[3] == 'l') {
		argc = 1;
		i = 5;
		j = 0;
		while (buffer[i] != '\0') {
			argv[0][j] = buffer[i];
			PID = buffer[i];
			i++;
			j++;
		}
		argv[0][j] = '\0';

		//setArgs
		interrupt(0x21, 0x20, curdir, argc, argv);

		//FORMAT INTERRUPT : interrupt(0x21, (AH << 8) | AL, BX, CX, DX);
		interrupt(0x21, 0x34, convPIDtoSegment(PID), &success, 0x00);

	}else {
		interrupt(0x21, 0x0 , "Tidak ada command yang Anda maksud" , 0x0 , 0x0);
		interrupt(0x21, 0x0 , "\n" , 0x0 , 0x0);
		interrupt(0x21, 0x0 , "\r" , 0x0 , 0x0);
		//interrupt(0x21, 0x07, &success , 0x0, 0x0);
	}

	/*
	for (i = 0 ; i < 512 ; i++) {
		for (j = 0 ; j < 512 ; j++) {
			argv[i][j] = '\0';
		}
	} 
	*/
	//Clear argv
	/* 
	if (success < 0) {
		//interrupt(0x21, 0x0 , "Tidak ada" , 0x0 , 0x0);
		interrupt(0x21, 0x0 , "\n" , 0x0 , 0x0);
		interrupt(0x21, 0x0 , "\r" , 0x0 , 0x0);
	}
	*/
	
	//EXECUTE PROGRAM ULANG
	interrupt(0x21, curdir << 8 | 0x06, "shell", &success, 0x00);
}

void changeDirectory(char *path, int *result, char parentIndex, int *curdir) {
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
   interrupt(0x21, 0x02, dirs, DIRS_SECTOR, 0x00);
   
    i = 0;
    if (path[0] == '.' && path[1] == '.') {
    	if (parentIndex != 0xFF)
    		*curdir = dirs[parentIndex*16];
    	*result = 0;
    } else {
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
      //Cari direktori trakhir.
      fileIndex = findIndexDirectory(name, currentRoot);
      *result = INSUFFICIENT_DIR_ENTRIES;
      //Kalau belum ada
      if (fileIndex == -1){
         *result = -1;
         return;
      //Kalau ada.
      } else {
      	*result = 0;
      	*curdir = fileIndex;
      }
    } 
   }


int findIndexDirectory(char *name, int root){
   int i, j, hsl;
   char dirs[SECTOR_SIZE];
   char elemen;
   interrupt(0x21, 0x02, dirs, DIRS_SECTOR, 0x00);
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

int convPIDtoSegment(char PID){
	if (PID == '0')
		return 0x2000;
	else if (PID == '1')
		return 0x3000;
	else if (PID == '2')
		return 0x4000;
	else if (PID == '3')
		return 0x5000;
	else if (PID == '4')
		return 0x6000;
	else if (PID == '5')
		return 0x7000;
	else if (PID == '6')
		return 0x8000;
	else if (PID == '7')
		return 0x9000;
}
