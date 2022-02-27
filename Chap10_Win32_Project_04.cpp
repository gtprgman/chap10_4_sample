// Chap10_Win32_Project_04.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Chap10_Win32_Project_04.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Custom Global Variables:
HWND hwndEdit = nullptr;
HWND hwndList = nullptr;
HWND hwndButton = nullptr;


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
    LoadStringW(hInstance, IDC_CHAP10_WIN32_PROJECT_04, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CHAP10_WIN32_PROJECT_04));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CHAP10_WIN32_PROJECT_04));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CHAP10_WIN32_PROJECT_04);
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
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            // Parse the menu selections:
            switch ( LOWORD(wParam) )
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;

			case IDC_BUTTON_CTL:
			{
				LPWSTR lsItem = nullptr;
				int inpLen = GetWindowTextLengthW(hwndEdit);
				if (inpLen > 0) lsItem = new WCHAR[inpLen*sizeof(WCHAR)];
				GetWindowTextW(hwndEdit, lsItem, inpLen + 1);
				SendMessageW(hwndList, LB_ADDSTRING, NULL, (LPARAM)lsItem);

				if (nullptr != lsItem) delete[] lsItem;
				lsItem = nullptr;
			}
				break;
			
			case IDL_LIST_BOX:
				switch (HIWORD(wParam)) {
					case LBN_SELCHANGE:
					{
						
						int selIndex = (int)SendMessageW(hwndList, LB_GETCURSEL, NULL, NULL);
						int selLen = (int)SendMessageW(hwndList, LB_GETTEXTLEN, selIndex, NULL);
						LPWSTR selItem = new WCHAR[selLen * sizeof(WCHAR)];
						
						if (SendMessageW(hwndList, LB_GETTEXT, selIndex, (LPARAM)selItem) > 0)
							MessageBoxW(NULL, selItem, TEXT("Item Selection Info"), MB_OK);

						if (NULL != selItem) delete[] selItem;
						selItem = nullptr;
					
					}break;

					default: 
						break;
				}
				break;

            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;

	case WM_CREATE:
	{
		hwndEdit = CreateWindowW(TEXT("Edit"), NULL, WS_CHILD | WS_BORDER | WS_VISIBLE | ES_LEFT, 10, 10, 80, 30, hWnd, NULL, ::hInst, NULL);

		hwndButton = CreateWindowW(TEXT("Button"), TEXT("Add"), WS_CHILD | WS_TABSTOP| WS_VISIBLE | BS_PUSHBUTTON, 100, 10, 50, 30, hWnd,
			(HMENU)(int)IDC_BUTTON_CTL, ::hInst, NULL);

		hwndList = CreateWindowW(TEXT("LISTBOX"), NULL, WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL |LBS_STANDARD | LBS_HASSTRINGS, 10, 60, 150, 200, hWnd,
			(HMENU)(int)IDL_LIST_BOX, ::hInst, NULL);

		

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
