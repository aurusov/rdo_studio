//-----------------------------------------------------------------------------
// File: ddutil.cpp
//
// Desc: DirectDraw framewark classes. Feel free to use this class as a 
//       starting point for adding extra functionality.
//
//
// Copyright (c) 1995-2001 Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------
#define STRICT
#include "stdafx.h"
#include <tchar.h>
#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>
#include "ddutil.h"
#include "dxutil.h"

#include <algorithm>

CDisplay::CDisplay():
	m_pDD( NULL ),
	m_pdds( NULL )
{
}

CDisplay::~CDisplay()
{
	DestroyObjects();
	modes.clear();
}

HRESULT CDisplay::DestroyObjects()
{
	SAFE_RELEASE( m_pdds );

	if ( m_pDD ) {
		m_pDD->SetCooperativeLevel( parentWnd->m_hWnd, DDSCL_NORMAL );
	}

	SAFE_RELEASE( m_pDD );

	return S_OK;
}

HRESULT CDisplay::CreateWindowedDisplay( CWnd* _parentWnd, int width, int height )
{
	parentWnd = _parentWnd;

	HRESULT hr;

	DestroyObjects();

	if( FAILED( hr = DirectDrawCreateEx( NULL, (VOID**)&m_pDD, IID_IDirectDraw7, NULL ) ) ) {
		return E_FAIL;
	}

	hr = m_pDD->SetCooperativeLevel( parentWnd->m_hWnd, DDSCL_NORMAL );
	if( FAILED( hr ) ) {
		return E_FAIL;
	}

	RECT rc;
	::SetRect( &rc, 0, 0, width, height );
	::AdjustWindowRectEx( &rc, parentWnd->GetStyle(), parentWnd->GetMenu() != NULL, parentWnd->GetExStyle() );
	parentWnd->SetWindowPos( NULL, 0, 0, rc.right - rc.left, rc.bottom - rc.top, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE );
	parentWnd->SetWindowPos( &CWnd::wndNoTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE );

	//  Make sure our window does not hang outside of the work area
	RECT rcWork;
	::SystemParametersInfo( SPI_GETWORKAREA, 0, &rcWork, 0 );
	parentWnd->GetWindowRect( &rc );
	if ( rc.left < rcWork.left ) rc.left = rcWork.left;
	if ( rc.top  < rcWork.top )  rc.top  = rcWork.top;
	parentWnd->SetWindowPos( NULL, rc.left, rc.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE );

	// Create the primary surface
	DDSURFACEDESC2 ddsd;
	::ZeroMemory( &ddsd, sizeof( ddsd ) );
	ddsd.dwSize         = sizeof( ddsd );
	ddsd.dwFlags        = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

	if( FAILED( m_pDD->CreateSurface( &ddsd, &m_pdds, NULL ) ) ) {
		return E_FAIL;
	}

	m_ddsd.dwSize = sizeof( m_ddsd );
	m_pdds->GetSurfaceDesc( &m_ddsd );

	LPDIRECTDRAWCLIPPER pcClipper;
	if( FAILED( hr = m_pDD->CreateClipper( 0, &pcClipper, NULL ) ) ) {
		return E_FAIL;
	}

	if( FAILED( hr = pcClipper->SetHWnd( 0, parentWnd->m_hWnd ) ) ) {
		pcClipper->Release();
		return E_FAIL;
	}

	if( FAILED( hr = m_pdds->SetClipper( pcClipper ) ) ) {
		pcClipper->Release();
		return E_FAIL;
	}

	// Done with clipper
	pcClipper->Release();

	return S_OK;
}

HRESULT WINAPI CDisplay::EnumModesCallback( LPDDSURFACEDESC2 lpDDSurfaceDesc, LPVOID lpDisplay )
{
	CDisplay* display = static_cast<CDisplay*>(lpDisplay);
	if ( display ) {
		mode m;
		m.width  = lpDDSurfaceDesc->dwWidth;
		m.height = lpDDSurfaceDesc->dwHeight;
		m.bpp    = lpDDSurfaceDesc->ddpfPixelFormat.dwRGBBitCount;
		display->modes.push_back( m );
	}
	return DDENUMRET_OK;
}

HRESULT CDisplay::CreateFullScreenDisplay( CWnd* _parentWnd, int width, int height, int bpp )
{
	parentWnd = _parentWnd;

	HRESULT hr;

	DestroyObjects();

	if( FAILED( hr = DirectDrawCreateEx( NULL, (VOID**)&m_pDD, IID_IDirectDraw7, NULL ) ) ) {
		return E_FAIL;
	}

	hr = m_pDD->SetCooperativeLevel( parentWnd->m_hWnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN );
	if( FAILED( hr ) ) {
		return E_FAIL;
	}

	m_pDD->EnumDisplayModes( DDEDM_STANDARDVGAMODES, NULL, this, EnumModesCallback );
	std::sort( modes.begin(), modes.end() );
	std::vector< mode >::const_iterator it = modes.begin();
	int w = 0;
	int h = 0;
	while ( it != modes.end() ) {
		if ( it->bpp == bpp ) {
			if ( it->width >= width && it->height >= height ) {
				w = it->width;
				h = it->height;
				break;
			}
		}
		it++;
	}
	if ( !w || !h ) {
		return E_FAIL;
	}

	if( FAILED( m_pDD->SetDisplayMode( w, h, bpp, 0, 0 ) ) ) {
		return E_FAIL;
	}

	// Create the primary surface
	DDSURFACEDESC2 ddsd;
	::ZeroMemory( &ddsd, sizeof( ddsd ) );
	ddsd.dwSize            = sizeof( ddsd );
	ddsd.dwFlags           = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps    = DDSCAPS_PRIMARYSURFACE;

	if( FAILED( hr = m_pDD->CreateSurface( &ddsd, &m_pdds, NULL ) ) ) {
		return E_FAIL;
	}

	m_ddsd.dwSize = sizeof( m_ddsd );
	m_pdds->GetSurfaceDesc( &m_ddsd );

	RECT rc;
	::SetRect( &rc, 0, 0, width, height );
	::AdjustWindowRectEx( &rc, parentWnd->GetStyle(), parentWnd->GetMenu() != NULL, parentWnd->GetExStyle() );
	parentWnd->SetWindowPos( NULL, 0, 0, rc.right - rc.left, rc.bottom - rc.top, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE );
	parentWnd->SetWindowPos( &CWnd::wndNoTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE );

	return S_OK;
}
