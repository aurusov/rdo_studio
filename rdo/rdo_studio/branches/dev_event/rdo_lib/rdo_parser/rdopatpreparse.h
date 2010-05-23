/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdopatpreparse.h
 * author   : Ћущан ƒмитрий
 * date     : 22.05.10
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOPATPREPARSE_H_
#define _RDOPATPREPARSE_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/rdosmart_ptr.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOEvent
// ----------------------------------------------------------------------------
class RDOEvent: public rdo::smart_ptr_counter_reference
{
DECLARE_FACTORY(RDOEvent);
public:
	CREF(tstring) name() const;

private:
	RDOEvent(CREF(tstring) name);
	virtual ~RDOEvent();

	tstring m_name;
};

DECLARE_POINTER(RDOEvent);

CLOSE_RDO_PARSER_NAMESPACE

#include "rdo_lib/rdo_parser/rdopatpreparse.inl"

#endif // _RDOPATPREPARSE_H_
