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

CDisplay::CDisplay():
	m_pDD( NULL ),
	m_pdds( NULL )
{
}

CDisplay::~CDisplay()
{
	DestroyObjects();
}

HRESULT CDisplay::DestroyObjects()
{
	SAFE_RELEASE( m_pdds );

	if( m_pDD )
		m_pDD->SetCooperativeLevel( parentWnd->m_hWnd, DDSCL_NORMAL );

	SAFE_RELEASE( m_pDD );

	return S_OK;
}

HRESULT CDisplay::CreateWindowedDisplay( CWnd* _parentWnd, int _width, int _height )
{
	parentWnd = _parentWnd;
	width     = _width;
	height    = _height;

	HRESULT hr;

	// Cleanup anything from a previous call
	DestroyObjects();

	// DDraw stuff begins here
	if( FAILED( hr = DirectDrawCreateEx( NULL, (VOID**)&m_pDD, IID_IDirectDraw7, NULL ) ) )
		return E_FAIL;

	// Set cooperative level
	hr = m_pDD->SetCooperativeLevel( parentWnd->m_hWnd, DDSCL_NORMAL );
	if( FAILED(hr) )
		return E_FAIL;

	RECT  rcWork;
	RECT  rc;

	// Aet window size
	SetRect( &rc, 0, 0, width, height );

	AdjustWindowRectEx( &rc, GetWindowStyle(parentWnd->m_hWnd), GetMenu(parentWnd->m_hWnd) != NULL, GetWindowExStyle(parentWnd->m_hWnd) );

	SetWindowPos( parentWnd->m_hWnd, NULL, 0, 0, rc.right-rc.left, rc.bottom-rc.top, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE );

	SetWindowPos( parentWnd->m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE );

	//  Make sure our window does not hang outside of the work area
	SystemParametersInfo( SPI_GETWORKAREA, 0, &rcWork, 0 );
	GetWindowRect( parentWnd->m_hWnd, &rc );
	if( rc.left < rcWork.left ) rc.left = rcWork.left;
	if( rc.top  < rcWork.top )  rc.top  = rcWork.top;
	SetWindowPos( parentWnd->m_hWnd, NULL, rc.left, rc.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE );

	LPDIRECTDRAWCLIPPER pcClipper;

	// Create the primary surface
	DDSURFACEDESC2 ddsd;
	ZeroMemory( &ddsd, sizeof( ddsd ) );
	ddsd.dwSize         = sizeof( ddsd );
	ddsd.dwFlags        = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

	if( FAILED( m_pDD->CreateSurface( &ddsd, &m_pdds, NULL ) ) )
		return E_FAIL;

	m_ddsd.dwSize = sizeof( m_ddsd );
	m_pdds->GetSurfaceDesc( &m_ddsd );

	if( FAILED( hr = m_pDD->CreateClipper( 0, &pcClipper, NULL ) ) )
		return E_FAIL;

	if( FAILED( hr = pcClipper->SetHWnd( 0, parentWnd->m_hWnd ) ) )
	{
		pcClipper->Release();
		return E_FAIL;
	}

	if( FAILED( hr = m_pdds->SetClipper( pcClipper ) ) )
	{
		pcClipper->Release();
		return E_FAIL;
	}

	// Done with clipper
	pcClipper->Release();

	return S_OK;
}

HRESULT CDisplay::CreateFullScreenDisplay( CWnd* _parentWnd, int _width, int _height, int _bpp )
{
	parentWnd = _parentWnd;
	width     = _width;
	height    = _height;

	HRESULT hr;

	// Cleanup anything from a previous call
	DestroyObjects();

	// DDraw stuff begins here
	if( FAILED( hr = DirectDrawCreateEx( NULL, (VOID**)&m_pDD, IID_IDirectDraw7, NULL ) ) )
		return E_FAIL;

	// Set cooperative level
	hr = m_pDD->SetCooperativeLevel( parentWnd->m_hWnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN );
	if( FAILED(hr) )
		return E_FAIL;

	// Set the display mode
	if( FAILED( m_pDD->SetDisplayMode( width, height, _bpp, 0, 0 ) ) )
		return E_FAIL;

	// Create primary surface (with backbuffer attached)
	DDSURFACEDESC2 ddsd;
	ZeroMemory( &ddsd, sizeof( ddsd ) );
	ddsd.dwSize            = sizeof( ddsd );
	ddsd.dwFlags           = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
	ddsd.ddsCaps.dwCaps    = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX | DDSCAPS_3DDEVICE;
	ddsd.dwBackBufferCount = 1;

	if( FAILED( hr = m_pDD->CreateSurface( &ddsd, &m_pdds, NULL ) ) )
		return E_FAIL;

	return S_OK;
}
