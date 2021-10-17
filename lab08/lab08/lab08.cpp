// lab08.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "lab08.h"
#include <TlHelp32.h>
#include <windowsx.h>
#include <psapi.h>
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
PROCESSENTRY32 procs[1024];
HWND hListBox1;
HWND hListBox2;
DWORD aProcesses[1024];
HMENU hPopupMenu;

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
    LoadStringW(hInstance, IDC_LAB08, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB08));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB08));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB08);
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
#define ID_IDLE 10
#define ID_NORMAL 11
#define ID_HIGH 12
#define ID_REAL_TIME 13
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
void setPriority(DWORD priorityClass, HWND listBox) {
    int itemId = SendMessage(listBox, LB_GETCURSEL, 0, NULL);
    if (itemId != -1) {
        HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procs[itemId].th32ProcessID);
        SetPriorityClass(hProcess, priorityClass);
        CloseHandle(hProcess);
    }
}
//void getProcesses(HWND listBox)
//{
//    DWORD cbNeeded, cProcesses;
//    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
//    {
//        return;
//    }
//    cProcesses = cbNeeded / sizeof(DWORD);
//    TCHAR szProcessName[1024] = TEXT("<unknown>");
//    HMODULE hMod;
//
//    for (int i = 0; i < cProcesses; i++) {
//        if (aProcesses[i] != 0) {
//            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, aProcesses[i]);
//            EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded);
//            GetModuleBaseName(hProcess, hMod, szProcessName, 1024);
//            OutputDebugString(szProcessName );
//            /*if (EnumProcessModules(hProcess, &hMod, sizeof(hMod),&cbNeeded)) {
//                GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR));
//            }*/
//            SendMessage(listBox, LB_ADDSTRING, 0, (LPARAM)szProcessName);
//            CloseHandle(hProcess);
//        }
//
//    }
//}

void getModules(DWORD pid, HWND listBox)
{
    HANDLE th = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
    if (th != INVALID_HANDLE_VALUE)
    {
        MODULEENTRY32 mod;
        mod.dwSize = sizeof(MODULEENTRY32);
        Module32First(th, &mod);
        do {
            SendMessage(listBox, LB_ADDSTRING, 0, (LPARAM)mod.szModule);
        } while (Module32Next(th, &mod));
    }
    CloseHandle(th);
}
void getProcesses(HWND listBox)
{
    int procCount = 0;
    procs[0].dwSize = sizeof(PROCESSENTRY32);

    HANDLE th = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    Process32First(th, &procs[0]);
    while (Process32Next(th, &procs[procCount]))
    {
        procCount++;
        procs[procCount].dwSize = sizeof(PROCESSENTRY32);
    }
    CloseHandle(th);
    for (int i = 0; i < procCount; i++) {
        SendMessage(listBox, LB_ADDSTRING, 0, (LPARAM)procs[i].szExeFile);
    }
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static RECT rt;

    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            int wmEvent = HIWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case ID_LISTBOX1:
                if (wmEvent == LBN_SELCHANGE)
                {
                    int itemId = SendMessage(hListBox1, LB_GETCURSEL, 0, NULL);
                    if (itemId != -1) {
                        SendMessage(hListBox2, LB_RESETCONTENT, 0, NULL);
                        getModules(procs[itemId].th32ProcessID, hListBox2);
                    }
                }
                break;
            case ID_IDLE:
                setPriority(IDLE_PRIORITY_CLASS, hListBox1);
                break;
            case ID_NORMAL:
                setPriority(NORMAL_PRIORITY_CLASS, hListBox1);
                break;
            case ID_HIGH:
                setPriority(HIGH_PRIORITY_CLASS, hListBox1);
                break;
            case ID_REAL_TIME:
                setPriority(REALTIME_PRIORITY_CLASS, hListBox1);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_CREATE:
        GetWindowRect(hWnd, &rt);
        rt.top = 0;
        rt.left = 0;

        hListBox1 = CreateWindow(L"listbox", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | LBS_NOTIFY,
            0, 0, 300, 500, hWnd, (HMENU)ID_LISTBOX1, hInst, NULL);
        hListBox2 = CreateWindow(L"listbox", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | LBS_NOTIFY,
            320, 0, 300, 500, hWnd, (HMENU)ID_LISTBOX2, hInst, NULL);
        getProcesses(hListBox1);
        break;
    case WM_CONTEXTMENU:
        if ((HWND)wParam == hListBox1) {
            int itemId = SendMessage(hListBox1, LB_GETCURSEL, 0, NULL);
            if (itemId != -1) {
                HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procs[itemId].th32ProcessID);
                int pC = GetPriorityClass(hProcess);
                CloseHandle(hProcess);

                hPopupMenu = CreatePopupMenu();
                AppendMenu(hPopupMenu, pC == REALTIME_PRIORITY_CLASS ? MF_CHECKED : MF_STRING, ID_REAL_TIME, L"RT");
                AppendMenu(hPopupMenu, pC == HIGH_PRIORITY_CLASS ? MF_CHECKED : MF_STRING, ID_HIGH, L"High");
                AppendMenu(hPopupMenu, pC == NORMAL_PRIORITY_CLASS ? MF_CHECKED : MF_STRING, ID_NORMAL, L"Normal");
                AppendMenu(hPopupMenu, pC == IDLE_PRIORITY_CLASS ? MF_CHECKED : MF_STRING, ID_IDLE, L"Idle");
                
                
                
                TrackPopupMenu(hPopupMenu, TPM_TOPALIGN | TPM_LEFTALIGN, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 0, hWnd, NULL);
            }
        }
        break;
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
