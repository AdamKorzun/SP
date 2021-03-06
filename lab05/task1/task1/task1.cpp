// task1.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "task1.h"

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

HWND hRadioButtonGreen;
HWND hRadioButtonRed;
HWND hRadioButtonBlue;
HWND hDrawCheckbox;
HWND hSecondWindow;
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
    LoadStringW(hInstance, IDC_TASK1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TASK1));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TASK1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TASK1);
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
   SetTimer(hWnd, 600, 1000, NULL);
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
typedef struct drawingParams {
    COLORREF  color;
    BOOL draw;
}DrParams;
void sSendMessage(DrParams &params, COPYDATASTRUCT messageStruct, HWND hWnd) {
    messageStruct.cbData = sizeof(params);
    messageStruct.dwData = 1;
    messageStruct.lpData = &params;
    SendMessage(hSecondWindow, WM_COPYDATA, (WPARAM)hWnd, (LPARAM)(LPVOID)&messageStruct);
}
DrParams params;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    BOOL locDraw;
    static COPYDATASTRUCT messageStruct;
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
            case ID_RADIORED:
                params.color = 0x000000FF;
                sSendMessage(params, messageStruct, hWnd);
                break;
            case ID_RADIOGREEN:
                params.color = 0x0000FF00;
                sSendMessage(params, messageStruct, hWnd);
                break;
            case ID_RADIOBLUE:
                params.color = 0x00FF0000;
                sSendMessage(params, messageStruct, hWnd);
                break;
            case ID_DRAWCHECKBOX:
                if (SendMessage(hDrawCheckbox, BM_GETCHECK, 0, 0L) == BST_CHECKED) {
                    locDraw = true;
                }
                else {
                    locDraw = false;
                }
                params.draw = locDraw;
                sSendMessage(params, messageStruct, hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_CREATE:
        hRadioButtonRed = CreateWindow(L"button", L"Red", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP | WS_TABSTOP,
            0, 0, 100, 50, hWnd, (HMENU)ID_RADIORED, hInst, NULL);
        hRadioButtonBlue = CreateWindow(L"button", L"Blue", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
            0, 50, 100, 50, hWnd, (HMENU)ID_RADIOBLUE, hInst, NULL);
        hRadioButtonGreen = CreateWindow(L"button", L"Green", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
            0, 100, 100, 50, hWnd, (HMENU)ID_RADIOGREEN, hInst, NULL);
        hDrawCheckbox = CreateWindow(L"button", L"Draw", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | WS_GROUP | WS_TABSTOP,
            0, 150, 100, 50, hWnd, (HMENU)ID_DRAWCHECKBOX, hInst, NULL);
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_TIMER:
        if (!hSecondWindow) {
            OutputDebugString(L"not found\n");
            hSecondWindow = FindWindow(NULL, L"task2");
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
