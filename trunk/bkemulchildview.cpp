#include "stdafx.h"
#include "bkemulchildview.h"
#include "bkemul.h"

#include <ddraw.h>
#include "ddutil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------
// ---------- BKChildView
// --------------------------------------------------------------
#define SAFE_DELETE( p )  { if( p ) { delete ( p );     ( p ) = NULL; } }
#define SAFE_RELEASE( p ) { if( p ) { ( p )->Release(); ( p ) = NULL; } }
const int bk_width  = 512;
const int bk_height = 256;

BEGIN_MESSAGE_MAP(BKChildView,CWnd )
	//{{AFX_MSG_MAP(BKChildView)
	ON_WM_PAINT()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BKChildView::BKChildView():
	CWnd(),
	display( NULL ),
	surface( NULL ),
	bytePerPixel( 0 ),
	pitch( 0 ),
	rBits( 0 ),
	gBits( 0 ),
	bBits( 0 ),
	rZero( 0 ),
	gZero( 0 ),
	bZero( 0 ),
	rColor( 0 ),
	gColor( 0 ),
	bColor( 0 ),
	grayColor( 0 )
{
}

BKChildView::~BKChildView()
{
	if ( surface ) { delete surface; surface = NULL; }
	if ( display ) { delete display; display = NULL; }
}

BOOL BKChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if ( !CWnd::PreCreateWindow(cs) ) return FALSE;

	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass( CS_DBLCLKS, ::LoadCursor(NULL, IDC_ARROW) /*, reinterpret_cast<HBRUSH>( COLOR_WINDOW + 1 ), NULL*/ );

	return TRUE;
}

int BKChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if ( CWnd ::OnCreate(lpCreateStruct) == -1 ) return -1;

	initDirectDraw();

	return 0;
}

DWORD BKChildView::getColor( const COLORREF color ) const
{
	return ( GetRValue( color ) >> rBits ) << rZero |
	       ( GetGValue( color ) >> gBits ) << gZero |
		   ( GetBValue( color ) >> bBits ) << bZero;
}

HRESULT BKChildView::initDirectDraw()
{
	display = new CDisplay();

	if ( display->CreateWindowedDisplay( this, bk_width, bk_height ) != S_OK ) {
		AfxMessageBox( "Failed initializing DirectDraw." );
		return -1;
	}

	if ( display->CreateSurface( &surface, bk_width, bk_height ) != S_OK ) {
		return -1;
	}

	bytePerPixel = surface->GetSurfaceDesc()->ddpfPixelFormat.dwRGBBitCount >> 3;
	if ( bytePerPixel > 1 ) {
		DWORD rMask = surface->GetSurfaceDesc()->ddpfPixelFormat.dwRBitMask;
		DWORD gMask = surface->GetSurfaceDesc()->ddpfPixelFormat.dwGBitMask;
		DWORD bMask = surface->GetSurfaceDesc()->ddpfPixelFormat.dwBBitMask;
		DWORD mask;
		mask = rMask;
		while ( (mask & 0x1) == 0 ) {
			mask = mask >> 1;
			rZero++;
		}
		while ( mask & 0x1 ) {
			mask = mask >> 1;
			rBits++;
		}
		mask = gMask;
		while ( (mask & 0x1) == 0 ) {
			mask = mask >> 1;
			gZero++;
		}
		while ( mask & 0x1 ) {
			mask = mask >> 1;
			gBits++;
		}
		mask = bMask;
		while ( (mask & 0x1) == 0 ) {
			mask = mask >> 1;
			bZero++;
		}
		while ( mask & 0x1 ) {
			mask = mask >> 1;
			bBits++;
		}
		rBits = 8 - rBits;
		gBits = 8 - gBits;
		bBits = 8 - bBits;
	}
	rColor    = getColor( RGB( 0xFF, 0x00, 0x00 ) );
	gColor    = getColor( RGB( 0x00, 0xFF, 0x00 ) );
	bColor    = getColor( RGB( 0x00, 0x00, 0xFF ) );
	grayColor = getColor( RGB( 0x80, 0x80, 0x80 ) );

    pitch = surface->GetSurfaceDesc()->lPitch;

	return S_OK;
}

void BKChildView::OnPaint()
{
	CPaintDC dc( this );
	if ( displayFrame() == DDERR_SURFACELOST ) {
		restoreSurfaces();
		displayFrame();
	}
	CRect rect;
	GetClientRect( rect );
	if ( rect.Width() > bk_width ) {
		dc.FillSolidRect( bk_width, 0, rect.right - bk_width, rect.bottom, RGB( 0xFF, 0xFF, 0xFF ) );
	}
	if ( rect.Height() > bk_height ) {
		dc.FillSolidRect( 0, bk_height, bk_width, rect.bottom - bk_height, RGB( 0xFF, 0xFF, 0xFF ) );
	}
}

bool BKChildView::lockSurface()
{
	try {
		HRESULT rValue = surface->GetDDrawSurface()->Lock( NULL, surface->GetSurfaceDesc(), DDLOCK_WAIT, NULL );
		if ( rValue != DD_OK ) {
			if ( rValue == DDERR_SURFACELOST ) {
				restoreSurfaces();
				if ( rValue != DD_OK ) return false;
				return surface->GetDDrawSurface()->Lock( NULL, surface->GetSurfaceDesc(), DDLOCK_WAIT, NULL ) == DD_OK;
			} else {
				return false;
			}
		}
		return true;
	} catch ( ... ) {
		return false;
	}
}

