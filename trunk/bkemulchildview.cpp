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
const int timer_ID  = 1;

BEGIN_MESSAGE_MAP(BKChildView,CWnd )
	//{{AFX_MSG_MAP(BKChildView)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BKChildView::BKChildView():
	CWnd(),
	display( NULL ),
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
	grayColor( 0 ),
	windowRect( 0, 0, 0, 0 ),
	screenRect( 0, 0, 0, 0 ),
	lock( false ),
	timer( 0 )
{
	::SystemParametersInfo( SPI_GETWORKAREA, 0, &screenRect, 0 );
	updateVideoMemory.reserve( emul.video.getMemorySize() );
}

BKChildView::~BKChildView()
{
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

	updateBounds();
	initDirectDraw();

	timer = SetTimer( timer_ID, 20, NULL );

	return 0;
}

void BKChildView::OnClose()
{
	if ( timer ) {
		KillTimer( timer );
		timer = 0;
	}
	CWnd::OnClose();
}

HRESULT BKChildView::initDirectDraw()
{
	display = new CDisplay();

	if ( display->CreateWindowedDisplay( this, bk_width, bk_height ) != S_OK ) {
		AfxMessageBox( "Failed initializing DirectDraw." );
		return -1;
	}

	bytePerPixel = display->getSurfaceDesc()->ddpfPixelFormat.dwRGBBitCount >> 3;
	if ( bytePerPixel > 1 ) {
		DWORD rMask = display->getSurfaceDesc()->ddpfPixelFormat.dwRBitMask;
		DWORD gMask = display->getSurfaceDesc()->ddpfPixelFormat.dwGBitMask;
		DWORD bMask = display->getSurfaceDesc()->ddpfPixelFormat.dwBBitMask;
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
	grayColor = getColor( RGB( 0xF0, 0xF0, 0xF0 ) );
//	grayColor = getColor( RGB( 0x80, 0x80, 0x80 ) );

	pitch = display->getSurfaceDesc()->lPitch;

	return S_OK;
}

void BKChildView::OnPaint()
{
	CPaintDC dc( this );
	updateMonitor();
	CRect rect;
	GetClientRect( rect );
	if ( rect.Width() > bk_width ) {
		dc.FillSolidRect( bk_width, 0, rect.right - bk_width, rect.bottom, RGB( 0xFF, 0xFF, 0xFF ) );
	}
	if ( rect.Height() > bk_height ) {
		dc.FillSolidRect( 0, bk_height, bk_width, rect.bottom - bk_height, RGB( 0xFF, 0xFF, 0xFF ) );
	}
}

HRESULT BKChildView::lockSurface() const
{
	try {
		HRESULT rValue = display->getSurface()->Lock( NULL, display->getSurfaceDesc(), DDLOCK_WAIT, NULL );
		if ( rValue != DD_OK ) {
			if ( rValue == DDERR_SURFACELOST ) {
				restoreSurfaces();
				if ( rValue != DD_OK ) return false;
				return display->getSurface()->Lock( NULL, display->getSurfaceDesc(), DDLOCK_WAIT, NULL ) == DD_OK;
			} else {
				return DD_FALSE;
			}
		}
		return DD_OK;
	} catch ( ... ) {
		return DD_FALSE;
	}
}

HRESULT BKChildView::unlockSurface() const
{
	return display->getSurface()->Unlock( NULL );
}

void BKChildView::draw( const BYTE* bk_video_from, int count_byte, BYTE BK_byte_X, BYTE BK_line_Y ) const
{
	BYTE* lpSurfMemory = static_cast<BYTE*>(display->getSurfaceDesc()->lpSurface);
	if ( !lpSurfMemory ) return;
	bool colorMonitor = emul.video.isColorMonitor();
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
			while ( count_byte-- ) {
				BYTE bk_byte_value = *bk_video_from++;
				BYTE maska = 0001;
				int y = windowRect.top + BK_line_Y;
				if ( y < screenRect.bottom && y >= 0 && y < windowRect.bottom ) {
					int t_memory1 = reinterpret_cast<int>(lpSurfMemory + y * pitch);
					int t_memory2 = windowRect.left + (BK_byte_X << 3);
					if ( colorMonitor ) {
						WORD* pByte = reinterpret_cast<WORD*>( t_memory1 + t_memory2 * bytePerPixel );
						for ( int i = 0; i < 4; i++ ) {
							if ( t_memory2 >= screenRect.left && t_memory2 + 1 <= screenRect.right && t_memory2 + 1 <= windowRect.right ) {
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
								*pByte++ = color;
								*pByte++ = color;
							} else {
								pByte += 2;
								maska <<= 2;
							}
							t_memory2 += 2;
						}
					} else {
						color = grayColor;
						WORD* pByte = reinterpret_cast<WORD*>( t_memory1 + t_memory2 * bytePerPixel );
						for ( int i = 0; i < 8; i++ ) {
							if ( t_memory2 >= screenRect.left && t_memory2 <= screenRect.right && t_memory2 <= windowRect.right ) {
								*pByte++ = bk_byte_value & maska ? color : 0;
							} else {
								pByte++;
							}
							maska <<= 1;
							t_memory2++;
						}
					}
					BK_byte_X++;
					if ( BK_byte_X == 0100 ) {
						BK_byte_X = 0;
						BK_line_Y++;
					}
				} else {
					break;
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
			while ( count_byte-- ) {
				BYTE bk_byte_value = *bk_video_from++;
				BYTE maska = 0001;
				int y = windowRect.top + BK_line_Y;
				if ( y < screenRect.bottom && y >= 0 && y < windowRect.bottom ) {
					int t_memory1 = reinterpret_cast<int>(lpSurfMemory + y * pitch);
					int t_memory2 = windowRect.left + (BK_byte_X << 3);
					if ( colorMonitor ) {
						DWORD* pByte = reinterpret_cast<DWORD*>( t_memory1 + t_memory2 * bytePerPixel );
						for ( int i = 0; i < 4; i++ ) {
							if ( t_memory2 >= screenRect.left && t_memory2 + 1 <= screenRect.right && t_memory2 + 1 <= windowRect.right ) {
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
								*pByte++ = color;
								*pByte++ = color;
							} else {
								pByte += 2;
								maska <<= 2;
							}
							t_memory2 += 2;
						}
					} else {
						color = grayColor;
						DWORD* pByte = reinterpret_cast<DWORD*>( t_memory1 + t_memory2 * bytePerPixel );
						for ( int i = 0; i < 8; i++ ) {
							if ( t_memory2 >= screenRect.left && t_memory2 <= screenRect.right && t_memory2 <= windowRect.right ) {
								*pByte++ = bk_byte_value & maska ? color : 0;
							} else {
								pByte++;
							}
							maska <<= 1;
							t_memory2++;
						}
					}
					BK_byte_X++;
					if ( BK_byte_X == 0100 ) {
						BK_byte_X = 0;
						BK_line_Y++;
					}
				} else {
					break;
				}
			}
			break;
		}
	}
}

HRESULT BKChildView::displayFrame() const
{
	if ( lockSurface() == DD_OK ) {
		if ( emul.isPowerON() ) {
			draw( emul.video.getMemory(), emul.video.getMemorySize() );
		} else {
			BYTE tmp_memory = 0;
			for ( int i = 0; i < emul.video.getMemorySize(); i++ ) {
				draw( &tmp_memory, 1, i & 077, i / 64 );
			}
		}
		unlockSurface();
	}
	return DD_OK;
}

HRESULT BKChildView::restoreSurfaces() const
{
	return display->GetDirectDraw()->RestoreAllSurfaces();
}

DWORD BKChildView::getColor( const COLORREF color ) const
{
	return ( GetRValue( color ) >> rBits ) << rZero |
	       ( GetGValue( color ) >> gBits ) << gZero |
		   ( GetBValue( color ) >> bBits ) << bZero;
}

void BKChildView::updateMonitor() const
{
	if ( displayFrame() == DDERR_SURFACELOST ) {
		restoreSurfaces();
		displayFrame();
	}
}

void BKChildView::updateScrolling( BYTE delta ) const
{
}

void BKChildView::updateBounds()
{
	GetWindowRect( windowRect );
}

void BKChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonDown( nFlags, point );

	return;

	for ( int i = 040000; i < 0100000; i++ ) {
		emul.video.setMemoryWord( i, 031463 + point.x );
//		emul.video.setMemoryWord( i, 031463 );
	}
}

void BKChildView::OnTimer(UINT nIDEvent)
{
	CWnd::OnTimer( nIDEvent );
	if ( nIDEvent == timer ) {
		if ( !lock ) {
/*
			static int fps      = 0;
			static int mseconds = 0;
			fps++;
			mseconds += 20;
			if ( mseconds == 1000 ) {
				CClientDC dc( this );
				std::string str = format( "fps = %d", fps );
				dc.TextOut( 550, 0, str.c_str(), str.length() );
				fps      = 0;
				mseconds = 0;
			}
*/
			std::vector< WORD >::const_iterator it = updateVideoMemory.begin();
			if ( it != updateVideoMemory.end() && lockSurface() == DD_OK ) {
				lock = true;
				if ( emul.isPowerON() ) {
					while ( it != updateVideoMemory.end() ) {
						draw( emul.video.getMemory( *it ), 1, *it & 077, (*it - 040000) / 64 );
						it++;
					}
				}
				updateVideoMemory.clear();
				unlockSurface();
				lock = false;
			}
		}
	}
}
