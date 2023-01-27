#include <Windows.h>
#include <stdio.h>
#include <conio.h>

#define IDI_ICON1 101

#define codeSize 0xFF
#define memorySize 0xFF
#define frameBufferSize 0xFFFF
#define bytesPerPixel 3

HBITMAP hBitmap;
HDC hdcMem;
BITMAPINFO bmi;

char errorMessage[50];
char debugMessage[250];

unsigned char codeMemory[codeSize]={0};
unsigned char ramMemory[memorySize]={0};
unsigned char frameBuffer[frameBufferSize*bytesPerPixel]={0};

unsigned char lastRead=-1;

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

//DWORD WINAPI refresh(LPVOID lpParam){
//	HWND hwnd = (HWND) lpParam;
//	RECT rect;
//	GetClientRect(hwnd, &rect);
//	InvalidateRect(hwnd,&rect,TRUE);
//}

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

//DWORD WINAPI runOp( LPVOID lpParam ) {
void runOp(HWND hwnd){
//	HWND hwnd = (HWND) lpParam;
	if (lastRead == 0x00){
		isRunning = 0;
	}
	else if (lastRead==0x01){
		r1=readByte();
	} else if (lastRead==0x02){
		r2=readByte();
	} else if (lastRead==0x03){
		r3=readByte();
	} else if (lastRead==0x04){
		r4=readByte();
	} else if (lastRead==0x05){
		r5=readByte();
	} else if (lastRead==0x06){
		r1 = r1 + r2;
	} else if (lastRead==0x07){
		r2 = r2 + r3;	
	} else if (lastRead==0x08){
		r1 = r2;	
	} else if (lastRead==0x09){
		r1 = r1 + r3;
	} else if (lastRead==0x0A){
		r4++;	
	} else if (lastRead==0x0B){
		r5++;		
	} else if (lastRead==0x0C){
		r1=r5;
	} else if (lastRead==0x0D){
		r1--;
	} else if (lastRead==0x0E){
		r5--;
	} else if (lastRead==0x0F){
		r2--;
	} else if (lastRead==0x10){
		ip = readByte();
	} else if (lastRead==0x11){
		flag = (r1==readByte());
	} else if (lastRead==0x12){
		if (flag)
			ip = readByte();
		else
			readByte();
	} else if (lastRead==0x13){
		if (!flag)
			ip = readByte();
		else
			readByte();
	} else if (lastRead==0x14){
		flag = (r2==readByte());
	} else if (lastRead==0x15){
		r1++;
	} else if (lastRead==0x16){
		r2++;
	} else if (lastRead==0x17){
		flag=0;
	} else if (lastRead==0x18){
		flag = (r5==readByte());
	} else if (lastRead==0x19){
		Beep(1000, 100);
	} else if (lastRead==0x1A){
		PlaySound(TEXT("SystemAsteriskk"), NULL, SND_ALIAS | SND_ASYNC);
	} else if (lastRead==0x1B){
		Sleep(1000);
	} else if (lastRead==0x1C){
		MessageBeep(MB_OK);
	} else if (lastRead==0x90){
	 //NO-OP
	} else if (lastRead==0xFD){
		SetPixel(hdcMem, r1, r2, RGB(r3, r4, r5));
	} else if (lastRead==0xFF){
//		int offset = (r1*bytesPerPixel)+(r2*0xFF*bytesPerPixel);
//		frameBuffer[offset] = 0;
//		frameBuffer[offset+1] = 255;
//		frameBuffer[offset+2] = 0;
		SetPixel(hdcMem, r1, r2, RGB(r3, r4, r5));
//		HANDLE hThread = CreateThread(NULL, 0, refresh, &hwnd, 0, NULL);
//		WaitForSingleObject(hThread, INFINITE);
//		CloseHandle(hThread);
//		refresh(hwnd);
//        SetDIBitsToDevice(hdcMem, 0, 0, 255, 255, 0, 0, 0, 255, frameBuffer, &bmi, DIB_RGB_COLORS);
		refresh(hwnd);
	} else if (lastRead==0xEE){
		int i;
//		printf("made it here");
//		char c = gettt();

		HANDLE hThread = CreateThread(NULL, 0, gettt, NULL, 0, NULL);
		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread);
		printf("%c\n", lastPressed);
		r5=lastPressed;
//		printf("%c", c);
//	    char ch = getch();
		//sprintf(errorMessage, "Key Pressed: %c", c);
		//MessageBox(0,errorMessage,"Alert",MB_OK); 	
//		}		while(1){
//			for (i=8;i<=190;i++){
//				if (GetKeyState(i)&0x8000){
//					printf("Key %d is pressed", i);
//				}
//			}
//		}
	} else if (lastRead==0xDD){
//		SetDIBitsToDevice(hdcMem, 0, 0, 255, 255, 0, 0, 0, 255, frameBuffer, &bmi, DIB_RGB_COLORS);
		refresh(hwnd);
	} else {
		sprintf(errorMessage, "Illegal instruction detected: 0x%2X", lastRead);
		MessageBox(0,errorMessage,"Runtime Error",MB_OK); 	
		isRunning = 0;
	}
	
}

