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
#include "utils/src/smart_ptr/factory/factory.h"

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

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

int  cnv_dptparse      (void* lexer);
int  cnv_dptlex        (YYSTYPE* lpval, YYLTYPE* llocp, void* lexer);
void cnv_dpterror      (const char* message);

int  cnv_proc_rtp_parse(void* lexer);
int  cnv_proc_rtp_lex  (YYSTYPE* lpval, YYLTYPE* llocp, void* lexer);
void cnv_proc_rtp_error(const char* message);

int  cnv_proc_rss_parse(void* lexer);
int  cnv_proc_rss_lex  (YYSTYPE* lpval, YYLTYPE* llocp, void* lexer);
void cnv_proc_rss_error(const char* message);

int  cnv_proc_opr_parse(void* lexer);
int  cnv_proc_opr_lex  (YYSTYPE* lpval, YYLTYPE* llocp, void* lexer);
void cnv_proc_opr_error(const char* message);

// --------------------------------------------------------------------------------
// -------------------- RDODPTActivity
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RDODPTActivity);
class RDODPTActivity
	: public rdo::counter_reference
	, public RDOParserSrcInfo
{
DECLARE_FACTORY(RDODPTActivity);
public:
	const std::string& name() const { return src_info().src_text(); }
	LPRDOPATPattern pattern() const { return m_pPattern; }

	void addParam(const LPRDOValue& pParam   );
	void endParam(const YYLTYPE&    param_pos);

	bool setPrior(LPRDOFUNArithm& pPrior);

protected:
	RDODPTActivity(const RDOParserSrcInfo& src_info, const RDOParserSrcInfo& pattern_src_info);
	virtual ~RDODPTActivity();

private:
	std::size_t m_currParam;
	LPRDOPATPattern m_pPattern;
	std::vector<std::string> m_paramValuesAsString;

	void planningInsertIntoSMR() const;
};

// --------------------------------------------------------------------------------
// -------------------- RDODPTActivityHotKey
// --------------------------------------------------------------------------------
class RDODPTActivityHotKey: public RDODPTActivity
{
public:
	void addHotKey(const std::string& hotKey, const YYLTYPE& hotkey_pos);
	bool hasHotKey() const;

protected:
	RDODPTActivityHotKey(LPIBaseOperationContainer pDPT, const RDOParserSrcInfo& src_info, const RDOParserSrcInfo& pattern_src_info);

private:
	IKeyboard::AddHotKeyResult addHotKey(const std::string& hotKey);

	typedef std::vector<std::size_t> ScanCodeList;
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
	RDODPTFreeActivity(LPIBaseOperationContainer pDPT, const RDOParserSrcInfo& src_info, const RDOParserSrcInfo& pattern_src_info);
};
DECLARE_POINTER(RDODPTFreeActivity);

// --------------------------------------------------------------------------------
// -------------------- RDODPTFree
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RDODPTFree);
class RDODPTFree
	: public rdo::counter_reference
	, public RDOLogicActivity<rdo::runtime::RDODPTFree, RDODPTFreeActivity>
{
DECLARE_FACTORY(RDODPTFree);
private:
	RDODPTFree(const RDOParserSrcInfo& src_info);
};

// --------------------------------------------------------------------------------
// -------------------- RDODPTSomeActivity
// --------------------------------------------------------------------------------
class RDODPTSomeActivity: public RDODPTActivityHotKey
{
DECLARE_FACTORY(RDODPTSomeActivity);
private:
	RDODPTSomeActivity(LPIBaseOperationContainer pDPT, const RDOParserSrcInfo& src_info, const RDOParserSrcInfo& pattern_src_info);
};

// --------------------------------------------------------------------------------
// -------------------- RDODPTPriorActivity
// --------------------------------------------------------------------------------
class RDODPTPriorActivity: public RDODPTActivityHotKey
{
DECLARE_FACTORY(RDODPTPriorActivity);
private:
	RDODPTPriorActivity(LPIBaseOperationContainer pDPT, const RDOParserSrcInfo& src_info, const RDOParserSrcInfo& pattern_src_info);
};

