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

	CWnd*                parentWnd;
	int                  width;
	int                  height;

public:
	CDisplay();
	~CDisplay();

	HRESULT CreateWindowedDisplay( CWnd* _parentWnd, int _width, int _height );
    HRESULT CreateFullScreenDisplay( CWnd* _parentWnd, int _width, int _height, int _bpp );
	virtual HRESULT DestroyObjects();

	LPDIRECTDRAW7        GetDirectDraw()     { return m_pDD;   }
	LPDIRECTDRAWSURFACE7 getSurface()        { return m_pdds;  }
	DDSURFACEDESC2*      getSurfaceDesc()    { return &m_ddsd; }
};

#endif // DDUTIL_H
