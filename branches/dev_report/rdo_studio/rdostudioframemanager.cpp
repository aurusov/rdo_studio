#include "stdafx.h"
#include "rdostudioframemanager.h"
#include "rdostudiomodel.h"
#include "rdostudioapp.h"
#include "rdostudiomainfrm.h"
#include "rdostudiochildfrm.h"
#include "rdostudioworkspace.h"
#include "rdostudioframetreectrl.h"
#include "edit_ctrls/rdodebugedit.h"
#include "resource.h"

#include <rdokernel.h>
#include <rdosimwin.h>
#include <rdorepository.h>
#include <rdobinarystream.h>
#include <rdothread.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RDOStudioFrameManager
// ----------------------------------------------------------------------------
//std::vector< RDOStudioFrameManager::Frame* > RDOStudioFrameManager::frames;

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
	std::vector< Frame* >::iterator it = frames.begin();
	while ( it != frames.end() ) {
		delete *it++;
	};
}

void RDOStudioFrameManager::insertItem( const std::string& name )
{
	Frame* item = new Frame;
	item->hitem = studioApp.mainFrame->workspace.frames->InsertItem( name.c_str(), 1, 1, studioApp.mainFrame->workspace.frames->GetRootItem() );
	item->name  = name;
	item->doc   = NULL;
	item->view  = NULL;
	frames.push_back( item );
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
			doc->SetTitle( rdo::format( IDS_FRAME_NAME, getFrameName( index ).c_str() ).c_str()  );
			setCurrentShowingFrame( index );
		}

		lock.Unlock();
	}
	return doc;
}

void RDOStudioFrameManager::disconnectFrameDoc( const RDOStudioFrameDoc* doc )
{
	int index = findFrameIndex( doc );
	if ( index != -1 ) {

		CSingleLock lock( getFrameMutexUsed( index ) );
		lock.Lock();

		frames[index]->doc  = NULL;
		frames[index]->view = NULL;

		lock.Unlock();
	}
	changed = true;
}

