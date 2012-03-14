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
#include "ui/gdiplus/headers/bitmap/bitmap.h"
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
// -------------------- RDOStudioFrameManager::Frame
// --------------------------------------------------------------------------------
RDOStudioFrameManager::Frame::Frame()
	: m_hitem(0   )
	, m_pDoc (NULL)
	, m_pView(NULL)
{}

RDOStudioFrameManager::Frame::~Frame()
{
	clear();
}

void RDOStudioFrameManager::Frame::clear()
{
	STL_FOR_ALL(m_areaList, it)
	{
		delete *it;
	}
	m_areaList.clear();
}

// --------------------------------------------------------------------------------
// -------------------- RDOStudioFrameManager
// --------------------------------------------------------------------------------
RDOStudioFrameManager::FrameDocTemplate::FrameDocTemplate(UINT nIDResource, PTR(CRuntimeClass) pDocClass, PTR(CRuntimeClass) pFrameClass, PTR(CRuntimeClass) pViewClass)
	: CMultiDocTemplate(nIDResource, pDocClass, pFrameClass, pViewClass)
{}

PTR(CFrameWnd) RDOStudioFrameManager::FrameDocTemplate::CreateNewFrame(PTR(CDocument) pDoc, PTR(CFrameWnd) pOther)
{
	PTR(CFrameWnd) pFrame = CMultiDocTemplate::CreateNewFrame(pDoc, pOther);
	static_cast<PTR(RDOStudioFrameDoc)>(pDoc)->frame = pFrame;
	return pFrame;
}

// --------------------------------------------------------------------------------
// -------------------- RDOStudioFrameManager
// --------------------------------------------------------------------------------
RDOStudioFrameManager::RDOStudioFrameManager()
	: m_pFrameDocTemplate  (NULL     )
	, m_lastShowedFrame    (ruint(~0))
	, m_currentShowingFrame(ruint(~0))
	, m_changed            (false    )
{
	//! @todo А почему объект не удаляется ? Это происходит автоматически ?
	m_pFrameDocTemplate = new FrameDocTemplate(IDR_FRAME_TYPE, RUNTIME_CLASS(RDOStudioFrameDoc), RUNTIME_CLASS(RDOStudioChildFrame), RUNTIME_CLASS(RDOStudioFrameView));
	AfxGetApp()->AddDocTemplate(m_pFrameDocTemplate);
}

RDOStudioFrameManager::~RDOStudioFrameManager()
{
	clear();

	STL_FOR_ALL(m_frameList, it)
	{
		delete *it;
	}
}

void RDOStudioFrameManager::insertFrame(CREF(tstring) frameName)
{
	PTR(Frame) item = new Frame();
	item->m_hitem = studioApp.m_pMainFrame->workspace.frames->InsertItem(frameName.c_str(), 1, 1, studioApp.m_pMainFrame->workspace.frames->GetRootItem());
	item->m_name  = frameName;
	item->m_pDoc  = NULL;
	item->m_pView = NULL;
	m_frameList.push_back(item);
}

ruint RDOStudioFrameManager::findFrameIndex(const HTREEITEM hitem) const
{
	ruint index = 0;
	STL_FOR_ALL_CONST(m_frameList, it)
	{
		if ((*it)->m_hitem == hitem)
		{
			return index;
		}
		index++;
	}
	return ruint(~0);
}

ruint RDOStudioFrameManager::findFrameIndex(CPTR(RDOStudioFrameDoc) pDoc) const
{
	ruint index = 0;
	STL_FOR_ALL_CONST(m_frameList, it)
	{
		if ((*it)->m_pDoc == pDoc)
		{
			return index;
		}
		index++;
	}
	return ruint(~0);
}

ruint RDOStudioFrameManager::findFrameIndex(CPTR(RDOStudioFrameView) pView) const
{
	ruint index = 0;
	STL_FOR_ALL_CONST(m_frameList, it)
	{
		if ((*it)->m_pView == pView)
		{
			return index;
		}
		index++;
	}
	return ruint(~0);
}

CREF(tstring) RDOStudioFrameManager::getFrameName(ruint index) const
{
	ASSERT(index < m_frameList.size());
	return m_frameList[index]->m_name;
}

PTR(RDOStudioFrameDoc) RDOStudioFrameManager::getFrameDoc(ruint index) const
{
	ASSERT(index < m_frameList.size());
	return m_frameList[index]->m_pDoc;
}

PTR(RDOStudioFrameView) RDOStudioFrameManager::getFrameView(ruint index) const
{
	ASSERT(index < m_frameList.size());
	return m_frameList[index]->m_pView;
}

ruint RDOStudioFrameManager::count() const
{
	return m_frameList.size();
}

rbool RDOStudioFrameManager::isChanged()
{
	rbool res = m_changed;
	m_changed = false;
	return res;
}

void RDOStudioFrameManager::areaDown(ruint frameIndex, CREF(Gdiplus::Point) point) const
{
	ASSERT(frameIndex != ruint(~0) && frameIndex < m_frameList.size());

	CREF(RDOStudioFrameManager::Frame::AreaList) areaList = m_frameList[frameIndex]->m_areaList;
	STL_FOR_ALL_CONST(areaList, it)
	{
		if ((*it)->m_rect.Contains(point))
		{
			model->sendMessage(kernel->runtime(), RDOThread::RT_RUNTIME_FRAME_AREA_DOWN, &(*it)->m_name);
		}
	}
}

PTR(RDOStudioFrameDoc) RDOStudioFrameManager::connectFrameDoc(ruint index)
{
	PTR(RDOStudioFrameDoc) pDoc = NULL;
	if (index != ~0)
	{
		pDoc = static_cast<PTR(RDOStudioFrameDoc)>(m_pFrameDocTemplate->OpenDocumentFile(NULL));
		if (pDoc)
		{
			m_frameList[index]->m_pDoc  = pDoc;
			m_frameList[index]->m_pView = pDoc->getView();
			m_lastShowedFrame           = index;
			pDoc->SetTitle(rdo::format(IDS_FRAME_NAME, getFrameName(index).c_str()).c_str());
			setCurrentShowingFrame(index);
		}
	}
	return pDoc;
}

void RDOStudioFrameManager::disconnectFrameDoc(CPTR(RDOStudioFrameDoc) pDoc)
{
	ruint index = findFrameIndex(pDoc);
	if (index != ~0)
	{
		m_frameList[index]->m_pDoc  = NULL;
		m_frameList[index]->m_pView = NULL;
	}
	m_changed = true;
}

void RDOStudioFrameManager::closeAll()
{
	ruint backup = m_lastShowedFrame;
	STL_FOR_ALL(m_frameList, it)
	{
		PTR(RDOStudioFrameDoc) pFrameDoc = (*it)->m_pDoc;
		if (isValidFrameDoc(pFrameDoc))
		{
			if (pFrameDoc->frame && pFrameDoc->frame->GetSafeHwnd())
			{
				pFrameDoc->frame->SendNotifyMessage(WM_CLOSE, 0, 0);
			}
		}
	}
	m_lastShowedFrame = backup;
}

void RDOStudioFrameManager::clear()
{
	studioApp.m_pMainFrame->workspace.frames->deleteChildren(studioApp.m_pMainFrame->workspace.frames->GetRootItem());
	STL_FOR_ALL(m_frameList, it)
	{
		PTR(RDOStudioFrameDoc) pFrameDoc = (*it)->m_pDoc;
		if (isValidFrameDoc(pFrameDoc))
		{
			if (pFrameDoc->frame && pFrameDoc->frame->GetSafeHwnd())
			{
				pFrameDoc->frame->SendMessage(WM_CLOSE, 0, 0);
			}
		}
		delete *it;
	}

	STL_FOR_ALL(m_bitmapList, it)
	{
		delete it->second;
	}

	STL_FOR_ALL(m_bitmapMaskInvertList, it)
	{
		delete it->second;
	}

	m_frameList           .clear();
	m_bitmapList          .clear();
	m_bitmapMaskInvertList.clear();

	m_lastShowedFrame = ruint(~0);
	setCurrentShowingFrame(ruint(~0));
}

