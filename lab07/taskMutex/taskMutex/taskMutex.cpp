// taskMutex.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "taskMutex.h"

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



HANDLE hMutex;

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
    LoadStringW(hInstance, IDC_TASKMUTEX, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TASKMUTEX));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TASKMUTEX));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TASKMUTEX);
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
enum drawEnum
{
    House = 2,
    Tree = 3,
    Rhombus = 4
};
struct drawingParams
{
    HWND hWnd;
    RECT rt;
    INT option;
};
DWORD WINAPI ThreadFunc(LPVOID lpParam) {
    drawingParams* drp = (drawingParams*)lpParam;
    INT option = drp->option;
    RECT rt = drp->rt;
    DWORD dwWaitResult;
    INT midX = (rt.right - rt.left) / 2;
    INT midY = (rt.bottom - rt.top) / 2;
    while (true) {
        dwWaitResult = WaitForSingleObject(hMutex,INFINITE);
        if (dwWaitResult == WAIT_OBJECT_0) {
            HDC hDC = GetDC(drp->hWnd);

            if (option == House) {
                OutputDebugString(L"House\n");
                FillRect(hDC, &rt, CreateSolidBrush(RGB(255, 188, 0)));
                SelectObject(hDC, GetStockObject(DC_BRUSH));
                SetDCBrushColor(hDC, 0x000161FF);
                POINT drawingPoints[] = { { midX , midY - 70 }, { midX - 40 , midY - 40}, { midX - 40, midY + 40},
                    {midX + 40 , midY + 40} , { midX + 40 , midY - 40 }, { midX , midY - 70 } };
                Polygon(hDC, drawingPoints, 6);
            }
            else if (option == Tree) {
                OutputDebugString(L"Tree\n");
                FillRect(hDC, &rt, CreateSolidBrush(RGB(255, 188, 0)));
               
                SelectObject(hDC, GetStockObject(DC_BRUSH));
                SetDCBrushColor(hDC, 0x00035fA0);
                POINT drawingPoints[] = { { midX - 10 , midY }, { midX - 20 , midY + 50}, { midX + 20, midY  + 50} , {midX + 10 , midY } , { midX - 10 , midY } };
                Polygon(hDC, drawingPoints, 5);
                SetDCBrushColor(hDC, 0x002A9628);
                Ellipse(hDC, midX - 40, midY - 40, midX + 40, midY + 40);

            }
            else if (option == Rhombus) {
                OutputDebugString(L"Rhombus\n");
                FillRect(hDC, &rt, CreateSolidBrush(RGB(255, 188, 0)));
                SelectObject(hDC, GetStockObject(DC_BRUSH));
                SetDCBrushColor(hDC, 0x00C63018);
                POINT drawingPoints[] = { { midX , midY - 50 }, { midX - 50 , midY}, { midX, midY + 50} , {midX  + 50 , midY } , { midX , midY - 50 } };
                Polygon(hDC, drawingPoints, 5);
            }
            ReleaseDC(drp->hWnd, hDC);
        }
        Sleep(1000);
        ReleaseMutex(hMutex);

    }
    return 0;
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

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static RECT rt;
    static HANDLE threads[3];
    /*static HANDLE hHouseThread;
    static HANDLE hTreeThread;
    static HANDLE hRhombusThread;*/
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
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_CREATE:
    {
        GetWindowRect(hWnd, &rt);
        rt.top = 0;
        rt.left = 0;
        hMutex = CreateMutex(NULL, FALSE, NULL);

        drawingParams* drp1 = new drawingParams{ hWnd, rt, House };
        drawingParams* drp2 = new drawingParams{ hWnd, rt, Tree };
        drawingParams* drp3 = new drawingParams{ hWnd, rt, Rhombus };
        threads[0] = CreateThread(NULL, 0, ThreadFunc, (LPVOID*)drp1, NULL, NULL);
        threads[1] = CreateThread(NULL, 0, ThreadFunc, (LPVOID*)drp2, NULL, NULL);
        threads[2] = CreateThread(NULL, 0, ThreadFunc, (LPVOID*)drp3, NULL, NULL);


        break;
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
        WaitForMultipleObjects(3, threads, TRUE, INFINITE);
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
