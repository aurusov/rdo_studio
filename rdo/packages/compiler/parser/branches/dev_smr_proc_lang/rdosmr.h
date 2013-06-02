/*!
  \copyright (c) RDO-Team, 2011
  \file      rdosmr.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

#ifndef _RDOSMR_H_
#define _RDOSMR_H_

// ----------------------------------------------------------------------- INCLUDES
#include <map>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdotrace.h"
#include "simulator/compiler/parser/rdo_object.h"
#include "simulator/compiler/parser/rdofun.h"
#include "simulator/service/show_mode.h"
#include "simulator/compiler/parser/rdopatpreparse.h"
#include "simulator/runtime/calc/calc_event.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE
class RDOCalc;
CLOSE_RDO_RUNTIME_NAMESPACE

OPEN_RDO_PARSER_NAMESPACE

int  smr_file_parse(PTR(void) lexer);
int  smr_file_lex  (PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer);
void smr_file_error(PTR(char) message);

int  smr_sim_parse (PTR(void) lexer);
int  smr_sim_lex   (PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer);
void smr_sim_error (PTR(char) message);

// --------------------------------------------------------------------------------
// -------------------- RDOSMR
// --------------------------------------------------------------------------------
OBJECT(RDOSMR)
{
DECLARE_FACTORY(RDOSMR);
public:
	typedef std::map<tstring, tstring>         StringTable;
	typedef rdo::service::simulation::ShowMode ShowMode;

#ifdef CORBA_ENABLE
    void              setExternalModelName(CREF(tstring) alias, CREF(tstring) modelID);
    tstring           getExternalModelName(CREF(tstring) alias) const;
    CREF(StringTable) getExternalModelList()                    const;
#endif

	ShowMode getShowMode      () const;
	int      getFrameNumber   () const;
	double   getShowRate      () const;
	double   getRunStartTime  () const;
	double   getTraceStartTime() const;
	double   getTraceEndTime  () const;

	LPRDOFUNLogic     m_pTerminateIf;

	typedef std::vector<rdo::runtime::LPRDOCalcEventPlan> SMREvent;
	SMREvent                    SMREventList;

	void  pushEventPlan(REF(rdo::runtime::LPRDOCalcEventPlan) pEventPlan);
	SMREvent getSMREvent();

	void  setShowMode         (rdo::service::simulation::ShowMode showMode);
	void  setFrameNumber      (int value,    CREF(YYLTYPE) pos);
	void  setShowRate         (double value, CREF(YYLTYPE) pos);
	void  setRunStartTime     (double value, CREF(YYLTYPE) pos);
	void  setTraceStartTime   (double value, CREF(YYLTYPE) pos);
	void  setTraceEndTime     (double value, CREF(YYLTYPE) pos);

	void  setTerminateIf  (REF(LPRDOFUNLogic) pLogic);
	void  setEvent        (REF(LPRDOEvent) pEvent);
	void  setTimeArithm   (REF(LPRDOFUNArithm) pTimeArithm);
	void  setSMRParamList (REF(LPArithmContainer) pParamList);
	void  setConstValue   (CREF(RDOParserSrcInfo) const_info, REF(LPRDOFUNArithm)    pArithm);
	void  setResParValue  (CREF(RDOParserSrcInfo) res_info,   CREF(RDOParserSrcInfo) par_info, REF(LPRDOFUNArithm) pArithm);
	void  setSeed         (CREF(RDOParserSrcInfo) seq_info,   int base);
	void  insertBreakPoint(CREF(RDOParserSrcInfo) src_info,   REF(LPRDOFUNLogic) pLogic);

private:
	RDOSMR();
	~RDOSMR();

	OBJECT(BreakPoint) IS INSTANCE_OF(RDOParserSrcInfo)
	{
	DECLARE_FACTORY(BreakPoint);
	private:
		BreakPoint(CREF(RDOParserSrcInfo) src_info, LPRDOFUNLogic pLogic);
	};
	typedef std::vector<LPBreakPoint> BreakPointList;

	StringTable    m_extModelList;
	ShowMode       m_showMode;
	int            m_frameNumber;
	double         m_showRate;
	double         m_runStartTime;
	double         m_traceStartTime;
	double         m_traceEndTime;
	YYLTYPE        m_traceStartTime_pos;
	YYLTYPE        m_traceEndTime_pos;
	BreakPointList m_breakPointList;
};

CLOSE_RDO_PARSER_NAMESPACE

#endif // _RDOSMR_H_