void BKChildView::unlockSurface()
{
	surface->GetDDrawSurface()->Unlock( NULL );
}

HRESULT BKChildView::displayFrame()
{
	if ( lockSurface() ) {
		BYTE* lpSurfMemory = static_cast<BYTE*>(surface->GetSurfaceDesc()->lpSurface);
		const unsigned char* bk_video = emul.getVideoMemory();
		int count                     = emul.getVideoMemorySize();
		int  BK_byte_X = 0;
		BYTE BK_line_Y = 0;
		bool colorMonitor = emul.isColorMonitor();
		switch ( bytePerPixel ) {
			case 2: {
//				BYTE BK_line_Y = (BYTE)((~((BK->Memory_word[0177664] & 0xFF) - (BYTE)0330))+1);
/*
				// Режим вывода 1/4 водео-памяти (адреса 070000-077777)
				bool smallVideo = !(BK->Memory_word[0177664] & 0001000);
				if (smallVideo) {
					bk_video  += 0300*0100;
					count     -= 0300*0100;
					BK_line_Y += (BYTE)0300;
				}
*/
				WORD color;
				while ( count-- ) {
					BYTE bk_byte_value = *bk_video++;
					BYTE maska = 0001;
					int t_memory1 = reinterpret_cast<int>(lpSurfMemory + BK_line_Y * pitch);
					int t_memory2 = BK_byte_X << 3;
					if ( colorMonitor ) {
						for ( int i = 0; i < 4; i++ ) {
							bool bit0 = bk_byte_value & maska ? true : false;
							maska <<= 1;
							bool bit1 = bk_byte_value & maska ? true : false;
							maska <<= 1;
							color = 0;
							if ( bit0 && bit1 ) {
								color = static_cast<WORD>(rColor);
							} else if ( bit0 && !bit1 ) {
								color = static_cast<WORD>(bColor);
							} else if ( !bit0 && bit1 ) {
								color = static_cast<WORD>(gColor);
							}
							WORD* pByte = reinterpret_cast<WORD*>( t_memory1 + (t_memory2 + i*2) * bytePerPixel );
							*pByte++ = color;
							*pByte   = color;
						}
					} else {
						color = static_cast<WORD>(grayColor);
						WORD* pByte = reinterpret_cast<WORD*>( t_memory1 + (t_memory2) * bytePerPixel );
						for ( int i = 0; i < 8; i++ ) {
							*pByte++ = bk_byte_value & maska ? color : 0;
							maska <<= 1;
						}
					}
					BK_byte_X++;
					if ( BK_byte_X == 0100 ) {
						BK_byte_X = 0;
						BK_line_Y++;
					}
				}
/*
				// Очистить нижние 3/4 экрана монитора в случае вывода только 1/4 видео-памяти
				if (smallVideo) {
					int lineCount = 0300;
					while (lineCount--) {
						int t_memory = (int)(lpSurfMemory + (BK_rect.top + BK_line_Y) * pitch);
						for (int i = 0; i < 8*0100; i++) {
							*(WORD*)(t_memory + (BK_rect.left + i) * bytePerPixel) = (WORD)0;
						}
						BK_line_Y++;
					}
				}
*/
				break;
			}
			case 4: {
				DWORD color;
				while ( count-- ) {
					BYTE bk_byte_value = *bk_video++;
					BYTE maska = 0001;
					int t_memory1 = reinterpret_cast<int>(lpSurfMemory + BK_line_Y * pitch);
					int t_memory2 = BK_byte_X << 3;
					if ( colorMonitor ) {
						for ( int i = 0; i < 4; i++ ) {
							bool bit0 = bk_byte_value & maska ? true : false;
							maska <<= 1;
							bool bit1 = bk_byte_value & maska ? true : false;
							maska <<= 1;
							color = 0;
							if ( bit0 && bit1 ) {
								color = rColor;
							} else if ( bit0 && !bit1 ) {
								color = bColor;
							} else if ( !bit0 && bit1 ) {
								color = gColor;
							}
							DWORD* pByte = reinterpret_cast<DWORD*>( t_memory1 + (t_memory2 + i*2) * bytePerPixel );
							*pByte++ = color;
							*pByte   = color;
						}
					} else {
						color = grayColor;
						DWORD* pByte = reinterpret_cast<DWORD*>( t_memory1 + (t_memory2) * bytePerPixel );
						for ( int i = 0; i < 8; i++ ) {
							*pByte++ = bk_byte_value & maska ? color : 0;
							maska <<= 1;
						}
					}
					BK_byte_X++;
					if ( BK_byte_X == 0100 ) {
						BK_byte_X = 0;
						BK_line_Y++;
					}
				}
				break;
			}
		}
		unlockSurface();
	}
	display->Blt( 0, 0, surface, NULL );
	return display->Present();
}

HRESULT BKChildView::restoreSurfaces()
{
	return display->GetDirectDraw()->RestoreAllSurfaces();
}

void BKChildView::updateBounds()
{
	display->UpdateBounds();
	InvalidateRect( NULL );
	UpdateWindow();
}
