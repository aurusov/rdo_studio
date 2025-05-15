#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include <boost/noncopyable.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/rdo_object.h"
#include "converter/smr2rdox/rdo_value.h"
#include "converter/smr2rdox/rdortp.h"
#include "simulator/runtime/rdo_object.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE
class RDOCalc;
CLOSE_RDO_RUNTIME_NAMESPACE

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

int cnv_rssparse(void* lexer);
int cnv_rsslex(YYSTYPE* lpval, YYLTYPE* llocp, void* lexer);
void cnv_rsserror(YYLTYPE* llocp, void* lexer, const char* message);

// --------------------------------------------------------------------------------
// -------------------- RDORSSResource
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RDORSSResource);
class RDORSSResource
    : public rdo::counter_reference
    , public RDOParserSrcInfo
    , private boost::noncopyable
{
DECLARE_FACTORY(RDORSSResource);
public:
    class Param
    {
    public:
        explicit Param(const LPRDOValue& pValue)
            : m_pValue(pValue)
        {}

        const LPRDOValue& param() const
        {
            return m_pValue;
        }

    private:
        LPRDOValue m_pValue;
    };
    typedef std::vector<Param> ParamList;
    static const std::size_t UNDEFINED_ID = std::size_t(~0);

    virtual rdo::runtime::LPRDOCalc createCalc() const;

    const std::string& name() const { return src_info().src_text(); }
    LPRDORTPResType getType() const { return m_pResType; }

    int getID() const { return m_id; }

    const ParamList& params() const { return m_paramList; }

    void addParam(const LPRDOValue& pParam);
    bool getTrace() const { return trace; }
    void setTrace(bool value) { trace = value; }
    bool defined () const;

    void writeModelStructure(std::ostream& stream) const;

protected:
    RDORSSResource(Converter* pParser, const RDOParserSrcInfo& src_info, const LPRDORTPResType& pResType, std::size_t id = UNDEFINED_ID);

    LPRDORTPResType m_pResType;
    const int m_id; // in system
    ParamList m_paramList;
    bool trace;

private:
    RDORTPResType::ParamList::const_iterator m_currParam;
};

// --------------------------------------------------------------------------------
// -------------------- RDOPROCResource
// --------------------------------------------------------------------------------
class RDOPROCResource: public RDORSSResource
{
DECLARE_FACTORY(RDOPROCResource);
private:
    RDOPROCResource(Converter* pParser, const RDOParserSrcInfo& src_info, const LPRDORTPResType& pResType, std::size_t id = UNDEFINED_ID);
    virtual rdo::runtime::LPRDOCalc createCalc() const;
};
DECLARE_POINTER(RDOPROCResource);

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE
