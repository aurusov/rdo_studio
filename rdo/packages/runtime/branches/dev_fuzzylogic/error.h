/*!
  \copyright (c) RDO-Team, 2012
  \file      error.h
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      09.01.2012
  \brief     
  \indent    4T
*/

#ifndef _LIB_RUNTIME_ERROR_H_
#define _LIB_RUNTIME_ERROR_H_

// ----------------------------------------------------------------------- PLATFORM
// ----------------------------------------------------------------------- INCLUDES
#include <vector>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_object.h"
#include "simulator/report/error_code.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

class Error
{
public:
	typedef  std::vector<rdo::simulation::report::RDOSyntaxMessage>  ErrorList;

	void            push(CREF(rdo::simulation::report::RDOSyntaxMessage) error);
	void            push(CREF(tstring) message, CREF(RDOSrcInfo) srcInfo);
	CREF(ErrorList) list() const;

private:
	typedef rdo::simulation::report::RDOSyntaxMessage RDOSyntaxMessage;

	ErrorList m_errorList;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_ERROR_H_
