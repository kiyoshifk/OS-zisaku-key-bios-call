#!/bin/sh
#!/bin/sh -x

rm boot_loader.bin
rm boot_loader.lst
rm key_mouse.o
rm key_mouse.lst
rm os_start.o
rm os_start.lst
rm asm_lib.o
rm asm_lib.lst

rm gonbe_rear.elf
rm gonbe_rear.srec
rm gonbe_rear.img
rm gonbe.img
rm gonbe_rear.map

rm gonbe_main.o
rm gonbe_main.s
rm gonbe_main_x.s
rm gonbe_main.lst

rm font_lib.o
rm font_lib.s
rm font_lib_x.s
#rm font_lib.lst

rm sjis.o
rm sjis.s
rm sjis_x.s
rm sjis.lst

rm lib.o
rm lib.s
rm lib_x.s
rm lib.lst

nasm -f bin -o boot_loader.bin -l boot_loader.lst boot_loader.nasm
nasm -f elf32 -o key_mouse.o -l key_mouse.lst key_mouse.nasm
nasm -f elf32 -o os_start.o -l os_start.lst os_start.nasm
nasm -f elf32 -o asm_lib.o -l asm_lib.lst asm_lib.nasm

gcc -x c -c -O0 -Werror -Wall -o gonbe_main.s -S gonbe_main.c 
gcc -x c -c -O0 -Werror -Wall -o font_lib.s -S font_lib.c 
gcc -x c -c -O0 -Werror -Wall -o sjis.s -S sjis.c 
gcc -x c -c -O0 -Werror -Wall -o lib.s -S lib.c 

C:/work/OS自作/win-build/tools/del_.rdata/del_.rdata gonbe_main.s gonbe_main_x.s
C:/work/OS自作/win-build/tools/del_.rdata/del_.rdata font_lib.s font_lib_x.s
C:/work/OS自作/win-build/tools/del_.rdata/del_.rdata sjis.s sjis_x.s
C:/work/OS自作/win-build/tools/del_.rdata/del_.rdata lib.s lib_x.s

as -c -Wall -o gonbe_main.o gonbe_main_x.s > gonbe_main.lst
as -c -Wall -o font_lib.o font_lib_x.s > /dev/null
as -c -Wall -o sjis.o sjis_x.s > sjis.lst
as -c -Wall -o lib.o lib_x.s > lib.lst

gcc -o gonbe_rear.elf os_start.o key_mouse.o gonbe_main.o lib.o sjis.o asm_lib.o font_lib.o -nostdlib -Tlinker.ld -Wl,-Map=gonbe_rear.map
objcopy -O srec gonbe_rear.elf gonbe_rear.srec
C:/work/OS自作/win-build/tools/srec_to_img/srec_to_img.exe 0x8400 0x00280000 gonbe_rear.srec gonbe_rear.img	# gonbe_rear.img は 0x7e00番地からの生データ
#C:/work/OS自作/win-build/tools/srec_to_img/srec_to_img-0x00280000.exe gonbe_rear.srec gonbe_rear.img	# gonbe_rear.img は 0x8000番地からの生データ
cat boot_loader.bin gonbe_rear.img > gonbe.img