// --------------------------------------------------------------------------------
// -------------------- RDODPTSome
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RDODPTSome);
class RDODPTSome
	: public rdo::counter_reference
	, public RDOLogicActivity<rdo::runtime::RDODPTSome, RDODPTSomeActivity>
{
DECLARE_FACTORY(RDODPTSome);
public:
	LPILogic      getLogic    () const                                { return m_pRuntimeLogic;  }
	LPRDOFUNLogic getConditon () const                                { return m_pConditon;      }
	void          setCondition(const LPRDOFUNLogic& pConditon = NULL) { m_pConditon = pConditon; }

	void end();

private:
	RDODPTSome(const RDOParserSrcInfo& src_info, LPILogic pParent = NULL);

	LPRDOFUNLogic m_pConditon;
};

// --------------------------------------------------------------------------------
// -------------------- RDODPTPrior
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RDODPTPrior);
class RDODPTPrior
	: public rdo::counter_reference
	, public RDOLogicActivity<rdo::runtime::RDODPTPrior, RDODPTPriorActivity>
{
DECLARE_FACTORY(RDODPTPrior);
public:
	LPILogic      getLogic    () const                                { return m_pRuntimeLogic;  }
	LPRDOFUNLogic getConditon () const                                { return m_pConditon;      }
	void          setCondition(const LPRDOFUNLogic& pConditon = NULL) { m_pConditon = pConditon; }

	void end();

private:
	RDODPTPrior(const RDOParserSrcInfo& src_info, LPILogic pParent = NULL);

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
	                                       const LPRDOFUNArithm&         pRuleCost,
	                                       const YYLTYPE&                param_pos);

	LPRDOFUNArithm getRuleCost() const { return m_pRuleCost; }

private:
	RDODPTSearchActivity(LPIBaseOperationContainer pDPT, const RDOParserSrcInfo& src_info, const RDOParserSrcInfo& pattern_src_info);

	IDPTSearchActivity::ValueTime  m_value;
	LPRDOFUNArithm                 m_pRuleCost;
};
DECLARE_POINTER(RDODPTSearchActivity);

// --------------------------------------------------------------------------------
// -------------------- RDODPTSearch
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RDODPTSearch);
class RDODPTSearch
	: public rdo::counter_reference
	, public RDOLogicActivity<rdo::runtime::RDODPTSearchRuntime, RDODPTSearchActivity>
{
DECLARE_FACTORY(RDODPTSearch);
public:
	void setCondition(const LPRDOFUNLogic& pConditon = NULL) { m_pConditon = pConditon; }
	void setTermCondition(const LPRDOFUNLogic& pTermConditon = NULL) { m_pTermConditon = pTermConditon; }
	void setEvaluateBy(const LPRDOFUNArithm& pEvalBy) { m_pEvalBy = pEvalBy; }
	void setCompareTops(bool compTops ) { m_compTops = compTops; }

	void end();
	bool closed() const { return m_closed; }

private:
	RDODPTSearch(const RDOParserSrcInfo& src_info, rdo::runtime::RDODPTSearchTrace::DPT_TraceFlag trace = rdo::runtime::RDODPTSearchTrace::DPT_no_trace, LPILogic pParent = NULL);

	LPRDOFUNLogic m_pConditon;
	LPRDOFUNLogic m_pTermConditon;
	LPRDOFUNArithm m_pEvalBy;
	LPILogic m_pParent;
	bool m_compTops;
	bool m_closed;
	rdo::runtime::RDODPTSearchTrace::DPT_TraceFlag m_trace;
};

// --------------------------------------------------------------------------------
// -------------------- RDOPROCProcess
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RDOPROCOperator);

PREDECLARE_POINTER(RDOPROCProcess);
class RDOPROCProcess
	: public rdo::counter_reference
	, public RDOParserSrcInfo
{
DECLARE_FACTORY(RDOPROCProcess);
public:
	static std::string s_name_prefix;
	static std::string s_name_sufix;

	void end();
	bool closed() const { return m_closed; }

	void          setCondition(const LPRDOFUNLogic& pConditon = NULL) { m_pConditon = pConditon; }
	LPRDOFUNLogic getConditon () const                                { return m_pConditon;      }

	bool setPrior(LPRDOFUNArithm& pPrior);
	void insertBlock(const LPRDOPROCOperator& pBlock);
	void insertChild(LPRDOPROCProcess& pProcess);

	LPILogic getRunTime () const { return m_pRuntime; }

protected:
	typedef  std::list<LPRDOPROCProcess >  ProcessList;
	typedef  std::list<LPRDOPROCOperator>  BlockList;

	bool m_closed;
	LPRDOPROCProcess m_pParentProcess;
	ProcessList m_childProcessList;
	BlockList m_blockList;
	LPILogic m_pRuntime;

private:
	RDOPROCProcess(const RDOParserSrcInfo& info);

	LPRDOFUNLogic m_pConditon;
};

