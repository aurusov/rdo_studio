#include "stdafx.h"
#include "bkemulmainfrm.h"
#include "bkemul.h"
#include "resource.h"

#include <ddraw.h>
#include "ddutil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------
// ---------- BKMainFrame
// --------------------------------------------------------------
#define SAFE_DELETE( p )  { if( p ) { delete ( p );     ( p ) = NULL; } }
#define SAFE_RELEASE( p ) { if( p ) { ( p )->Release(); ( p ) = NULL; } }
const int bk_width  = 512;
const int bk_height = 256;
const int timer_ID  = 1;

IMPLEMENT_DYNAMIC(BKMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(BKMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(BKMainFrame)
	ON_WM_CREATE()
	ON_WM_MOVE()
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_GETMINMAXINFO()
	ON_COMMAND(ID_VIEW_FULLSCREEN, OnViewFullScreen)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FULLSCREEN, OnUpdateViewFullScreen)
	ON_COMMAND(ID_VIEW_COLORMONITOR, OnViewColorMonitor)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COLORMONITOR, OnUpdateViewColorMonitor)
	ON_COMMAND(ID_EMULATOR_RESET, OnEmulatorReset)
	ON_COMMAND(ID_EMULATOR_SOFTRESET, OnEmulatorSoftReset)
	ON_COMMAND(ID_EMULATOR_POWERON, OnEmulatorPowerOn)
	ON_COMMAND(ID_EMULATOR_POWEOFF, OnEmulatorPoweOff)
	ON_UPDATE_COMMAND_UI(ID_EMULATOR_POWERON, OnUpdateEmulatorPowerOn)
	ON_UPDATE_COMMAND_UI(ID_EMULATOR_POWEOFF, OnUpdateEmulatorPoweOff)
	ON_UPDATE_COMMAND_UI(ID_EMULATOR_RESET, OnUpdateEmulatorReset)
	ON_UPDATE_COMMAND_UI(ID_EMULATOR_SOFTRESET, OnUpdateEmulatorSoftReset)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE( ID_VIEW_FONT_DEFAULT, ID_VIEW_FONT_WORKS, OnFontClicked )
END_MESSAGE_MAP()

BKMainFrame::BKMainFrame():
	CFrameWnd(),
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
	fullScreenMode( false ),
	fullWindowWidth( 0 ),
	fullWindowHeight( 0 ),
	lock( false ),
	timer( 0 )
{
	updateVideoMemory.reserve( emul.video.getMemorySize() );
}

BKMainFrame::~BKMainFrame()
{
	if ( display ) { delete display; display = NULL; }
}

BOOL BKMainFrame::PreCreateWindow( CREATESTRUCT& cs )
{
	if( !CFrameWnd::PreCreateWindow(cs) ) return FALSE;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass( CS_DBLCLKS, ::LoadCursor( NULL, IDC_ARROW ) );

	return TRUE;
}

int BKMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if ( CFrameWnd::OnCreate( lpCreateStruct ) == -1 ) return -1;

	initDirectDraw();

	timer = SetTimer( timer_ID, 20, NULL );

	GetMenu()->CheckMenuItem( ID_VIEW_FONT_DEFAULT, MF_CHECKED | MF_BYCOMMAND );

	return 0;
}

void BKMainFrame::OnClose()
{
	if ( timer ) {
		KillTimer( timer );
		timer = 0;
	}
	CFrameWnd::OnClose();
}

#ifdef _DEBUG
void BKMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void BKMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif

void BKMainFrame::OnMove( int x, int y )
{
	CFrameWnd::OnMove( x, y );
	updateBounds();
}

void BKMainFrame::RecalcLayout(BOOL bNotify)
{
	CFrameWnd::RecalcLayout(bNotify);
	updateBounds();
}

