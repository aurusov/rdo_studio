/*
 * copyright: (c) RDO-Team, 2011
 * filename : operation_type.h
 * author   : Óðóñîâ Àíäðåé
 * date     : 13.03.2011
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOCALC_OPERATION_TYPE_H_
#define _RDOCALC_OPERATION_TYPE_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- OperatorType
// ----------------------------------------------------------------------------
class OperatorType
{
public:
	enum Type
	{
		OT_ARITHM,
		OT_LOGIC
	};

	template <Type>
	static REF(ruint) getCalcCounter()
	{
		static ruint s_ñounter = 0;
		return s_ñounter;
	}
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif //! _RDOCALC_OPERATION_TYPE_H_