void RDOStudioFrameManager::closeAll()
{
	int backup = lastShowedFrame;
	std::vector< Frame* >::iterator it = frames.begin();
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
	std::vector< Frame* >::iterator it = frames.begin();
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
	std::vector< Frame* >::const_iterator it = frames.begin();
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
		output->appendStringToDebug( rdo::format( IDS_MODEL_RESOURCE_LOADING_NAME, name.c_str() ) );
		const_cast<rdoEditCtrl::RDODebugEdit*>(output->getDebug())->UpdateWindow();

		bitmaps[name] = NULL;

		rdo::binarystream stream;
		rdoRepository::RDOThreadRepository::BinaryFile data( name, stream );
		model->sendMessage( kernel->repository(), RDOThread::RT_REPOSITORY_LOAD_BINARY, &data );

		char* bmInfo   = NULL;
		char* pBits    = NULL;
		CDC* desktopDC = NULL;

		try {
			// ¬ потоке, перед битовой картой, идет заголовок файла битовой карты
			BITMAPFILEHEADER bmFileHeader;
			stream.read( reinterpret_cast<char*>(&bmFileHeader), sizeof(bmFileHeader) );
			if ( !stream.good() ) throw BMPReadError();

			// ѕровер€ем заголовок битовой карты на магическое число "BM"
			if ( bmFileHeader.bfType != 0x4D42 ) throw BMPReadError();

			// ¬от теперь читаем сам заголовок битовой карты
			BITMAPINFOHEADER bmInfoHeader;
			stream.read( reinterpret_cast<char*>(&bmInfoHeader), sizeof(bmInfoHeader) );
			if ( !stream.good() ) throw BMPReadError();
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
			if ( !stream.good() ) throw BMPReadError();

			bmInfo = new char[ sizeof(bmInfoHeader) + nNumColors * sizeof(RGBQUAD) ];
			memcpy( bmInfo, &bmInfoHeader, sizeof(bmInfoHeader) );
			memcpy( bmInfo + sizeof(bmInfoHeader), &rgb_q, nNumColors * sizeof(RGBQUAD) );

			pBits = new char[ bmInfoHeader.biSizeImage ];
			stream.seekg( bmFileHeader.bfOffBits, std::ios::beg );
			if ( !stream.good() ) throw BMPReadError();
			stream.read( pBits, bmInfoHeader.biSizeImage );
			if ( !(stream.good() || stream.eof()) ) throw BMPReadError();

			CDC* desktopDC = CWnd::GetDesktopWindow()->GetDC();
			CDC memDC;
			memDC.CreateCompatibleDC( desktopDC );
			CBitmap memBMP;
			memBMP.CreateCompatibleBitmap( desktopDC, bmInfoHeader.biWidth, bmInfoHeader.biHeight );
			::SetDIBits( desktopDC->m_hDC, static_cast<HBITMAP>(memBMP), 0, bmInfoHeader.biHeight, pBits, reinterpret_cast<BITMAPINFO*>(bmInfo), DIB_RGB_COLORS );
			CBitmap* hOldBitmap1 = memDC.SelectObject( &memBMP );

			CDC dc;
			dc.CreateCompatibleDC( desktopDC );
			int i = sizeof( CBitmap );
			BMP* bmp = new BMP;
			bitmaps[name] = bmp;
			bitmaps[name]->w = bmInfoHeader.biWidth;
			bitmaps[name]->h = bmInfoHeader.biHeight;
			bitmaps[name]->bmp.CreateCompatibleBitmap( desktopDC, bmInfoHeader.biWidth, bmInfoHeader.biHeight );
			CBitmap* hOldBitmap2 = dc.SelectObject( &bitmaps[name]->bmp );
			dc.BitBlt( 0, 0, bmInfoHeader.biWidth, bmInfoHeader.biHeight, &memDC, 0, 0, SRCCOPY );

			memDC.SelectObject( hOldBitmap1 );
			dc.SelectObject( hOldBitmap2 );

			output->appendStringToDebug( rdo::format( IDS_MODEL_RESOURCE_LOADING_NAME_OK ) );
			const_cast<rdoEditCtrl::RDODebugEdit*>(output->getDebug())->UpdateWindow();

		} catch ( BMPReadError ) {
			output->appendStringToDebug( rdo::format( IDS_MODEL_RESOURCE_LOADING_NAME_FAILED ) );
			const_cast<rdoEditCtrl::RDODebugEdit*>(output->getDebug())->UpdateWindow();
		}

		if ( bmInfo ) delete bmInfo;
		if ( pBits ) delete pBits;
		if ( desktopDC ) CWnd::GetDesktopWindow()->ReleaseDC( desktopDC );
	}
}

void RDOStudioFrameManager::bmp_clear()
{
	std::map< std::string, BMP* >::iterator it = bitmaps.begin();
	while ( it != bitmaps.end() ) {
		delete it->second;
		it++;
	};
	bitmaps.clear();
}

void RDOStudioFrameManager::showFrame( const rdoSimulator::RDOFrame* const frame, const int index )
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
				bool show_fillrect = true;
				if ( frame->hasBackPicture ) {
					BMP* bmp = bitmaps[frame->picFileName];
					if ( bmp ) {
						view->frameBmpRect.right  = bmp->w;
						view->frameBmpRect.bottom = bmp->h;
						show_fillrect = false;
					}
				}
				if ( show_fillrect ) {
					view->frameBmpRect.right  = frame->width;
					view->frameBmpRect.bottom = frame->height;
				}
				view->points[0].x = 0;
				view->points[0].y = 0;
				view->points[1].x = view->frameBmpRect.right - 1;
				view->points[1].y = 0;
				view->points[2].x = view->frameBmpRect.right - 1;
				view->points[2].y = view->frameBmpRect.bottom - 1;
				view->points[3].x = 0;
				view->points[3].y = view->frameBmpRect.bottom - 1;
				view->points[4].x = 0;
				view->points[4].y = 0;
				view->hbmp = ::CreateCompatibleBitmap( view->hdc, view->frameBmpRect.right, view->frameBmpRect.bottom );
				::SelectObject( view->hmemdc, view->hbmp );
				view->mustBeInit = false;
				view->updateScrollBars();
			}

			HDC hdc = view->hmemdc;

			bool show_fillrect = true;
			if ( frame->hasBackPicture ) {
				BMP* bmp = bitmaps[frame->picFileName];
				if ( bmp ) {
					CBitmap* pOldBitmap = dcBmp.SelectObject( &bmp->bmp );
					::BitBlt( hdc, 0, 0, bmp->w, bmp->h, dcBmp.m_hDC, 0, 0, SRCCOPY );
					dcBmp.SelectObject( pOldBitmap );
					show_fillrect = false;
				}
			}
			if ( frame->bgColor.isTransparent ) {
				view->bgColor = studioApp.mainFrame->style_frame.theme->backgroundColor;
			} else {
				view->bgColor = RGB( frame->bgColor.r, frame->bgColor.g, frame->bgColor.b );
			}
			if ( show_fillrect ) {
				HBRUSH brush     = ::CreateSolidBrush( view->bgColor );
				HBRUSH pOldBrush = static_cast<HBRUSH>(::SelectObject( hdc, brush ));
				HPEN pen     = ::CreatePen( PS_SOLID, 0, studioApp.mainFrame->style_frame.theme->defaultColor );
				HPEN pOldPen = static_cast<HPEN>(::SelectObject( hdc, pen ));
				::FillRect( hdc, view->frameBmpRect, brush );
				::Polyline( hdc, view->points, 5 );
				::SelectObject( hdc, pOldBrush );
				::SelectObject( hdc, pOldPen );
				::DeleteObject( brush );
				::DeleteObject( pen );
			}
