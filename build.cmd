gcc -O2 -c sl.c
gcc -O2 -c slhook.c
link386 /exepack:2 /bas:0x10000 sl, sl, sl, os2 \gnu\lib\static\c, sl
link386 /exepack:2 slhook, slhook.dll, slhook, os2, slhook
