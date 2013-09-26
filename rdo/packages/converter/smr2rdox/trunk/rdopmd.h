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

int  cnv_pmdparse(PTR(void) lexer);
int  cnv_pmdlex  (PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer);
void cnv_pmderror(const char* message);

// --------------------------------------------------------------------------------
// -------------------- RDOPMDResult
// --------------------------------------------------------------------------------
OBJECT(RDOPMDResult) IS INSTANCE_OF(RDOParserSrcInfo)
{
DECLARE_FACTORY(RDOPMDResult);
public:
	CREF(tstring) name() const { return src_text(); }

protected:
	RDOPMDResult(CREF(RDOParserSrcInfo) src_info);
	virtual ~RDOPMDResult();

	void endOfCreation(CREF(LPIResult) pResult);

	LPIResult m_pResult;
};

// --------------------------------------------------------------------------------
// -------------------- RDOPMDWatchPar
// --------------------------------------------------------------------------------
class RDOPMDWatchPar: public RDOPMDResult
{
DECLARE_FACTORY(RDOPMDWatchPar);
private:
	RDOPMDWatchPar(CREF(RDOParserSrcInfo) src_info, rbool trace, CREF(RDOParserSrcInfo) res_src_info, CREF(RDOParserSrcInfo) par_src_info);
};

// --------------------------------------------------------------------------------
// -------------------- RDOPMDWatchState
// --------------------------------------------------------------------------------
class RDOPMDWatchState: public RDOPMDResult
{
DECLARE_FACTORY(RDOPMDWatchState);
private:
	RDOPMDWatchState(CREF(RDOParserSrcInfo) src_info, rbool trace, LPRDOFUNLogic pLogic);
};

// --------------------------------------------------------------------------------
// -------------------- RDOPMDWatchTemp
// --------------------------------------------------------------------------------
class RDOPMDWatchTemp: public RDOPMDResult
{
protected:
	RDOPMDWatchTemp(CREF(RDOParserSrcInfo) src_info, CREF(RDOParserSrcInfo) res_type_src_info);
};

// --------------------------------------------------------------------------------
// -------------------- RDOPMDWatchQuant
// --------------------------------------------------------------------------------
class RDOPMDWatchQuant: public RDOPMDWatchTemp
{
DECLARE_FACTORY(RDOPMDWatchQuant);
public:
	void setLogic       (REF(LPRDOFUNLogic) pLogic);
	void setLogicNoCheck();

private:
	RDOPMDWatchQuant(CREF(RDOParserSrcInfo) src_info, rbool trace, CREF(RDOParserSrcInfo) res_type_src_info);
};
DECLARE_POINTER(RDOPMDWatchQuant);

// --------------------------------------------------------------------------------
// -------------------- RDOPMDWatchValue
// --------------------------------------------------------------------------------
class RDOPMDWatchValue: public RDOPMDWatchTemp
{
DECLARE_FACTORY(RDOPMDWatchValue);
public:
	void setLogic       (REF(LPRDOFUNLogic) pLogic, REF(LPRDOFUNArithm) pArithm);
	void setLogicNoCheck(REF(LPRDOFUNArithm) pArithm);

private:
	RDOPMDWatchValue(CREF(RDOParserSrcInfo) src_info, rbool trace, CREF(RDOParserSrcInfo) res_type_src_info);
};
DECLARE_POINTER(RDOPMDWatchValue);

// --------------------------------------------------------------------------------
// -------------------- RDOPMDGetValue
// --------------------------------------------------------------------------------
class RDOPMDGetValue: public RDOPMDResult
{
DECLARE_FACTORY(RDOPMDGetValue);
private:
	RDOPMDGetValue(CREF(RDOParserSrcInfo) src_info, LPRDOFUNArithm pArithm);
};

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE

#endif // _CONVERTOR_RDOPMD_PMD_H_