/*
			if( !frame->hasBackPicture ) {
				HBRUSH brush     = ::CreateSolidBrush( RGB( frame->r, frame->g, frame->b ) );
				HBRUSH pOldBrush = static_cast<HBRUSH>(::SelectObject( hdc, brush ));
				HPEN pen     = ::CreatePen( PS_SOLID, 0, studioApp.mainFrame->style_frame.theme->defaultColor );
				HPEN pOldPen = static_cast<HPEN>(::SelectObject( hdc, pen ));
				::FillRect( hdc, view->frameBmpRect, brush );
				::Polyline( hdc, view->points, 5 );
				::SelectObject( hdc, pOldBrush );
				::SelectObject( hdc, pOldPen );
				::DeleteObject( brush );
				::DeleteObject( pen );
				view->bgColor = studioApp.mainFrame->style_frame.theme->backgroundColor;

//				HBRUSH brush     = ::CreateSolidBrush( RGB( frame->r, frame->g, frame->b ) );
//				HBRUSH pOldBrush = static_cast<HBRUSH>(::SelectObject( hdc, brush ));
//				HPEN pen     = ::CreatePen( PS_SOLID, 0, RGB( 0x00, 0x00, 0x00 ) );
//				HPEN pOldPen = static_cast<HPEN>(::SelectObject( hdc, pen ));
//				::Rectangle( hdc, 0, 0, frame->width, frame->height );
//				::SelectObject( hdc, pOldBrush );
//				::SelectObject( hdc, pOldPen );
//				::DeleteObject( brush );
//				::DeleteObject( pen );

			} else {
				BMP* bmp = bitmaps[frame->picFileName];
				if ( bmp ) {
					CBitmap* pOldBitmap = dcBmp.SelectObject( &bmp->bmp );
					::BitBlt( hdc, 0, 0, bmp->w, bmp->h, dcBmp.m_hDC, 0, 0, SRCCOPY );
					dcBmp.SelectObject( pOldBitmap );
				}
				view->bgColor = RGB( frame->r, frame->g, frame->b );
			}
*/
			int size = frame->elements.size();
			for ( int i = 0; i < size; i++ ) {
				rdoSimulator::RDOFrameElement* currElement = frame->elements.at(i);
				switch( currElement->type ) {
					case rdoSimulator::RDOFrameElement::text_type: {
						rdoSimulator::RDOTextElement* element = static_cast<rdoSimulator::RDOTextElement*>(currElement);
						if( !element->background.isTransparent ) {
							::SetBkMode( hdc, OPAQUE );
							::SetBkColor( hdc, RGB(element->background.r, element->background.g, element->background.b) );
						} else {
							::SetBkMode( hdc, TRANSPARENT );
						}

						if( !element->foreground.isTransparent ) {
							::SetTextColor( hdc, RGB(element->foreground.r, element->foreground.g, element->foreground.b) );
						}

						UINT nFormat = DT_SINGLELINE | DT_VCENTER;
						switch( element->align ) {
							case rdoSimulator::RDOTextElement::left  : nFormat |= DT_LEFT; break;
							case rdoSimulator::RDOTextElement::right : nFormat |= DT_RIGHT; break;
							case rdoSimulator::RDOTextElement::center: nFormat |= DT_CENTER; break;
						}

						::DrawText( hdc, element->strText.c_str(), element->strText.length(), CRect( (int)element->x, (int)element->y, (int)(element->x + element->w), (int)(element->y + element->h) ), nFormat );

						break;
					}
					case rdoSimulator::RDOFrameElement::rect_type: {
						rdoSimulator::RDORectElement* element = static_cast<rdoSimulator::RDORectElement*>(currElement);
						HBRUSH brush = ::CreateSolidBrush( RGB(element->background.r, element->background.g, element->background.b) );
						HBRUSH pOldBrush;
						if( !element->background.isTransparent ) {
							pOldBrush = static_cast<HBRUSH>(::SelectObject( hdc, brush ));
						} else {
							pOldBrush = static_cast<HBRUSH>(::GetStockObject( NULL_BRUSH ));
						}

						HPEN pen     = NULL;
						HPEN pOldPen = NULL;
						if( !element->foreground.isTransparent ) {
							pen     = ::CreatePen( PS_SOLID, 0, RGB(element->foreground.r, element->foreground.g, element->foreground.b) );
							pOldPen = static_cast<HPEN>(::SelectObject( hdc, pen ));
						}

						::Rectangle( hdc, (int)element->x, (int)element->y, (int)(element->x + element->w), (int)(element->y + element->h) );

						::SelectObject( hdc, pOldBrush );
						::DeleteObject( brush );
						if ( pen ) {
							::SelectObject( hdc, pOldPen );
							::DeleteObject( pen );
						}

						break;
					}
					case rdoSimulator::RDOFrameElement::r_rect_type: {
						rdoSimulator::RDORRectElement* element = static_cast<rdoSimulator::RDORRectElement*>(currElement);
						HBRUSH brush = ::CreateSolidBrush( RGB(element->background.r, element->background.g, element->background.b) );
						HBRUSH pOldBrush;
						if( !element->background.isTransparent ) {
							pOldBrush = static_cast<HBRUSH>(::SelectObject( hdc, brush ));
						} else {
							pOldBrush = static_cast<HBRUSH>(::GetStockObject( NULL_BRUSH ));
						}

						HPEN pen     = NULL;
						HPEN pOldPen = NULL;
						if( !element->foreground.isTransparent ) {
							pen     = ::CreatePen( PS_SOLID, 0, RGB(element->foreground.r, element->foreground.g, element->foreground.b) );
							pOldPen = static_cast<HPEN>(::SelectObject( hdc, pen ));
						}

						int w = (int)(std::min<double>( element->w, element->h ) / 3);
						RoundRect( hdc, (int)(element->x), (int)(element->y), (int)(element->x + element->w), (int)(element->y + element->h), w, w );

						::SelectObject( hdc, pOldBrush );
						::DeleteObject( brush );
						if ( pen ) {
							::SelectObject( hdc, pOldPen );
							::DeleteObject( pen );
						}

						break;
					}
					case rdoSimulator::RDOFrameElement::line_type: {
						rdoSimulator::RDOLineElement* element = static_cast<rdoSimulator::RDOLineElement*>(currElement);
						if( !element->foreground.isTransparent ) {
							HPEN pen     = ::CreatePen( PS_SOLID, 0, RGB(element->foreground.r, element->foreground.g, element->foreground.b) );
							HPEN pOldPen = static_cast<HPEN>(::SelectObject( hdc, pen ));

							::MoveToEx( hdc, (int)(element->x), (int)(element->y), NULL );
							::LineTo( hdc, (int)(element->w), (int)(element->h) );

							::SelectObject( hdc, pOldPen );
							::DeleteObject( pen );
						}

						break;
					}
					case rdoSimulator::RDOFrameElement::triang_type: {
						rdoSimulator::RDOTriangElement* element = static_cast<rdoSimulator::RDOTriangElement*>(currElement);
						HBRUSH brush = ::CreateSolidBrush( RGB(element->background.r, element->background.g, element->background.b) );
						HBRUSH pOldBrush;
						if( !element->background.isTransparent ) {
							pOldBrush = static_cast<HBRUSH>(::SelectObject( hdc, brush ));
						} else {
							pOldBrush = static_cast<HBRUSH>(::GetStockObject( NULL_BRUSH ));
						}

						HPEN pen     = NULL;
						HPEN pOldPen = NULL;
						if( !element->foreground.isTransparent ) {
							pen     = ::CreatePen( PS_SOLID, 0, RGB(element->foreground.r, element->foreground.g, element->foreground.b) );
							pOldPen = static_cast<HPEN>(::SelectObject( hdc, pen ));
						}

						CPoint pts[3];
						pts[0].x = (int)(element->x1);
						pts[0].y = (int)(element->y1);
						pts[1].x = (int)(element->x2);
						pts[1].y = (int)(element->y2);
						pts[2].x = (int)(element->x3);
						pts[2].y = (int)(element->y3);
						::Polygon( hdc, pts, 3 );

						::SelectObject( hdc, pOldBrush );
						::DeleteObject( brush );
						if ( pen ) {
							::SelectObject( hdc, pOldPen );
							::DeleteObject( pen );
						}

						break;
					}
					case rdoSimulator::RDOFrameElement::ellipse_type: {
						rdoSimulator::RDOEllipseElement* element = static_cast<rdoSimulator::RDOEllipseElement*>(currElement);
						HBRUSH brush = ::CreateSolidBrush( RGB(element->background.r, element->background.g, element->background.b) );
						HBRUSH pOldBrush;
						if( !element->background.isTransparent ) {
							pOldBrush = static_cast<HBRUSH>(::SelectObject( hdc, brush ));
						} else {
							pOldBrush = static_cast<HBRUSH>(::GetStockObject( NULL_BRUSH ));
						}

						HPEN pen     = NULL;
						HPEN pOldPen = NULL;
						if( !element->foreground.isTransparent ) {
							pen     = ::CreatePen( PS_SOLID, 0, RGB(element->foreground.r, element->foreground.g, element->foreground.b) );
							pOldPen = static_cast<HPEN>(::SelectObject( hdc, pen ));
						}

						::Ellipse( hdc, (int)(element->x), (int)(element->y), (int)(element->x + element->w), (int)(element->y + element->h) );

						::SelectObject( hdc, pOldBrush );
						::DeleteObject( brush );
						if ( pen ) {
							::SelectObject( hdc, pOldPen );
							::DeleteObject( pen );
						}

						break;
					}
					case rdoSimulator::RDOFrameElement::bitmap_type: {
						rdoSimulator::RDOBitmapElement* element = static_cast<rdoSimulator::RDOBitmapElement*>(currElement);
						BMP* bmp = bitmaps[element->bmp];
						if ( bmp ) {
							BMP* mask = element->hasMask ? bitmaps[element->mask] : NULL;
							CBitmap* pOldBitmap = dcBmp.SelectObject( &bmp->bmp );
							if ( mask ) {
								CBitmap* pOldMask = dcMask.SelectObject( &mask->bmp );
								::BitBlt( hdc, (int)(element->x), (int)(element->y), mask->w, mask->h, dcMask.m_hDC, 0, 0, SRCAND );
								::BitBlt( hdc, (int)(element->x), (int)(element->y), bmp->w, bmp->h, dcBmp.m_hDC, 0, 0, SRCPAINT );
								dcMask.SelectObject( pOldMask );
							} else {
								::BitBlt( hdc, (int)(element->x), (int)(element->y), bmp->w, bmp->h, dcBmp.m_hDC, 0, 0, SRCCOPY );
							}
							dcBmp.SelectObject( pOldBitmap );
						}
						break;
					}
					case rdoSimulator::RDOFrameElement::s_bmp_type: {
						rdoSimulator::RDOSBmpElement* element = static_cast<rdoSimulator::RDOSBmpElement*>(currElement);
						BMP* bmp = bitmaps[element->bmp];
						if ( bmp ) {
							BMP* mask = element->hasMask ? bitmaps[element->mask] : NULL;
							CBitmap* pOldBitmap = dcBmp.SelectObject( &bmp->bmp );
							if ( mask ) {
								CBitmap* pOldMask = dcMask.SelectObject( &mask->bmp );
								::StretchBlt( hdc, (int)(element->x), (int)(element->y), (int)(element->w), (int)(element->h), dcMask.m_hDC, 0, 0, mask->w, mask->h, SRCAND );
								::StretchBlt( hdc, (int)(element->x), (int)(element->y), (int)(element->w), (int)(element->h), dcBmp.m_hDC, 0, 0, bmp->w, bmp->h, SRCPAINT );
								dcMask.SelectObject( pOldMask );
							} else {
								::StretchBlt( hdc, (int)(element->x), (int)(element->y), (int)(element->w), (int)(element->h), dcBmp.m_hDC, 0, 0, bmp->w, bmp->h, SRCCOPY );
							}
							dcBmp.SelectObject( pOldBitmap );
						}
						break;
					}
					case rdoSimulator::RDOFrameElement::active_type: {
						rdoSimulator::RDOActiveElement* element = static_cast<rdoSimulator::RDOActiveElement*>(currElement);
						std::vector< Area* >::const_iterator it = frames[index]->areas_sim.begin();
						while ( it != frames[index]->areas_sim.end() ) {
							if ( (*it)->name == element->operName ) break;
							it++;
						}
						if ( it == frames[index]->areas_sim.end() ) {
							Area* area = new Area;
							area->name = element->operName;
							area->x    = (int)(element->x);
							area->y    = (int)(element->y);
							area->w    = (int)(element->w);
							area->h    = (int)(element->h);
							frames[index]->areas_sim.push_back( area );
						} else {
							(*it)->x    = (int)(element->x);
							(*it)->y    = (int)(element->y);
							(*it)->w    = (int)(element->w);
							(*it)->h    = (int)(element->h);
						}
						break;
					}
				}
			}

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
	if ( model->isRunning() && model->getRuntimeMode() != rdoRuntime::RTM_MaxSpeed && cnt > 1 && currentShowingFrame < cnt-1 ) {
		int index = currentShowingFrame + 1;
		RDOStudioFrameDoc* doc = getFrameDoc( index );
		if ( !doc ) {
			doc = connectFrameDoc( index );
		} else {
			doc->frame->ActivateFrame();
			setLastShowedFrame( index );
			setCurrentShowingFrame( index );
		}
	}
}

