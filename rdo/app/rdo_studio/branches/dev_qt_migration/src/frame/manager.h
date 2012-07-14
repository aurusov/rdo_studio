/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio_mfc/src/frame/manager.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      28.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_FRAME_MANAGER_H_
#define _RDO_STUDIO_MFC_FRAME_MANAGER_H_

// ----------------------------------------------------------------------- INCLUDES
#include <vector>
#include <map>
#include <memory>
#include <gdiplus.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/frame/document.h"
#include "app/rdo_studio_mfc/src/frame/view.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOStudioFrameManager
// --------------------------------------------------------------------------------
OPEN_RDO_ANIMATION_NAMESPACE
struct Frame;
CLOSE_RDO_ANIMATION_NAMESPACE

class RDOStudioFrameManager
{
public:
	RDOStudioFrameManager();
	virtual ~RDOStudioFrameManager();

	void insertFrame (CREF(tstring) frameName );
	void insertBitmap(CREF(tstring) bitmapName);

	ruint findFrameIndex(const HTREEITEM          hitem) const;
	ruint findFrameIndex(CPTR(RDOStudioFrameDoc)  pDoc ) const;
	ruint findFrameIndex(CPTR(RDOStudioFrameView) pView) const;

	CREF(tstring)           getFrameName      (ruint index) const;
	PTR(RDOStudioFrameDoc)  getFrameDoc       (ruint index) const;
	PTR(RDOStudioFrameView) getFrameView      (ruint index) const;
	ruint                   count             () const;
	rbool                   isChanged         ();

	void                    areaDown          (ruint frameIndex, CREF(Gdiplus::Point) point) const;

	PTR(RDOStudioFrameDoc)  connectFrameDoc   (ruint index);
	void                    disconnectFrameDoc(CPTR(RDOStudioFrameDoc) pDoc);
	PTR(RDOStudioFrameDoc)  getFirstExistDoc  () const;
	void                    closeAll          ();
	void                    clear             ();
	void                    expand            () const;

	ruint getLastShowedFrame      () const;
	void  setLastShowedFrame      (ruint index);
	void  setCurrentShowingFrame  (ruint index);
	void  resetCurrentShowingFrame(ruint index);
	void  showFrame               (CPTRC(rdo::animation::Frame) pFrame, ruint index);
	void  showNextFrame           ();
	void  showPrevFrame           ();
	void  showFrame               (ruint index);
	rbool canShowNextFrame        () const;
	rbool canShowPrevFrame        () const;
	void  updateStyles            () const;

private:
	class FrameDocTemplate: public CMultiDocTemplate
	{
	public:
		FrameDocTemplate(UINT nIDResource, PTR(CRuntimeClass) pDocClass, PTR(CRuntimeClass) pFrameClass, PTR(CRuntimeClass) pViewClass);

		virtual PTR(CFrameWnd) CreateNewFrame(PTR(CDocument) pDoc, PTR(CFrameWnd) pOther);
	};

	struct Frame
	{
		 Frame();
		~Frame();

		HTREEITEM                     m_hitem;
		tstring                       m_name;
		PTR(RDOStudioFrameDoc)        m_pDoc;
		PTR(RDOStudioFrameView)       m_pView;
		RDOStudioFrameView::AreaList  m_areaList;

	private:
		void clear();
	};

	typedef  std::vector<PTR(Frame)>  FrameList;

	FrameList             m_frameList;
	FrameAnimationContent::BitmapList  m_bitmapList;
	PTR(FrameDocTemplate) m_pFrameDocTemplate;
	ruint                 m_lastShowedFrame;
	ruint                 m_currentShowingFrame;
	rbool                 m_changed;

	rbool isValidFrameDoc(CPTRC(RDOStudioFrameDoc) pFrame) const;
};

#endif // _RDO_STUDIO_MFC_FRAME_MANAGER_H_
