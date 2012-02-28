/*!
  \copyright (c) RDO-Team, 2011
  \file      rdodpt.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      11.06.2006
  \brief     Точки принятия решений
  \indent    4T
*/

#ifndef _LIB_PARSER_DPT_H_
#define _LIB_PARSER_DPT_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/smart_ptr/factory.h"

#include "simulator/compiler/parser/rdo_object.h"
#include "simulator/compiler/parser/rdo_logic.h"
#include "simulator/compiler/parser/rdofun.h"
#include "simulator/compiler/parser/rdopat.h"
#include "simulator/compiler/parser/context/context.h"
#include "simulator/compiler/parser/context/context_find_i.h"

#include "simulator/runtime/rdo_activity.h"
#include "simulator/runtime/searchtrace.h"
#include "simulator/runtime/rdo_dptsearch_activity_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE
class RDOCalc;
class RDODPTSome;
class RDODPTPrior;
class RDODPTSearchRuntime;
class RDOPROCProcess;
class RDOPROCGenerate;
class RDOPROCSeize;
class RDOPROCTerminate;
class RDOPROCAdvance;
class RDOPROCRelease;
struct parser_for_Seize;
struct parser_for_Queue;
CLOSE_RDO_RUNTIME_NAMESPACE

OPEN_RDO_PARSER_NAMESPACE

int  dptparse      (PTR(void) lexer);
int  dptlex        (PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer);
void dpterror      (PTR(char) message);

int  proc_rtp_parse(PTR(void) lexer);
int  proc_rtp_lex  (PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer);
void proc_rtp_error(PTR(char) message);

int  proc_rss_parse(PTR(void) lexer);
int  proc_rss_lex  (PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer);
void proc_rss_error(PTR(char) message);

int  proc_opr_parse(PTR(void) lexer);
int  proc_opr_lex  (PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer);
void proc_opr_error(PTR(char) message);
/*
// --------------------------------------------------------------------------------
// -------------------- RDOParsEvent
// --------------------------------------------------------------------------------
CLASS(RDOParsEvent):
	INSTANCE_OF      (RDOParserSrcInfo)
AND INSTANCE_OF      (Context         )
AND IMPLEMENTATION_OF(IContextFind    )
{
	DECLARE_FACTORY(RDOParsEvent);
public:
	CREF(tstring)     name    () const { return src_info().src_text(); }
	CREF(LPIActivity) activity() const { return m_pActivity;           }
	LPRDOPATPattern   pattern () const { return m_pPattern;            }

	void addParam(CREF(RDOValue) param    );
	void endParam(CREF(YYLTYPE)  param_pos);

//	rbool setPrior(REF(LPRDOFUNArithm) pPrior);

protected:
	RDOParsEvent(CREF(RDOParserSrcInfo) src_info, CREF(RDOParserSrcInfo) pattern_src_info);
	virtual ~RDOParsEvent();

	LPIActivity m_pActivity;

private:
	ruint           m_currParam;
	LPRDOPATPattern m_pPattern;

	DECLARE_IContextFind;
};
DECLARE_POINTER(RDOParsEvent);
*/

// --------------------------------------------------------------------------------
// -------------------- RDODPTActivity
// --------------------------------------------------------------------------------
CLASS(RDODPTActivity):
	    INSTANCE_OF      (RDOParserSrcInfo)
	AND INSTANCE_OF      (Context         )
	AND IMPLEMENTATION_OF(IContextFind    )
{
DECLARE_FACTORY(RDODPTActivity);
public:
	CREF(tstring)     name    () const { return src_info().src_text(); }
	CREF(LPIActivity) activity() const { return m_pActivity;           }
	LPRDOPATPattern   pattern () const { return m_pPattern;            }

	void addParam(CREF(LPRDOValue) pParam   );
	void endParam(CREF(YYLTYPE)    param_pos);

	rbool setPrior(REF(LPRDOFUNArithm) pPrior);

protected:
	RDODPTActivity(CREF(RDOParserSrcInfo) src_info, CREF(RDOParserSrcInfo) pattern_src_info);
	virtual ~RDODPTActivity();

	LPIActivity m_pActivity;

private:
	ruint           m_currParam;
	LPRDOPATPattern m_pPattern;

	DECLARE_IContextFind;
};
DECLARE_POINTER(RDODPTActivity);

// --------------------------------------------------------------------------------
// -------------------- RDODPTActivityHotKey
// --------------------------------------------------------------------------------
class RDODPTActivityHotKey: public RDODPTActivity
{
DECLARE_FACTORY(RDODPTActivityHotKey)
public:
	void addHotKey(CREF(tstring) hotKey, CREF(YYLTYPE) hotkey_pos);

protected:
	RDODPTActivityHotKey(LPIBaseOperationContainer pDPT, CREF(RDOParserSrcInfo) src_info, CREF(RDOParserSrcInfo) pattern_src_info);
	virtual ~RDODPTActivityHotKey();
};
DECLARE_POINTER(RDODPTActivityHotKey);


