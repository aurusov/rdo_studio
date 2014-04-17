/*!
  \copyright (c) RDO-Team, 2011
  \file      rdopmd.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

#ifndef _CONVERTOR_RDOPMD_PMD_H_
#define _CONVERTOR_RDOPMD_PMD_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/smart_ptr/factory/factory.h"
#include "converter/smr2rdox/rdo_object.h"
#include "converter/smr2rdox/rdofun.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

int cnv_pmdparse(void* lexer);
int cnv_pmdlex(YYSTYPE* lpval, YYLTYPE* llocp, void* lexer);
void cnv_pmderror(const char* message);

// --------------------------------------------------------------------------------
// -------------------- RDOPMDResult
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RDOPMDResult);
class RDOPMDResult
	: public rdo::counter_reference
	, public RDOParserSrcInfo
{
DECLARE_FACTORY(RDOPMDResult);
public:
	const std::string& name() const { return src_text(); }

protected:
	RDOPMDResult(const RDOParserSrcInfo& src_info);
	virtual ~RDOPMDResult();

	void endOfCreation(const LPIResult& pResult);

	LPIResult m_pResult;
};

// --------------------------------------------------------------------------------
// -------------------- RDOPMDWatchPar
// --------------------------------------------------------------------------------
class RDOPMDWatchPar: public RDOPMDResult
{
DECLARE_FACTORY(RDOPMDWatchPar);
private:
	RDOPMDWatchPar(const RDOParserSrcInfo& src_info, bool trace, const RDOParserSrcInfo& res_src_info, const RDOParserSrcInfo& par_src_info);
};

// --------------------------------------------------------------------------------
// -------------------- RDOPMDWatchState
// --------------------------------------------------------------------------------
class RDOPMDWatchState: public RDOPMDResult
{
DECLARE_FACTORY(RDOPMDWatchState);
private:
	RDOPMDWatchState(const RDOParserSrcInfo& src_info, bool trace, LPRDOFUNLogic pLogic);
};

// --------------------------------------------------------------------------------
// -------------------- RDOPMDWatchTemp
// --------------------------------------------------------------------------------
class RDOPMDWatchTemp: public RDOPMDResult
{
protected:
	RDOPMDWatchTemp(const RDOParserSrcInfo& src_info, const RDOParserSrcInfo& res_type_src_info);
};

// --------------------------------------------------------------------------------
// -------------------- RDOPMDWatchQuant
// --------------------------------------------------------------------------------
class RDOPMDWatchQuant: public RDOPMDWatchTemp
{
DECLARE_FACTORY(RDOPMDWatchQuant);
public:
	void setLogic       (LPRDOFUNLogic& pLogic);
	void setLogicNoCheck();

private:
	RDOPMDWatchQuant(const RDOParserSrcInfo& src_info, bool trace, const RDOParserSrcInfo& res_type_src_info);
};
DECLARE_POINTER(RDOPMDWatchQuant);

// --------------------------------------------------------------------------------
// -------------------- RDOPMDWatchValue
// --------------------------------------------------------------------------------
class RDOPMDWatchValue: public RDOPMDWatchTemp
{
DECLARE_FACTORY(RDOPMDWatchValue);
public:
	void setLogic       (LPRDOFUNLogic& pLogic, LPRDOFUNArithm& pArithm);
	void setLogicNoCheck(LPRDOFUNArithm& pArithm);

private:
	RDOPMDWatchValue(const RDOParserSrcInfo& src_info, bool trace, const RDOParserSrcInfo& res_type_src_info);
};
DECLARE_POINTER(RDOPMDWatchValue);

// --------------------------------------------------------------------------------
// -------------------- RDOPMDGetValue
// --------------------------------------------------------------------------------
class RDOPMDGetValue: public RDOPMDResult
{
DECLARE_FACTORY(RDOPMDGetValue);
private:
	RDOPMDGetValue(const RDOParserSrcInfo& src_info, LPRDOFUNArithm pArithm);
};

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE

#endif // _CONVERTOR_RDOPMD_PMD_H_