PTR(RDOStudioFrameDoc) RDOStudioFrameManager::getFirstExistDoc() const
{
	STL_FOR_ALL_CONST(m_frameList, it)
	{
		if (isValidFrameDoc((*it)->m_pDoc))
		{
			return (*it)->m_pDoc;
		}
	}
	return NULL;
}

void RDOStudioFrameManager::expand() const
{
	studioApp.m_pMainFrame->workspace.frames->expand();
}

rbool RDOStudioFrameManager::isValidFrameDoc(CPTRC(RDOStudioFrameDoc) pFrame) const
{
	POSITION pos = m_pFrameDocTemplate->GetFirstDocPosition();
	while (pos)
	{
		PTR(RDOStudioFrameDoc) pDoc = static_cast<PTR(RDOStudioFrameDoc)>(m_pFrameDocTemplate->GetNextDoc(pos));
		if (pFrame == pDoc)
		{
			return true;
		}
	}
	return false;
}

ruint RDOStudioFrameManager::getLastShowedFrame() const
{
	return m_lastShowedFrame;
}

void RDOStudioFrameManager::setLastShowedFrame(ruint index)
{
	if (index != ruint(~0) && index < count())
	{
		m_lastShowedFrame = index;
	}
}

void RDOStudioFrameManager::setCurrentShowingFrame(ruint index)
{
	if (index == ruint(~0) || (index != ruint(~0) && index < count()))
	{
		m_currentShowingFrame = index;
		PTR(CTreeCtrl) pTree = studioApp.m_pMainFrame->workspace.frames;
		if (m_currentShowingFrame != ruint(~0))
		{
			HTREEITEM hitem = m_frameList[m_currentShowingFrame]->m_hitem;
			pTree->SelectItem(hitem);
		}
		else
		{
			pTree->SelectItem(NULL);
		}
	}
}

void RDOStudioFrameManager::resetCurrentShowingFrame(ruint index)
{
	if (index == m_currentShowingFrame)
	{
		setCurrentShowingFrame(ruint(~0));
	}
}

void RDOStudioFrameManager::insertBitmap(CREF(tstring) bitmapName)
{
	if (m_bitmapList.find(bitmapName) != m_bitmapList.end())
		return;

	PTR(RDOStudioOutput) pOutput = &studioApp.m_pMainFrame->output;
	ASSERT(pOutput);
	pOutput->appendStringToDebug(rdo::format(IDS_MODEL_RESOURCE_LOADING_NAME, bitmapName.c_str()));
	const_cast<PTR(rdoEditCtrl::RDODebugEdit)>(pOutput->getDebug())->UpdateWindow();

	rdo::binarystream stream;
	rdoRepository::RDOThreadRepository::BinaryFile data(bitmapName, stream);
	model->sendMessage(kernel->repository(), RDOThread::RT_REPOSITORY_LOAD_BINARY, &data);

	rbool ok = false;
	PTR(Gdiplus::Bitmap) pBitmap = rdo::gui::Bitmap::load(data.m_name);
	if (pBitmap)
	{
		std::pair<BitmapList::const_iterator, rbool> result = m_bitmapList.insert(BitmapList::value_type(bitmapName, pBitmap));
		if (result.second)
		{
			ok = true;
		}
	}

	pOutput->appendStringToDebug(rdo::format(ok ? IDS_MODEL_RESOURCE_LOADING_NAME_OK : IDS_MODEL_RESOURCE_LOADING_NAME_FAILED));
	const_cast<PTR(rdoEditCtrl::RDODebugEdit)>(pOutput->getDebug())->UpdateWindow();
}

