windres -i icon.rc -o icon.o
gcc CamsEmulator.c icon.o -o CamsEmulator.exe -lcomdlg32 -lgdi32  -lwinmm
pause