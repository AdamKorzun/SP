// lab06.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "lab06.h"
#include <iostream>
#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


HWND hStopButton;
HWND hStartButton;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LAB06, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB06));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB06));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB06);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
enum timeEnum
{
    Hours = 2,
    Minutes = 3,
    Seconds = 4
};
struct drawingParams
{
    HWND hWnd;
    RECT rt;
    INT option;
};
DWORD WINAPI ThreadFunc(LPVOID lpParam) {
    SYSTEMTIME st;
    drawingParams* drp = (drawingParams*)lpParam;
    RECT rt = drp->rt;
    rt.right = 300;
    rt.bottom = 100;
    RECT rt1 = rt;
    rt1.right = 150;
    rt1.left = 100;
    RECT rt2 = rt;
    rt2.left = 150;
    rt2.right = 200;
    RECT rt3 = rt;
    rt3.left = 200;
    rt3.right = 250;
    while (true) {

       
        HDC hDC = GetDC(drp->hWnd);

        //FillRect(hDC, &rt, CreateSolidBrush(RGB(255, 188, 0)));
        SetBkMode(hDC, TRANSPARENT);
        SetTextColor(hDC, RGB(240, 240, 240));
        GetSystemTime(&st);
        INT option = drp->option;
        if (option == Hours) {
            //OutputDebugString(L"Hours\n");
            FillRect(hDC, &rt1, CreateSolidBrush(RGB(255, 97, 1)));
            wchar_t buffer[256];
            wsprintfW(buffer, L"%d", st.wHour);
            DrawText(hDC, buffer, wcslen(buffer), &rt1, DT_SINGLELINE | DT_VCENTER | DT_CENTER | DT_NOCLIP);
        }
        else if (option == Minutes) {
            FillRect(hDC, &rt2, CreateSolidBrush(RGB(24, 48, 198)));

            //OutputDebugString(L"Minutes\n");
            wchar_t buffer[256];
            wsprintfW(buffer, L"%d", st.wMinute);
            DrawText(hDC, buffer, wcslen(buffer), &rt2, DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_NOCLIP);
        }
        else if (option == Seconds) {
            FillRect(hDC, &rt3, CreateSolidBrush(RGB(73, 195, 255)));

            //OutputDebugString(L"Seconds\n");
            wchar_t buffer[256];
            wsprintfW(buffer, L"%d", st.wSecond);
            DrawText(hDC, buffer, wcslen(buffer), &rt3, DT_SINGLELINE | DT_VCENTER | DT_CENTER |  DT_NOCLIP);

        }
        ReleaseDC(drp->hWnd, hDC);
        Sleep(500);
    }
    
    return 0;
}
BOOL working = false;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    static RECT rt;
    static HANDLE hHoursThread;
    static HANDLE hMinutesThread;
    static HANDLE hSecondsThread;
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case ID_STARTBUTTON:
                if (!working) {

                    ResumeThread(hHoursThread);
                    ResumeThread(hMinutesThread);
                    ResumeThread(hSecondsThread);
                    working = true;
                }

                break;
            case ID_STOPBUTTON:
                if (working) {
                    SuspendThread(hHoursThread);
                    SuspendThread(hMinutesThread);
                    SuspendThread(hSecondsThread);
                    working = false;
                    InvalidateRect(hWnd, &rt, TRUE);

                }
             
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_CREATE:
    {
        hStartButton = CreateWindow(L"button", L"Start",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            0, 0, 100, 50, hWnd, (HMENU)ID_STARTBUTTON, hInst, NULL);
        hStopButton = CreateWindow(L"button", L"Stop",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            0, 50, 100, 50, hWnd, (HMENU)ID_STOPBUTTON, hInst, NULL);
        GetWindowRect(hWnd, &rt);
        
        rt.left = 100;
        rt.top = 0;
        drawingParams* drp1 = new drawingParams{ hWnd, rt, Hours };
        drawingParams* drp2 = new drawingParams{ hWnd, rt, Minutes };
        drawingParams* drp3 = new drawingParams{ hWnd, rt, Seconds };
        hHoursThread = CreateThread(NULL, 0, ThreadFunc, (LPVOID*)drp1, CREATE_SUSPENDED, NULL);
        hMinutesThread = CreateThread(NULL, 0, ThreadFunc, (LPVOID*)drp2, CREATE_SUSPENDED, NULL);
        hSecondsThread = CreateThread(NULL, 0, ThreadFunc, (LPVOID*)drp3, CREATE_SUSPENDED, NULL);

    }
        
        
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            FillRect(hdc, &rt, CreateSolidBrush(RGB(255, 188, 0)));

            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
