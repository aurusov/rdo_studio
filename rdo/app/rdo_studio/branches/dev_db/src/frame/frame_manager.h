/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      frame_manager.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      28.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_FRAME_MANAGER_H_
#define _RDO_STUDIO_FRAME_MANAGER_H_

// ----------------------------------------------------------------------- INCLUDES
#include <vector>
#include <map>
#include <memory>
#include <QtWidgets/qmdisubwindow.h>
#include <QtWidgets/qtreewidget.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdointerface.h"
#include "app/rdo_studio/src/frame/frame_view.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOStudioFrameManager
// --------------------------------------------------------------------------------
OPEN_RDO_ANIMATION_NAMESPACE
struct Frame;
CLOSE_RDO_ANIMATION_NAMESPACE

class RDOStudioFrameManager
	: public QObject
	, public IInit
{
Q_OBJECT

public:
	typedef  boost::function<void (ruint)>  OnChangeFrame;

	RDOStudioFrameManager(CREF(OnChangeFrame) onChangeFrame);
	virtual ~RDOStudioFrameManager();

	void insertFrame (CREF(QString) frameName );
	void insertBitmap(CREF(QString) bitmapName);

	ruint findFrameIndex(CPTR(QTreeWidgetItem)       pTreeWidgetItem) const;
	ruint findFrameIndex(CPTR(FrameAnimationWnd)     pView          ) const;
	ruint findFrameIndex(CPTR(FrameAnimationContent) pContent       ) const;

	rbool                   isShowing         () const;
	CREF(QString)           getFrameName      (ruint index) const;
	PTR(FrameAnimationWnd)  getFrameView      (ruint index) const;
	PTR(FrameAnimationWnd)  getFrameViewFirst () const;
	ruint                   count             () const;
	rbool                   isChanged         ();

	void                    areaDown          (ruint frameIndex, CREF(QPoint) point) const;

	PTR(FrameAnimationWnd)  createView    (ruint index);
	void                    disconnectView(CPTR(FrameAnimationWnd) pView);
	void                    closeAll      ();
	void                    clear         ();

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
	struct Frame
	{
		 Frame();
		~Frame();

		PTR(QTreeWidgetItem)           m_pTreeWidgetItem;
		QString                        m_name;
		PTR(FrameAnimationWnd)         m_pView;
		PTR(FrameAnimationContent)     m_pContent;
		rdo::gui::animation::AreaList  m_areaList;

	private:
		void clear();
	};

	typedef  std::vector<PTR(Frame)>  FrameList;

	FrameList             m_frameList;
	rdo::gui::BitmapList  m_bitmapList;
	ruint                 m_lastShowedFrame;
	ruint                 m_currentShowingFrame;
	rbool                 m_changed;
	OnChangeFrame         m_onChangeFrame;

	DECLARE_IInit;

private slots:
	void onSubWindowActivated         (QMdiSubWindow* pWindow);
	void onTreeWidgetItemDoubleClicked(QTreeWidgetItem* pTreeWidgetItem, int);
};

#endif // _RDO_STUDIO_FRAME_MANAGER_H_
