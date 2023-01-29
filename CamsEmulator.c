#include "CamsEmulator.h"

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
            
			drawWelcomeMessage();

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
