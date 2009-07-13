/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdocalc.inl
 * author   : Александ Барс, Урусов Андрей
 * date     : 
 * bref     : 11.07.2009
 * indent   : 4T
 */

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOCalc
// ----------------------------------------------------------------------------
inline RDOCalc::RDOCalc(PTR(RDORuntimeParent) parent)
	: RDORuntimeObject(parent)
{}

inline RDOCalc::~RDOCalc()
{}

// ----------------------------------------------------------------------------
// ---------- Последовательности
// ----------------------------------------------------------------------------
inline RDOValue RDOCalcSeqNextByHist::getNextValue(PTR(RDORuntime) runtime)
{
	return m_gen->next();
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcCreateNumberedResource
// ----------------------------------------------------------------------------
inline RDOCalcCreateNumberedResource::RDOCalcCreateNumberedResource(PTR(RDORuntimeParent) parent, int _type, rbool _traceFlag, CREF(std::vector<RDOValue>) _paramsCalcs, int _number, rbool _isPermanent)
	: RDOCalc    (parent      )
	, type       (_type       )
	, traceFlag  (_traceFlag  )
	, number     (_number     )
	, isPermanent(_isPermanent)
{
	paramsCalcs.insert(paramsCalcs.begin(), _paramsCalcs.begin(), _paramsCalcs.end());
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcCreateProcessResource
// ----------------------------------------------------------------------------
inline RDOCalcCreateProcessResource::RDOCalcCreateProcessResource(PTR(RDORuntimeParent) parent, int _type, rbool _traceFlag, CREF(std::vector<RDOValue>) _paramsCalcs, int _number, rbool _isPermanent)
	: RDOCalcCreateNumberedResource(parent, _type, _traceFlag, _paramsCalcs, _number, _isPermanent)
{}

// ----------------------------------------------------------------------------
// ---------- RDOCalcCreateEmptyResource
// ----------------------------------------------------------------------------
inline RDOCalcCreateEmptyResource::RDOCalcCreateEmptyResource(PTR(RDORuntimeParent) parent, int _type, rbool _traceFlag, CREF(std::vector<RDOValue>) _params_default, int _rel_res_id)
	: RDOCalc   (parent     )
	, type      (_type      )
	, traceFlag (_traceFlag )
	, rel_res_id(_rel_res_id)
{
	params_default.insert(params_default.begin(), _params_default.begin(), _params_default.end());
}

// ----------------------------------------------------------------------------
// ---------- Выбор ресурсов
// ----------------------------------------------------------------------------
inline RDOSelectResourceCalc::RDOSelectResourceCalc(PTR(RDORuntimeParent) parent, int _rel_res_id, PTR(RDOCalc) _choice_calc, PTR(RDOCalc) _order_calc, Type _order_type)
	: RDOCalc    (parent      )
	, rel_res_id (_rel_res_id )
	, choice_calc(_choice_calc)
	, order_calc (_order_calc )
	, order_type (_order_type )
{}

CLOSE_RDO_RUNTIME_NAMESPACE
