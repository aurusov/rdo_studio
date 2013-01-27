/*!
  \copyright (c) RDO-Team, 2012-2012
  \file      dock_base.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      30.09.2012
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_DOCK_BASE_H_
#define _RDO_STUDIO_DOCK_BASE_H_

// ----------------------------------------------------------------------- INCLUDES
#include <QtGui/qdockwidget.h>
#include <QtGui/qboxlayout.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/mfc_qt_wrapper.h"
// --------------------------------------------------------------------------------

template <class T>
class DockBase: public QDockWidget
{
public:
	typedef  MFCQtWrapperSimpleConstructor<T>  Context;

	DockBase(CREF(QString) caption, PTR(QWidget) pParent, CREF(typename Context::CreateFunction) createFunction, CREF(QSize) minSize)
		: QDockWidget(caption, pParent)
	{
		PTR(QWidget) pDocWidget = new QWidget();
		setWidget(pDocWidget);

		m_pContext = new Context(pDocWidget, createFunction);
		m_pContext->setMinimumSize(minSize);
		//! @todo qt
		//m_pContext->setPopupMenu(&popupMenu);

		PTR(QVBoxLayout) pLayout = new QVBoxLayout(pDocWidget);
		pLayout->setSpacing(0);
		pLayout->setContentsMargins(0, 0, 0, 0);
		pLayout->addWidget(m_pContext);

		m_pContext->init();
		getContext().ShowWindow(SW_SHOW);
	}

	virtual ~DockBase()
	{}

	REF(typename Context::context_type) getContext()
	{
		ASSERT(m_pContext);
		return m_pContext->getContext();
	}

private:
	typedef  QDockWidget  parent_type;

	PTR(Context)  m_pContext;
};

#endif // _RDO_STUDIO_DOCK_BASE_H_
