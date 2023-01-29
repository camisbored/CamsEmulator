void load(HWND hwnd){
	
	resetState();

	OPENFILENAME ofn;
	TCHAR fileName[MAX_PATH];
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = fileName;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(fileName);
	ofn.lpstrFilter = "Cam Emulator Files (*.ce)\0*.ce\0All Files (*.*)\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	
	if (GetOpenFileName(&ofn)) {
	    HANDLE file;
	    DWORD bytesRead;
	
	    file = CreateFile(ofn.lpstrFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	    if (file == INVALID_HANDLE_VALUE) {
	        MessageBox(0,"Failed to Open","Error",1); 
	        return;
	    }
	
	    // Read codeSize bytes from the file into the buffer
	    if (!ReadFile(file, codeMemory, codeSize, &bytesRead, NULL)) {
	        MessageBox(0,"Error Reading File","Error",1); 
	        return;
	    }
	
	    CloseHandle(file);
	    MessageBox(0,ofn.lpstrFile,"File Loaded",1); 
	} else {
		sprintf(errorMessage, "The last error code was %d", GetLastError());
		MessageBox(0,errorMessage,"Unexpected Error",1); 
		return;
	}
    int i, j=0;
    for (j=0;j<255;j++){
        for (i=0;i<255;i++){
    				SetPixel(hdcMem, i, j, RGB(255, 255,255));
		}	
	}
	redraw(hwnd);
}


DWORD WINAPI run( LPVOID lpParam ) {
	HWND hwnd = (HWND) lpParam;
	isRunning = 1;
	while (isRunning){		
		readByte();
		runOp(hwnd);
	}
}

void step(HWND hwnd){
	isRunning = 1;
	readByte();
	runOp(hwnd);
}

struct DebugData {
    char* textValue;
    char* textType;
};

DWORD WINAPI showDebug(LPVOID lpParameter) {
	LPCSTR text = (LPCSTR) lpParameter;
	struct DebugData* params = (struct DebugData*)lpParameter;
    char* textValue = params->textValue;
    char* textType = params->textType;
    MessageBox(NULL, textValue, textType, MB_OK);
    return 0;
}

void debug(HWND hwnd){
	sprintf(debugMessage, 
	"r1: 0x%02X\nr2: 0x%02X\nr3: 0x%02X\nr4: 0x%02X\nr5: 0x%02X\nflag: 0x%02X\nip: 0x%02X\nsp: 0x%02X\nlastOp: 0x%02X\nlastRead: 0x%02X\nisRunning: 0x%02X\n",
	r1,r2,r3,r4,r5,flag,ip,sp,lastOp,lastRead,isRunning);
	
	struct DebugData params1;
    params1.textValue = debugMessage;
    params1.textType = "Current Register Values";
	CreateThread(NULL, 0, showDebug, &params1, 0, NULL);

	int i;
	char codeBuffer[256 * 4 + 1];
    sprintf(codeBuffer, "%02x ", codeMemory[0]);
    for (i = 1; i < 256; i++) {
        sprintf(codeBuffer + strlen(codeBuffer), "%02x ", codeMemory[i]);
		if (i==15 || (i+1)%16==0)
			sprintf(codeBuffer + strlen(codeBuffer), "\n");
    }

	struct DebugData params2;
    params2.textValue = codeBuffer;
    params2.textType = "Code Section";
	CreateThread(NULL, 0, showDebug, &params2, 0, NULL);

	char ramBuffer[256 * 4 + 1];
    sprintf(ramBuffer, "%02x ", ramMemory[0]);
    for (i = 1; i < 256; i++) {
        sprintf(ramBuffer + strlen(ramBuffer), "%02x ", ramMemory[i]);
		if (i==15 || (i+1)%16==0)
			sprintf(ramBuffer + strlen(ramBuffer), "\n");
    }
	
	struct DebugData params3;
    params3.textValue = ramBuffer;
    params3.textType = "Memory Section";
	CreateThread(NULL, 0, showDebug, &params3, 0, NULL);

	char stackBuffer[256 * 4 + 1];
    sprintf(stackBuffer, "%02x ", stack[0]);
    for (i = 1; i < 256; i++) {
        sprintf(stackBuffer + strlen(stackBuffer), "%02x ", stack[i]);
		if (i==15 || (i+1)%16==0)
			sprintf(stackBuffer + strlen(stackBuffer), "\n");
    }

	struct DebugData params4;
    params4.textValue = stackBuffer;
    params4.textType = "Stack";
	CreateThread(NULL, 0, showDebug, &params4, 0, NULL);

}