void RDOStudioFrameManager::showFrame(CPTRC(rdoAnimation::RDOFrame) pFrame, ruint index)
{
	if (index < count())
	{
		PTR(RDOStudioFrameDoc) pFrameDoc = getFrameDoc(index);
		if (pFrameDoc)
		{
			PTR(RDOStudioFrameView) pFrameView = getFrameView(index);
			if (!pFrameView->valid())
			{
				Gdiplus::Size size;
				rbool imageFound = false;
				if (pFrame->hasBgImage())
				{
					BitmapList::const_iterator bmpIt = m_bitmapList.find(pFrame->m_bgImageName);
					if (bmpIt != m_bitmapList.end())
					{
						size.Width  = bmpIt->second->GetWidth ();
						size.Height = bmpIt->second->GetHeight();
						imageFound  = true;
					}
				}
				if (!imageFound)
				{
					size.Width  = (ruint)pFrame->m_size.m_width;
					size.Height = (ruint)pFrame->m_size.m_height;
				}
				pFrameView->init(size);
			}

			rbool showFillrect = true;
			if (pFrame->hasBgImage())
			{
				BitmapList::const_iterator bmpIt = m_bitmapList.find(pFrame->m_bgImageName);
				if (bmpIt != m_bitmapList.end())
				{
					pFrameView->dc().DrawImage(bmpIt->second, 0, 0, bmpIt->second->GetWidth(), bmpIt->second->GetHeight());
					showFillrect = false;
				}
			}
			Gdiplus::Color bgColor;
			if (pFrame->m_bgColor.m_transparent)
			{
				bgColor.SetFromCOLORREF(studioApp.m_pMainFrame->style_frame.theme->backgroundColor);
			}
			else
			{
				bgColor.SetFromCOLORREF(RGB(pFrame->m_bgColor.m_r, pFrame->m_bgColor.m_g, pFrame->m_bgColor.m_b));
			}
			pFrameView->setBGColor(bgColor);

			ruint size = pFrame->m_elements.size();
			for (ruint i = 0; i < size; i++)
			{
				PTR(rdoAnimation::FrameItem) pCurrElement = pFrame->m_elements[i];
				ASSERT(pCurrElement);
				switch (pCurrElement->getType())
				{
					case rdoAnimation::FrameItem::FIT_TEXT:
					{
						Gdiplus::Color color(0, 0, 0);
						PTR(rdoAnimation::RDOTextElement) pElement = static_cast<PTR(rdoAnimation::RDOTextElement)>(pCurrElement);
						if (!pElement->m_background.m_transparent)
						{
//							::SetBkMode(hdc, OPAQUE);
//							::SetBkColor(hdc, RGB(pElement->m_background.m_r, pElement->m_background.m_g, pElement->m_background.m_b));
//							color = Gdiplus::Color(pElement->m_background.m_r, pElement->m_background.m_g, pElement->m_background.m_b);
						}
						else
						{
//							::SetBkMode(hdc, TRANSPARENT);
						}

						if(!pElement->m_foreground.m_transparent)
						{
//							::SetTextColor(hdc, RGB(pElement->m_foreground.m_r, pElement->m_foreground.m_g, pElement->m_foreground.m_b));
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

						//HDC hDC = pFrameView->dc().GetHDC();
						//Gdiplus::Font  font(hDC, pFrameView->hfontCurrent);
						//pFrameView->dc().ReleaseHDC(hDC);

						//if (font.GetLastStatus() == Gdiplus::Ok)
						//{
						//	Gdiplus::RectF rect(
						//		Gdiplus::REAL(pElement->m_point.m_x),
						//		Gdiplus::REAL(pElement->m_point.m_y),
						//		Gdiplus::REAL(pElement->m_size.m_width),
						//		Gdiplus::REAL(pElement->m_size.m_height)
						//	);

						//	wtext = L"text";

						//	status = pFrameView->dc().DrawString(wtext.c_str(), wtext.length(), &font, rect, &sformat, &brush);
//							::DrawText(hdc, pElement->m_text.c_str(), pElement->m_text.length(), CRect((int)pElement->m_point.m_x, (int)pElement->m_point.m_y, (int)(pElement->m_point.m_x + pElement->m_size.m_width), (int)(pElement->m_point.m_y + pElement->m_size.m_height)), nFormat);
						//}

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

						pFrameView->dc().FillRectangle(&brush, (int)pElement->m_point.m_x, (int)pElement->m_point.m_y, (int)pElement->m_size.m_width, (int)pElement->m_size.m_height);
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
						pFrameView->dc().FillRectangle(&brush, (int)pElement->m_point.m_x, (int)pElement->m_point.m_y, (int)pElement->m_size.m_width, (int)pElement->m_size.m_height);
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
							pFrameView->dc().DrawLine(
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
							pFrameView->dc().FillPolygon(&brush, &pointList[0], pountListCount);
						}

						if (!pElement->m_foreground.m_transparent)
						{
							Gdiplus::Color color;
							color.SetFromCOLORREF(RGB(pElement->m_foreground.m_r, pElement->m_foreground.m_g, pElement->m_foreground.m_b));
							Gdiplus::Pen pen(color);
							pFrameView->dc().DrawPolygon(&pen, &pointList[0], pountListCount);
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
							pFrameView->dc().FillEllipse(&brush, rect);
						}

						if (!pElement->m_foreground.m_transparent)
						{
							Gdiplus::Color color;
							color.SetFromCOLORREF(RGB(pElement->m_foreground.m_r, pElement->m_foreground.m_g, pElement->m_foreground.m_b));
							Gdiplus::Pen pen(color);
							pFrameView->dc().DrawEllipse(&pen, rect);
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
							pFrameView->dc().FillEllipse(&brush, rect);
						}

						if (!pElement->m_foreground.m_transparent)
						{
							Gdiplus::Color color;
							color.SetFromCOLORREF(RGB(pElement->m_foreground.m_r, pElement->m_foreground.m_g, pElement->m_foreground.m_b));
							Gdiplus::Pen pen(color);
							pFrameView->dc().DrawEllipse(&pen, rect);
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
									BitmapList::const_iterator maskInvertIt = m_bitmapMaskInvertList.find(pElement->m_mask_name);
									if (maskInvertIt == m_bitmapMaskInvertList.end())
									{
										PTR(Gdiplus::Bitmap) pMaskInvert = NULL;
										{
											Gdiplus::Graphics graphics(maskIt->second);
											pMaskInvert = new Gdiplus::Bitmap(
												maskIt->second->GetWidth (),
												maskIt->second->GetHeight(),
												&graphics
											);
										}
										if (pMaskInvert)
										{
											if (pMaskInvert->GetLastStatus() == Gdiplus::Ok)
											{
												std::pair<BitmapList::const_iterator, rbool> result = m_bitmapMaskInvertList.insert(BitmapList::value_type(pElement->m_mask_name, pMaskInvert));
												if (result.second)
												{
													maskInvertIt = result.first;

													Gdiplus::ColorMatrix colorMatrix = {
														-1,  0,  0, 0, 0,
														 0, -1,  0, 0, 0,
														 0,  0, -1, 0, 0,
														 0,  0,  0, 1, 0,
														 1,  1,  1, 0, 1
													};
													Gdiplus::ImageAttributes imageAttributes;
													imageAttributes.SetColorMatrix(&colorMatrix);

													Gdiplus::Graphics graphics(maskInvertIt->second);
													graphics.DrawImage(
														maskIt->second,
														Gdiplus::RectF(
															0.0,
															0.0,
															Gdiplus::REAL(maskIt->second->GetWidth()),
															Gdiplus::REAL(maskIt->second->GetHeight())
														),
														0.0,
														0.0,
														Gdiplus::REAL(maskIt->second->GetWidth()),
														Gdiplus::REAL(maskIt->second->GetHeight()),
														Gdiplus::UnitPixel,
														&imageAttributes
													);
												}
											}
											else
											{
												delete pMaskInvert;
											}
										}
									}

									if (maskInvertIt != m_bitmapMaskInvertList.end())
									{
										//Gdiplus::Graphics graphics(maskInvertIt->second);
										pFrameView->dc().DrawImage(maskInvertIt->second, 0, 0);
										break;
									}

									Gdiplus::ImageAttributes imageAttributes;
									imageAttributes.SetColorKey(
										Gdiplus::ARGB(Gdiplus::Color::Black),
										Gdiplus::ARGB(Gdiplus::Color::Black)
									);
									pFrameView->dc().DrawImage(
										maskIt->second,
										Gdiplus::RectF(
											Gdiplus::REAL(pElement->m_point.m_x),
											Gdiplus::REAL(pElement->m_point.m_y),
											Gdiplus::REAL(maskIt->second->GetWidth()),
											Gdiplus::REAL(maskIt->second->GetHeight())
										),
										0.0,
										0.0,
										Gdiplus::REAL(maskIt->second->GetWidth()),
										Gdiplus::REAL(maskIt->second->GetHeight()),
										Gdiplus::UnitPixel,
										&imageAttributes
									);
									maskDraw = true;

									//MemDC bg;
									//if (bg.create(maskIt->second->GetWidth(), maskIt->second->GetHeight(), pFrameView->dc()))
									//{
									//	Gdiplus::RectF bgRect    (0.0, 0.0, Gdiplus::REAL(bg.width()), Gdiplus::REAL(bg.height()));
									//	Gdiplus::RectF bufferRect(Gdiplus::REAL(pElement->m_point.m_x), Gdiplus::REAL(pElement->m_point.m_y), Gdiplus::REAL(bg.width()), Gdiplus::REAL(bg.height()));
									//	Gdiplus::ImageAttributes imageAttributes;
									//	bg.dc().DrawImage(&pFrameView->buffer(), 0, 0, Gdiplus::REAL(pElement->m_point.m_x), Gdiplus::REAL(pElement->m_point.m_y), Gdiplus::REAL(bg.width()), Gdiplus::REAL(bg.height()));
									//}
									//CBitmap* pOldMask = dcMask.SelectObject(&mask->bmp);
									//::BitBlt(hdc, (int)(element->m_point.m_x), (int)(element->m_point.m_y), mask->w, mask->h, dcMask.m_hDC, 0, 0, SRCAND);
									//::BitBlt(hdc, (int)(element->m_point.m_x), (int)(element->m_point.m_y), bmp->w, bmp->h, dcBmp.m_hDC, 0, 0, SRCPAINT);
									//dcMask.SelectObject(pOldMask);

//									NEVER_REACH_HERE;
//									maskDraw = true;
								}
							}
							if (!maskDraw)
							{
								pFrameView->dc().DrawImage(bmpIt->second, (int)(pElement->m_point.m_x), (int)(pElement->m_point.m_y));
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
								pFrameView->dc().DrawImage(bmpIt->second, (int)(pElement->m_point.m_x), (int)(pElement->m_point.m_y), (int)(pElement->m_size.m_width), (int)(pElement->m_size.m_height));
							}
						}
						break;
					}

					case rdoAnimation::FrameItem::FIT_ACTIVE:
					{
						PTR(rdoAnimation::RDOActiveElement) pElement = static_cast<PTR(rdoAnimation::RDOActiveElement)>(pCurrElement);
						Frame::AreaList::const_iterator it = m_frameList[index]->m_areaList.begin();
						while (it != m_frameList[index]->m_areaList.end())
						{
							if ((*it)->m_name == pElement->m_opr_name)
								break;
							++it;
						}
						if (it == m_frameList[index]->m_areaList.end())
						{
							PTR(Frame::Area) pArea = new Frame::Area();
							pArea->m_name = pElement->m_opr_name;
							m_frameList[index]->m_areaList.push_back(pArea);
							it = m_frameList[index]->m_areaList.end();
							--it;
						}
						(*it)->m_rect.X      = (int)(pElement->m_point.m_x);
						(*it)->m_rect.Y      = (int)(pElement->m_point.m_y);
						(*it)->m_rect.Width  = (int)(pElement->m_size.m_width);
						(*it)->m_rect.Height = (int)(pElement->m_size.m_height);
						break;
					}
				}
			}

			pFrameView->InvalidateRect   (NULL);
			pFrameView->SendNotifyMessage(WM_PAINT, 0, 0);
		}
	}
}

