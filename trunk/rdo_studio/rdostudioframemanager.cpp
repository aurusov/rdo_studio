#include "stdafx.h"
#include "rdostudioframemanager.h"
#include "rdostudiomodel.h"
#include "rdostudioframedoc.h"
#include "rdostudioframeview.h"
#include "rdostudioapp.h"
#include "rdostudiomainfrm.h"
#include "rdostudiochildfrm.h"
#include "rdostudioworkspace.h"
#include "rdostudioframetreectrl.h"
#include "resource.h"

#include <rdokernel.h>
#include <rdorepository.h>
#include <fstream>

using namespace std;
using namespace RDOSimulatorNS;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RDOStudioFrameManager
// ----------------------------------------------------------------------------
vector< RDOStudioFrameManager::Frame* > RDOStudioFrameManager::frames;

RDOStudioFrameManager::RDOStudioFrameManager():
	frameDocTemplate( NULL ),
	lastShowedFrame( -1 )
{
	frameDocTemplate = new FrameDocTemplate( IDR_FRAMETYPE, RUNTIME_CLASS(RDOStudioFrameDoc), RUNTIME_CLASS(RDOStudioChildFrame), RUNTIME_CLASS(RDOStudioFrameView) );
	AfxGetApp()->AddDocTemplate( frameDocTemplate );

	dcBmp.CreateCompatibleDC( NULL );
	dcMask.CreateCompatibleDC( NULL );
}

RDOStudioFrameManager::~RDOStudioFrameManager()
{
	bmp_clear();
	vector< Frame* >::iterator it = frames.begin();
	while ( it != frames.end() ) {
		delete *it++;
	};
}

void RDOStudioFrameManager::insertItem( const string& name )
{
	Frame* item = new Frame;
	item->hitem = studioApp.mainFrame->workspace.frames->InsertItem( name.c_str(), 1, 1, studioApp.mainFrame->workspace.frames->GetRootItem() );
	item->name  = name;
	item->doc   = NULL;
	item->view  = NULL;
	frames.push_back( item );
}

int RDOStudioFrameManager::findFrameIndex( const HTREEITEM hitem ) const
{
	vector< Frame* >::iterator it = frames.begin();
	int index = 0;
	while ( it != frames.end() ) {
		if ( (*it)->hitem == hitem ) {
			return index;
		}
		it++;
		index++;
	};
	return -1;
}

int RDOStudioFrameManager::findFrameIndex( const RDOStudioFrameDoc* doc ) const
{
	vector< Frame* >::iterator it = frames.begin();
	int index = 0;
	while ( it != frames.end() ) {
		if ( (*it)->doc == doc ) {
			return index;
		}
		it++;
		index++;
	};
	return -1;
}

int RDOStudioFrameManager::findFrameIndex( const RDOStudioFrameView* view ) const
{
	vector< Frame* >::iterator it = frames.begin();
	int index = 0;
	while ( it != frames.end() ) {
		if ( (*it)->view == view ) {
			return index;
		}
		it++;
		index++;
	};
	return -1;
}

RDOStudioFrameDoc* RDOStudioFrameManager::connectFrameDoc( const int index )
{
	RDOStudioFrameDoc* doc = NULL;
	if ( index != -1 ) {
		CSingleLock lock( getFrameUsed( index ) );
		lock.Lock();

		doc = static_cast<RDOStudioFrameDoc*>(frameDocTemplate->OpenDocumentFile( NULL ));
		if ( doc ) {
			frames[index]->doc  = doc;
			frames[index]->view = doc->getView();
			lastShowedFrame     = index;
		}

		lock.Unlock();
	}
	return doc;
}

void RDOStudioFrameManager::disconnectFrameDoc( const RDOStudioFrameDoc* doc ) const
{
	int index = findFrameIndex( doc );
	if ( index != -1 ) {

		CSingleLock lock( getFrameUsed( index ) );
		lock.Lock();

		frames[index]->doc  = NULL;
		frames[index]->view = NULL;

		lock.Unlock();
	}
}

