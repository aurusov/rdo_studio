/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio_mfc/src/frame/manager.cpp
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Копнин Андрей (kopninandrey@gmail.com)
  \date      28.03.2003
  \brief     Отрисовка кадров анимации
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <gdiplus.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdostream.h"
#include "utils/rdoanimation.h"
#include "kernel/rdokernel.h"
#include "kernel/rdothread.h"
#include "simulator/service/rdosimwin.h"
#include "repository/rdorepository.h"
#include "app/rdo_studio_mfc/src/frame/manager.h"
#include "app/rdo_studio_mfc/src/model/model.h"
#include "app/rdo_studio_mfc/src/application.h"
#include "app/rdo_studio_mfc/src/main_frm.h"
#include "app/rdo_studio_mfc/src/child_frm.h"
#include "app/rdo_studio_mfc/src/workspace.h"
#include "app/rdo_studio_mfc/src/frame/tree_ctrl.h"
#include "app/rdo_studio_mfc/edit_ctrls/rdodebugedit.h"
#include "app/rdo_studio_mfc/resource.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOStudioFrameManager
// --------------------------------------------------------------------------------
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

	STL_FOR_ALL(m_frameList, it)
	{
		delete *it;
	}
}

void RDOStudioFrameManager::insertItem( CREF(tstring) name )
{
	Frame* item = new Frame;
	item->hitem = studioApp.m_pMainFrame->workspace.frames->InsertItem( name.c_str(), 1, 1, studioApp.m_pMainFrame->workspace.frames->GetRootItem() );
	item->name  = name;
	item->doc   = NULL;
	item->view  = NULL;
	m_frameList.push_back(item);
}

RDOStudioFrameDoc* RDOStudioFrameManager::connectFrameDoc( const int index )
{
	RDOStudioFrameDoc* doc = NULL;
	if ( index != -1 )
	{
		doc = static_cast<RDOStudioFrameDoc*>(frameDocTemplate->OpenDocumentFile( NULL ));
		if ( doc )
		{
			m_frameList[index]->doc  = doc;
			m_frameList[index]->view = doc->getView();
			lastShowedFrame          = index;
			doc->SetTitle( rdo::format( IDS_FRAME_NAME, getFrameName( index ).c_str() ).c_str()  );
			setCurrentShowingFrame( index );
		}
	}
	return doc;
}

void RDOStudioFrameManager::disconnectFrameDoc( const RDOStudioFrameDoc* doc )
{
	int index = findFrameIndex( doc );
	if ( index != -1 )
	{
		m_frameList[index]->doc  = NULL;
		m_frameList[index]->view = NULL;
	}
	changed = true;
}

void RDOStudioFrameManager::closeAll()
{
	int backup = lastShowedFrame;
	STL_FOR_ALL(m_frameList, it)
	{
		PTR(RDOStudioFrameDoc) pFrameDoc = (*it)->doc;
		if (isValidFrameDoc(pFrameDoc))
		{
			if (pFrameDoc->frame && pFrameDoc->frame->GetSafeHwnd())
			{
				pFrameDoc->frame->SendNotifyMessage(WM_CLOSE, 0, 0);
			}
		}
	}
	lastShowedFrame = backup;
}

void RDOStudioFrameManager::clear()
{
	studioApp.m_pMainFrame->workspace.frames->deleteChildren(studioApp.m_pMainFrame->workspace.frames->GetRootItem());
	STL_FOR_ALL(m_frameList, it)
	{
		PTR(RDOStudioFrameDoc) pFrameDoc = (*it)->doc;
		if (isValidFrameDoc(pFrameDoc))
		{
			if (pFrameDoc->frame && pFrameDoc->frame->GetSafeHwnd())
			{
				pFrameDoc->frame->SendMessage(WM_CLOSE, 0, 0);
			}
		}
		delete *it;
	}
	m_frameList.clear();
	lastShowedFrame = -1;
	setCurrentShowingFrame(-1);
}

