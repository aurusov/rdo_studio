/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      mfc_qt_wrapper.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      03.10.2012
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_QT_WRAPPER_H_
#define _RDO_STUDIO_MFC_QT_WRAPPER_H_

// ----------------------------------------------------------------------- INCLUDES
#include <QtGui/qwidget.h>
#include <QtGui/qevent.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdointerface.h"
// --------------------------------------------------------------------------------

template <class T>
class MFCQtWrapper
	: public QWidget
	, public IInit
{
public:
	typedef  T        context_type;
	typedef  QWidget  parent_type;

	MFCQtWrapper(PTR(QWidget) pParent)
		: QWidget   (pParent)
		, m_pContext(NULL   )
	{}

	virtual ~MFCQtWrapper()
	{
		m_thisCWnd.Detach();
	}

	virtual rbool init()
	{
		ASSERT(!m_pContext);

		m_thisCWnd.Attach(winId());

		m_pContext = new T;
		m_pContext->Create(NULL, NULL, 0, CRect(0, 0, 0, 0), &m_thisCWnd, 0);

		return true;
	}

	REF(T) getContext()
	{
		ASSERT(m_pContext);
		return *m_pContext;
	}

private:
	PTR(T) m_pContext;
	CWnd   m_thisCWnd;

	virtual void resizeEvent(PTR(QResizeEvent) event)
	{
		parent_type::resizeEvent(event);

		if (m_pContext)
		{
			QSize size(event->size());
			m_pContext->MoveWindow(0, 0, size.width(), size.height());
		}
	}

	virtual void closeEvent(PTR(QCloseEvent) event)
	{
		ASSERT(m_pContext);
		m_pContext->DestroyWindow();
		delete m_pContext;

		parent_type::closeEvent(event);
	}
};

#endif // _RDO_STUDIO_MFC_QT_WRAPPER_H_
