// lab03.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "lab03.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

HWND hDrawButton;
HWND hClearButton;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

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
    LoadStringW(hInstance, IDC_LAB03, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB03));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB03));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB03);
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
BOOL DrawLine(HDC hdc, int x1, int y1, int x2, int y2)
{
    MoveToEx(hdc, x1, y1, NULL);
    return LineTo(hdc, x2, y2);
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HDC hdc;
    static RECT rt;
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
            case ID_DRAWBUTTON:
            {
                hdc = GetDC(hWnd);
                FillRect(hdc, &rt, CreateSolidBrush(RGB(255, 188, 0)));
                DrawLine(hdc, 20, 100, 20, 400);
                DrawLine(hdc, 20, 250, 100, 250);
                DrawLine(hdc, 100, 100, 100, 400);
                Rectangle(hdc, 180, 100, 220, 400);
                Rectangle(hdc, 120, 100, 160, 400);
                Rectangle(hdc, 120, 250, 220, 270);
                POINT pointArray[] = { {240, 100}, {240, 400}, {340, 400}, {340, 100}, {240, 100} };
                Polyline(hdc, pointArray, 5);
                Arc(hdc, 1000, 300, 1100, 400, 400, 1100, 300, 1000);
                Arc(hdc, 1200, 300, 1300, 400, 400, 1300, 300, 1200);
                POINT pointArray2[] = { {380, 100}, {480, 400}, {480, 100}, {380, 400} };
                Polyline(hdc, pointArray2, 4);
                Ellipse(hdc, 1000, 100, 1100, 200);
                Ellipse(hdc, 1200, 100, 1300, 200);
                break;
            }
            case ID_CLEARBUTTON:
                InvalidateRect(hWnd, &rt, TRUE);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_DRAWITEM:
    {
        LPDRAWITEMSTRUCT info = (LPDRAWITEMSTRUCT)lParam;
        RECT innerRt = info->rcItem;
        SetBkMode(info->hDC, TRANSPARENT);
        SetTextColor(info->hDC, RGB(240, 240, 240));

        if (info->CtlID == ID_DRAWBUTTON) {
            LPCWSTR text = L"Draw";
            FillRect(info->hDC, &innerRt, CreateSolidBrush(RGB(24, 48, 198)));
            DrawText(info->hDC, text, wcslen(text), &innerRt, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        }
        if (info->CtlID == ID_CLEARBUTTON) {
            LPCWSTR text = L"Clear";
            FillRect(info->hDC, &innerRt, CreateSolidBrush(RGB(255, 97, 1)));
            DrawText(info->hDC, text, wcslen(text), &innerRt, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        }
        break;
    }
    case WM_CREATE:
      
        GetWindowRect(hWnd, &rt);
        static INT height = rt.bottom;
        hdc = GetDC(hWnd);
        FillRect(hdc, &rt, CreateSolidBrush(RGB(255, 188, 0)));

        rt.top = 0;
        rt.bottom  = 500;
        rt.left = 0;
        
        hDrawButton = CreateWindow(L"Button", L"Draw", WS_CHILD | WS_VISIBLE |BS_OWNERDRAW,350, 520, 150, 50, hWnd, (HMENU)ID_DRAWBUTTON, hInst, NULL);
        hClearButton = CreateWindow(L"Button", L"Clear", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 750, 520, 150, 50, hWnd, (HMENU)ID_CLEARBUTTON, hInst, NULL);

        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
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