RDOStudioFrameDoc* RDOStudioFrameManager::getFirstExistDoc() const
{
	STL_FOR_ALL_CONST(m_frameList, it)
	{
		if (isValidFrameDoc((*it)->doc))
		{
			return (*it)->doc;
		}
	}
	return NULL;
}

void RDOStudioFrameManager::expand() const
{
	studioApp.m_pMainFrame->workspace.frames->expand();
}

rbool RDOStudioFrameManager::isValidFrameDoc( const RDOStudioFrameDoc* const frame ) const
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
		CTreeCtrl* tree = studioApp.m_pMainFrame->workspace.frames;
		if ( currentShowingFrame != -1 ) {
			HTREEITEM hitem = m_frameList[currentShowingFrame]->hitem;
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

void RDOStudioFrameManager::bmp_insert(CREF(tstring) name)
{
	if (m_bitmapList.find(name) != m_bitmapList.end())
		return;

	PTR(RDOStudioOutput) pOutput = &studioApp.m_pMainFrame->output;
	ASSERT(pOutput);
	pOutput->appendStringToDebug(rdo::format(IDS_MODEL_RESOURCE_LOADING_NAME, name.c_str()));
	const_cast<PTR(rdoEditCtrl::RDODebugEdit)>(pOutput->getDebug())->UpdateWindow();

	rdo::binarystream stream;
	rdoRepository::RDOThreadRepository::BinaryFile data(name, stream);
	model->sendMessage(kernel->repository(), RDOThread::RT_REPOSITORY_LOAD_BINARY, &data);

	rbool ok = false;
	PTR(Gdiplus::Bitmap) pBitmap = new Gdiplus::Bitmap(rdo::toUnicode(data.m_name).c_str());
	if (pBitmap && pBitmap->GetLastStatus() == Gdiplus::Ok)
	{
		std::pair<BitmapList::const_iterator, rbool> result = m_bitmapList.insert(BitmapList::value_type(name, pBitmap));
		if (result.second)
		{
			ok = true;
		}
	}

	pOutput->appendStringToDebug(rdo::format(ok ? IDS_MODEL_RESOURCE_LOADING_NAME_OK : IDS_MODEL_RESOURCE_LOADING_NAME_FAILED));
	const_cast<PTR(rdoEditCtrl::RDODebugEdit)>(pOutput->getDebug())->UpdateWindow();
}

void RDOStudioFrameManager::bmp_clear()
{
	STL_FOR_ALL(m_bitmapList, it)
	{
		delete it->second;
	}
	m_bitmapList.clear();
}

void RDOStudioFrameManager::showFrame( const rdoAnimation::RDOFrame* const frame, const int index )
{
	if (index < count())
	{
		PTR(RDOStudioFrameDoc) pFrameDoc = getFrameDoc(index);
		if (pFrameDoc)
		{
			PTR(RDOStudioFrameView) pFrameView = getFrameView(index);
			if (pFrameView->mustBeInit)
			{
				rbool showFillrect = true;
				if (frame->hasBgImage())
				{
					BitmapList::const_iterator bmpIt = m_bitmapList.find(frame->m_bgImageName);
					if (bmpIt != m_bitmapList.end())
					{
						pFrameView->frameBmpRect.Width  = bmpIt->second->GetWidth ();
						pFrameView->frameBmpRect.Height = bmpIt->second->GetHeight();
						showFillrect = false;
					}
				}
				if (showFillrect)
				{
					pFrameView->frameBmpRect.Width  = (ruint)frame->m_size.m_width;
					pFrameView->frameBmpRect.Height = (ruint)frame->m_size.m_height;
				}
				pFrameView->points[0].X = 0;
				pFrameView->points[0].Y = 0;
				pFrameView->points[1].X = pFrameView->frameBmpRect.GetRight() - 1;
				pFrameView->points[1].Y = 0;
				pFrameView->points[2].X = pFrameView->frameBmpRect.GetRight() - 1;
				pFrameView->points[2].Y = pFrameView->frameBmpRect.GetBottom() - 1;
				pFrameView->points[3].X = 0;
				pFrameView->points[3].Y = pFrameView->frameBmpRect.GetBottom() - 1;
				pFrameView->points[4].X = 0;
				pFrameView->points[4].Y = 0;
				pFrameView->m_memDC.resize(pFrameView->frameBmpRect.GetRight(), pFrameView->frameBmpRect.GetBottom());
				pFrameView->mustBeInit = false;
				pFrameView->updateScrollBars();
			}

			Gdiplus::Status status;

			rbool showFillrect = true;
			if (frame->hasBgImage())
			{
				BitmapList::const_iterator bmpIt = m_bitmapList.find(frame->m_bgImageName);
				if (bmpIt != m_bitmapList.end())
				{
					status = pFrameView->m_memDC.dc().DrawImage(bmpIt->second, 0, 0, bmpIt->second->GetWidth(), bmpIt->second->GetHeight());
					showFillrect = false;
				}
			}
			if (frame->m_bgColor.m_transparent)
			{
				pFrameView->m_bgColor.SetFromCOLORREF(studioApp.m_pMainFrame->style_frame.theme->backgroundColor);
			}
			else
			{
				pFrameView->m_bgColor.SetFromCOLORREF(RGB(frame->m_bgColor.m_r, frame->m_bgColor.m_g, frame->m_bgColor.m_b));
			}
			if (showFillrect)
			{
				Gdiplus::SolidBrush brush(pFrameView->m_bgColor);
				status = pFrameView->m_memDC.dc().FillRectangle(&brush, pFrameView->frameBmpRect);

				//! @todo проверить ыцет карандаша. возможен косяк без SetFromCOLORREF
				Gdiplus::Pen pen(studioApp.m_pMainFrame->style_frame.theme->defaultColor, 1.0);
				status = pFrameView->m_memDC.dc().DrawPolygon(&pen, pFrameView->points, 5);
			}

			ruint size = frame->m_elements.size();
			for (ruint i = 0; i < size; i++)
			{
				PTR(rdoAnimation::FrameItem) pCurrElement = frame->m_elements[i];
				ASSERT(pCurrElement);
				switch (pCurrElement->getType())
				{
					case rdoAnimation::FrameItem::FIT_TEXT:
					{
						Gdiplus::Color color(0, 0, 0);
						PTR(rdoAnimation::RDOTextElement) pElement = static_cast<PTR(rdoAnimation::RDOTextElement)>(pCurrElement);
						if (!pElement->m_background.m_transparent)
						{
//							::SetBkMode( hdc, OPAQUE );
//							::SetBkColor( hdc, RGB(pElement->m_background.m_r, pElement->m_background.m_g, pElement->m_background.m_b) );
//							color = Gdiplus::Color(pElement->m_background.m_r, pElement->m_background.m_g, pElement->m_background.m_b);
						}
						else
						{
//							::SetBkMode( hdc, TRANSPARENT );
						}

						if(!pElement->m_foreground.m_transparent)
						{
//							::SetTextColor( hdc, RGB(pElement->m_foreground.m_r, pElement->m_foreground.m_g, pElement->m_foreground.m_b) );
						}

						Gdiplus::StringFormat sformat;
						Gdiplus::SolidBrush   brush(color);

						UINT nFormat = DT_SINGLELINE | DT_VCENTER;
						switch (pElement->m_align)
						{
						case rdoAnimation::RDOTextElement::TETA_LEFT  : nFormat |= DT_LEFT;   sformat.SetAlignment(Gdiplus::StringAlignmentNear  ); break;
						case rdoAnimation::RDOTextElement::TETA_RIGHT : nFormat |= DT_RIGHT;  sformat.SetAlignment(Gdiplus::StringAlignmentFar   ); break;
						case rdoAnimation::RDOTextElement::TETA_CENTER: nFormat |= DT_CENTER; sformat.SetAlignment(Gdiplus::StringAlignmentCenter); break;
						}

						std::wstring wtext = rdo::toUnicode(pElement->m_text);

						HDC hDC = pFrameView->m_memDC.dc().GetHDC();
						Gdiplus::Font  font(hDC, pFrameView->hfontCurrent);
						pFrameView->m_memDC.dc().ReleaseHDC(hDC);

						if (font.GetLastStatus() == Gdiplus::Ok)
						{
							Gdiplus::RectF rect(
								Gdiplus::REAL(pElement->m_point.m_x),
								Gdiplus::REAL(pElement->m_point.m_y),
								Gdiplus::REAL(pElement->m_size.m_width),
								Gdiplus::REAL(pElement->m_size.m_height)
							);

							wtext = L"text";

							status = pFrameView->m_memDC.dc().DrawString(wtext.c_str(), wtext.length(), &font, rect, &sformat, &brush);
//							::DrawText( hdc, pElement->m_text.c_str(), pElement->m_text.length(), CRect( (int)pElement->m_point.m_x, (int)pElement->m_point.m_y, (int)(pElement->m_point.m_x + pElement->m_size.m_width), (int)(pElement->m_point.m_y + pElement->m_size.m_height) ), nFormat );
						}

						break;
					}

					case rdoAnimation::FrameItem::FIT_RECT:
					{
						PTR(rdoAnimation::RDORectElement) pElement = static_cast<PTR(rdoAnimation::RDORectElement)>(pCurrElement);
						Gdiplus::Color bgColor;
						if (!pElement->m_background.m_transparent)
						{
							bgColor = Gdiplus::Color(pElement->m_background.m_r, pElement->m_background.m_g, pElement->m_background.m_b);
						}
						else
						{
							bgColor.SetValue(0);
						}
						Gdiplus::SolidBrush brush(bgColor);

						status = pFrameView->m_memDC.dc().FillRectangle(&brush, (int)pElement->m_point.m_x, (int)pElement->m_point.m_y, (int)pElement->m_size.m_width, (int)pElement->m_size.m_height);
						//! @todo добавить вывод линии вокруг прямоугольника

						break;
					}

					case rdoAnimation::FrameItem::FIT_R_RECT:
					{
						PTR(rdoAnimation::RDORRectElement) pElement = static_cast<PTR(rdoAnimation::RDORRectElement)>(pCurrElement);
						Gdiplus::Color bgColor;
						if (!pElement->m_background.m_transparent)
						{
							bgColor = Gdiplus::Color(pElement->m_background.m_r, pElement->m_background.m_g, pElement->m_background.m_b);
						}
						else
						{
							bgColor.SetValue(0);
						}
						Gdiplus::SolidBrush brush(bgColor);

						//! @todo gdi+ не умеет выводить roundrect :(
						status = pFrameView->m_memDC.dc().FillRectangle(&brush, (int)pElement->m_point.m_x, (int)pElement->m_point.m_y, (int)pElement->m_size.m_width, (int)pElement->m_size.m_height);
						//! @todo добавить вывод линии вокруг прямоугольника

						break;
					}

					case rdoAnimation::FrameItem::FIT_LINE:
					{
						PTR(rdoAnimation::RDOLineElement) pElement = static_cast<PTR(rdoAnimation::RDOLineElement)>(pCurrElement);
						if (!pElement->m_color.m_transparent)
						{
							Gdiplus::Color color;
							color.SetFromCOLORREF(RGB(pElement->m_color.m_r, pElement->m_color.m_g, pElement->m_color.m_b));
							Gdiplus::Pen pen(color);
							pFrameView->m_memDC.dc().DrawLine(
								&pen,
								(int)(pElement->m_point1.m_x), (int)(pElement->m_point1.m_y),
								(int)(pElement->m_point2.m_x), (int)(pElement->m_point2.m_y)
							);
						}

						break;
					}

					case rdoAnimation::FrameItem::FIT_TRIANG:
					{
						PTR(rdoAnimation::RDOTriangElement) pElement = static_cast<PTR(rdoAnimation::RDOTriangElement)>(pCurrElement);

						const ruint pountListCount = 3;
						Gdiplus::Point pointList[pountListCount];
						pointList[0].X = (int)(pElement->m_point1.m_x);
						pointList[0].Y = (int)(pElement->m_point1.m_y);
						pointList[1].X = (int)(pElement->m_point2.m_x);
						pointList[1].Y = (int)(pElement->m_point2.m_y);
						pointList[2].X = (int)(pElement->m_point3.m_x);
						pointList[2].Y = (int)(pElement->m_point3.m_y);

						if (!pElement->m_background.m_transparent)
						{
							Gdiplus::Color color;
							color.SetFromCOLORREF(RGB(pElement->m_background.m_r, pElement->m_background.m_g, pElement->m_background.m_b));
							Gdiplus::SolidBrush brush(color);
							pFrameView->m_memDC.dc().FillPolygon(&brush, &pointList[0], pountListCount);
						}

						if (!pElement->m_foreground.m_transparent)
						{
							Gdiplus::Color color;
							color.SetFromCOLORREF(RGB(pElement->m_foreground.m_r, pElement->m_foreground.m_g, pElement->m_foreground.m_b));
							Gdiplus::Pen pen(color);
							pFrameView->m_memDC.dc().DrawPolygon(&pen, &pointList[0], pountListCount);
						}

						break;
					}

					case rdoAnimation::FrameItem::FIT_CIRCLE:
					{
						PTR(rdoAnimation::RDOCircleElement) pElement = static_cast<PTR(rdoAnimation::RDOCircleElement)>(pCurrElement);

						Gdiplus::Rect rect(
							(int)(pElement->m_center.m_x - pElement->m_radius.m_radius),
							(int)(pElement->m_center.m_y - pElement->m_radius.m_radius),
							(int)(pElement->m_center.m_x + pElement->m_radius.m_radius),
							(int)(pElement->m_center.m_y + pElement->m_radius.m_radius)
						);

						if (!pElement->m_background.m_transparent)
						{
							Gdiplus::Color color;
							color.SetFromCOLORREF(RGB(pElement->m_background.m_r, pElement->m_background.m_g, pElement->m_background.m_b));
							Gdiplus::SolidBrush brush(color);
							pFrameView->m_memDC.dc().FillEllipse(&brush, rect);
						}

						if (!pElement->m_foreground.m_transparent)
						{
							Gdiplus::Color color;
							color.SetFromCOLORREF(RGB(pElement->m_foreground.m_r, pElement->m_foreground.m_g, pElement->m_foreground.m_b));
							Gdiplus::Pen pen(color);
							pFrameView->m_memDC.dc().DrawEllipse(&pen, rect);
						}

						break;
					}

					case rdoAnimation::FrameItem::FIT_ELLIPSE:
					{
						PTR(rdoAnimation::RDOEllipseElement) pElement = static_cast<PTR(rdoAnimation::RDOEllipseElement)>(pCurrElement);

						Gdiplus::Rect rect(
							(int)(pElement->m_point.m_x),
							(int)(pElement->m_point.m_y),
							(int)(pElement->m_point.m_x + pElement->m_size.m_width),
							(int)(pElement->m_point.m_y + pElement->m_size.m_height)
						);

						if (!pElement->m_background.m_transparent)
						{
							Gdiplus::Color color;
							color.SetFromCOLORREF(RGB(pElement->m_background.m_r, pElement->m_background.m_g, pElement->m_background.m_b));
							Gdiplus::SolidBrush brush(color);
							pFrameView->m_memDC.dc().FillEllipse(&brush, rect);
						}

						if (!pElement->m_foreground.m_transparent)
						{
							Gdiplus::Color color;
							color.SetFromCOLORREF(RGB(pElement->m_foreground.m_r, pElement->m_foreground.m_g, pElement->m_foreground.m_b));
							Gdiplus::Pen pen(color);
							pFrameView->m_memDC.dc().DrawEllipse(&pen, rect);
						}

						break;
					}

					case rdoAnimation::FrameItem::FIT_BMP:
					{
						PTR(rdoAnimation::RDOBmpElement) pElement = static_cast<PTR(rdoAnimation::RDOBmpElement)>(pCurrElement);
						BitmapList::const_iterator bmpIt = m_bitmapList.find(pElement->m_bmp_name);
						if (bmpIt != m_bitmapList.end())
						{
							rbool maskDraw = false;
							if (pElement->hasMask())
							{
								BitmapList::const_iterator maskIt = m_bitmapList.find(pElement->m_mask_name);
								if (maskIt != m_bitmapList.end())
								{
//									NEVER_REACH_HERE;
//									maskDraw = true;
								}
							}
							if (!maskDraw)
							{
								status = pFrameView->m_memDC.dc().DrawImage(bmpIt->second, (int)(pElement->m_point.m_x), (int)(pElement->m_point.m_y));
							}
						}
						break;
					}

					case rdoAnimation::FrameItem::FIT_S_BMP:
					{
						PTR(rdoAnimation::RDOSBmpElement) pElement = static_cast<PTR(rdoAnimation::RDOSBmpElement)>(pCurrElement);
						BitmapList::const_iterator bmpIt = m_bitmapList.find(pElement->m_bmp_name);
						if (bmpIt != m_bitmapList.end())
						{
							rbool maskDraw = false;
							if (pElement->hasMask())
							{
								BitmapList::const_iterator maskIt = m_bitmapList.find(pElement->m_mask_name);
								if (maskIt != m_bitmapList.end())
								{
//									NEVER_REACH_HERE;
//									maskDraw = true;
								}
							}
							if (!maskDraw)
							{
								status = pFrameView->m_memDC.dc().DrawImage(bmpIt->second, (int)(pElement->m_point.m_x), (int)(pElement->m_point.m_y), (int)(pElement->m_size.m_width), (int)(pElement->m_size.m_height));
							}
						}
						break;
					}

					case rdoAnimation::FrameItem::FIT_ACTIVE:
					{
						rdoAnimation::RDOActiveElement* pElement = static_cast<rdoAnimation::RDOActiveElement*>(pCurrElement);
						std::vector< Area* >::const_iterator it = m_frameList[index]->areas_sim.begin();
						while ( it != m_frameList[index]->areas_sim.end() ) {
							if ( (*it)->name == pElement->m_opr_name ) break;
							it++;
						}
						if ( it == m_frameList[index]->areas_sim.end() ) {
							Area* area = new Area;
							area->name = pElement->m_opr_name;
							area->x    = (int)(pElement->m_point.m_x);
							area->y    = (int)(pElement->m_point.m_y);
							area->w    = (int)(pElement->m_size.m_width);
							area->h    = (int)(pElement->m_size.m_height);
							m_frameList[index]->areas_sim.push_back( area );
						} else {
							(*it)->x    = (int)(pElement->m_point.m_x);
							(*it)->y    = (int)(pElement->m_point.m_y);
							(*it)->w    = (int)(pElement->m_size.m_width);
							(*it)->h    = (int)(pElement->m_size.m_height);
						}
						break;
					}
				}
			}

			//pFrameView->InvalidateRect( NULL );
			//pFrameView->SendNotifyMessage( WM_PAINT, 0, 0 );
		}
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

rbool RDOStudioFrameManager::canShowNextFrame() const
{
	int cnt = count();
	return model->isRunning() && model->getRuntimeMode() != rdoRuntime::RTM_MaxSpeed && cnt > 1 && ( currentShowingFrame == -1 || currentShowingFrame < cnt-1 );
}

rbool RDOStudioFrameManager::canShowPrevFrame() const
{
	int cnt = count();
	return model->isRunning() && model->getRuntimeMode() != rdoRuntime::RTM_MaxSpeed && cnt > 1 && currentShowingFrame > 0;
}

void RDOStudioFrameManager::updateStyles() const
{
	STL_FOR_ALL_CONST(m_frameList, it)
	{
		PTR(RDOStudioFrameView) pFrameView = (*it)->view;
		if (pFrameView)
		{
			pFrameView->updateFont    ();
			pFrameView->InvalidateRect(NULL);
			pFrameView->UpdateWindow  ();
		}
	}
}
