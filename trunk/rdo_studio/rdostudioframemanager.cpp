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
#include "edit_ctrls/rdodebugedit.h"
#include "resource.h"

#include <rdokernel.h>
#include <rdorepository.h>
#include <rdobinarystream.h>

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
	lastShowedFrame( -1 ),
	currentShowingFrame( -1 )
{
	frameDocTemplate = new FrameDocTemplate( IDR_FRAME_TYPE, RUNTIME_CLASS(RDOStudioFrameDoc), RUNTIME_CLASS(RDOStudioChildFrame), RUNTIME_CLASS(RDOStudioFrameView) );
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
		CSingleLock lock( getFrameMutexUsed( index ) );
		lock.Lock();

		doc = static_cast<RDOStudioFrameDoc*>(frameDocTemplate->OpenDocumentFile( NULL ));
		if ( doc ) {
			frames[index]->doc  = doc;
			frames[index]->view = doc->getView();
			lastShowedFrame     = index;
			setCurrentShowingFrame( index );
		}

		lock.Unlock();
	}
	return doc;
}

void RDOStudioFrameManager::disconnectFrameDoc( const RDOStudioFrameDoc* doc ) const
{
	int index = findFrameIndex( doc );
	if ( index != -1 ) {

		CSingleLock lock( getFrameMutexUsed( index ) );
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
				doc->frame->SendMessage( WM_CLOSE, 0, 0 );
			}
		}
		delete *it++;
	};
	frames.clear();
	lastShowedFrame = -1;
	setCurrentShowingFrame( -1 );
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

void RDOStudioFrameManager::setLastShowedFrame( const int value )
{
	if ( value >= 0 && value < count() ) {
		lastShowedFrame = value;
	}
}

void RDOStudioFrameManager::setCurrentShowingFrame( const int value )
{
	if ( value == -1 || (value >= 0 && value < count()) ) {
		currentShowingFrame = value;
		CTreeCtrl* tree = studioApp.mainFrame->workspace.frames;
		if ( currentShowingFrame != -1 ) {
			HTREEITEM hitem = frames[currentShowingFrame]->hitem;
			tree->SelectItem( hitem );
		} else {
			tree->SelectItem( NULL );
		}
	}
}

void RDOStudioFrameManager::resetCurrentShowingFrame( const int value )
{
	if ( value == currentShowingFrame ) setCurrentShowingFrame( -1 );
}

