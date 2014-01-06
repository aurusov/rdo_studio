/*!
  \copyright (c) RDO-Team, 2011
  \file      context_find_i.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      03.03.2011
  \brief     
  \indent    4T
*/

#ifndef _RDOPARSER_CONTEXT_FIND_I_H_
#define _RDOPARSER_CONTEXT_FIND_I_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/context/context.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- IContextFind
// --------------------------------------------------------------------------------
OBJECT_INTERFACE(IContextFind)
{
DECLARE_FACTORY(IContextFind)
public:
	virtual Context::FindResult onFindContext(const std::string& method, const Context::Params& params, const RDOParserSrcInfo& srcInfo) const = 0;

protected:
	         IContextFind();
	virtual ~IContextFind();
};
#define DECLARE_IContextFind \
	Context::FindResult onFindContext(const std::string& method, const Context::Params& params, const RDOParserSrcInfo& srcInfo) const;

CLOSE_RDO_PARSER_NAMESPACE

#endif // _RDOPARSER_CONTEXT_FIND_I_H_
