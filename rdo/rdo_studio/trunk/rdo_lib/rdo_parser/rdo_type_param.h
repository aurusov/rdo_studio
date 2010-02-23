/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_type_param.h
 * author   : Урусов Андрей
 * date     : 12.02.2010
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOPARSER_TYPE_PARAM_H_
#define _RDOPARSER_TYPE_PARAM_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdo_type.h"
#include "rdo_lib/rdo_parser/rdo_value.h"
#include "rdo_lib/rdo_parser/rdo_object.h"
#include "rdo_lib/rdo_runtime/rdo_model_interface.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

class RDOTypeRange;

// ----------------------------------------------------------------------------
// ---------- RDOTypeParam
// ----------------------------------------------------------------------------
class RDOTypeParam: public RDOParserSrcInfo, public IModelStructure
{
public:
	virtual void checkValue(CREF(RDOValue) value);
	// добавить кастинг rdoParse::RDOValue -> rdoParse::RDOValue

protected:
	RDOTypeParam(CREF(RDOType) type, CREF(RDOParserSrcInfo) src_info);
	virtual ~RDOTypeParam();
};

// ----------------------------------------------------------------------------
// ---------- RDOTypeParamInt
// ----------------------------------------------------------------------------
class RDOTypeParamInt: RDOTypeParam
{
public:
	RDOTypeParamInt(PTR(RDOTypeRange) range, CREF(RDOParserSrcInfo) src_info);

	CPTR(RDOTypeRange) range() const { return m_range; }

private:
	PTR(RDOTypeRange) m_range;

	DECLARE_IModelStructure;
};

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _RDOPARSER_TYPE_PARAM_H_
