#ifndef _LIB_PARSER_PROCESS_H_
#define _LIB_PARSER_PROCESS_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/smart_ptr/factory/factory.h"

#include "simulator/compiler/parser/rdo_object.h"
#include "simulator/compiler/parser/rdo_logic.h"
#include "simulator/compiler/parser/rdofun.h"
#include "simulator/compiler/parser/rdopat.h"
#include "simulator/compiler/parser/context/context.h"
#include "simulator/compiler/parser/context/context_find_i.h"

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

// --------------------------------------------------------------------------------
// -------------------- RDOPROCProcess
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RDOPROCOperator);

PREDECLARE_POINTER(RDOPROCProcess);
class RDOPROCProcess
	: public RDOParserSrcInfo
	, public Context
	, public IContextFind
{
DECLARE_FACTORY(RDOPROCProcess);
public:
	typedef  std::list<LPRDOPROCProcess >  ProcessList;
	typedef  std::list<LPRDOPROCOperator>  BlockList;

	static std::string s_name_prefix;
	static std::string s_name_sufix;

	std::string name() const { return m_name; }

	void end();
	bool closed() const { return m_closed; }

	void             setCondition(const LPRDOFUNLogic& pConditon = NULL) { m_pConditon = pConditon; }
	LPRDOFUNLogic    getConditon () const                                { return m_pConditon;      }
	const BlockList& getBlockList() const                                { return m_blockList;      }
	LPRDORTPResType  getTransacType()                                    { return m_transactType;   }

	bool checkTransactType(const std::string& name) const;

	bool setPrior(LPRDOFUNArithm& pPrior);
	void insertBlock(const LPRDOPROCOperator& pBlock);
	void insertChild(LPRDOPROCProcess& pProcess);

	LPILogic getRunTime () const { return m_pRuntime; }

protected:
	bool m_closed;
	LPRDOPROCProcess m_pParentProcess;
	ProcessList m_childProcessList;
	BlockList m_blockList;
	LPILogic m_pRuntime;

private:
	RDOPROCProcess(const RDOParserSrcInfo& info, const std::string& name, LPRDORTPResType transactType);

	LPRDOFUNLogic m_pConditon;
	std::string m_name;
	LPRDORTPResType m_transactType;

	virtual Context::LPFindResult onFindContext(const std::string& method, const Context::Params& params, const RDOParserSrcInfo& srcInfo) const;
};

// --------------------------------------------------------------------------------
// -------------------- RDOPROCOperator
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RDOPROCOperator);
class RDOPROCOperator: public rdo::counter_reference
{
DECLARE_FACTORY(RDOPROCOperator);
public:
	virtual LPIPROCBlock getRuntimeBlock() const = 0;
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
public:
	LPIPROCBlock getRuntimeBlock() const { return m_pRuntime; }
protected:
	LPIPROCBlock m_pRuntime;

private:
	RDOPROCGenerate(const LPRDOPROCProcess& pProcess, const std::string& name, const rdo::runtime::LPRDOCalc& pTimeCalc, const rdo::runtime::LPRDOCalc& pCreateAndGoOnTransactCalc);
};
DECLARE_POINTER(RDOPROCGenerate);

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
	LPIPROCBlock getRuntimeBlock() const { return m_pRuntime; }
	void createRuntime();
	void setResource  (const std::string& name);

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
	LPIPROCBlock getRuntimeBlock() const { return m_pRuntime; }
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
	LPIPROCBlock getRuntimeBlock() const { return m_pRuntime; }
	void createRuntime();
	void addResource(const std::string& name);

protected:
	typedef std::list<std::string> ResourceList;
	typedef std::vector<rdo::runtime::parser_for_Seize> ParserForRuntime;

	ResourceList m_resourceList;
	ParserForRuntime m_parserForRuntime;
	LPIPROCBlock m_pRuntime;

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
	LPIPROCBlock getRuntimeBlock() const { return m_pRuntime; }
	void createRuntime();
	void addResource(const std::string& name);

protected:
	typedef std::list<std::string> ResourceList;
	typedef std::vector<rdo::runtime::parser_for_Seize> ParserForRuntime;

	ResourceList m_resourceList;
	ParserForRuntime m_parserForRuntime;
	LPIPROCBlock m_pRuntime;

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
public:
	LPIPROCBlock getRuntimeBlock() const { return m_pRuntime; }
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

public:
	LPIPROCBlock getRuntimeBlock() const { return m_pRuntime; }
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
public:
	LPIPROCBlock getRuntimeBlock() const { return m_pRuntime; }
protected:
	LPIPROCBlock m_pRuntime;

private:
	RDOPROCAssign(const LPRDOPROCProcess& pProcess, const std::string& name, const rdo::runtime::LPRDOCalc& pCalc);
};

CLOSE_RDO_PARSER_NAMESPACE

#endif // _LIB_PARSER_PROCESS_H_