HRESULT BKMainFrame::initDirectDraw()
{
	if ( display ) { delete display; display = NULL; }
	display = new CDisplay();

	if ( isFullScreenMode() ) {
		CDC* dc = CWnd::GetDesktopWindow()->GetDC();
		int bpp = dc->GetDeviceCaps( BITSPIXEL );
		CWnd::GetDesktopWindow()->ReleaseDC( dc );
		if ( display->CreateFullScreenDisplay( this, 512, 256, bpp ) != S_OK ) {
			AfxMessageBox( "Failed initializing DirectDraw." );
			return -1;
		}
		ModifyStyle( WS_OVERLAPPEDWINDOW, 0 );
		SetMenu( NULL );

		fullWindowWidth  = display->getSurfaceDesc()->dwWidth;
		fullWindowHeight = display->getSurfaceDesc()->dwHeight;
	} else {
		if ( display->CreateWindowedDisplay( this, bk_width, bk_height ) != S_OK ) {
			AfxMessageBox( "Failed initializing DirectDraw." );
			return -1;
		}
		ModifyStyle( 0, WS_OVERLAPPEDWINDOW );
		SetMenu( CMenu::FromHandle( ::LoadMenu( NULL, MAKEINTRESOURCE( IDR_MAINFRAME ) ) ) );
	}
	::SystemParametersInfo( SPI_GETWORKAREA, 0, &screenRect, 0 );
	updateBounds();
	ShowCursor( !fullScreenMode );

	bytePerPixel = display->getSurfaceDesc()->ddpfPixelFormat.dwRGBBitCount >> 3;
	if ( bytePerPixel > 1 ) {
		DWORD rMask = display->getSurfaceDesc()->ddpfPixelFormat.dwRBitMask;
		DWORD gMask = display->getSurfaceDesc()->ddpfPixelFormat.dwGBitMask;
		DWORD bMask = display->getSurfaceDesc()->ddpfPixelFormat.dwBBitMask;
		DWORD mask;
		rBits = 0;
		gBits = 0;
		bBits = 0;
		rZero = 0;
		gZero = 0;
		bZero = 0;
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

	pitch = display->getSurfaceDesc()->lPitch;

	return S_OK;
}

HRESULT BKMainFrame::lockSurface() const
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

HRESULT BKMainFrame::unlockSurface() const
{
	return display->getSurface()->Unlock( NULL );
}

void BKMainFrame::draw( const BYTE* bk_video_from, int count_byte, BYTE BK_byte_X, BYTE BK_line_Y ) const
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

HRESULT BKMainFrame::displayFrame() const
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
		if ( isFullScreenMode() ) {
			CClientDC dc( (CWnd*)this );
			CRect rect;
			GetClientRect( rect );
			COLORREF color = RGB( 0x1F, 0x1F, 0x1F );
			if ( windowRect.left ) {
				dc.FillSolidRect( 0, 0, windowRect.left, rect.bottom, color );
			}
			if ( windowRect.right != rect.right ) {
				dc.FillSolidRect( windowRect.right, 0, rect.right - windowRect.right, rect.bottom, color );
			}
			if ( windowRect.top ) {
				dc.FillSolidRect( 0, 0, rect.right, windowRect.top, color );
			}
			if ( windowRect.bottom != rect.bottom ) {
				dc.FillSolidRect( 0, windowRect.bottom, rect.right, rect.bottom - windowRect.bottom, color );
			}
		}
		unlockSurface();
	}
	return DD_OK;
}

HRESULT BKMainFrame::restoreSurfaces() const
{
	return display->GetDirectDraw()->RestoreAllSurfaces();
}

DWORD BKMainFrame::getColor( const COLORREF color ) const
{
	return ( GetRValue( color ) >> rBits ) << rZero |
	       ( GetGValue( color ) >> gBits ) << gZero |
		   ( GetBValue( color ) >> bBits ) << bZero;
}

void BKMainFrame::updateMonitor() const
{
	if ( displayFrame() == DDERR_SURFACELOST ) {
		restoreSurfaces();
		displayFrame();
	}
}

void BKMainFrame::updateScrolling( BYTE delta ) const
{
}

void BKMainFrame::updateBounds()
{
	GetClientRect( windowRect );
	ClientToScreen( windowRect );
	if ( isFullScreenMode() && fullWindowWidth && fullWindowHeight ) {
		windowRect.left  += (fullWindowWidth - bk_width) / 2;
		windowRect.right  = windowRect.left + bk_width;
		windowRect.top   += (fullWindowHeight - bk_height) / 2;
		windowRect.bottom = windowRect.top + bk_height;
	}
}

