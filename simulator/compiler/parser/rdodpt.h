#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/smart_ptr/factory/factory.h"

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

/*
// --------------------------------------------------------------------------------
// -------------------- RDOParsEvent
// --------------------------------------------------------------------------------
class RDOParsEvent
    : public RDOParserSrcInfo
    , public Context
    , public IContextFind
{
    DECLARE_FACTORY(RDOParsEvent);
public:
    const std::string& name() const { return src_info().src_text(); }
    const LPIActivity& activity() const { return m_pActivity; }
    LPRDOPATPattern pattern () const { return m_pPattern; }

    void addParam(const RDOValue& param);
    void endParam(const YYLTYPE& param_pos);

//    bool setPrior(LPRDOFUNArithm& pPrior);

protected:
    RDOParsEvent(const RDOParserSrcInfo& src_info, const RDOParserSrcInfo& pattern_src_info);
    virtual ~RDOParsEvent();

    LPIActivity m_pActivity;

private:
    std::size_t m_currParam;
    LPRDOPATPattern m_pPattern;

    DECLARE_IContextFind;
};
DECLARE_POINTER(RDOParsEvent);
*/

// --------------------------------------------------------------------------------
// -------------------- RDODPTActivity
// --------------------------------------------------------------------------------
class RDODPTActivity
    : public RDOParserSrcInfo
    , public Context
    , public IContextFind
{
DECLARE_FACTORY(RDODPTActivity);
public:
    const std::string& name() const { return src_info().src_text(); }
    const LPIActivity& activity() const { return m_pActivity; }
    LPRDOPATPattern pattern () const { return m_pPattern; }

    void addParam(const LPRDOValue& pParam);
    void endParam(const YYLTYPE& param_pos);

    bool setPrior(LPRDOFUNArithm& pPrior);

protected:
    RDODPTActivity(const RDOParserSrcInfo& src_info, const RDOParserSrcInfo& pattern_src_info);
    virtual ~RDODPTActivity();

    LPIActivity m_pActivity;

private:
    std::size_t m_currParam;
    LPRDOPATPattern m_pPattern;

    virtual Context::LPFindResult onFindContext(const std::string& method, const Context::Params& params, const RDOParserSrcInfo& srcInfo) const;
};
DECLARE_POINTER(RDODPTActivity);

// --------------------------------------------------------------------------------
// -------------------- RDODPTActivityHotKey
// --------------------------------------------------------------------------------
class RDODPTActivityHotKey: public RDODPTActivity
{
DECLARE_FACTORY(RDODPTActivityHotKey)
public:
    void addHotKey(const std::string& hotKey, const YYLTYPE& hotkey_pos);

protected:
    RDODPTActivityHotKey(LPIBaseOperationContainer pDPT, const RDOParserSrcInfo& src_info, const RDOParserSrcInfo& pattern_src_info);
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
    RDODPTSomeActivity(LPIBaseOperationContainer pDPT, const RDOParserSrcInfo& src_info, const RDOParserSrcInfo& pattern_src_info);
    virtual ~RDODPTSomeActivity();
};

// --------------------------------------------------------------------------------
// -------------------- RDODPTPriorActivity
// --------------------------------------------------------------------------------
class RDODPTPriorActivity: public RDODPTActivityHotKey
{
DECLARE_FACTORY(RDODPTPriorActivity);
private:
    RDODPTPriorActivity(LPIBaseOperationContainer pDPT, const RDOParserSrcInfo& src_info, const RDOParserSrcInfo& pattern_src_info);
    virtual ~RDODPTPriorActivity();
};

// --------------------------------------------------------------------------------
// -------------------- RDODPTSome
// --------------------------------------------------------------------------------
class RDODPTSome
    : public RDOLogic<rdo::runtime::RDODPTSome, RDODPTSomeActivity>
    , public Context
    , public IContextFind
{
DECLARE_FACTORY(RDODPTSome);

private:
    RDODPTSome(const RDOParserSrcInfo& src_info, LPILogic pParent = NULL);
    virtual ~RDODPTSome();

    virtual Context::LPFindResult onFindContext(const std::string& method, const Context::Params& params, const RDOParserSrcInfo& srcInfo) const;
};
DECLARE_POINTER(RDODPTSome);

// --------------------------------------------------------------------------------
// -------------------- RDODPTPrior
// --------------------------------------------------------------------------------
class RDODPTPrior
    : public RDOLogic<rdo::runtime::RDODPTPrior, RDODPTPriorActivity>
    , public Context
    , public IContextFind
{
DECLARE_FACTORY(RDODPTPrior);

private:
    RDODPTPrior(const RDOParserSrcInfo& src_info, LPILogic pParent = NULL);
    virtual ~RDODPTPrior();

    virtual Context::LPFindResult onFindContext(const std::string& method, const Context::Params& params, const RDOParserSrcInfo& srcInfo) const;
};
DECLARE_POINTER(RDODPTPrior);

// --------------------------------------------------------------------------------
// -------------------- RDODPTSearchActivity
// --------------------------------------------------------------------------------
class RDODPTSearchActivity: public RDODPTActivity
{
DECLARE_FACTORY(RDODPTSearchActivity);
public:
    IDPTSearchActivity::CostTime getValue() const { return m_value; }
    void                          setValue(IDPTSearchActivity::CostTime value,
                                           const LPRDOFUNArithm& pRuleCost);

    LPRDOFUNArithm getRuleCost() const { return m_pRuleCost; }

private:
    RDODPTSearchActivity(LPIBaseOperationContainer pDPT, const RDOParserSrcInfo& src_info, const RDOParserSrcInfo& pattern_src_info);
    virtual ~RDODPTSearchActivity();

    IDPTSearchActivity::CostTime  m_value;
    LPRDOFUNArithm                 m_pRuleCost;
};
DECLARE_POINTER(RDODPTSearchActivity);

// --------------------------------------------------------------------------------
// -------------------- RDODPTSearch
// --------------------------------------------------------------------------------
class RDODPTSearch
    : public RDOLogic<rdo::runtime::RDODPTSearchRuntime, RDODPTSearchActivity>
    , public Context
    , public IContextFind
{
DECLARE_FACTORY(RDODPTSearch);
public:
    void setTermCondition(const LPRDOFUNLogic& pTermConditon = NULL) { m_pTermConditon = pTermConditon; }
    void setEvaluateBy(const LPRDOFUNArithm& pEvalBy) { m_pEvalBy = pEvalBy; }
    void setCompareTops(bool compTops) { m_compTops = compTops; }
    void end();
    bool closed() const { return m_closed; }

private:
    RDODPTSearch(const RDOParserSrcInfo& src_info, rdo::runtime::RDODPTSearchTrace::TraceFlag trace = rdo::runtime::RDODPTSearchTrace::TraceFlag::NO_TRACE, LPILogic pParent = NULL);
    virtual ~RDODPTSearch();

    LPRDOFUNLogic m_pTermConditon;
    LPRDOFUNArithm m_pEvalBy;
    LPILogic m_pParent;
    bool m_compTops;
    bool m_closed;
    rdo::runtime::RDODPTSearchTrace::TraceFlag m_trace;

    virtual Context::LPFindResult onFindContext(const std::string& method, const Context::Params& params, const RDOParserSrcInfo& srcInfo) const;
};
DECLARE_POINTER(RDODPTSearch);

CLOSE_RDO_PARSER_NAMESPACE
