/*!
  \copyright (c) RDO-Team, 2011
  \file      rdoparser_error.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      09.02.2010
  \brief     основан на rdoparser.h/cpp
  \indent    4T
*/

#ifndef _RDOPARSER_ERROR_H_
#define _RDOPARSER_ERROR_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdocommon.h"
#include "simulator/runtime/rdo_exception.h"
#include "simulator/compiler/parser/namespace.h"
#include "simulator/compiler/parser/rdo_object.h"
#include "simulator/service/error_code.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOSyntaxException
// --------------------------------------------------------------------------------
class RDOSyntaxException: public rdo::runtime::RDOException
{
public:
   RDOSyntaxException(CREF(tstring) message)
	   : RDOException(message)
   {}
   virtual tstring getType() const { return _T("RDO Syntax Error"); }
};

// --------------------------------------------------------------------------------
// -------------------- Error
// --------------------------------------------------------------------------------
class Error
{
public:
	typedef std::vector<rdo::service::simulation::RDOSyntaxMessage> ErrorList;

	Error();

	//! 1
	void error    (CREF(RDOParserSrcInfo) src_info, CREF(tstring) message);
	void warning  (CREF(RDOParserSrcInfo) src_info, CREF(tstring) message);
	void push_only(CREF(RDOParserSrcInfo) src_info, CREF(tstring) message);

	//! 2
	void error    (CREF(RDOParserSrcInfo) src_info1, CREF(RDOParserSrcInfo) src_info2, CREF(tstring) message);

	//! misc
	void            push_done();
	void            modify   (CREF(tstring) message);
	void            clear    ();
	CREF(ErrorList) getList  () const;
	void            block    ();
	void            unblock  ();
	rbool           blocked  () const;

private:
	ErrorList  m_errorList;
	rbool      m_blocked;
};

REF(Error) g_error();

class ErrorBlockMonicker
{
public:
	ErrorBlockMonicker()
	{
		g_error().block();
	}
	~ErrorBlockMonicker()
	{
		g_error().unblock();
	}
};

CLOSE_RDO_PARSER_NAMESPACE

#endif // _RDOPARSER_ERROR_H_
