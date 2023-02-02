#include <Windows.h>
#include <stdio.h>

#define IDI_ICON1 101

#define codeSize 0xFF
#define memorySize 0xFF
#define stackSize 0xFF
#define frameBufferSize 0xFFFF
#define bytesPerPixel 3

HBITMAP hBitmap;
HDC hdcMem;
BITMAPINFO bmi;

char errorMessage[50];
char debugMessage[250];
char bufferMessage[0xFF*4]={0};

unsigned char codeMemory[codeSize]={0};
unsigned char ramMemory[memorySize]={0};
unsigned char stack[stackSize]={0};
unsigned char frameBuffer[frameBufferSize*bytesPerPixel]={0};

unsigned char lastRead=0;
unsigned char lastOp=0;

unsigned char r1 = 0;
unsigned char r2 = 0;
unsigned char r3 = 0;
unsigned char r4 = 0;
unsigned char r5 = 0;
unsigned char flag = 0;
unsigned char ip = 0;
unsigned char sp = 0;

char lastPressed = 0x00;
char keyPressedForCheck = 0x00;
unsigned char isRunning = 0x00;

#include "functions.c"
#include "opCodes.c"
#include "menus.c"