// --------------------------------------------------------------------------------
// -------------------- RDOPROCOperator
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RDOPROCOperator);
class RDOPROCOperator: public rdo::counter_reference
{
DECLARE_FACTORY(RDOPROCOperator);
protected:
	std::string m_name;
	LPRDOPROCProcess m_pProcess;

	RDOPROCOperator(const LPRDOPROCProcess& pProcess, const std::string& name);
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
	RDOPROCGenerate(const LPRDOPROCProcess& pProcess, const std::string& name, const rdo::runtime::LPRDOCalc& pTimeCalc);
};

// --------------------------------------------------------------------------------
// -------------------- RDOPROCBlockForQueue
// --------------------------------------------------------------------------------
class RDOPROCBlockForQueue: public RDOPROCOperator
{
protected:
	RDOPROCBlockForQueue(const LPRDOPROCProcess& pProcess, const std::string& name);

	//! m_parserForRuntime служит для передачи информации о параметре "Состояние" ресурса
	rdo::runtime::parser_for_Queue m_parserForRuntime;
};

// --------------------------------------------------------------------------------
// -------------------- RDOPROCQueue
// --------------------------------------------------------------------------------
class RDOPROCQueue: public RDOPROCBlockForQueue
{
DECLARE_FACTORY(RDOPROCQueue);
public:
	void createRuntime();
	void setResource(const std::string& name);

protected:
	std::string m_resourceName;
	LPIPROCBlock m_pRuntime;

private:
	RDOPROCQueue(const LPRDOPROCProcess& pProcess, const std::string& name);
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
	void setResource(const std::string& name);

protected:
	std::string m_resourceName;
	LPIPROCBlock m_pRuntime;

private:
	RDOPROCDepart(const LPRDOPROCProcess& pProcess, const std::string& name);
};
DECLARE_POINTER(RDOPROCDepart);

// --------------------------------------------------------------------------------
// -------------------- RDOPROCBlockForSeize
// --------------------------------------------------------------------------------
class RDOPROCBlockForSeize: public RDOPROCOperator
{
protected:
	RDOPROCBlockForSeize(const LPRDOPROCProcess& pProcess, const std::string& name);

	//! m_parserForRuntime служит для передачи информации о параметре "Состояние" ресурса
	rdo::runtime::parser_for_Seize m_parserForRuntime;
};

// --------------------------------------------------------------------------------
// -------------------- RDOPROCSeize
// --------------------------------------------------------------------------------
class RDOPROCSeize: public RDOPROCBlockForSeize
{
DECLARE_FACTORY(RDOPROCSeize);
public:
	void createRuntime();
	void addResource(const std::string& name);

protected:
	typedef std::list<std::string> ResourceList;
	typedef std::vector<rdo::runtime::parser_for_Seize> ParserForRuntime;

	ResourceList     m_resourceList;
	ParserForRuntime m_parserForRuntime;
	LPIPROCBlock     m_pRuntime;

private:
	RDOPROCSeize(const LPRDOPROCProcess& pProcess, const std::string& name);
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
	void addResource(const std::string& name);

protected:
	typedef std::list<std::string> ResourceList;
	typedef std::vector<rdo::runtime::parser_for_Seize> ParserForRuntime;

	ResourceList     m_resourceList;
	ParserForRuntime m_parserForRuntime;
	LPIPROCBlock     m_pRuntime;

private:
	RDOPROCRelease(const LPRDOPROCProcess& pProcess, const std::string& name);
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
	RDOPROCAdvance(const LPRDOPROCProcess& pProcess, const std::string& name, const rdo::runtime::LPRDOCalc& pTimeCalc);
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
	RDOPROCTerminate(const LPRDOPROCProcess& pProcess, const std::string& name, const rdo::runtime::LPRDOCalc& pCalc);
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
	RDOPROCAssign(const LPRDOPROCProcess& pProcess, const std::string& name, const rdo::runtime::LPRDOCalc& pValue);
};

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE

#endif // _CONVERTOR_RDODPT_H_