void RDOStudioFrameManager::closeAll()
{
	int backup = lastShowedFrame;
	vector< Frame* >::iterator it = frames.begin();
	while ( it != frames.end() ) {
		RDOStudioFrameDoc* doc = (*it)->doc;
		if ( isValidFrameDoc( doc ) ) {
			if ( doc->frame && doc->frame->GetSafeHwnd() ) {
				doc->frame->SendNotifyMessage( WM_CLOSE, 0, 0 );
			}
		}
		it++;
	};
	lastShowedFrame = backup;
}

void RDOStudioFrameManager::clear()
{
	studioApp.mainFrame->workspace.frames->deleteChildren( studioApp.mainFrame->workspace.frames->GetRootItem() );
	vector< Frame* >::iterator it = frames.begin();
	while ( it != frames.end() ) {
		RDOStudioFrameDoc* doc = (*it)->doc;
		if ( isValidFrameDoc( doc ) ) {
			if ( doc->frame && doc->frame->GetSafeHwnd() ) {
				doc->frame->SendNotifyMessage( WM_CLOSE, 0, 0 );
			}
		}
		delete *it++;
	};
	frames.clear();
	lastShowedFrame = -1;
}

RDOStudioFrameDoc* RDOStudioFrameManager::getFirstExistDoc() const
{
	vector< Frame* >::const_iterator it = frames.begin();
	while ( it != frames.end() ) {
		if ( isValidFrameDoc( (*it)->doc ) ) {
			return (*it)->doc;
		}
		it++;
	};
	return NULL;
}

void RDOStudioFrameManager::expand() const
{
	studioApp.mainFrame->workspace.frames->expand();
}

void RDOStudioFrameManager::setLastShowedFrame( const int value )
{
	if ( value >= 0 && value < count() ) {
		lastShowedFrame = value;
	}
}

bool RDOStudioFrameManager::isValidFrameDoc( const RDOStudioFrameDoc* const frame ) const
{
	POSITION pos = frameDocTemplate->GetFirstDocPosition();
	while ( pos ) {
		RDOStudioFrameDoc* doc = static_cast<RDOStudioFrameDoc*>(frameDocTemplate->GetNextDoc( pos ));
		if ( frame == doc ) {
			return true;
		}
	}

	return false;
}

