/*!
  \copyright (c) RDO-Team, 2011
  \file      rdoparser_error.h
  \authors   ���� ���������
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \date      09.02.2010
  \brief     ������� �� rdoparser.h/cpp
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
#include "simulator/report/error_code.h"
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
	typedef std::vector<rdo::simulation::report::RDOSyntaxMessage> ErrorList;

	Error();

	//! 1
	void error    (CREF(RDOParserSrcInfo) src_info, rdo::simulation::report::RDOSyntaxMessage::ErrorCode error_code);
	void push_only(CREF(RDOParserSrcInfo) src_info, rdo::simulation::report::RDOSyntaxMessage::ErrorCode error_code);

	//! 2
	void error    (CREF(RDOParserSrcInfo) src_info, CREF(tstring) message, rdo::simulation::report::RDOSyntaxMessage::ErrorCode errorCode = rdo::simulation::report::RDOSyntaxMessage::UNKNOWN);
	void warning  (CREF(RDOParserSrcInfo) src_info, CREF(tstring) message, rdo::simulation::report::RDOSyntaxMessage::ErrorCode errorCode = rdo::simulation::report::RDOSyntaxMessage::UNKNOWN);
	void push_only(CREF(RDOParserSrcInfo) src_info, CREF(tstring) message, rdo::simulation::report::RDOSyntaxMessage::ErrorCode errorCode = rdo::simulation::report::RDOSyntaxMessage::UNKNOWN);

	//! 3
	void error    (CREF(RDOParserSrcInfo) src_info1, CREF(RDOParserSrcInfo) src_info2, CREF(tstring) message, rdo::simulation::report::RDOSyntaxMessage::ErrorCode error_code = rdo::simulation::report::RDOSyntaxMessage::UNKNOWN);

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
