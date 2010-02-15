/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdortp_param.h
 * author   : Александ Барс, Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDORTP_PARAM_H_
#define _RDORTP_PARAM_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdo_type_param.h"
#include "rdo_lib/rdo_runtime/rdo_model_interface.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDORTPParam
// ----------------------------------------------------------------------------
class RDORTPResType;

class RDORTPParam: public RDOParserObject, public RDOParserSrcInfo, IModelStructure, IName
{
public:
	RDORTPParam(CPTR(RDORTPResType) const resType, CPTR(RDOTypeParam) const parType, CREF(RDOParserSrcInfo) src_info);
	virtual ~RDORTPParam();

	CPTR(RDORTPResType) const getResType  () const { ASSERT(m_resType); return m_resType; }
	CPTR(RDOTypeParam)  const getParamType() const { ASSERT(m_parType); return m_parType; }

	DECLARE_IModelStructure;
	DECLARE_IName;

protected:
	CPTR(RDORTPResType) const m_resType;
	CPTR(RDOTypeParam)  const m_parType;
};

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _RDORTP_PARAM_H_