void RDOStudioFrameManager::bmp_insert( const std::string& name )
{
	bitmaps[name] = NULL;

	stringstream stream( ios::in | ios_base::out | ios::binary );
	kernel.getRepository()->loadBMP( name, stream );

	HANDLE hDIBInfo = ::GlobalAlloc( GMEM_MOVEABLE, sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD) );

	BITMAPINFOHEADER* lpbi = static_cast<BITMAPINFOHEADER*>(::GlobalLock( hDIBInfo ));

	try {
		BITMAPFILEHEADER bf;
		stream.read( (LPSTR)(&bf), sizeof(bf) );
		if ( stream.rdstate() != ios_base::goodbit ) throw BMPReadError();
		if ( bf.bfType != 0x4D42 ) throw BMPReadError();

		stream.read( (LPSTR)(lpbi), sizeof(BITMAPINFOHEADER) );
		if ( stream.rdstate() != ios_base::goodbit ) throw BMPReadError();

		if ( lpbi->biSize == sizeof(BITMAPCOREHEADER) ) throw BMPReadError();

	    WORD nNumColors;
		if ( !(nNumColors = (WORD)lpbi->biClrUsed) ) {
			if ( lpbi->biBitCount != 24 ) {
				nNumColors = 1 << lpbi->biBitCount;
			}
		}
		if ( !lpbi->biClrUsed ) {
			lpbi->biClrUsed = nNumColors;
		}

		if ( !lpbi->biSizeImage ) {
			lpbi->biSizeImage = ((((lpbi->biWidth * (DWORD)lpbi->biBitCount) + 31) & ~31) >> 3) * lpbi->biHeight;
		}

		::GlobalUnlock( hDIBInfo );
		hDIBInfo = ::GlobalReAlloc( hDIBInfo, lpbi->biSize + nNumColors * sizeof(RGBQUAD) + lpbi->biSizeImage, 0 );
		lpbi     = static_cast<BITMAPINFOHEADER*>(::GlobalLock( hDIBInfo ));

		stream.read( (LPSTR)(lpbi) + lpbi->biSize, nNumColors * sizeof(RGBQUAD) );
		if ( stream.rdstate() != ios_base::goodbit ) throw BMPReadError();

		int offBits = lpbi->biSize + nNumColors * sizeof(RGBQUAD);

		if ( bf.bfOffBits ) {
			stream.seekg( bf.bfOffBits, ios::beg );
			if ( stream.rdstate() != ios_base::goodbit ) throw BMPReadError();
		}
		stream.read( (LPSTR)(lpbi) + offBits, lpbi->biSizeImage );
		if ( stream.rdstate() != ios_base::goodbit ) throw BMPReadError();

		CDC* desktopDC = CWnd::GetDesktopWindow()->GetDC();
		CDC memDC;
		memDC.CreateCompatibleDC( desktopDC );
		CBitmap memBMP;
		memBMP.CreateCompatibleBitmap( lpbi->biBitCount != 1 ? desktopDC : &memDC, lpbi->biWidth, lpbi->biHeight );
		::SetDIBits( desktopDC->m_hDC, static_cast<HBITMAP>(memBMP), 0, lpbi->biHeight, (LPSTR)(lpbi) + offBits, reinterpret_cast<BITMAPINFO*>(lpbi), DIB_RGB_COLORS );
		CBitmap* hOldBitmap1 = memDC.SelectObject( &memBMP );

		CDC dc;
		dc.CreateCompatibleDC( desktopDC );
		BMP* bmp = new BMP;
		bitmaps[name] = bmp;
		bitmaps[name]->w = lpbi->biWidth;
		bitmaps[name]->h = lpbi->biHeight;
		bitmaps[name]->bmp.CreateCompatibleBitmap( lpbi->biBitCount != 1 ? desktopDC : &dc, lpbi->biWidth, lpbi->biHeight );
		CBitmap* hOldBitmap2 = dc.SelectObject( &bitmaps[name]->bmp );
		dc.BitBlt( 0, 0, lpbi->biWidth, lpbi->biHeight, &memDC, 0, 0, SRCCOPY );

		memDC.SelectObject( hOldBitmap1 );
		dc.SelectObject( hOldBitmap2 );

	} catch ( BMPReadError ) {
		TRACE( "catch for %s\r\n", name.c_str() );
	}

	::GlobalUnlock( hDIBInfo );
	::GlobalFree( hDIBInfo );