void load(HWND hwnd){
	
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
	
	    // Read 1024 bytes from the file into the buffer
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
	
	    CloseHandle(file); // Close the file
	    MessageBox(0,ofn.lpstrFile,"File Loaded",1); 
	} else {
		sprintf(errorMessage, "The last error code was %d", GetLastError());
		MessageBox(0,errorMessage,"Unexpected Error",1); 
		return;
	}
//	RECT rect = { 0, 0, 255, 255 };
//    FillRect(hdcMem, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH));
//    refresh(hwnd);
//    int i, j;
//    for (j=0;j<255;j++){
//        for (i=0;i<255;i++){
//    		SetPixel(hdcMem, i, j, RGB(0, rand()%255, 0));
//		}	
//	}
    int i, j=0;
    for (j=0;j<255;j++){
        for (i=0;i<255;i++){
    				SetPixel(hdcMem, i, j, RGB(255, 255,255));
		}	
	}
	refresh(hwnd);
}


//void run(HWND hwnd){
DWORD WINAPI run( LPVOID lpParam ) {
	HWND hwnd = (HWND) lpParam;
	isRunning = 1;
	while (isRunning){		
		readByte();
//		if (lastRead == 0x00){
//			break;
//		}
//		HANDLE hThread = CreateThread(NULL, 0, runOp, hwnd, 0, NULL);
//		WaitForSingleObject(hThread, INFINITE);
//		CloseHandle(hThread);
		runOp(hwnd);
	}
}

void step(HWND hwnd){
	isRunning = 1;
//	int i, j;
//    for (j=0;j<255;j++){
//        for (i=0;i<255;i++){
//    		SetPixel(hdcMem, i, j, RGB(0, 0, rand()%255));
//		}	
//	}
//	refresh(hwnd);
		readByte();
//		HANDLE hThread = CreateThread(NULL, 0, runOp, hwnd, 0, NULL);
//		WaitForSingleObject(hThread, INFINITE);
//		CloseHandle(hThread);
		runOp(hwnd);
}

void debug(HWND hwnd){
	sprintf(debugMessage, 
	"r1: 0x%02X\nr2: 0x%02X\nr3: 0x%02X\nr4: 0x%02X\nr5: 0x%02X\nflag: 0x%02X\nip: 0x%02X\nsp: 0x%02X\nlastRead: 0x%02X\nisRunning: 0x%02X\n",
	r1,r2,r3,r4,r5,flag,ip,sp,lastRead,isRunning);
	MessageBox(0,debugMessage,"Current Register Values",MB_OK); 
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
                    //run(hwnd);
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
//            hBitmap = CreateDIBSection(hdcMem, &bmi, DIB_RGB_COLORS, (void**)&frameBuffer, NULL, 0);

            SelectObject(hdcMem, hBitmap);

//            int i, j=0;
//            for (j=0;j<255;j++){
//                for (i=0;i<255;i++){
//            				SetPixel(hdcMem, i, j, RGB(255, 255,255));
//				}	
//			}

            // Clear the framebuffer to black
            RECT rect = { 0, 0, 255, 255 };
            FillRect(hdcMem, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH));
//
            HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
            SelectObject(hdcMem,hPen);
            
        
//        	int m;
//            for (m=0;m<frameBufferSize*bytesPerPixel;m++){
//            	frameBuffer[m]= 122;
//			}
            
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

//            SetDIBitsToDevice(hdcMem, 0, 0, 255, 255, 0, 0, 0, 255, frameBuffer, &bmi, DIB_RGB_COLORS);
//            for (m=0;m<frameBufferSize*bytesPerPixel;m++){
//            	frameBuffer[m]= 255;
//			}
//			SetDIBitsToDevice(hdcMem, 0, 0, 255, 255, 0, 0, 0, 255, frameBuffer, &bmi, DIB_RGB_COLORS);

//            int m;
//            for (m=0;m<frameBufferSize;m++){
//            	frameBuffer[m]=122;
//			}
            
            
//            HBRUSH hbrRed = CreateSolidBrush(RGB(255, 0, 0));
//            SelectObject(hdcMem,hbrRed);
//            MoveToEx(hdcMem, 0, 0, NULL);
//            LineTo(hdcMem, 100, 100);

            return 0;
        }
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

//            int i, j=0;
//            for (j=0;j<255;j++){
//                for (i=0;i<255;i++){
//            				SetPixel(hdcMem, i, j, RGB(rand()%255, 0,0));
//				}	
//			}
//            HBRUSH hbrWhite = (HBRUSH)GetStockObject(WHITE_BRUSH);
//            SelectObject(hdcMem,hbrWhite);
//            MoveToEx(hdcMem, 10, 10, NULL);
//            LineTo(hdcMem, 50, 50);
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
//        case WM_KEYDOWN:{
//        	if (wParam == 'A'){
//        		MessageBox(0,"Hit","Hit",1); 
//			}
//			break;
//		}
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
    SetWindowPos(hwnd, 0, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

//	HICON hicon = LoadIcon(NULL, IDI_EXCLAMATION);
//	HICON hicon = LoadIcon(NULL, IDI_ICON1);
	HICON hicon = LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON1));

//	HICON hicon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM) hicon);
//	SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM) hicon);

    ShowWindow(hwnd, nCmdShow);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}