void RDOStudioFrameManager::bmp_insert( const std::string& name )
{
	if ( bitmaps.find( name ) == bitmaps.end() ) {

		RDOStudioOutput* output = &studioApp.mainFrame->output;
		output->appendStringToDebug( format( IDS_MODEL_RESOURCE_LOADING_NAME, name.c_str() ) );
		const_cast<rdoEditCtrl::RDODebugEdit*>(output->getDebug())->UpdateWindow();

		bitmaps[name] = NULL;

		rdo::binarystream stream;
		kernel.getRepository()->loadBMP( name, stream.vec() );

		char* bmInfo   = NULL;
		char* pBits    = NULL;
		CDC* desktopDC = NULL;

		try {
			// ¬ потоке, перед битовой картой, идет заголовок файла битовой карты
			BITMAPFILEHEADER bmFileHeader;
			stream.read( reinterpret_cast<char*>(&bmFileHeader), sizeof(bmFileHeader) );
			if ( stream.rdstate() != ios_base::goodbit ) throw BMPReadError();

			// ѕровер€ем заголовок битовой карты на магическое число "BM"
			if ( bmFileHeader.bfType != 0x4D42 ) throw BMPReadError();

			// ¬от теперь читаем сам заголовок битовой карты
			BITMAPINFOHEADER bmInfoHeader;
			stream.read( reinterpret_cast<char*>(&bmInfoHeader), sizeof(bmInfoHeader) );
			if ( stream.rdstate() != ios_base::goodbit ) throw BMPReadError();
			if ( bmInfoHeader.biSize == sizeof(BITMAPCOREHEADER) ) throw BMPReadError();

			WORD nNumColors = static_cast<WORD>(bmInfoHeader.biClrUsed);
			if ( !nNumColors && bmInfoHeader.biBitCount != 24 ) {
				nNumColors = 1 << bmInfoHeader.biBitCount;
			}
			if ( !bmInfoHeader.biClrUsed ) {
				bmInfoHeader.biClrUsed = nNumColors;
			}

			if ( !bmInfoHeader.biSizeImage ) {
				bmInfoHeader.biSizeImage = ((((bmInfoHeader.biWidth * static_cast<WORD>(bmInfoHeader.biBitCount)) + 31) & ~31) >> 3) * bmInfoHeader.biHeight;
			}

			RGBQUAD rgb_q[256];
			memset( &rgb_q, 0, sizeof(rgb_q) );
			stream.read( reinterpret_cast<char*>(&rgb_q), nNumColors * sizeof(RGBQUAD) );
			if ( stream.rdstate() != ios_base::goodbit ) throw BMPReadError();

			bmInfo = new char[ sizeof(bmInfoHeader) + nNumColors * sizeof(RGBQUAD) ];
			memcpy( bmInfo, &bmInfoHeader, sizeof(bmInfoHeader) );
			memcpy( bmInfo + sizeof(bmInfoHeader), &rgb_q, nNumColors * sizeof(RGBQUAD) );

			pBits = new char[ bmInfoHeader.biSizeImage ];
			stream.seekg( bmFileHeader.bfOffBits, ios::beg );
			if ( stream.rdstate() != ios_base::goodbit ) throw BMPReadError();
			stream.read( pBits, bmInfoHeader.biSizeImage );
			if ( stream.rdstate() != ios_base::goodbit ) throw BMPReadError();

			CDC* desktopDC = CWnd::GetDesktopWindow()->GetDC();
			CDC memDC;
			memDC.CreateCompatibleDC( desktopDC );
			CBitmap memBMP;
			memBMP.CreateCompatibleBitmap( bmInfoHeader.biBitCount != 1 ? desktopDC : &memDC, bmInfoHeader.biWidth, bmInfoHeader.biHeight );
			::SetDIBits( desktopDC->m_hDC, static_cast<HBITMAP>(memBMP), 0, bmInfoHeader.biHeight, pBits, reinterpret_cast<BITMAPINFO*>(bmInfo), DIB_RGB_COLORS );
			CBitmap* hOldBitmap1 = memDC.SelectObject( &memBMP );

			CDC dc;
			dc.CreateCompatibleDC( desktopDC );
			int i = sizeof( CBitmap );
			BMP* bmp = new BMP;
			bitmaps[name] = bmp;
			bitmaps[name]->w = bmInfoHeader.biWidth;
			bitmaps[name]->h = bmInfoHeader.biHeight;
			bitmaps[name]->bmp.CreateCompatibleBitmap( bmInfoHeader.biBitCount != 1 ? desktopDC : &dc, bmInfoHeader.biWidth, bmInfoHeader.biHeight );
			CBitmap* hOldBitmap2 = dc.SelectObject( &bitmaps[name]->bmp );
			dc.BitBlt( 0, 0, bmInfoHeader.biWidth, bmInfoHeader.biHeight, &memDC, 0, 0, SRCCOPY );

			memDC.SelectObject( hOldBitmap1 );
			dc.SelectObject( hOldBitmap2 );

			output->appendStringToDebug( format( IDS_MODEL_RESOURCE_LOADING_NAME_OK ) );
			const_cast<rdoEditCtrl::RDODebugEdit*>(output->getDebug())->UpdateWindow();

		} catch ( BMPReadError ) {
			output->appendStringToDebug( format( IDS_MODEL_RESOURCE_LOADING_NAME_FAILED ) );
			const_cast<rdoEditCtrl::RDODebugEdit*>(output->getDebug())->UpdateWindow();
		}

		if ( bmInfo ) delete bmInfo;
		if ( pBits ) delete pBits;
		if ( desktopDC ) CWnd::GetDesktopWindow()->ReleaseDC( desktopDC );
	}
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

void RDOStudioFrameManager::showFrame( const RDOFrame* const frame, const int index )
{
	if ( index < count() ) {

		CSingleLock lock_used( getFrameMutexUsed( index ) );
		lock_used.Lock();

		RDOStudioFrameDoc* doc = getFrameDoc( index );
		if ( doc ) {

			CSingleLock lock_draw( getFrameMutexDraw( index ) );
			lock_draw.Lock();

			RDOStudioFrameView* view = getFrameView( index );
			if ( view->mustBeInit ) {
				if ( frame->hasBackPicture ) {
					BMP* bmp = bitmaps[*frame->picFileName];
					if ( bmp ) {
						view->frameBmpRect.right  = bmp->w;
						view->frameBmpRect.bottom = bmp->h;
					}
				} else {
					view->frameBmpRect.right  = frame->width;
					view->frameBmpRect.bottom = frame->height;
				}
				CDC* dc = view->GetDC();
				view->frameBmp.CreateCompatibleBitmap( dc, view->frameBmpRect.Width(), view->frameBmpRect.Height() );
				view->ReleaseDC( dc );
				view->mustBeInit = false;
				view->updateScrollBars();
			}

			CDC dc;
			CDC* viewDC = view->GetDC();
			dc.CreateCompatibleDC( viewDC );
			view->ReleaseDC( viewDC );
			CBitmap* pOldBitmap = dc.SelectObject( &view->frameBmp );

			if( !frame->hasBackPicture ) {
				CBrush brush( RGB( frame->r, frame->g, frame->b ) );
				CBrush* pOldBrush = dc.SelectObject( &brush );
				CPen pen( PS_SOLID, 0, RGB( 0x00, 0x00, 0x00 ) );
				CPen* pOldPen = dc.SelectObject( &pen );
				CRect rect( 0, 0, frame->width, frame->height );
				dc.Rectangle( rect );
				dc.SelectObject( pOldBrush );
				dc.SelectObject( pOldPen );
			} else {
				BMP* bmp = bitmaps[*frame->picFileName];
				if ( bmp ) {
					CBitmap* pOldBitmap = dcBmp.SelectObject( &bmp->bmp );
					dc.BitBlt( 0, 0, bmp->w, bmp->h, &dcBmp, 0, 0, SRCCOPY );
					dcBmp.SelectObject( pOldBitmap );
				}
			}

			vector< string >* areas_clicked = &frames[index]->areas_clicked;
			vector< string >::iterator it = areas_clicked->begin();
			while ( it != areas_clicked->end() ) {
				kernel.getSimulator()->addAreaPressed( *it++ );
			};
			areas_clicked->clear();
			frames[index]->areas_sim_clear();

			vector< int >::iterator key = frames[index]->keys_pressed.begin();
			while ( key != frames[index]->keys_pressed.end() ) {
				kernel.getSimulator()->addKeyPressed( *key++ );
			};
			frames[index]->keys_pressed.clear();

			int size = frame->elements.size();
			for(int i = 0; i < size; i++) {
				RDOFrameElement* currElement = frame->elements.at(i);
				switch( currElement->type ) {
					case RDOFrameElement::text_type: {
						RDOTextElement* element = static_cast<RDOTextElement*>(currElement);
						int oldBkMode;
						COLORREF oldBkColor;
						COLORREF oldTextColor;
						bool restoreBkColor   = false;
						bool restoreTextColor = false;
						if( !element->background.isTransparent ) {
							oldBkMode  = dc.SetBkMode( OPAQUE );
							oldBkColor = dc.SetBkColor( RGB(element->background.r, element->background.g, element->background.b) );
							restoreBkColor = true;
						} else {
							oldBkMode = dc.SetBkMode( TRANSPARENT );
						}

						if( !element->foreground.isTransparent ) {
							oldTextColor = dc.SetTextColor( RGB(element->foreground.r, element->foreground.g, element->foreground.b) );
							restoreTextColor = true;
						}

						UINT nFormat = DT_SINGLELINE;
						switch( element->align ) {
							case RDOTextElement::left  : nFormat |= DT_LEFT; break;
							case RDOTextElement::right : nFormat |= DT_RIGHT; break;
							case RDOTextElement::center: nFormat |= DT_CENTER; break;
						}

						dc.DrawText( element->strText.c_str(), element->strText.length(), CRect( element->x, element->y, element->x + element->w, element->y + element->h ), nFormat );

						dc.SetBkMode( oldBkMode );
						if ( restoreBkColor ) dc.SetBkColor( oldBkColor );
						if ( restoreTextColor ) dc.SetTextColor( oldTextColor );

						break;
					}
					case RDOFrameElement::rect_type: {
						RDORectElement* element = static_cast<RDORectElement*>(currElement);
						CBrush brush( RGB(element->background.r, element->background.g, element->background.b) );
						CBrush* pOldBrush;
						if( !element->background.isTransparent ) {
							pOldBrush = dc.SelectObject( &brush );
						} else {
							pOldBrush = static_cast<CBrush*>(dc.SelectStockObject( NULL_BRUSH ));
						}

						CPen pen( PS_SOLID, 0, RGB(element->foreground.r, element->foreground.g, element->foreground.b) );
						CPen* pOldPen;
						bool restorePen = false;
						if( !element->foreground.isTransparent ) {
							pOldPen = dc.SelectObject( &pen );
							restorePen = true;
						}

						dc.Rectangle( element->x, element->y, element->x + element->w, element->y + element->h );

						dc.SelectObject( pOldBrush );
						if ( restorePen ) dc.SelectObject( pOldPen );

						break;
					}
					case RDOFrameElement::r_rect_type: {
						RDORRectElement* element = static_cast<RDORRectElement*>(currElement);
						CBrush brush( RGB(element->background.r, element->background.g, element->background.b) );
						CBrush* pOldBrush;
						if( !element->background.isTransparent ) {
							pOldBrush = dc.SelectObject( &brush );
						} else {
							pOldBrush = static_cast<CBrush*>(dc.SelectStockObject( NULL_BRUSH ));
						}

						CPen pen( PS_SOLID, 0, RGB(element->foreground.r, element->foreground.g, element->foreground.b) );
						CPen* pOldPen;
						bool restorePen = false;
						if( !element->foreground.isTransparent ) {
							pOldPen = dc.SelectObject( &pen );
							restorePen = true;
						}

						int w = min( element->w, element->h ) / 3;
						dc.RoundRect( element->x, element->y, element->x + element->w, element->y + element->h, w, w );

						dc.SelectObject( pOldBrush );
						if ( restorePen ) dc.SelectObject( pOldPen );

						break;
					}
					case RDOFrameElement::line_type: {
						RDOLineElement* element = static_cast<RDOLineElement*>(currElement);
						CPen pen( PS_SOLID, 0, RGB(element->foreground.r, element->foreground.g, element->foreground.b) );
						CPen* pOldPen;
						bool restorePen = false;
						if( !element->foreground.isTransparent ) {
							pOldPen = dc.SelectObject( &pen );
							restorePen = true;
						}

						dc.MoveTo( element->x, element->y );
						dc.LineTo( element->w, element->h );

						if ( restorePen ) dc.SelectObject( pOldPen );

						break;
					}
					case RDOFrameElement::triang_type: {
						RDOTriangElement* element = static_cast<RDOTriangElement*>(currElement);
						CBrush brush( RGB(element->background.r, element->background.g, element->background.b) );
						CBrush* pOldBrush;
						if( !element->background.isTransparent ) {
							pOldBrush = dc.SelectObject( &brush );
						} else {
							pOldBrush = static_cast<CBrush*>(dc.SelectStockObject( NULL_BRUSH ));
						}

						CPen pen( PS_SOLID, 0, RGB(element->foreground.r, element->foreground.g, element->foreground.b) );
						CPen* pOldPen;
						bool restorePen = false;
						if( !element->foreground.isTransparent ) {
							pOldPen = dc.SelectObject( &pen );
							restorePen = true;
						}

						CPoint pts[3];
						pts[0].x = element->x1;
						pts[0].y = element->y1;
						pts[1].x = element->x2;
						pts[1].y = element->y2;
						pts[2].x = element->x3;
						pts[2].y = element->y3;
						dc.Polygon( pts, 3 );

						dc.SelectObject( pOldBrush );
						if ( restorePen ) dc.SelectObject( pOldPen );

						break;
					}
					case RDOFrameElement::ellipse_type: {
						RDOEllipseElement* element = static_cast<RDOEllipseElement*>(currElement);
						CBrush brush( RGB(element->background.r, element->background.g, element->background.b) );
						CBrush* pOldBrush;
						if( !element->background.isTransparent ) {
							pOldBrush = dc.SelectObject( &brush );
						} else {
							pOldBrush = static_cast<CBrush*>(dc.SelectStockObject( NULL_BRUSH ));
						}

						CPen pen( PS_SOLID, 0, RGB(element->foreground.r, element->foreground.g, element->foreground.b) );
						CPen* pOldPen;
						bool restorePen = false;
						if( !element->foreground.isTransparent ) {
							pOldPen = dc.SelectObject( &pen );
							restorePen = true;
						}

						dc.Ellipse( element->x, element->y, element->x + element->w, element->y + element->h );

						dc.SelectObject( pOldBrush );
						if ( restorePen ) dc.SelectObject( pOldPen );

						break;
					}
					case RDOFrameElement::bitmap_type: {
						RDOBitmapElement* element = static_cast<RDOBitmapElement*>(currElement);
						BMP* bmp = bitmaps[element->bmp];
						if ( bmp ) {
							BMP* mask = element->hasMask ? bitmaps[element->mask] : NULL;
							CBitmap* pOldBitmap = dcBmp.SelectObject( &bmp->bmp );
							if ( mask ) {
								CBitmap* pOldMask = dcMask.SelectObject( &mask->bmp );
								dc.BitBlt( element->x, element->y, mask->w, mask->h, &dcMask, 0, 0, SRCAND );
								dc.BitBlt( element->x, element->y, bmp->w, bmp->h, &dcBmp, 0, 0, SRCPAINT );
								dcMask.SelectObject( pOldMask );
							} else {
								dc.BitBlt( element->x, element->y, bmp->w, bmp->h, &dcBmp, 0, 0, SRCCOPY );
							}
							dcBmp.SelectObject( pOldBitmap );
						}
						break;
					}
					case RDOFrameElement::s_bmp_type: {
						RDOSBmpElement* element = static_cast<RDOSBmpElement*>(currElement);
						BMP* bmp = bitmaps[element->bmp];
						if ( bmp ) {
							BMP* mask = element->hasMask ? bitmaps[element->mask] : NULL;
							CBitmap* pOldBitmap = dcBmp.SelectObject( &bmp->bmp );
							if ( mask ) {
								CBitmap* pOldMask = dcMask.SelectObject( &mask->bmp );
								dc.StretchBlt( element->x, element->y, element->w, element->h, &dcMask, 0, 0, mask->w, mask->h, SRCAND );
								dc.StretchBlt( element->x, element->y, element->w, element->h, &dcBmp, 0, 0, bmp->w, bmp->h, SRCPAINT );
								dcMask.SelectObject( pOldMask );
							} else {
								dc.StretchBlt( element->x, element->y, element->w, element->h, &dcBmp, 0, 0, bmp->w, bmp->h, SRCCOPY );
							}
							dcBmp.SelectObject( pOldBitmap );
						}
						break;
					}
					case RDOFrameElement::active_type: {
						RDOActiveElement* element = static_cast<RDOActiveElement*>(currElement);
						Area* area = new Area;
						area->name = element->operName;
						area->x    = element->x;
						area->y    = element->y;
						area->w    = element->w;
						area->h    = element->h;
						frames[index]->areas_sim.push_back( area );
						break;
					}
				}
			}
			dc.SelectObject( pOldBitmap );

			lock_draw.Unlock();

			getFrameEventTimer( index )->ResetEvent();

//			CRect rect;
//			view->GetClientRect( rect );
			view->InvalidateRect( NULL );
			view->SendNotifyMessage( WM_PAINT, 0, 0 );

			CONST HANDLE events[2] = { getFrameEventTimer( index )->m_hObject, getFrameEventClose( index )->m_hObject };
			DWORD res = ::WaitForMultipleObjects( 2, events, FALSE, INFINITE );
			if ( res == WAIT_OBJECT_0 ) {               // timer
			} else if ( res == WAIT_OBJECT_0 + 1 ) {    // close
			} else {
			}
		}
		lock_used.Unlock();
	}
}

void RDOStudioFrameManager::showNextFrame()
{
	int cnt = count();
	if ( model->isRunning() && model->getShowMode() != RDOSimulatorNS::SM_NoShow && cnt > 1 && currentShowingFrame < cnt-1 ) {
		int index = currentShowingFrame + 1;
		RDOStudioFrameDoc* doc = getFrameDoc( index );
		if ( !doc ) {
			doc = connectFrameDoc( index );
		} else {
			doc->frame->ActivateFrame();
		}
	}
}

void RDOStudioFrameManager::showPrevFrame()
{
	int cnt = count();
	if ( model->isRunning() && model->getShowMode() != RDOSimulatorNS::SM_NoShow && cnt > 1 && currentShowingFrame > 0 ) {
		int index = currentShowingFrame - 1;
		RDOStudioFrameDoc* doc = getFrameDoc( index );
		if ( !doc ) {
			doc = connectFrameDoc( index );
		} else {
			doc->frame->ActivateFrame();
		}
	}
}

bool RDOStudioFrameManager::canShowNextFrame() const
{
	int cnt = count();
	return model->isRunning() && model->getShowMode() != RDOSimulatorNS::SM_NoShow && cnt > 1 && ( currentShowingFrame == -1 || currentShowingFrame < cnt-1 );
}

bool RDOStudioFrameManager::canShowPrevFrame() const
{
	int cnt = count();
	return model->isRunning() && model->getShowMode() != RDOSimulatorNS::SM_NoShow && cnt > 1 && currentShowingFrame > 0;
}
