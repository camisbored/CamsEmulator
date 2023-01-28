#include <Windows.h>
#include <stdio.h>
#include <conio.h>

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
unsigned char isRunning = 0x00;

unsigned char readByte(){
	lastRead = codeMemory[ip++];
	return lastRead;
}

void refresh(HWND hwnd){
	RECT rect;
	GetClientRect(hwnd, &rect);
	InvalidateRect(hwnd,&rect,TRUE);
}

DWORD WINAPI gettt( LPVOID lpParam ) {
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

void runOp(HWND hwnd){
	lastOp = lastRead;
	switch(lastRead){
		case 0x00:
			isRunning = 0;
			break;
		case 0x01:
			r1 = readByte();
			break;
		case 0x02:
			r2 = readByte();
			break;
		case 0x03:
			r3 = readByte();
			break;
		case 0x04:
			r4 = readByte();
			break;
		case 0x05:
			r5 = readByte();
			break;
		case 0x06:
			r1 = ramMemory[readByte()];
			break;
		case 0x07:
			r2 = ramMemory[readByte()];
			break;
		case 0x08:
			r3 = ramMemory[readByte()];
			break;
		case 0x09:
			r4 = ramMemory[readByte()];
			break;
		case 0x0A:
			r5 = ramMemory[readByte()];
			break;
		case 0x0B:
			ramMemory[readByte()]=r1;
			break;
		case 0x0C:
			ramMemory[readByte()]=r2;
			break;
		case 0x0D:
			ramMemory[readByte()]=r3;
			break;
		case 0x0E:
			ramMemory[readByte()]=r4;
			break;
		case 0x0F:
			ramMemory[readByte()]=r5;
			break;
		case 0x10:
			r1 = r1+readByte();
			break;
		case 0x11:
			r2 = r2+readByte();
			break;
		case 0x12:
			r3 = r3+readByte();
			break;
		case 0x13:
			r4 = r4+readByte();
			break;
		case 0x14:
			r5 = r5+readByte();
			break;
		case 0x15:
			r1 = r1-readByte();
			break;
		case 0x16:
			r2 = r2-readByte();
			break;
		case 0x17:
			r3 = r3-readByte();
			break;
		case 0x18:
			r4 = r4-readByte();
			break;
		case 0x19:
			r5 = r5-readByte();
			break;
		case 0x1A:
			r1 = r1*readByte();
			break;
		case 0x1B:
			r2 = r2*readByte();
			break;
		case 0x1C:
			r2 = r2*readByte();
			break;
		case 0x1D:
			r3 = r3*readByte();
			break;
		case 0x1E:
			r4 = r4*readByte();
			break;
		case 0x1F:
			r5 = r5*readByte();
			break;
		case 0x20:
			r1 = r1/readByte();
			break;
		case 0x21:
			r2 = r2/readByte();
			break;
		case 0x22:
			r3 = r3/readByte();
			break;
		case 0x23:
			r4 = r4/readByte();
			break;
		case 0x24:
			r5 = r5/readByte();
			break;
		case 0x25:
			push(r1);
			break;
		case 0x26:
			push(r2);
			break;
		case 0x27:
			push(r3);
			break;
		case 0x28:
			push(r4);
			break;
		case 0x29:
			push(r5);
			break;
		case 0x2A:
			pop(0x01);
			break;
		case 0x2B:
			pop(0x02);
			break;
		case 0x2C:
			pop(0x03);
			break;
		case 0x2D:
			pop(0x04);
			break;
		case 0x2E:
			pop(0x05);
			break;
		case 0x2F:

			break;

		default:
			isRunning = 0;
			sprintf(errorMessage, "Illegal instruction detected: 0x%2X", lastRead);
			MessageBox(0,errorMessage,"Runtime Error",MB_OK); 	
	}	
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
	    if (ReadFile(file, codeMemory, codeSize, &bytesRead, NULL)) {
	    		int i;
				for (i = 0; i < codeSize; i++){
					printf("%02X ", codeMemory[i]);
			}
			printf("\n\n\n");
	    }
	    else {
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
	refresh(hwnd);
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

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    	case WM_COMMAND: {
            int menuId = LOWORD(wParam);
            switch (menuId) {
                case 1:
                    load(hwnd);
                    break;
                case 2:{
			        HANDLE hThread = CreateThread(NULL, 0, run, hwnd, 0, NULL);
					WaitForSingleObject(hThread, IGNORE);
					CloseHandle(hThread);
                }    
                    break;
                case 3:
                    step(hwnd);
                    break;
                case 4:
                    debug(hwnd);
                    break;
                default:
                    break;
            }
            return 0;
        }
        case WM_CREATE: {
            // Create a framebuffer
            hdcMem = CreateCompatibleDC(NULL);
            bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
            bmi.bmiHeader.biWidth = 255;
            bmi.bmiHeader.biHeight = -255;
            bmi.bmiHeader.biPlanes = 1;
            bmi.bmiHeader.biBitCount = 24;
            bmi.bmiHeader.biCompression = BI_RGB;
            hBitmap = CreateDIBSection(hdcMem, &bmi, DIB_RGB_COLORS, NULL, NULL, 0);

            SelectObject(hdcMem, hBitmap);

            // Clear the framebuffer to black
            RECT rect = { 0, 0, 255, 255 };
            FillRect(hdcMem, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH));

            HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
            SelectObject(hdcMem,hPen);
            
            MoveToEx(hdcMem, 10, 10, NULL);//W
            LineTo(hdcMem, 20, 40);
            LineTo(hdcMem, 30, 15);
            LineTo(hdcMem, 40, 40);
            LineTo(hdcMem, 50, 10);
            
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

            return 0;
        }
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Draw the framebuffer to the window
            BitBlt(hdc, 0, 0, 255, 255, hdcMem, 0, 0, SRCCOPY);

            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_DESTROY: {
            // Clean up the framebuffer
            DeleteDC(hdcMem);
            DeleteObject(hBitmap);

            PostQuitMessage(0);
            return 0;
        }
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	HWND hWnd = GetConsoleWindow();
    ShowWindow( hWnd, SW_MINIMIZE );
    ShowWindow( hWnd, SW_HIDE );
	
    WNDCLASSEX wc;
    ZeroMemory(&wc, sizeof(WNDCLASSEX));
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszClassName = "MyWindowClass";
    RegisterClassEx(&wc);

    RECT rc = { 0, 0, 255, 255 };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

    HWND hwnd = CreateWindowEx(
        0, "MyWindowClass", "Cams Emulator", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top,
        NULL, NULL, hInstance, NULL);
    
    HMENU hMenu = CreateMenu();
    AppendMenu(hMenu, MF_STRING, 1, "Load");
    AppendMenu(hMenu, MF_STRING, 2, "Run");
    AppendMenu(hMenu, MF_STRING, 3, "Step");
    AppendMenu(hMenu, MF_STRING, 4, "Debug");
    SetMenu(hwnd, hMenu);


    RECT desktop;
    GetWindowRect(GetDesktopWindow(), &desktop);
    int screenWidth = desktop.right;
    int screenHeight = desktop.bottom;
    
    RECT windowRect;
    GetWindowRect(hwnd, &windowRect);
    int windowWidth = windowRect.right - windowRect.left;
    int windowHeight = windowRect.bottom - windowRect.top;
    
    
    int x = (screenWidth-windowWidth)/2;
    int y = (screenHeight-windowHeight)/2;
    SetWindowPos(hwnd, HWND_TOPMOST, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	HICON hicon = LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON1));
	SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM) hicon);

    ShowWindow(hwnd, nCmdShow);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}
