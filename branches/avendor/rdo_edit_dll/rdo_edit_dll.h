#ifndef RDO_EDIT_DLLH
#define RDO_EDIT_DLLH
#pragma once

#include <windows.h>

void connectRdoApp( HWND rdoApp );
void disconnectRdoApp( HWND rdoApp );
int isConnected( HWND rdoApp );
void setHook( HWND rdoApp, HHOOK hook );

#endif // RDO_EDIT_DLLH
