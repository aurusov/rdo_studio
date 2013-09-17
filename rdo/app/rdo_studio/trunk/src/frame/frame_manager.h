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
#include "utils/src/common/warning_disable.h"
#include <vector>
#include <map>
#include <memory>
#include <QMdiSubWindow>
#include <QTreeWidget>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/interface/rdointerface.h"
#include "app/rdo_studio/src/frame/frame_view.h"
// --------------------------------------------------------------------------------

OPEN_RDO_ANIMATION_NAMESPACE
struct Frame;
CLOSE_RDO_ANIMATION_NAMESPACE

namespace rdo { namespace gui { namespace frame {

class Manager
	: public QObject
	, public IInit
{
Q_OBJECT

public:
	typedef  boost::function<void (ruint)>  OnChangeFrame;

	Manager(CREF(OnChangeFrame) onChangeFrame);
	virtual ~Manager();

	void insertFrame (CREF(QString) frameName );
	void insertBitmap(CREF(QString) bitmapName);

	ruint findFrameIndex(CPTR(QTreeWidgetItem) pTreeWidgetItem) const;
	ruint findFrameIndex(CPTR(View)            pView          ) const;
	ruint findFrameIndex(CPTR(Content)         pContent       ) const;

	bool           isShowing         () const;
	CREF(QString)  getFrameName      (ruint index) const;
	PTR(View)      getFrameView      (ruint index) const;
	PTR(View)      getFrameViewFirst () const;
	ruint          count             () const;
	bool           isChanged         ();

	void           areaDown          (ruint frameIndex, CREF(QPoint) point) const;

	PTR(View)  createView    (ruint index);
	void       disconnectView(CPTR(View) pView);
	void       closeAll      ();
	void       clear         ();

	ruint getLastShowedFrame      () const;
	void  setLastShowedFrame      (ruint index);
	void  setCurrentShowingFrame  (ruint index);
	void  resetCurrentShowingFrame(ruint index);
	void  showFrame               (CPTRC(rdo::animation::Frame) pFrame, ruint index);
	void  showNextFrame           ();
	void  showPrevFrame           ();
	void  showFrame               (ruint index);
	bool  canShowNextFrame        () const;
	bool  canShowPrevFrame        () const;
	void  updateStyles            () const;

private:
	struct Frame
	{
		 Frame();
		~Frame();

		PTR(QTreeWidgetItem)           m_pTreeWidgetItem;
		QString                        m_name;
		PTR(View)                      m_pView;
		PTR(Content)                   m_pContent;
		rdo::gui::animation::AreaList  m_areaList;

	private:
		void clear();
	};

	typedef  std::vector<PTR(Frame)>  FrameList;

	FrameList             m_frameList;
	rdo::gui::BitmapList  m_bitmapList;
	ruint                 m_lastShowedFrame;
	ruint                 m_currentShowingFrame;
	bool                  m_changed;
	OnChangeFrame         m_onChangeFrame;

	DECLARE_IInit;

private slots:
	void onSubWindowActivated         (QMdiSubWindow* pWindow);
	void onTreeWidgetItemDoubleClicked(QTreeWidgetItem* pTreeWidgetItem, int);
};

}}} // namespace rdo::gui::frame

#endif // _RDO_STUDIO_FRAME_MANAGER_H_
