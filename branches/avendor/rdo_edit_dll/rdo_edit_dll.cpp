#include "rdo_edit_dll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int appCount = 100;

struct AppArray {
	HWND  rdoAppHWnd;
	HHOOK hHook;
};

#pragma data_seg ( ".RdoEditDllData" )
AppArray apps[ appCount ] = {{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0},
                             {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0},
                             {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0},
                             {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0},
                             {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0},
                             {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0},
                             {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0},
                             {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0},
                             {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0},
                             {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}};
#pragma data_seg ()

LRESULT CALLBACK callWndProc( int nCode, WPARAM wParam, LPARAM lParam )
{
	if ( nCode == HC_ACTION ) {
		CWPSTRUCT* cwp = (CWPSTRUCT*)lParam;
		if ( cwp->message == WM_CREATE ) {
			for ( int i = 0; i < appCount; i++ ) {
				if ( apps[i].rdoAppHWnd ) {
					::SendMessage( apps[i].rdoAppHWnd, WM_USER + 144, cwp->message, (LPARAM)cwp->hwnd );
				}
			}
		} else if ( cwp->message == WM_NCDESTROY ) {
			for ( int i = 0; i < appCount; i++ ) {
				if ( apps[i].rdoAppHWnd ) {
					::SendMessage( apps[i].rdoAppHWnd, WM_USER + 144, cwp->message, (LPARAM)cwp->hwnd );
				}
			}
		}
		int res = 0;
		for ( int i = 0; i < appCount; i++ ) {
			if ( apps[i].rdoAppHWnd && apps[i].hHook ) {
				res = ::CallNextHookEx( apps[i].hHook, nCode, wParam, lParam );
				break;
			}
		}
		return res;
	} else {
		int res = 0;
		for ( int i = 0; i < appCount; i++ ) {
			if ( apps[i].rdoAppHWnd && apps[i].hHook ) {
				res = ::CallNextHookEx( apps[i].hHook, nCode, wParam, lParam );
				break;
			}
		}
		return res;
	}
}

void connectRdoApp( HWND rdoApp )
{
	if ( isConnected( rdoApp ) == -1 ) {
		for ( int i = 0; i < appCount; i++ ) {
			if ( !apps[i].rdoAppHWnd ) {
				apps[i].rdoAppHWnd = rdoApp;
				apps[i].hHook      = 0;
				break;
			}
		}
	}
}

void disconnectRdoApp( HWND rdoApp )
{
	int i = isConnected( rdoApp );
	if ( i != -1 ) {
		apps[i].rdoAppHWnd = 0;
		apps[i].hHook      = 0;
	}
}

int isConnected( HWND rdoApp )
{
	if ( !rdoApp ) return -1;

	for ( int i = 0; i < appCount; i++ ) {
		if ( apps[i].rdoAppHWnd == rdoApp ) return i;
	}
	return -1;
}

void setHook( HWND rdoApp, HHOOK hook )
{
	int i = isConnected( rdoApp );
	if ( i != -1 ) {
		apps[i].hHook = hook;
	}
}
