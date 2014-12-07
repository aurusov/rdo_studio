#ifndef _LIB_RUNTIME_ERROR_H_
#define _LIB_RUNTIME_ERROR_H_

// ----------------------------------------------------------------------- PLATFORM
// ----------------------------------------------------------------------- INCLUDES
#include <vector>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_object.h"
#include "simulator/report/src/file_message.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

class Error
{
public:
	typedef  std::vector<rdo::simulation::report::FileMessage>  ErrorList;

	void push(const rdo::simulation::report::FileMessage& error);
	void push(const std::string& message, const RDOSrcInfo& srcInfo);
	const ErrorList& list() const;

private:
	typedef rdo::simulation::report::FileMessage RDOSyntaxMessage;

	ErrorList m_errorList;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_ERROR_H_
