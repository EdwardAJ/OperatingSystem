as86 lib.asm -o lib_asm.o
bcc -ansi -c -o program.o program.c
ld86 -o program -d program.o lib_asm.o