/*
	char* pBits = NULL;

	try {

		// В потоке, перед битовой картой, идет заголовок файла битовой карты
		BITMAPFILEHEADER bmFileHeader;
		stream.read( reinterpret_cast<char*>(&bmFileHeader), sizeof(bmFileHeader) );
		if ( stream.rdstate() != ios_base::goodbit ) throw BMPReadError();

		// Проверяем заголовок битовой карты на магическое число "BM"
		if ( bmFileHeader.bfType != 0x4D42 ) throw BMPReadError();

		// Вот теперь читаем сам заголовок битовой карты
		BITMAPINFOHEADER bmInfoHeader;
		stream.read( reinterpret_cast<char*>(&bmInfoHeader), sizeof(bmInfoHeader) );
		if ( stream.rdstate() != ios_base::goodbit ) throw BMPReadError();

		RGBQUAD rgb_q[256];
		memset( &rgb_q, 0, sizeof(rgb_q) );
		int size_ColorTable = 0;

		// Если ПОЛЕ 'размер', только что прочитанной структуры, не соврадает
		// с sizeof(BITMAPINFOHEADER), то остаются два варианта:
		// 1. Это была структура BITMAPCOREHEADER, которая меньше по размеру,
		//    но она нас всетаки устраивает, т.к. мы можем сами дополнить необходимые
		//    поля структуры BITMAPINFOHEADER
		// 2. Это не файл битовой карты, что никуда не годится -> throw
		// Ну а если ПОЛЕ 'размер' соврадает, то мы прочитали то, что нам нужно
		if ( bmInfoHeader.biSize == sizeof(BITMAPINFOHEADER) ) {
			// Читаем палитру из потока
			bmInfoHeader.biClrUsed = getNumColors( &bmInfoHeader );
			size_ColorTable = bmInfoHeader.biClrUsed * sizeof(RGBQUAD);
			if ( size_ColorTable > sizeof(rgb_q) ) throw BMPReadError();

			stream.read( reinterpret_cast<char*>(&rgb_q), size_ColorTable );
			if ( stream.rdstate() != ios_base::goodbit ) throw BMPReadError();

		} else if ( bmInfoHeader.biSize == sizeof(BITMAPCOREHEADER) ) {
			// Необходима конвертация файла PM в DIB
			// Откатываемся в потоке на начало структуры BITMAPCOREHEADER и читаем ее
			stream.seekg( sizeof(BITMAPFILEHEADER), ios::beg );
			BITMAPCOREHEADER bmCoreHeader;
			stream.read( reinterpret_cast<char*>(&bmCoreHeader), sizeof(bmCoreHeader) );
			bmInfoHeader.biSize     = sizeof(BITMAPINFOHEADER);
			bmInfoHeader.biWidth    = bmCoreHeader.bcWidth;
			bmInfoHeader.biHeight   = bmCoreHeader.bcHeight;
			bmInfoHeader.biPlanes   = bmCoreHeader.bcPlanes;
			bmInfoHeader.biBitCount = bmCoreHeader.bcBitCount;
			bmInfoHeader.biCompression = BI_RGB;
			bmInfoHeader.biSizeImage     = 0;
			bmInfoHeader.biXPelsPerMeter = 0;
			bmInfoHeader.biYPelsPerMeter = 0;
			bmInfoHeader.biClrUsed       = getNumColors(&bmInfoHeader);
			if ( bmInfoHeader.biClrUsed <= 256 ) {
				bmInfoHeader.biClrImportant = 0;
				size_ColorTable = bmInfoHeader.biClrUsed * sizeof(RGBQUAD);
				// Читаем каждый PM цевт и переводим его в DIB
				RGBTRIPLE rgb_t;
				for (int i = 0; i < (int)bmInfoHeader.biClrUsed; i++) {
					stream.read( reinterpret_cast<char*>(&rgb_t), sizeof(RGBTRIPLE) );
					rgb_q[i].rgbRed      = rgb_t.rgbtRed;
					rgb_q[i].rgbGreen    = rgb_t.rgbtGreen;
					rgb_q[i].rgbBlue     = rgb_t.rgbtBlue;
					rgb_q[i].rgbReserved = 0;
				}
			} else {
				throw BMPReadError();
			}
		} else {
			throw BMPReadError();
		}
		if ( !bmInfoHeader.biSizeImage ) {
			bmInfoHeader.biSizeImage = ((((bmInfoHeader.biWidth * bmInfoHeader.biBitCount) + 31) & ~31) >> 3) * bmInfoHeader.biHeight;
		}
		pBits = new char[ size_ColorTable + bmInfoHeader.biSizeImage ];
		memcpy( pBits, &rgb_q, size_ColorTable );

		stream.seekg( bmFileHeader.bfOffBits, ios::beg );
		stream.read( pBits + size_ColorTable, bmInfoHeader.biSizeImage );

		BITMAP hBmp;
		hBmp.bmType       = 0;
		hBmp.bmWidth      = bmInfoHeader.biWidth;
		hBmp.bmHeight     = bmInfoHeader.biHeight;
		hBmp.bmWidthBytes = (((bmInfoHeader.biWidth * bmInfoHeader.biBitCount) + 31) & ~31) >> 3;
		hBmp.bmPlanes     = bmInfoHeader.biPlanes;
		hBmp.bmBitsPixel  = bmInfoHeader.biBitCount;
		hBmp.bmBits       = pBits;

		CBitmap cBmp;
		cBmp.CreateBitmapIndirect( &hBmp );

		CDC dcMemory;
		dcMemory.CreateCompatibleDC( CWnd::GetDesktopWindow()->GetDC() );
		CBitmap* pOldBitmap1 = dcMemory.SelectObject( &cBmp );

		bitmaps[name] = new CBitmap;
		bitmaps[name]->CreateCompatibleBitmap( CWnd::GetDesktopWindow()->GetDC(), hBmp.bmWidth, hBmp.bmHeight );
		CDC dcBmp;
		dcBmp.CreateCompatibleDC( CWnd::GetDesktopWindow()->GetDC() );
		CBitmap* pOldBitmap2 = dcBmp.SelectObject( bitmaps[name] );
		dcBmp.BitBlt( 0, 0, hBmp.bmWidth, hBmp.bmHeight, &dcMemory, 0, 0, SRCCOPY );

		dcMemory.SelectObject( pOldBitmap1 );
		dcBmp.SelectObject( pOldBitmap2 );

//		cBmp.DeleteObject();
//		delete pBits;

	} catch ( BMPReadError ) {
	}

//	if ( pBits ) delete pBits;
*/
}

