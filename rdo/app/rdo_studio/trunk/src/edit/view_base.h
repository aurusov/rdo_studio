/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio_mfc/src/edit/view_base.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_EDIT_VIEW_BASE_H_
#define _RDO_STUDIO_MFC_EDIT_VIEW_BASE_H_

// ----------------------------------------------------------------------- INCLUDES
#include <QtGui/qwidget.h>
#include <QtGui/qmenu.h>
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOStudioEditBaseView
// --------------------------------------------------------------------------------
namespace rdoEditor {
	class RDOEditorEdit;
}

class RDOStudioEditBaseView: public QWidget
{
Q_OBJECT

public:
	virtual ~RDOStudioEditBaseView();

	struct Buffer
	{
		tstring  value;
		rbool    reset;
		QTimer*  pTimer;

		Buffer(QTimer* pTimer);
	};
	typedef  std::map<ruint, Buffer>  BufferList;

	QMenu*      m_pPopupMenu;
	BufferList  m_bufferList;
	ruint       m_currentBuffer;

	void restartBufTimer(ruint bufferID);

	virtual rdoEditor::RDOEditorEdit* getEdit() const;

protected:
	RDOStudioEditBaseView(QWidget* pParent);

	CWnd m_thisCWnd;

private:
	void stopTimer(QTimer* pTimer);

private slots:
	virtual void timerEvent(QTimerEvent* event);
};

#endif // _RDO_STUDIO_MFC_EDIT_VIEW_BASE_H_
