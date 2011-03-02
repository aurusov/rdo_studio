/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdopmd.h
 * author   : Александ Барс, Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOPMD_PMD_H_
#define _RDOPMD_PMD_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/smart_ptr/intrusive_ptr.h"
#include "rdo_lib/rdo_runtime/rdopokaz.h"
#include "rdo_lib/rdo_parser/rdo_object.h"
#include "rdo_lib/rdo_parser/rdofun.h"
#include "rdo_lib/rdo_parser/context/context.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

int  pmdparse(PTR(void) lexer);
int  pmdlex  (PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer);
void pmderror(PTR(char) mes);

// ----------------------------------------------------------------------------
// ---------- RDOPMDPokaz
// ----------------------------------------------------------------------------
OBJECT(RDOPMDPokaz) IS INSTANCE_OF(RDOParserSrcInfo)
{
DECLARE_FACTORY(RDOPMDPokaz);
public:
	CREF(tstring) name() const { return src_text(); }

protected:
	RDOPMDPokaz(CREF(RDOParserSrcInfo) src_info);
	virtual ~RDOPMDPokaz();

	void endOfCreation(CREF(LPIPokaz) pPokaz);

	LPIPokaz m_pPokaz;
};

// ----------------------------------------------------------------------------
// ---------- ResultGroup
// ----------------------------------------------------------------------------
CLASS(ResultGroup):
	    INSTANCE_OF(RDOParserSrcInfo)
	AND INSTANCE_OF(Context         )
{
DECLARE_FACTORY(ResultGroup);
public:
	void          append(CREF(LPRDOPMDPokaz) pResult   );
	LPRDOPMDPokaz find  (CREF(tstring)       resultName) const;

private:
	ResultGroup(CREF(RDOParserSrcInfo) src_info);
	virtual ~ResultGroup();

	typedef std::list<LPRDOPMDPokaz> ResultList;

	ResultList m_resultList;
};
DECLARE_POINTER(ResultGroup);

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchPar
// ----------------------------------------------------------------------------
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

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchState
// ----------------------------------------------------------------------------
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

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchTemp
// ----------------------------------------------------------------------------
class RDOPMDWatchTemp: public RDOPMDPokaz
{
protected:
	RDOPMDWatchTemp(CREF(RDOParserSrcInfo) src_info, CREF(RDOParserSrcInfo) res_type_src_info);
	virtual ~RDOPMDWatchTemp();
};

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchQuant
// ----------------------------------------------------------------------------
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

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchValue
// ----------------------------------------------------------------------------
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

// ----------------------------------------------------------------------------
// ---------- RDOPMDGetValue
// ----------------------------------------------------------------------------
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

#endif //! _RDOPMD_PMD_H_
