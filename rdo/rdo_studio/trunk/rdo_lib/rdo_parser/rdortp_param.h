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
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDORTPParam
// ----------------------------------------------------------------------------
class RDORTPResType;

class RDORTPParam: public RDOParserObject, public RDOParserSrcInfo
{
public:
	RDORTPParam(CPTR(RDORTPResType) const resType, CPTR(RDOTypeParam) const parType, CREF(RDOParserSrcInfo) src_info);
	virtual ~RDORTPParam();

	CREF(tstring)             name      () const { return src_info().src_text();        }
	CPTR(RDORTPResType) const getResType() const { ASSERT(m_resType); return m_resType; }
	CPTR(RDOTypeParam)  const getType   () const { ASSERT(m_parType); return m_parType; }

	void writeModelStructure(REF(std::ostream) stream) const;

protected:
	CPTR(RDORTPResType) const m_resType;
	CPTR(RDOTypeParam)  const m_parType;
};

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _RDORTP_PARAM_H_
