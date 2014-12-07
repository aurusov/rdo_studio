#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/context/context.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- IContextFind
// --------------------------------------------------------------------------------
PREDECLARE_OBJECT_INTERFACE(IContextFind)
struct IContextFind: public rdo::RefCounter<IContextFind>
{
DECLARE_FACTORY(IContextFind)
public:
	virtual Context::LPFindResult onFindContext(const std::string& method, const Context::Params& params, const RDOParserSrcInfo& srcInfo) const = 0;

protected:
	         IContextFind();
	virtual ~IContextFind();
};
#define DECLARE_IContextFind \
	Context::LPFindResult onFindContext(const std::string& method, const Context::Params& params, const RDOParserSrcInfo& srcInfo) const;

CLOSE_RDO_PARSER_NAMESPACE