void RDOStudioFrameManager::showNextFrame()
{
	ruint cnt = count();
	if (model->isRunning() && model->getRuntimeMode() != rdoRuntime::RTM_MaxSpeed && cnt > 1 && m_currentShowingFrame < cnt-1)
	{
		ruint index = m_currentShowingFrame + 1;
		PTR(RDOStudioFrameDoc) pDoc = getFrameDoc(index);
		if (!pDoc)
		{
			pDoc = connectFrameDoc(index);
		}
		else
		{
			pDoc->frame->ActivateFrame();
			setLastShowedFrame    (index);
			setCurrentShowingFrame(index);
		}
	}
}

void RDOStudioFrameManager::showPrevFrame()
{
	ruint cnt = count();
	if (model->isRunning() && model->getRuntimeMode() != rdoRuntime::RTM_MaxSpeed && cnt > 1 && m_currentShowingFrame != ruint(~0))
	{
		ruint index = m_currentShowingFrame - 1;
		PTR(RDOStudioFrameDoc) pDoc = getFrameDoc(index);
		if (!pDoc)
		{
			pDoc = connectFrameDoc(index);
		}
		else
		{
			pDoc->frame->ActivateFrame();
			setLastShowedFrame    (index);
			setCurrentShowingFrame(index);
		}
	}
}