void RDOStudioFrameManager::bmp_clear()
{
	map< string, BMP* >::iterator it = bitmaps.begin();
	while ( it != bitmaps.end() ) {
		delete it->second;
		it++;
	};
	bitmaps.clear();
}

void RDOStudioFrameManager::showFrame( vector<RDOFrame *>::const_iterator& frame, const int index )
{
	if ( *frame && index < count() ) {

		CSingleLock lock_used( getFrameUsed( index ) );
		lock_used.Lock();

		RDOStudioFrameDoc* doc = getFrameDoc( index );
		if ( doc ) {

			CSingleLock lock_draw( getFrameDraw( index ) );
			lock_draw.Lock();

			RDOStudioFrameView* view = getFrameView( index );
			if ( view->mustBeInit ) {
				if ( (*frame)->hasBackPicture ) {
					BMP* bmp = bitmaps[*(*frame)->picFileName];
					view->frameBmpRect.right  = bmp->w;
					view->frameBmpRect.bottom = bmp->h;
				} else {
					view->frameBmpRect.right  = (*frame)->width;
					view->frameBmpRect.bottom = (*frame)->height;
				}
				view->frameBmp.CreateCompatibleBitmap( view->GetDC(), view->frameBmpRect.Width(), view->frameBmpRect.Height() );
				view->mustBeInit = false;
				view->updateScrollBars();
			}

			CDC dc;
			dc.CreateCompatibleDC( view->GetDC() );
			CBitmap* pOldBitmap = dc.SelectObject( &view->frameBmp );

			if( !(*frame)->hasBackPicture ) {
				CBrush brush( RGB( (*frame)->r, (*frame)->g, (*frame)->b ) );
				CBrush* pOldBrush = dc.SelectObject( &brush );
				CPen pen( PS_SOLID, 0, RGB( 0x00, 0x00, 0x00 ) );
				CPen* pOldPen = dc.SelectObject( &pen );
				CRect rect( 0, 0, (*frame)->width, (*frame)->height );
				dc.Rectangle( rect );
				dc.SelectObject( pOldBrush );
				dc.SelectObject( pOldPen );
			} else {
				BMP* bmp = bitmaps[*(*frame)->picFileName];
				if ( bmp ) {
					CBitmap* pOldBitmap = dcBmp.SelectObject( &bmp->bmp );
					dc.BitBlt( 0, 0, bmp->w, bmp->h, &dcBmp, 0, 0, SRCCOPY );
					dcBmp.SelectObject( pOldBitmap );
				}
			}

			int size = (*frame)->elements.size();
			for(int i = 0; i < size; i++)
			{
				RDOFrameElement *currElement = (*frame)->elements.at(i);
				switch(currElement->type)
				{
				case RDOFrameElement::text_type:
					{				
						int oldBkMode;
						COLORREF oldBkColor;
						COLORREF oldTextColor;
						bool restoreBkColor   = false;
						bool restoreTextColor = false;
						RDOTextElement *textEl = (RDOTextElement *)currElement;
						if( !textEl->background.isTransparent ) {
							oldBkMode  = dc.SetBkMode(OPAQUE);
							oldBkColor = dc.SetBkColor(RGB(textEl->background.r, textEl->background.g, textEl->background.b));
							restoreBkColor = true;
						} else {
							oldBkMode = dc.SetBkMode(TRANSPARENT);
						}

						if( !textEl->foreground.isTransparent ) {
							oldTextColor = dc.SetTextColor(RGB(textEl->foreground.r, textEl->foreground.g, textEl->foreground.b));
							restoreTextColor = true;
						}

						UINT nFormat = DT_SINGLELINE;
						switch(textEl->align)
						{
						case RDOTextElement::left:
							nFormat |= DT_LEFT; break;
						case RDOTextElement::right:
							nFormat |= DT_RIGHT; break;
						case RDOTextElement::center:
							nFormat |= DT_CENTER; break;
						}

						dc.DrawText(textEl->strText.c_str(), textEl->strText.length(),
							CRect(textEl->x, textEl->y, textEl->x + textEl->w, textEl->y + textEl->h),
							nFormat);

						dc.SetBkMode( oldBkMode );
						if ( restoreBkColor ) dc.SetBkColor( oldBkColor );
						if ( restoreTextColor ) dc.SetTextColor( oldTextColor );
					}
					break;

				case RDOFrameElement::rect_type:
					{																		
						RDORectElement *rectEl = (RDORectElement *)currElement;
						CBrush brush( RGB(rectEl->background.r, rectEl->background.g, rectEl->background.b) );
						CBrush* pOldBrush;
						if( !rectEl->background.isTransparent ) {
							pOldBrush = dc.SelectObject( &brush );
						} else {
							pOldBrush = static_cast<CBrush*>(dc.SelectStockObject( NULL_BRUSH ));
						}

						CPen pen( PS_SOLID, 0, RGB(rectEl->foreground.r, rectEl->foreground.g, rectEl->foreground.b) );
						CPen* pOldPen;
						bool restorePen = false;
						if( !rectEl->foreground.isTransparent ) {
							pOldPen = dc.SelectObject( &pen );
							restorePen = true;
						}

						dc.Rectangle(rectEl->x, rectEl->y, rectEl->x + rectEl->w, rectEl->y + rectEl->h);

						dc.SelectObject( pOldBrush );
						if ( restorePen ) dc.SelectObject( pOldPen );
					}
					break;

				case RDOFrameElement::line_type:
					{																		
						RDOLineElement *lineEl = (RDOLineElement *)currElement;
						CPen pen( PS_SOLID, 0, RGB(lineEl->foreground.r, lineEl->foreground.g, lineEl->foreground.b) );
						CPen* pOldPen;
						bool restorePen = false;
						if( !lineEl->foreground.isTransparent ) {
							pOldPen = dc.SelectObject( &pen );
							restorePen = true;
						}

						dc.MoveTo(lineEl->x, lineEl->y);
						dc.LineTo(lineEl->w, lineEl->h);

						if ( restorePen ) dc.SelectObject( pOldPen );
					}
					break;

				case RDOFrameElement::bitmap_type: {
						RDOBitmapElement* bmpEl = (RDOBitmapElement *)currElement;
						BMP* bmp = bitmaps[bmpEl->bmp];
						if ( bmp ) {
							BMP* mask = bmpEl->hasMask ? bitmaps[bmpEl->mask] : NULL;
							CBitmap* pOldBitmap = dcBmp.SelectObject( &bmp->bmp );
							if ( mask ) {
								CBitmap* pOldMask = dcMask.SelectObject( &mask->bmp );
								dc.BitBlt( bmpEl->x, bmpEl->y, mask->w, mask->h, &dcMask, 0, 0, SRCAND );
								dc.BitBlt( bmpEl->x, bmpEl->y, bmp->w, bmp->h, &dcBmp, 0, 0, SRCPAINT );
								dcMask.SelectObject( pOldMask );
							} else {
								dc.BitBlt( bmpEl->x, bmpEl->y, bmp->w, bmp->h, &dcBmp, 0, 0, SRCCOPY );
							}
							dcBmp.SelectObject( pOldBitmap );
						}
					}
					break;
				case RDOFrameElement::s_bmp_type: {
						RDOSBmpElement *sbmpEl = (RDOSBmpElement *)currElement;
						BMP* bmp = bitmaps[sbmpEl->bmp];
						if ( bmp ) {
							BMP* mask = sbmpEl->hasMask ? bitmaps[sbmpEl->mask] : NULL;
							CBitmap* pOldBitmap = dcBmp.SelectObject( &bmp->bmp );
							if ( mask ) {
								CBitmap* pOldMask = dcMask.SelectObject( &mask->bmp );
								dc.StretchBlt( sbmpEl->x, sbmpEl->y, sbmpEl->w, sbmpEl->h, &dcMask, 0, 0, mask->w, mask->h, SRCAND );
								dc.StretchBlt( sbmpEl->x, sbmpEl->y, sbmpEl->w, sbmpEl->h, &dcBmp, 0, 0, bmp->w, bmp->h, SRCPAINT );
								dcMask.SelectObject( pOldMask );
							} else {
								dc.StretchBlt( sbmpEl->x, sbmpEl->y, sbmpEl->w, sbmpEl->h, &dcBmp, 0, 0, bmp->w, bmp->h, SRCCOPY );
							}
							dcBmp.SelectObject( pOldBitmap );
						}
					}
					break;

				case RDOFrameElement::active_type:
					{							
/*							
						RDOActiveElement *activeEl = (RDOActiveElement *)currElement;
						CBrush brush( RGB(196, 0, 196) );
						CBrush* pOldBrush = dc.SelectObject( &brush );
						CPen pen( PS_SOLID, 2, RGB(196, 196, 0) );
						CPen* pOldPen = dc.SelectObject( &pen );
						dc.Rectangle(activeEl->x, activeEl->y, activeEl->x + activeEl->w, activeEl->y + activeEl->h);
						dc.MoveTo(activeEl->x, activeEl->y);
						dc.LineTo(activeEl->x + activeEl->w, activeEl->y + activeEl->h);
						dc.MoveTo(activeEl->x + activeEl->w, activeEl->y);
						dc.LineTo(activeEl->x, activeEl->y + activeEl->h);
						dc.SelectObject( pOldBrush );
						dc.SelectObject( pOldPen );
*/
					}
					break;
				}
			}
			dc.SelectObject( pOldBitmap );

			lock_draw.Unlock();

			getFrameTimer( index )->ResetEvent();

//			CRect rect;
//			view->GetClientRect( rect );
			view->InvalidateRect( NULL );
			view->SendNotifyMessage( WM_PAINT, 0, 0 );

			CONST HANDLE events[2] = { getFrameTimer( index )->m_hObject, getFrameClose( index )->m_hObject };
			DWORD res = ::WaitForMultipleObjects( 2, events, FALSE, INFINITE );
			if ( res == WAIT_OBJECT_0 ) {               // timer
			} else if ( res == WAIT_OBJECT_0 + 1 ) {    // close
			} else {
			}
		}
		lock_used.Unlock();
	}
}
