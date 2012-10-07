/*!
  \copyright (c) RDO-Team, 2012-2012
  \file      dock_results.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      03.10.2012
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/bind.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/dock/dock_results.h"
// --------------------------------------------------------------------------------

DockResults::DockResults(PTR(QWidget) pParent)
	: parent_class(
		"Результаты",
		pParent,
		parent_class::Context::CreateFunction(
			boost::bind<BOOL>(&parent_class::Context::context_type::Create, _1, LPCTSTR(NULL), LPCTSTR(NULL), DWORD(0), CRect(0, 0, 0, 0), _2, UINT(0), static_cast<CCreateContext*>(NULL))
		)
	)
{}

DockResults::~DockResults()
{}

void DockResults::appendString(CREF(tstring) str)
{
	int pos = getContext().getCurrentPos();
	getContext().setCurrentPos(getContext().getLength());
	getContext().setReadOnly  (false);
	getContext().appendText   (str  );
	getContext().setReadOnly  (true );
	getContext().setCurrentPos(pos  );
}

void DockResults::clear()
{
	getContext().clearAll();
}
