/*!
  \copyright (c) RDO-Team, 2011
  \file      rdopmd.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

#ifndef _RDOPMD_PMD_H_
#define _RDOPMD_PMD_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/smart_ptr/factory.h"
#include "simulator/runtime/rdopokaz.h"
#include "simulator/runtime/rdopokaz_group_i.h"
#include "simulator/compiler/parser/rdo_object.h"
#include "simulator/compiler/parser/rdofun.h"
#include "simulator/compiler/parser/context/context.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

int  pmdparse(PTR(void) lexer);
int  pmdlex  (PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer);
void pmderror(PTR(char) mes);

// --------------------------------------------------------------------------------
// -------------------- RDOPMDPokaz
// --------------------------------------------------------------------------------
OBJECT(RDOPMDPokaz) IS INSTANCE_OF(RDOParserSrcInfo)
{
DECLARE_FACTORY(RDOPMDPokaz);
public:
	CREF(tstring)  name      () const { return src_text(); }
	CREF(LPIPokaz) getRuntime() const { return m_pPokaz;   }

protected:
	RDOPMDPokaz(CREF(RDOParserSrcInfo) src_info);
	virtual ~RDOPMDPokaz();

	void endOfCreation(CREF(LPIPokaz) pPokaz);

	LPIPokaz m_pPokaz;
};

// --------------------------------------------------------------------------------
// -------------------- RDOResultGroup
// --------------------------------------------------------------------------------
CLASS(RDOResultGroup):
	    INSTANCE_OF(RDOParserSrcInfo)
	AND INSTANCE_OF(Context         )
{
DECLARE_FACTORY(RDOResultGroup);
public:
	void                init      (CREF(RDOParserSrcInfo) src_info);
	CREF(tstring)       name      () const;
	CREF(LPIPokazGroup) getRuntime() const;
	void                append    (CREF(LPRDOPMDPokaz) pResult   );
	LPRDOPMDPokaz       find      (CREF(tstring)       resultName) const;

private:
	RDOResultGroup();
	virtual ~RDOResultGroup();

	typedef std::list<LPRDOPMDPokaz> ResultList;

	ResultList     m_resultList;
	LPIPokazGroup  m_pPokazGroup;
};
DECLARE_POINTER(RDOResultGroup);

// --------------------------------------------------------------------------------
// -------------------- RDOPMDWatchPar
// --------------------------------------------------------------------------------
class RDOPMDWatchPar: public RDOPMDPokaz
{
DECLARE_FACTORY(RDOPMDWatchPar);
public:
	void init(rbool trace, CREF(RDOParserSrcInfo) res_src_info, CREF(RDOParserSrcInfo) par_src_info);

private:
	RDOPMDWatchPar(CREF(RDOParserSrcInfo) src_info);
	virtual ~RDOPMDWatchPar();
};
DECLARE_POINTER(RDOPMDWatchPar);

// --------------------------------------------------------------------------------
// -------------------- RDOPMDWatchState
// --------------------------------------------------------------------------------
class RDOPMDWatchState: public RDOPMDPokaz
{
DECLARE_FACTORY(RDOPMDWatchState);
public:
	void init(rbool trace, LPRDOFUNLogic pLogic);

private:
	RDOPMDWatchState(CREF(RDOParserSrcInfo) src_info);
	virtual ~RDOPMDWatchState();
};
DECLARE_POINTER(RDOPMDWatchState);

// --------------------------------------------------------------------------------
// -------------------- RDOPMDWatchTemp
// --------------------------------------------------------------------------------
class RDOPMDWatchTemp: public RDOPMDPokaz
{
protected:
	RDOPMDWatchTemp(CREF(RDOParserSrcInfo) src_info, CREF(RDOParserSrcInfo) res_type_src_info);
	virtual ~RDOPMDWatchTemp();
};

// --------------------------------------------------------------------------------
// -------------------- RDOPMDWatchQuant
// --------------------------------------------------------------------------------
class RDOPMDWatchQuant: public RDOPMDWatchTemp
{
DECLARE_FACTORY(RDOPMDWatchQuant);
public:
	void init           (rbool trace, CREF(RDOParserSrcInfo) res_type_src_info);
	void setLogic       (REF(LPRDOFUNLogic) pLogic);
	void setLogicNoCheck();

private:
	RDOPMDWatchQuant(CREF(RDOParserSrcInfo) src_info, CREF(RDOParserSrcInfo) res_type_src_info);
	virtual ~RDOPMDWatchQuant();
};
DECLARE_POINTER(RDOPMDWatchQuant);

// --------------------------------------------------------------------------------
// -------------------- RDOPMDWatchValue
// --------------------------------------------------------------------------------
class RDOPMDWatchValue: public RDOPMDWatchTemp
{
DECLARE_FACTORY(RDOPMDWatchValue);
public:
	void init           (rbool trace, CREF(RDOParserSrcInfo) res_type_src_info);
	void setLogic       (REF(LPRDOFUNLogic) pLogic, REF(LPRDOFUNArithm) pArithm);
	void setLogicNoCheck(REF(LPRDOFUNArithm) pArithm);

private:
	RDOPMDWatchValue(CREF(RDOParserSrcInfo) src_info, CREF(RDOParserSrcInfo) res_type_src_info);
	virtual ~RDOPMDWatchValue();
};
DECLARE_POINTER(RDOPMDWatchValue);

// --------------------------------------------------------------------------------
// -------------------- RDOPMDGetValue
// --------------------------------------------------------------------------------
class RDOPMDGetValue: public RDOPMDPokaz
{
DECLARE_FACTORY(RDOPMDGetValue);
public:
	void init(LPRDOFUNArithm pArithm);

private:
	RDOPMDGetValue(CREF(RDOParserSrcInfo) src_info);
	virtual ~RDOPMDGetValue();
};
DECLARE_POINTER(RDOPMDGetValue);

CLOSE_RDO_PARSER_NAMESPACE

#endif // _RDOPMD_PMD_H_
