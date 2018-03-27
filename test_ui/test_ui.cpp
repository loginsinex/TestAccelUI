// test_ui.cpp : Defines the entry point for the application.
//


#include "stdafx.h"
#include "test_ui.h"
#include <vector>
#include <map>
#include <algorithm>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
std::map<HWND,HACCEL>	l_mAccelTable;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
HWND                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

BOOL AddAccelerators(HWND hWnd, HACCEL hAccel)
{
	if ( IsWindow( hWnd ) )
	{
		l_mAccelTable[ hWnd ] = hAccel;
		return TRUE;
	}
	return FALSE;
}

BOOL AddAccelerators( HWND hWnd, LPCTSTR accel )
{
	return AddAccelerators( hWnd, LoadAccelerators( hInst, accel ) );
}

BOOL AddAccelerators(HWND hWnd, int accel)
{
	return AddAccelerators( hWnd, MAKEINTRESOURCE( accel ) );
}

BOOL AddAccelerators( HWND hWnd )
{
	return AddAccelerators( hWnd, HACCEL( NULL ) );
}

void DelAccel(HWND hWnd)
{
	std::map<HWND, HACCEL>::iterator end = l_mAccelTable.end();
	std::map<HWND, HACCEL>::iterator me = l_mAccelTable.find( hWnd );

	if ( me != end )
	{
		if ( me->second )
		{
			DestroyAcceleratorTable( me->second );
		}

		l_mAccelTable.erase( me );
	}
}

BOOL HandleAccelWindow(std::map<HWND,HACCEL>::const_iterator mh, MSG & msg)
{
	const HWND & hWnd = mh->first;
	const HACCEL & hAccel = mh->second;

	if ( !TranslateAccelerator( hWnd, hAccel, &msg ) )
	{
		if ( !IsDialogMessage( hWnd, &msg ) )
		{
			return FALSE;
		}
	}

	return TRUE;
}

BOOL HandleAccelArray( HWND hActive, MSG & msg )
{
	if ( !hActive )
		return FALSE;

	std::map<HWND, HACCEL>::const_iterator mh = l_mAccelTable.find( hActive );
	if ( mh != l_mAccelTable.end() )
	{
		return HandleAccelWindow( mh, msg );
	}

	return FALSE;
}

WPARAM Loop()
{
	// Main message loop:
	MSG msg;
	BOOL bRet = 0;
	while ( bRet = GetMessage( &msg, nullptr, 0, 0 ) )
	{
		if ( -1 == bRet )
			return 0;

		if ( !HandleAccelArray( GetActiveWindow(), msg ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
	}
	return msg.wParam;
}

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
    LoadStringW(hInstance, IDC_TESTUI, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    HWND hMainWnd = InitInstance( hInstance, nCmdShow );    
    if (!hMainWnd)
    {
        return FALSE;
    }

	AddAccelerators( hMainWnd, IDC_TESTUI );

    
	WPARAM ret = Loop();
 

	DelAccel( hMainWnd );

    return (int) ret;
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TESTUI));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_BTNFACE+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TESTUI);
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
HWND InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   SendMessage( hWnd, WM_SETFONT, (WPARAM)GetStockObject( DEFAULT_GUI_FONT ), 1 );

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return hWnd;
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

INT_PTR CALLBACK DlgTestProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch(message)
	{
		case WM_INITDIALOG:
			{
				AddAccelerators( hWnd, IDR_ACCELERATOR1 );
				break;
			}
		case WM_COMMAND:
			{
				if ( BN_CLICKED == HIWORD( wParam ) || 1 == HIWORD( wParam ) )
				{
					if ( ID_F9 == LOWORD( wParam ) )
					{
						SendDlgItemMessage( hWnd, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)TEXT( "F9 PRESSED!" ) );
					}
					else if ( IDOK == LOWORD( wParam ) || IDCANCEL == LOWORD( wParam ) )
					{
						DestroyWindow( hWnd );
						break;
					}
				}
				break;
			}
		case WM_DESTROY:
			{
				DelAccel( hWnd );
				break;
			}
	}
	return 0;
}

INT_PTR CALLBACK Dlg2TestProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch ( message )
	{
		case WM_INITDIALOG:
			{
				AddAccelerators( hWnd, IDR_ACCELERATOR2 );
				break;
			}
		case WM_COMMAND:
			{
				if ( BN_CLICKED == HIWORD( wParam ) || 1 == HIWORD( wParam ) )
				{
					if ( ID_F10 == LOWORD( wParam ) )
					{
						SendDlgItemMessage( hWnd, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)TEXT( "F10 PRESSED!" ) );
					}
					else if ( IDOK == LOWORD( wParam ) || IDCANCEL == LOWORD( wParam ) )
					{
						DestroyWindow( hWnd );
						break;
					}
				}
				break;
			}
		case WM_DESTROY:
			{
				PostQuitMessage( 0 );
				DelAccel( hWnd );
				break;
			}
	}
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
		{
			LPCREATESTRUCT lpcs = (LPCREATESTRUCT)lParam;
			CreateWindow( TEXT( "BUTTON" ), TEXT( "Button1" ), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON, 10, 10, 75, 20, hWnd, (HMENU)0x200, lpcs->hInstance, NULL );
			CreateWindow( TEXT( "BUTTON" ), TEXT( "Button2" ), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON, 10, 50, 75, 20, hWnd, (HMENU)0x201, lpcs->hInstance, NULL );
			break;
		}
	case WM_SETFONT:
		{
			EnumChildWindows( 
				hWnd, 
				[]( HWND hwnd, LPARAM lparam )->BOOL CALLBACK 
				{ SendMessage( hwnd, WM_SETFONT, (WPARAM)lparam, TRUE ); return TRUE; },
				(LPARAM)wParam );
			break;
		}
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
			case 0x200:
				{
					HWND hdlg = CreateDialog( hInst, MAKEINTRESOURCE( IDD_DIALOG1 ), hWnd, DlgTestProc );					
					ShowWindow( hdlg, SW_SHOW );
					break;
				}
			case 0x201:
				{
					BOOL fIsWindowEnabled = IsWindowEnabled( hWnd );
					HWND hDlg = CreateDialog( hInst, MAKEINTRESOURCE( IDD_DIALOG2 ), hWnd, Dlg2TestProc );
					if ( hDlg )
					{
						ShowWindow( hDlg, SW_NORMAL );
						EnableWindow( hWnd, FALSE );
						Loop();
						EnableWindow( hWnd, fIsWindowEnabled );
					}
					break;
				}
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