void BKMainFrame::OnPaint()
{
	CPaintDC dc( this );
	updateMonitor();
}

void BKMainFrame::OnLButtonDown(UINT nFlags, CPoint point)
{
	CFrameWnd::OnLButtonDown(nFlags, point);

	return;

	for ( int i = 040000; i < 0100000; i++ ) {
		emul.video.setMemoryWord( i, 031463 + point.x );
//		emul.video.setMemoryWord( i, 031463 );
	}
}

void BKMainFrame::OnTimer(UINT nIDEvent)
{
	CFrameWnd::OnTimer( nIDEvent );

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

void BKMainFrame::OnGetMinMaxInfo( MINMAXINFO FAR* lpMMI )
{
	if ( isFullScreenMode() ) {
		if ( display && display->getSurfaceDesc() ) {
			lpMMI->ptMinTrackSize.x = display->getSurfaceDesc()->dwWidth;
			lpMMI->ptMinTrackSize.y = display->getSurfaceDesc()->dwHeight;
		}
	} else {
		DWORD dwFrameWidth    = ::GetSystemMetrics( SM_CXSIZEFRAME );
		DWORD dwFrameHeight   = ::GetSystemMetrics( SM_CYSIZEFRAME );
		DWORD dwMenuHeight    = ::GetSystemMetrics( SM_CYMENU );
		DWORD dwCaptionHeight = ::GetSystemMetrics( SM_CYCAPTION );
		lpMMI->ptMinTrackSize.x = bk_width  + dwFrameWidth * 2;
		lpMMI->ptMinTrackSize.y = bk_height + dwFrameHeight * 2 + dwMenuHeight + dwCaptionHeight;
	}
	lpMMI->ptMaxTrackSize.x = lpMMI->ptMinTrackSize.x;
	lpMMI->ptMaxTrackSize.y = lpMMI->ptMinTrackSize.y;

	CFrameWnd::OnGetMinMaxInfo(lpMMI);
}

void BKMainFrame::setFullScreenMode( const bool value )
{
	if ( value != fullScreenMode ) {
		fullScreenMode = value;
		initDirectDraw();
		ShowCursor( !fullScreenMode );
	}
}

void BKMainFrame::OnViewFullScreen()
{
	setFullScreenMode( !isFullScreenMode() );
}

void BKMainFrame::OnUpdateViewFullScreen(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck( isFullScreenMode() );
}

void BKMainFrame::OnViewColorMonitor()
{
	emul.video.setColorMonitor( !emul.video.isColorMonitor() );
}

void BKMainFrame::OnUpdateViewColorMonitor(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck( emul.video.isColorMonitor() );
}

void BKMainFrame::OnFontClicked( UINT nID )
{
	for ( int i = ID_VIEW_FONT_DEFAULT; i <= ID_VIEW_FONT_WORKS; i++ ) {
		AfxGetMainWnd()->GetMenu()->CheckMenuItem( i, MF_UNCHECKED | MF_BYCOMMAND );
	}
	CString s = "";
	AfxGetMainWnd()->GetMenu()->GetMenuString( nID, s, MF_BYCOMMAND );
	AfxGetMainWnd()->GetMenu()->CheckMenuItem( nID, MF_CHECKED | MF_BYCOMMAND );
	if ( !s.IsEmpty() ) {
		s.MakeUpper();
		emul.loadFont( static_cast<LPCSTR>(s) );
	}
}

void BKMainFrame::OnEmulatorPowerOn()
{
	emul.powerON();
}

void BKMainFrame::OnEmulatorPoweOff()
{
	emul.powerOFF();
}

void BKMainFrame::OnUpdateEmulatorPowerOn(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( !emul.isPowerON() );
}

void BKMainFrame::OnUpdateEmulatorPoweOff(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( emul.isPowerON() );
}

void BKMainFrame::OnEmulatorReset() 
{
	emul.reset();
}

void BKMainFrame::OnEmulatorSoftReset()
{
	emul.softReset();
}

void BKMainFrame::OnUpdateEmulatorReset(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( emul.isPowerON() );
}

void BKMainFrame::OnUpdateEmulatorSoftReset(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( emul.isPowerON() );
}
