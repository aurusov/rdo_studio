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
#include "rdo_common/rdosmart_ptr.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDORTPParam
// ----------------------------------------------------------------------------
class RDORTPResType;

class RDORTPParam: public RDOParserSrcInfo, IModelStructure, IName
{
DECLARE_FACTORY(RDORTPParam);
public:
	CPTR(RDORTPResType) const getResType  () const { ASSERT(m_resType);   return m_resType;   }
	LPRDOTypeParam            getParamType() const { ASSERT(m_paramType); return m_paramType; }
	CREF(RDOValue)            getDefault  () const { return getParamType()->default();        }

	void checkDefault() const;

	DECLARE_IModelStructure;
	DECLARE_IName;

protected:
	CPTR(RDORTPResType) const m_resType;
	LPRDOTypeParam            m_paramType;

protected:
	RDORTPParam(CPTR(RDORTPResType) const resType, CREF(LPRDOTypeParam) pParamType, CREF(RDOParserSrcInfo) src_info);
	virtual ~RDORTPParam();
};
DECLARE_POINTER(RDORTPParam);

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _RDORTP_PARAM_H_