// --------------------------------------------------------------------------------
// -------------------- RDODPTSomeActivity
// --------------------------------------------------------------------------------
class RDODPTSomeActivity: public RDODPTActivityHotKey
{
DECLARE_FACTORY(RDODPTSomeActivity);
private:
	RDODPTSomeActivity(LPIBaseOperationContainer pDPT, CREF(RDOParserSrcInfo) src_info, CREF(RDOParserSrcInfo) pattern_src_info);
	virtual ~RDODPTSomeActivity();
};

// --------------------------------------------------------------------------------
// -------------------- RDODPTPriorActivity
// --------------------------------------------------------------------------------
class RDODPTPriorActivity: public RDODPTActivityHotKey
{
DECLARE_FACTORY(RDODPTPriorActivity);
private:
	RDODPTPriorActivity(LPIBaseOperationContainer pDPT, CREF(RDOParserSrcInfo) src_info, CREF(RDOParserSrcInfo) pattern_src_info);
	virtual ~RDODPTPriorActivity();
};

// --------------------------------------------------------------------------------
// -------------------- RDODPTSome
// --------------------------------------------------------------------------------
CLASS(RDODPTSome): public RDOLogic<rdoRuntime::RDODPTSome, RDODPTSomeActivity>
	AND INSTANCE_OF      (Context     )
	AND IMPLEMENTATION_OF(IContextFind)
{
DECLARE_FACTORY(RDODPTSome);

private:
	RDODPTSome(CREF(RDOParserSrcInfo) src_info, LPILogic pParent = NULL);
	virtual ~RDODPTSome();

	DECLARE_IContextFind;
};
DECLARE_POINTER(RDODPTSome);

// --------------------------------------------------------------------------------
// -------------------- RDODPTPrior
// --------------------------------------------------------------------------------
CLASS(RDODPTPrior): public RDOLogic<rdoRuntime::RDODPTPrior, RDODPTPriorActivity>
	AND INSTANCE_OF      (Context     )
	AND IMPLEMENTATION_OF(IContextFind)
{
DECLARE_FACTORY(RDODPTPrior);

private:
	RDODPTPrior(CREF(RDOParserSrcInfo) src_info, LPILogic pParent = NULL);
	virtual ~RDODPTPrior();

	DECLARE_IContextFind;
};
DECLARE_POINTER(RDODPTPrior);

// --------------------------------------------------------------------------------
// -------------------- RDODPTSearchActivity
// --------------------------------------------------------------------------------
class RDODPTSearchActivity: public RDODPTActivity
{
DECLARE_FACTORY(RDODPTSearchActivity);
public:
	IDPTSearchActivity::ValueTime getValue() const { return m_value; }
	void                          setValue(IDPTSearchActivity::ValueTime value,
	                                       CREF(LPRDOFUNArithm)          pRuleCost);

	LPRDOFUNArithm getRuleCost() const { return m_pRuleCost; }

private:
	RDODPTSearchActivity(LPIBaseOperationContainer pDPT, CREF(RDOParserSrcInfo) src_info, CREF(RDOParserSrcInfo) pattern_src_info);
	virtual ~RDODPTSearchActivity();

	IDPTSearchActivity::ValueTime  m_value;
	LPRDOFUNArithm                 m_pRuleCost;
};
DECLARE_POINTER(RDODPTSearchActivity);

// --------------------------------------------------------------------------------
// -------------------- RDODPTSearch
// --------------------------------------------------------------------------------
CLASS(RDODPTSearch): public RDOLogic<rdoRuntime::RDODPTSearchRuntime, RDODPTSearchActivity>
	AND INSTANCE_OF      (Context     )
	AND IMPLEMENTATION_OF(IContextFind)
{
DECLARE_FACTORY(RDODPTSearch);
public:
	void  setTermCondition(CREF(LPRDOFUNLogic)  pTermConditon = NULL) { m_pTermConditon = pTermConditon; }
	void  setEvaluateBy   (CREF(LPRDOFUNArithm) pEvalBy             ) { m_pEvalBy       = pEvalBy;       }
	void  setCompareTops  (rbool compTops                           ) { m_compTops      = compTops;      }
	void  end             ();
	rbool closed          () const                                    { return m_closed;                 }

private:
	RDODPTSearch(CREF(RDOParserSrcInfo) src_info, rdoRuntime::RDODPTSearchTrace::DPT_TraceFlag trace = rdoRuntime::RDODPTSearchTrace::DPT_no_trace, LPILogic pParent = NULL);
	virtual ~RDODPTSearch();

	LPRDOFUNLogic                                 m_pTermConditon;
	LPRDOFUNArithm                                m_pEvalBy;
	LPILogic                                      m_pParent;
	rbool                                         m_compTops;
	rbool                                         m_closed;
	rdoRuntime::RDODPTSearchTrace::DPT_TraceFlag  m_trace;

	DECLARE_IContextFind;
};
DECLARE_POINTER(RDODPTSearch);

CLOSE_RDO_PARSER_NAMESPACE

#endif // _LIB_PARSER_DPT_H_
