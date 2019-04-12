as86 lib.asm -o lib_asm.o
bcc -ansi -c -o shell.o shell.c
ld86 -o shell -d shell.o lib_asm.o