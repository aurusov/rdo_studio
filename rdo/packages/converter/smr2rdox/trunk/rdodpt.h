/*!
  \copyright (c) RDO-Team, 2011
  \file      rdodpt.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

#ifndef _CONVERTOR_RDODPT_H_
#define _CONVERTOR_RDODPT_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/smart_ptr/factory.h"

#include "converter/smr2rdox/rdo_object.h"
#include "converter/smr2rdox/rdo_logic.h"
#include "converter/smr2rdox/rdofun.h"
#include "converter/smr2rdox/rdopat.h"
#include "simulator/runtime/searchtrace.h"
#include "simulator/runtime/rdo_dptsearch_activity_i.h"
#include "simulator/runtime/rdo_keyboard_i.h"
#include "simulator/runtime/process/rdoprocess.h"
#include "simulator/runtime/process/queue_depart.h"
#include "simulator/runtime/process/advance.h"
#include "simulator/runtime/process/seize_release.h"
#include "simulator/runtime/process/assign.h"
#include "simulator/runtime/process/terminate.h"
#include "simulator/runtime/process/generate.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE
class RDOCalc;
class RDODPTFree;
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

OPEN_RDO_CONVERTER_NAMESPACE

int  cnv_dptparse      (PTR(void) lexer);
int  cnv_dptlex        (PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer);
void cnv_dpterror      (PTR(char) message);

int  cnv_proc_rtp_parse(PTR(void) lexer);
int  cnv_proc_rtp_lex  (PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer);
void cnv_proc_rtp_error(PTR(char) message);

int  cnv_proc_rss_parse(PTR(void) lexer);
int  cnv_proc_rss_lex  (PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer);
void cnv_proc_rss_error(PTR(char) message);

int  cnv_proc_opr_parse(PTR(void) lexer);
int  cnv_proc_opr_lex  (PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer);
void cnv_proc_opr_error(PTR(char) message);

// --------------------------------------------------------------------------------
// -------------------- RDODPTActivity
// --------------------------------------------------------------------------------
OBJECT(RDODPTActivity) IS INSTANCE_OF(RDOParserSrcInfo)
{
DECLARE_FACTORY(RDODPTActivity);
public:
	CREF(tstring)   name   () const { return src_info().src_text(); }
	LPRDOPATPattern pattern() const { return m_pPattern;            }

	void addParam(CREF(RDOValue) param    );
	void endParam(CREF(YYLTYPE)  param_pos);

	rbool setPrior(REF(LPRDOFUNArithm) pPrior);

protected:
	RDODPTActivity(CREF(RDOParserSrcInfo) src_info, CREF(RDOParserSrcInfo) pattern_src_info);
	virtual ~RDODPTActivity();

private:
	ruint           m_currParam;
	LPRDOPATPattern m_pPattern;
};

// --------------------------------------------------------------------------------
// -------------------- RDODPTActivityHotKey
// --------------------------------------------------------------------------------
class RDODPTActivityHotKey: public RDODPTActivity
{
public:
	void  addHotKey(CREF(tstring) hotKey, CREF(YYLTYPE) hotkey_pos);
	rbool hasHotKey() const;

protected:
	RDODPTActivityHotKey(LPIBaseOperationContainer pDPT, CREF(RDOParserSrcInfo) src_info, CREF(RDOParserSrcInfo) pattern_src_info);

private:
	IKeyboard::AddHotKeyResult addHotKey(CREF(tstring) hotKey);

	typedef std::vector<ruint> ScanCodeList;
	ScanCodeList m_scanCodeList;
};
DECLARE_POINTER(RDODPTActivityHotKey);

// --------------------------------------------------------------------------------
// -------------------- RDODPTFreeActivity
// --------------------------------------------------------------------------------
class RDODPTFreeActivity: public RDODPTActivityHotKey
{
DECLARE_FACTORY(RDODPTFreeActivity);
private:
	RDODPTFreeActivity(LPIBaseOperationContainer pDPT, CREF(RDOParserSrcInfo) src_info, CREF(RDOParserSrcInfo) pattern_src_info);
};
DECLARE_POINTER(RDODPTFreeActivity);

// --------------------------------------------------------------------------------
// -------------------- RDODPTFree
// --------------------------------------------------------------------------------
OBJECT(RDODPTFree) IS public RDOLogicActivity<rdoRuntime::RDODPTFree, RDODPTFreeActivity>
{
DECLARE_FACTORY(RDODPTFree);
private:
	RDODPTFree(CREF(RDOParserSrcInfo) src_info);
};

// --------------------------------------------------------------------------------
// -------------------- RDODPTSomeActivity
// --------------------------------------------------------------------------------
class RDODPTSomeActivity: public RDODPTActivityHotKey
{
DECLARE_FACTORY(RDODPTSomeActivity);
private:
	RDODPTSomeActivity(LPIBaseOperationContainer pDPT, CREF(RDOParserSrcInfo) src_info, CREF(RDOParserSrcInfo) pattern_src_info);
};

// --------------------------------------------------------------------------------
// -------------------- RDODPTPriorActivity
// --------------------------------------------------------------------------------
class RDODPTPriorActivity: public RDODPTActivityHotKey
{
DECLARE_FACTORY(RDODPTPriorActivity);
private:
	RDODPTPriorActivity(LPIBaseOperationContainer pDPT, CREF(RDOParserSrcInfo) src_info, CREF(RDOParserSrcInfo) pattern_src_info);
};

// --------------------------------------------------------------------------------
// -------------------- RDODPTSome
// --------------------------------------------------------------------------------
OBJECT(RDODPTSome) IS public RDOLogicActivity<rdoRuntime::RDODPTSome, RDODPTSomeActivity>
{
DECLARE_FACTORY(RDODPTSome);
public:
	LPILogic      getLogic    () const                               { return m_pRuntimeLogic;  }
	LPRDOFUNLogic getConditon () const                               { return m_pConditon;      }
	void          setCondition(CREF(LPRDOFUNLogic) pConditon = NULL) { m_pConditon = pConditon; }

	void end();

private:
	RDODPTSome(CREF(RDOParserSrcInfo) src_info, LPILogic pParent = NULL);

	LPRDOFUNLogic m_pConditon;
};

// --------------------------------------------------------------------------------
// -------------------- RDODPTPrior
// --------------------------------------------------------------------------------
OBJECT(RDODPTPrior) IS public RDOLogicActivity<rdoRuntime::RDODPTPrior, RDODPTPriorActivity>
{
DECLARE_FACTORY(RDODPTPrior);
public:
	LPILogic      getLogic    () const                               { return m_pRuntimeLogic;  }
	LPRDOFUNLogic getConditon () const                               { return m_pConditon;      }
	void          setCondition(CREF(LPRDOFUNLogic) pConditon = NULL) { m_pConditon = pConditon; }

	void end();

private:
	RDODPTPrior(CREF(RDOParserSrcInfo) src_info, LPILogic pParent = NULL);

	LPRDOFUNLogic m_pConditon;
};

// --------------------------------------------------------------------------------
// -------------------- RDODPTSearchActivity
// --------------------------------------------------------------------------------
class RDODPTSearchActivity: public RDODPTActivity
{
DECLARE_FACTORY(RDODPTSearchActivity);
public:
	IDPTSearchActivity::ValueTime getValue() const { return m_value; }
	void                          setValue(IDPTSearchActivity::ValueTime value,
	                                       CREF(LPRDOFUNArithm)          pRuleCost,
	                                       CREF(YYLTYPE)                 param_pos);

	LPRDOFUNArithm getRuleCost() const { return m_pRuleCost; }

private:
	RDODPTSearchActivity(LPIBaseOperationContainer pDPT, CREF(RDOParserSrcInfo) src_info, CREF(RDOParserSrcInfo) pattern_src_info);

	IDPTSearchActivity::ValueTime  m_value;
	LPRDOFUNArithm                 m_pRuleCost;
};
DECLARE_POINTER(RDODPTSearchActivity);

// --------------------------------------------------------------------------------
// -------------------- RDODPTSearch
// --------------------------------------------------------------------------------
OBJECT(RDODPTSearch) IS public RDOLogicActivity<rdoRuntime::RDODPTSearchRuntime, RDODPTSearchActivity>
{
DECLARE_FACTORY(RDODPTSearch);
public:
	void setCondition    (CREF(LPRDOFUNLogic)  pConditon     = NULL) { m_pConditon     = pConditon;     }
	void setTermCondition(CREF(LPRDOFUNLogic)  pTermConditon = NULL) { m_pTermConditon = pTermConditon; }
	void setEvaluateBy   (CREF(LPRDOFUNArithm) pEvalBy             ) { m_pEvalBy       = pEvalBy;       }
	void setCompareTops  (rbool compTops                           ) { m_compTops      = compTops;      }

	void  end   ();
	rbool closed() const { return m_closed; }

private:
	RDODPTSearch(CREF(RDOParserSrcInfo) src_info, rdoRuntime::RDODPTSearchTrace::DPT_TraceFlag trace = rdoRuntime::RDODPTSearchTrace::DPT_no_trace, LPILogic pParent = NULL);

	LPRDOFUNLogic                                 m_pConditon;
	LPRDOFUNLogic                                 m_pTermConditon;
	LPRDOFUNArithm                                m_pEvalBy;
	LPILogic                                      m_pParent;
	rbool                                         m_compTops;
	rbool                                         m_closed;
	rdoRuntime::RDODPTSearchTrace::DPT_TraceFlag  m_trace;
};

// --------------------------------------------------------------------------------
// -------------------- RDOPROCProcess
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RDOPROCOperator);

OBJECT(RDOPROCProcess) IS INSTANCE_OF(RDOParserSrcInfo)
{
DECLARE_FACTORY(RDOPROCProcess);
public:
	static tstring s_name_prefix;
	static tstring s_name_sufix;

	void  end   ();
	rbool closed() const { return m_closed; }

	void          setCondition(CREF(LPRDOFUNLogic) pConditon = NULL) { m_pConditon = pConditon; }
	LPRDOFUNLogic getConditon () const                               { return m_pConditon;      }

	rbool    setPrior   (REF(LPRDOFUNArithm)     pPrior  );
	void     insertBlock(CREF(LPRDOPROCOperator) pBlock  );
	void     insertChild(REF(LPRDOPROCProcess)   pProcess);

	LPILogic getRunTime () const { return m_pRuntime; }

protected:
	typedef  std::list<LPRDOPROCProcess >  ProcessList;
	typedef  std::list<LPRDOPROCOperator>  BlockList;

	rbool             m_closed;
	LPRDOPROCProcess  m_pParentProcess;
	ProcessList       m_childProcessList;
	BlockList         m_blockList;
	LPILogic          m_pRuntime;

private:
	RDOPROCProcess(CREF(RDOParserSrcInfo) info);

	LPRDOFUNLogic m_pConditon;
};

// --------------------------------------------------------------------------------
// -------------------- RDOPROCOperator
// --------------------------------------------------------------------------------
OBJECT(RDOPROCOperator)
{
DECLARE_FACTORY(RDOPROCOperator);
protected:
	tstring          m_name;
	LPRDOPROCProcess m_pProcess;

	RDOPROCOperator(CREF(LPRDOPROCProcess) pProcess, CREF(tstring) name);
	virtual ~RDOPROCOperator();
};

// --------------------------------------------------------------------------------
// -------------------- RDOPROCGenerate
// --------------------------------------------------------------------------------
class RDOPROCGenerate: public RDOPROCOperator
{
DECLARE_FACTORY(RDOPROCGenerate);
protected:
	LPIPROCBlock m_pRuntime;

private:
	RDOPROCGenerate(CREF(LPRDOPROCProcess) pProcess, CREF(tstring) name, CREF(rdoRuntime::LPRDOCalc) pTimeCalc);
};

// --------------------------------------------------------------------------------
// -------------------- RDOPROCBlockForQueue
// --------------------------------------------------------------------------------
class RDOPROCBlockForQueue: public RDOPROCOperator
{
protected:
	RDOPROCBlockForQueue(CREF(LPRDOPROCProcess) pProcess, CREF(tstring) name);

	//! m_parserForRuntime служит для передачи информации о параметре "Состояние" ресурса
	rdoRuntime::parser_for_Queue m_parserForRuntime;
};

// --------------------------------------------------------------------------------
// -------------------- RDOPROCQueue
// --------------------------------------------------------------------------------
class RDOPROCQueue: public RDOPROCBlockForQueue
{
DECLARE_FACTORY(RDOPROCQueue);
public:
	void createRuntime();
	void setResource  (CREF(tstring) name);

protected:
	tstring      m_resourceName;
	LPIPROCBlock m_pRuntime;

private:
	RDOPROCQueue(CREF(LPRDOPROCProcess) pProcess, CREF(tstring) name);
};
DECLARE_POINTER(RDOPROCQueue);

// --------------------------------------------------------------------------------
// -------------------- RDOPROCDepart
// --------------------------------------------------------------------------------
class RDOPROCDepart: public RDOPROCBlockForQueue
{
DECLARE_FACTORY(RDOPROCDepart);
public:
	void createRuntime();
	void setResource  (CREF(tstring) name);

protected:
	tstring      m_resourceName;
	LPIPROCBlock m_pRuntime;

private:
	RDOPROCDepart(CREF(LPRDOPROCProcess) pProcess, CREF(tstring) name);
};
DECLARE_POINTER(RDOPROCDepart);

// --------------------------------------------------------------------------------
// -------------------- RDOPROCBlockForSeize
// --------------------------------------------------------------------------------
class RDOPROCBlockForSeize: public RDOPROCOperator
{
protected:
	RDOPROCBlockForSeize(CREF(LPRDOPROCProcess) pProcess, CREF(tstring) name);

	//! m_parserForRuntime служит для передачи информации о параметре "Состояние" ресурса
	rdoRuntime::parser_for_Seize m_parserForRuntime;
};

// --------------------------------------------------------------------------------
// -------------------- RDOPROCSeize
// --------------------------------------------------------------------------------
class RDOPROCSeize: public RDOPROCBlockForSeize
{
DECLARE_FACTORY(RDOPROCSeize);
public:
	void createRuntime();
	void addResource  (CREF(tstring) name);

protected:
	typedef std::list  <tstring>                      ResourceList;
	typedef std::vector<rdoRuntime::parser_for_Seize> ParserForRuntime;

	ResourceList     m_resourceList;
	ParserForRuntime m_parserForRuntime;
	LPIPROCBlock     m_pRuntime;

private:
	RDOPROCSeize(CREF(LPRDOPROCProcess) pProcess, CREF(tstring) name);
};
DECLARE_POINTER(RDOPROCSeize);

// --------------------------------------------------------------------------------
// -------------------- RDOPROCRelease
// --------------------------------------------------------------------------------
class RDOPROCRelease: public RDOPROCBlockForSeize
{
DECLARE_FACTORY(RDOPROCRelease);
public:
	void createRuntime();
	void addResource  (CREF(tstring) name);

protected:
	typedef std::list  <tstring>                      ResourceList;
	typedef std::vector<rdoRuntime::parser_for_Seize> ParserForRuntime;

	ResourceList     m_resourceList;
	ParserForRuntime m_parserForRuntime;
	LPIPROCBlock     m_pRuntime;

private:
	RDOPROCRelease(CREF(LPRDOPROCProcess) pProcess, CREF(tstring) name);
};
DECLARE_POINTER(RDOPROCRelease);

// --------------------------------------------------------------------------------
// -------------------- RDOPROCAdvance
// --------------------------------------------------------------------------------
class RDOPROCAdvance: public RDOPROCOperator
{
DECLARE_FACTORY(RDOPROCAdvance);
protected:
	LPIPROCBlock m_pRuntime;

private:
	RDOPROCAdvance(CREF(LPRDOPROCProcess) pProcess, CREF(tstring) name, CREF(rdoRuntime::LPRDOCalc) pTimeCalc);
};

// --------------------------------------------------------------------------------
// -------------------- RDOPROCTerminate
// --------------------------------------------------------------------------------
class RDOPROCTerminate: public RDOPROCOperator
{
DECLARE_FACTORY(RDOPROCTerminate);

protected:
	LPIPROCBlock m_pRuntime;

private:
	RDOPROCTerminate(CREF(LPRDOPROCProcess) pProcess, CREF(tstring) name, CREF(ruint) term);
};

// --------------------------------------------------------------------------------
// -------------------- RDOPROCAssign
// --------------------------------------------------------------------------------
class RDOPROCAssign: public RDOPROCOperator
{
DECLARE_FACTORY(RDOPROCAssign);
protected:
	LPIPROCBlock m_pRuntime;

private:
	RDOPROCAssign(CREF(LPRDOPROCProcess) pProcess, CREF(tstring) name, CREF(rdoRuntime::LPRDOCalc) pValue, int resID, int paramID);
};

CLOSE_RDO_CONVERTER_NAMESPACE

#endif // _CONVERTOR_RDODPT_H_