void RDOStudioFrameManager::showFrame(ruint index)
{
	ruint cnt = count();
	if (model->isRunning() && model->getRuntimeMode() != rdoRuntime::RTM_MaxSpeed && cnt > 1 && index >= 0 && index < cnt)
	{
		PTR(RDOStudioFrameDoc) pDoc = getFrameDoc(index);
		if (!pDoc)
		{
			pDoc = connectFrameDoc(index);
		}
		else
		{
			pDoc->frame->ActivateFrame();
			setLastShowedFrame    (index);
			setCurrentShowingFrame(index);
		}
	}
}

rbool RDOStudioFrameManager::canShowNextFrame() const
{
	ruint cnt = count();
	return model->isRunning() && model->getRuntimeMode() != rdoRuntime::RTM_MaxSpeed && cnt > 1 && (m_currentShowingFrame == ruint(~0) || m_currentShowingFrame < cnt-1);
}

rbool RDOStudioFrameManager::canShowPrevFrame() const
{
	int cnt = count();
	return model->isRunning() && model->getRuntimeMode() != rdoRuntime::RTM_MaxSpeed && cnt > 1 && (m_currentShowingFrame != ruint(~0) && m_currentShowingFrame > 0);
}

void RDOStudioFrameManager::updateStyles() const
{
	STL_FOR_ALL_CONST(m_frameList, it)
	{
		PTR(RDOStudioFrameView) pFrameView = (*it)->m_pView;
		if (pFrameView)
		{
			pFrameView->updateFont    ();
			pFrameView->InvalidateRect(NULL);
			pFrameView->UpdateWindow  ();
		}
	}
}
