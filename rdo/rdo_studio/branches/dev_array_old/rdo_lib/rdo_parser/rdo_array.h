/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdo_array.h
 * author   : Чирков Михаил
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOPARSER_ARRAY_H_
#define _RDOPARSER_ARRAY_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdo_type.h"
#include "rdo_lib/rdo_parser/rdo_value.h"
#include "rdo_lib/rdo_runtime/rdo_array.h"
#include "rdo_common/smart_ptr/intrusive_ptr.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOArrayType
// ----------------------------------------------------------------------------
class RDOArrayType: public RDOType, public RDOParserSrcInfo
{
DECLARE_FACTORY(RDOArrayType);
public:
	CREF(LPRDOType)            getItemType        () const;
	rdoRuntime::LPRDOArrayType getRuntimeArrayType() const;

private:
	RDOArrayType         (CREF(LPRDOType) pItemType, CREF(RDOParserSrcInfo) src_info);
	virtual ~RDOArrayType();

	LPRDOType m_pItemType;

	DECLARE_IType;
	DECLARE_IModelStructure;
};
DECLARE_POINTER(RDOArrayType)

// ----------------------------------------------------------------------------
// ---------- RDOArrayValue
// ----------------------------------------------------------------------------
OBJECT(RDOArrayValue) IS INSTANCE_OF(RDOParserSrcInfo)
{
DECLARE_FACTORY(RDOArrayValue);
public:
	typedef std::vector<RDOValue> Container;

	void insertItem(CREF(RDOValue) value);

	CREF(LPRDOArrayType) getArrayType() const;
	 REF(LPRDOArrayType) getArrayType();
	rdoRuntime::RDOValue getRArray   () const;
	tstring              getAsString () const;
	CREF(Container)      getContainer() const;

private:
	RDOArrayValue(CREF(LPRDOArrayType) pArrayType);
	virtual ~RDOArrayValue();

	Container      m_container;
	LPRDOArrayType m_pArrayType;
};

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _RDOPARSER_ARRAY_H_