void RDOStudioFrameManager::showPrevFrame()
{
	int cnt = count();
	if ( model->isRunning() && model->getRuntimeMode() != rdoRuntime::RTM_MaxSpeed && cnt > 1 && currentShowingFrame > 0 ) {
		int index = currentShowingFrame - 1;
		RDOStudioFrameDoc* doc = getFrameDoc( index );
		if ( !doc ) {
			doc = connectFrameDoc( index );
		} else {
			doc->frame->ActivateFrame();
			setLastShowedFrame( index );
			setCurrentShowingFrame( index );
		}
	}
}

void RDOStudioFrameManager::showFrame( const int index )
{
	int cnt = count();
	if ( model->isRunning() && model->getRuntimeMode() != rdoRuntime::RTM_MaxSpeed && cnt > 1 && index >= 0 && index < cnt ) {
		RDOStudioFrameDoc* doc = getFrameDoc( index );
		if ( !doc ) {
			doc = connectFrameDoc( index );
		} else {
			doc->frame->ActivateFrame();
			setLastShowedFrame( index );
			setCurrentShowingFrame( index );
		}
	}
}

bool RDOStudioFrameManager::canShowNextFrame() const
{
	int cnt = count();
	return model->isRunning() && model->getRuntimeMode() != rdoRuntime::RTM_MaxSpeed && cnt > 1 && ( currentShowingFrame == -1 || currentShowingFrame < cnt-1 );
}

bool RDOStudioFrameManager::canShowPrevFrame() const
{
	int cnt = count();
	return model->isRunning() && model->getRuntimeMode() != rdoRuntime::RTM_MaxSpeed && cnt > 1 && currentShowingFrame > 0;
}

void RDOStudioFrameManager::updateStyles() const
{
	std::vector< Frame* >::const_iterator it = frames.begin();
	while ( it != frames.end() ) {
		RDOStudioFrameView* view = (*it++)->view;
		if ( view ) {
			view->updateFont();
			view->InvalidateRect( NULL );
			view->UpdateWindow();
		}
	}
}
