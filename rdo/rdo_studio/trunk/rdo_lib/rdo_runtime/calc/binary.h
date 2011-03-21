/*
 * copyright: (c) RDO-Team, 2011
 * filename : binary.h
 * author   : Александ Барс, Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOCALC_BINARY_H_
#define _RDOCALC_BINARY_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/rdocalc.h"
#include "rdo_lib/rdo_runtime/calc/operation_type.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOCalcBinary
// ----------------------------------------------------------------------------
template <typename ret_type, ret_type (RDOValue::*pOperator)(CREF(RDOValue) rdovalue) const, typename OperatorType::Type CalcType>
class RDOCalcBinary: public RDOCalc
{
friend class rdo::Factory<RDOCalcBinary<ret_type, pOperator, CalcType> >;
public:
	enum { calc_type = CalcType };
	typedef ret_type (RDOValue::*value_operator)(CREF(RDOValue) rdovalue) const;

	static RDOSrcInfo getStaticSrcInfo(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight);

	LPRDOCalc      getLeft        () const;
	LPRDOCalcConst getRightAsConst() const;
	void           setRight       (CREF(LPRDOCalc) pRight);

	static value_operator getOperation();

protected:
	RDOCalcBinary(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight);

	LPRDOCalc  m_pLeft;
	LPRDOCalc  m_pRight;

private:
	REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "rdo_lib/rdo_runtime/calc/binary.inl"

#endif //! _RDOCALC_BINARY_H_
