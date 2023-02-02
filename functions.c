unsigned char readByte(){
	lastRead = codeMemory[ip++];
	return lastRead;
}

void redraw(HWND hwnd){
	RECT rect;
	GetClientRect(hwnd, &rect);
	InvalidateRect(hwnd,&rect,TRUE);
}

DWORD WINAPI getKey( LPVOID lpParam ) {
	   char i;
	   while(1){
	    for(i = 8; i <= 190; i++) {
	        if (GetAsyncKeyState(i) == -32767){
	            	lastPressed = i;
					return;
			}
	    }
	}
}

void keyWrapper(){
		HANDLE hThread = CreateThread(NULL, 0, getKey, NULL, 0, NULL);
		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread);
}

DWORD WINAPI checkIfKeyPressed( LPVOID lpParam ){
	char i;
		for(i = 8; i <= 190; i++) {
	        if (GetAsyncKeyState(i) == -32767){
				lastPressed = i;
				keyPressedForCheck = 1;
				return;
			}
			keyPressedForCheck = 0;
			return;
	    }
}

DWORD WINAPI beepAsync( LPVOID lpParam ){
	Beep(2500, 50);
	return;
}

void beepAsyncWrapper(){
		HANDLE hThread = CreateThread(NULL, 0, beepAsync, NULL, 0, NULL);
		WaitForSingleObject(hThread, IGNORE);
		CloseHandle(hThread);
}

void checkKeyWrapper(){
		HANDLE hThread = CreateThread(NULL, 0, checkIfKeyPressed, NULL, 0, NULL);
		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread);
}

push(unsigned char val){
	int i;
	for (i=memorySize;i>0;i--)
		stack[i]= stack[i-1];
    stack[0]=val;
    sp = sp+1;
}

pop(unsigned char val){
	int i;
	if (val == 0x01)
		r1 = stack[0];
	else if (val == 0x02)
		r2 = stack[0];
	else if (val == 0x03)
		r3 = stack[0];
	else if (val == 0x04)
		r4 = stack[0];
	else if (val == 0x05)
		r5 = stack[0];
	for (i=1;i<memorySize;i++)
    	stack[i-1]=stack[i];
    stack[sp]=0;
    sp = sp-1;  
}

void resetState(){
	r1 = 0;
	r2 = 0;
	r3 = 0;
	r4 = 0;
	r5 = 0;
    flag = 0;
	ip = 0;
	sp = 0;
	lastPressed = 0x00;
	isRunning = 0x00;
}

void drawW(unsigned char x, unsigned char y){
            MoveToEx(hdcMem, x-20, y-15, NULL);//W
            LineTo(hdcMem, x-10, y+15);
            LineTo(hdcMem, x, y-10);
            LineTo(hdcMem, x+10, y+15);
            LineTo(hdcMem, x+20, y-15);
}

void drawWelcomeMessage(){
         
            drawW(30, 25);

            MoveToEx(hdcMem, 60, 10, NULL);//E
            LineTo(hdcMem, 60, 40);
            LineTo(hdcMem, 75, 40);
            MoveToEx(hdcMem, 60, 25, NULL);
            LineTo(hdcMem, 75, 25);
            MoveToEx(hdcMem, 60, 10, NULL);
            LineTo(hdcMem, 75, 10);
            
            MoveToEx(hdcMem, 90, 10, NULL);//L
            LineTo(hdcMem, 90, 40);
            LineTo(hdcMem, 105, 40);
            
            MoveToEx(hdcMem, 115, 10, NULL);//C
            LineTo(hdcMem, 135, 10);
            MoveToEx(hdcMem, 115, 10, NULL);
            LineTo(hdcMem, 115, 40);
            LineTo(hdcMem, 135, 40);
            
            MoveToEx(hdcMem, 145, 10, NULL);//O
            LineTo(hdcMem, 165, 10);
            LineTo(hdcMem, 165, 40);
            LineTo(hdcMem, 145, 40);
            LineTo(hdcMem, 145, 10);
            
            MoveToEx(hdcMem, 175, 40, NULL);//M
            LineTo(hdcMem, 185, 10);
            LineTo(hdcMem, 195, 35);
            LineTo(hdcMem, 205, 10);
            LineTo(hdcMem, 215, 40);

            MoveToEx(hdcMem, 222, 10, NULL);//E
            LineTo(hdcMem, 222, 40);
            LineTo(hdcMem, 237, 40);
            MoveToEx(hdcMem, 222, 25, NULL);
            LineTo(hdcMem, 237, 25);
            MoveToEx(hdcMem, 222, 10, NULL);
            LineTo(hdcMem, 237, 10);
            
            MoveToEx(hdcMem, 245, 10, NULL);//!
            LineTo(hdcMem, 245, 32);
            MoveToEx(hdcMem, 245, 38, NULL);
            LineTo(hdcMem, 245, 40);
}