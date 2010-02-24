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
#include "rdo_lib/rdo_parser/rdo_type_range.h"
#include "rdo_lib/rdo_runtime/rdo_model_interface.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOTypeParam
// ----------------------------------------------------------------------------
class RDOTypeParam: public RDOParserSrcInfo, public IModelStructure
{
public:
	RDOTypeParam(PTR(RDOType) type, CREF(RDOParserSrcInfo) src_info);
	virtual ~RDOTypeParam();

	virtual void     checkValue(CREF(RDOValue) value);
	// добавить кастинг rdoParse::RDOValue -> rdoParse::RDOValue
	virtual RDOValue value_cast(CREF(RDOValue) value) const;

	DECLARE_IModelStructure;

protected:
	PTR(RDOType) m_type;
};

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _RDOPARSER_TYPE_PARAM_H_
