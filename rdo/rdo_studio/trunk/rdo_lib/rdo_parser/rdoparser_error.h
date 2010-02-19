/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdoparser_error.h
 * author   : Александ Барс, Урусов Андрей
 * date     : 09.02.2010
 * bref     : основан на rdoparser.h/cpp
 * indent   : 4T
 */

#ifndef _RDOPARSER_ERROR_H_
#define _RDOPARSER_ERROR_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/rdocommon.h"
#include "rdo_lib/rdo_parser/namespace.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOSyntaxException
// ----------------------------------------------------------------------------
class RDOSyntaxException: public rdoRuntime::RDOException
{
public:
   RDOSyntaxException(CREF(tstring) message)
	   : RDOException(message)
   {}
   virtual tstring getType() const { return _T("RDO Syntax Error"); }
};

// ----------------------------------------------------------------------------
// ---------- Error
// ----------------------------------------------------------------------------
class Error
{
public:
	typedef std::vector<rdoSimulator::RDOSyntaxError> ErrorList;

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

private:
	ErrorList m_errors;
};

REF(Error) g_error();

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _RDOPARSER_ERROR_H_
