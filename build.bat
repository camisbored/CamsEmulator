@echo off
setlocal enabledelayedexpansion

rem this script has three parts, it will compile the c code using gcc to make the emulator executable
rem it will then compile the assembler, assuming javac is installed correctly
rem it will then use the java assembler to make each of the roms
rem requirements are java8 min jdk and gcc w/ win32 api. if one phase of the script fails, the other ones 
rem will still attempt (etc you can compile the executable and not the java and reassembly part)

windres -i res/icon.rc -o res/icon.o
gcc src/emulator/CamsEmulator.c res/icon.o -o bin/emulator/CamsEmulator.exe -lcomdlg32 -lgdi32  -lwinmm
echo If no error messges (there will be warnings) then emulator built.

echo Beginning assembler compilation...
pause
javac -d bin/assembler src/assembler/CEAssembler.java
echo If no error messges then assembler built.

echo Beginning rom assembly...
pause
set "sourceFolder=src\roms"
set "destinationFolder=bin\roms"
for %%F in (%sourceFolder%\*.asm) do (
    set "fileName=%%~nF"
    java -cp bin/assembler CEAssembler "%sourceFolder%\%%~nxF" "%destinationFolder%\!fileName!.ce"
)
echo If no errors, all .asm files have been assembled.
pause