//-----------------------------------------------------------------------------
// File: ddutil.cpp
//
// Desc: Routines for loading bitmap and palettes from resources
//
// Copyright (C) 1998-2001 Microsoft Corporation. All Rights Reserved.
//-----------------------------------------------------------------------------
#ifndef DDUTIL_H
#define DDUTIL_H

#include <ddraw.h>
#include <d3d.h>
#include <afxwin.h>

#define DSURFACELOCK_READ
#define DSURFACELOCK_WRITE

class CDisplay
{
protected:
	LPDIRECTDRAW7        m_pDD;
	LPDIRECTDRAWSURFACE7 m_pdds;
	DDSURFACEDESC2       m_ddsd;

	CWnd* parentWnd;

	static HRESULT WINAPI EnumModesCallback( LPDDSURFACEDESC2 lpDDSurfaceDesc, LPVOID lpDisplay );

	struct mode {
		int width;
		int height;
		int bpp;
		bool operator<( const mode& m ) {
			bool flag = width < m.width;
			if ( !flag && width == m.width ) {
				flag = height < m.height;
			}
			return flag;
		}
	};
	std::vector< mode > modes;

public:
	CDisplay();
	~CDisplay();

	HRESULT CreateWindowedDisplay( CWnd* _parentWnd, int width, int height );
    HRESULT CreateFullScreenDisplay( CWnd* _parentWnd, int width, int height, int bpp );
	virtual HRESULT DestroyObjects();

	LPDIRECTDRAW7        GetDirectDraw()     { return m_pDD;   }
	LPDIRECTDRAWSURFACE7 getSurface()        { return m_pdds;  }
	DDSURFACEDESC2*      getSurfaceDesc()    { return &m_ddsd; }
};

#endif // DDUTIL_H
