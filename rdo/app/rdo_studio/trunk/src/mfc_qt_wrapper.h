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
#include <boost/function.hpp>
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
	typedef  boost::function<T* ()>             ConstructFunction;
	typedef  boost::function<void (T*, CWnd*)>  CreateFunction;

	MFCQtWrapper(PTR(QWidget) pParent, CREF(ConstructFunction) constructFunction, CREF(CreateFunction) createFunction)
		: QWidget            (pParent          )
		, m_pContext         (NULL             )
		, m_constructFunction(constructFunction)
		, m_createFunction   (createFunction   )
	{}

	virtual ~MFCQtWrapper()
	{
		m_thisCWnd.Detach();
	}

	virtual rbool init()
	{
		ASSERT(!m_pContext);

		m_thisCWnd.Attach(winId());

		m_pContext = m_constructFunction();
		m_createFunction(m_pContext, &m_thisCWnd);

		return true;
	}

	REF(T) getContext()
	{
		ASSERT(m_pContext);
		return *m_pContext;
	}

private:
	PTR(T)             m_pContext;
	CWnd               m_thisCWnd;
	ConstructFunction  m_constructFunction;
	CreateFunction     m_createFunction;

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

template <class T>
class MFCQtWrapperSimpleConstructor: public MFCQtWrapper<T>
{
public:
	MFCQtWrapperSimpleConstructor(PTR(QWidget) pParent, CREF(CreateFunction) createFunction)
		: MFCQtWrapper(
			pParent,
			boost::bind<PTR(T)>(&MFCQtWrapperSimpleConstructor<T>::construct),
			createFunction
		)
	{}

	virtual ~MFCQtWrapperSimpleConstructor()
	{}

private:
	static PTR(T) construct()
	{
		return new T;
	}
};

#endif // _RDO_STUDIO_MFC_QT_WRAPPER_H_
