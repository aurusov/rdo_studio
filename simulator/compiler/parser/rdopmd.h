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
#include "utils/src/smart_ptr/factory/factory.h"
#include "simulator/runtime/result/result_group_i.h"
#include "simulator/compiler/parser/rdo_object.h"
#include "simulator/compiler/parser/rdofun.h"
#include "simulator/compiler/parser/context/context.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

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
	const LPIResult& getRuntime() const { return m_pResult; }

protected:
	RDOPMDResult(const RDOParserSrcInfo& src_info);
	virtual ~RDOPMDResult();

	void endOfCreation(const LPIResult& pResult);

	LPIResult m_pResult;
};

// --------------------------------------------------------------------------------
// -------------------- RDOResultGroup
// --------------------------------------------------------------------------------
class RDOResultGroup
	: public RDOParserSrcInfo
	, public Context
{
DECLARE_FACTORY(RDOResultGroup);
public:
	void init(const RDOParserSrcInfo& src_info);
	const std::string& name() const;
	const LPIResultGroup& getRuntime() const;
	void append(const LPRDOPMDResult& pResult);
	LPRDOPMDResult find(const std::string& resultName) const;

private:
	RDOResultGroup();
	virtual ~RDOResultGroup();

	typedef std::list<LPRDOPMDResult> ResultList;

	ResultList      m_resultList;
	LPIResultGroup  m_pResultGroup;
};
DECLARE_POINTER(RDOResultGroup);

// --------------------------------------------------------------------------------
// -------------------- RDOPMDWatchPar
// --------------------------------------------------------------------------------
class RDOPMDWatchPar: public RDOPMDResult
{
DECLARE_FACTORY(RDOPMDWatchPar);
public:
	void init(bool trace, const RDOParserSrcInfo& res_src_info, const RDOParserSrcInfo& par_src_info);

private:
	RDOPMDWatchPar(const RDOParserSrcInfo& src_info);
	virtual ~RDOPMDWatchPar();
};
DECLARE_POINTER(RDOPMDWatchPar);

// --------------------------------------------------------------------------------
// -------------------- RDOPMDWatchState
// --------------------------------------------------------------------------------
class RDOPMDWatchState: public RDOPMDResult
{
DECLARE_FACTORY(RDOPMDWatchState);
public:
	void init(bool trace, LPRDOFUNLogic pLogic);

private:
	RDOPMDWatchState(const RDOParserSrcInfo& src_info);
	virtual ~RDOPMDWatchState();
};
DECLARE_POINTER(RDOPMDWatchState);

// --------------------------------------------------------------------------------
// -------------------- RDOPMDWatchTemp
// --------------------------------------------------------------------------------
class RDOPMDWatchTemp: public RDOPMDResult
{
protected:
	RDOPMDWatchTemp(const RDOParserSrcInfo& src_info, const RDOParserSrcInfo& res_type_src_info);
	virtual ~RDOPMDWatchTemp();
};

// --------------------------------------------------------------------------------
// -------------------- RDOPMDWatchQuant
// --------------------------------------------------------------------------------
class RDOPMDWatchQuant: public RDOPMDWatchTemp
{
DECLARE_FACTORY(RDOPMDWatchQuant);
public:
	void init           (bool trace, const RDOParserSrcInfo& res_type_src_info);
	void setLogic       (LPRDOFUNLogic& pLogic);
	void setLogicNoCheck();

private:
	RDOPMDWatchQuant(const RDOParserSrcInfo& src_info, const RDOParserSrcInfo& res_type_src_info);
	virtual ~RDOPMDWatchQuant();

	LPRDOFUNGroupLogic m_pGroupLogic;
};
DECLARE_POINTER(RDOPMDWatchQuant);

// --------------------------------------------------------------------------------
// -------------------- RDOPMDWatchValue
// --------------------------------------------------------------------------------
class RDOPMDWatchValue: public RDOPMDWatchTemp
{
DECLARE_FACTORY(RDOPMDWatchValue);
public:
	void init           (bool trace, const RDOParserSrcInfo& res_type_src_info);
	void setLogic       (LPRDOFUNLogic& pLogic, LPRDOFUNArithm& pArithm);
	void setLogicNoCheck(LPRDOFUNArithm& pArithm);

private:
	RDOPMDWatchValue(const RDOParserSrcInfo& src_info, const RDOParserSrcInfo& res_type_src_info);
	virtual ~RDOPMDWatchValue();

	LPRDOFUNGroupLogic m_pGroupLogic;
};
DECLARE_POINTER(RDOPMDWatchValue);

// --------------------------------------------------------------------------------
// -------------------- RDOPMDGetValue
// --------------------------------------------------------------------------------
class RDOPMDGetValue: public RDOPMDResult
{
DECLARE_FACTORY(RDOPMDGetValue);
public:
	void init(LPRDOFUNArithm pArithm);

private:
	RDOPMDGetValue(const RDOParserSrcInfo& src_info);
	virtual ~RDOPMDGetValue();
};
DECLARE_POINTER(RDOPMDGetValue);

CLOSE_RDO_PARSER_NAMESPACE

#endif // _RDOPMD_PMD_H_
