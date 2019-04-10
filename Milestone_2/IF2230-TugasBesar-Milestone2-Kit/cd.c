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