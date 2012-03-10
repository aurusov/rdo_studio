/*!
  \copyright (c) RDO-Team, 2012
  \file      rdoprocess.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      21.02.2012
  \brief     Процессы
  \indent    4T
*/

#ifndef _LIB_PARSER_PROCESS_H_
#define _LIB_PARSER_PROCESS_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/smart_ptr/factory.h"

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

// --------------------------------------------------------------------------------
// -------------------- RDOPROCProcess
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RDOPROCOperator);

OBJECT(RDOPROCProcess) IS INSTANCE_OF(RDOParserSrcInfo)
{
DECLARE_FACTORY(RDOPROCProcess);
public:
	typedef  std::list<LPRDOPROCProcess >  ProcessList;
	typedef  std::list<LPRDOPROCOperator>  BlockList;

	static tstring s_name_prefix;
	static tstring s_name_sufix;

	tstring name() const { return m_name; }

	void  end   ();
	rbool closed() const { return m_closed; }

	void            setCondition(CREF(LPRDOFUNLogic) pConditon = NULL) { m_pConditon = pConditon; }
	LPRDOFUNLogic   getConditon () const                               { return m_pConditon;      }
	CREF(BlockList) getBlockList() const                               { return m_blockList;      }
	LPRDORTPResType getTransacType()                                   { return m_transactType;   }

	rbool checkTransactType(CREF(tstring) name) const;

	rbool    setPrior   (REF(LPRDOFUNArithm)     pPrior  );
	void     insertBlock(CREF(LPRDOPROCOperator) pBlock  );
	void     insertChild(REF(LPRDOPROCProcess)   pProcess);

	LPILogic getRunTime () const { return m_pRuntime; }

protected:
	rbool             m_closed;
	LPRDOPROCProcess  m_pParentProcess;
	ProcessList       m_childProcessList;
	BlockList         m_blockList;
	LPILogic          m_pRuntime;

private:
	RDOPROCProcess(CREF(RDOParserSrcInfo) info, CREF(tstring) name, LPRDORTPResType transactType);

	LPRDOFUNLogic   m_pConditon;
	tstring         m_name;
	LPRDORTPResType m_transactType;
};

// --------------------------------------------------------------------------------
// -------------------- RDOPROCOperator
// --------------------------------------------------------------------------------
OBJECT(RDOPROCOperator)
{
DECLARE_FACTORY(RDOPROCOperator);
public:
	virtual LPIPROCBlock getRuntimeBlock() const = 0;
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
public:
	LPIPROCBlock getRuntimeBlock() const { return m_pRuntime; }
protected:
	LPIPROCBlock m_pRuntime;

private:
	RDOPROCGenerate(CREF(LPRDOPROCProcess) pProcess, CREF(tstring) name, CREF(rdoRuntime::LPRDOCalc) pTimeCalc, CREF(rdoRuntime::LPRDOCalc) pCreateAndGoOnTransactCalc);
};
DECLARE_POINTER(RDOPROCGenerate);

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
	LPIPROCBlock getRuntimeBlock() const { return m_pRuntime; }
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
	LPIPROCBlock getRuntimeBlock() const { return m_pRuntime; }
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
	LPIPROCBlock getRuntimeBlock() const { return m_pRuntime; }
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
	LPIPROCBlock getRuntimeBlock() const { return m_pRuntime; }
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
public:
	LPIPROCBlock getRuntimeBlock() const { return m_pRuntime; }
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

public:
	LPIPROCBlock getRuntimeBlock() const { return m_pRuntime; }
protected:
	LPIPROCBlock m_pRuntime;

private:
	RDOPROCTerminate(CREF(LPRDOPROCProcess) pProcess, CREF(tstring) name, CREF(rdoRuntime::LPRDOCalc) pCalc);
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
	RDOPROCAssign(CREF(LPRDOPROCProcess) pProcess, CREF(tstring) name, CREF(rdoRuntime::LPRDOCalc) pCalc);
};

CLOSE_RDO_PARSER_NAMESPACE

#endif // _LIB_PARSER_PROCESS_H_
