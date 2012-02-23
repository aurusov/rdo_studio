/*!
  \copyright (c) RDO-Team, 2011
  \file      rdoparser_error.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      09.02.2010
  \brief     основан на rdoparser.h/cpp
  \indent    4T
*/

#ifndef _CONVERTOR_RDOCONVERTER_ERROR_H_
#define _CONVERTOR_RDOCONVERTER_ERROR_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdocommon.h"
#include "converter/smr2rdox/namespace.h"
#include "converter/smr2rdox/rdo_object.h"
#include "simulator/runtime/rdo_exception.h"
#include "simulator/service/error_code.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOSyntaxException
// --------------------------------------------------------------------------------
class RDOSyntaxException: public rdoRuntime::RDOException
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
	typedef std::vector<rdoSimulator::RDOSyntaxError> ErrorList;

	Error();

	//! 1
	void error    (CREF(RDOParserSrcInfo) src_info, rdoSimulator::RDOSyntaxError::ErrorCode error_code);
	void push_only(CREF(RDOParserSrcInfo) src_info, rdoSimulator::RDOSyntaxError::ErrorCode error_code);

	//! 2
	void error    (CREF(RDOParserSrcInfo) src_info, CREF(tstring) message, rdoSimulator::RDOSyntaxError::ErrorCode error_code = rdoSimulator::RDOSyntaxError::UNKNOWN);
	void warning  (CREF(RDOParserSrcInfo) src_info, CREF(tstring) message, rdoSimulator::RDOSyntaxError::ErrorCode error_code = rdoSimulator::RDOSyntaxError::UNKNOWN);
	void push_only(CREF(RDOParserSrcInfo) src_info, CREF(tstring) message, rdoSimulator::RDOSyntaxError::ErrorCode error_code = rdoSimulator::RDOSyntaxError::UNKNOWN);

	//! 3
	void error    (CREF(RDOParserSrcInfo) src_info1, CREF(RDOParserSrcInfo) src_info2, CREF(tstring) message, rdoSimulator::RDOSyntaxError::ErrorCode error_code = rdoSimulator::RDOSyntaxError::UNKNOWN);

	//! misc
	void            push_done();
	void            modify   (CREF(tstring) message);
	void            clear    ();
	CREF(ErrorList) getList  () const;
	void            block    ();
	void            unblock  ();
	rbool           blocked  () const;

private:
	ErrorList m_errors;
	rbool     m_blocked;
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

CLOSE_RDO_CONVERTER_NAMESPACE

#endif // _CONVERTOR_RDOCONVERTER_ERROR_H_
