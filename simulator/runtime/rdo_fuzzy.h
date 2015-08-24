#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include <map>
#include <boost/icl/interval.hpp>
#include <boost/icl/interval_set.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_value.h"
#include "simulator/runtime/rdo_type.h"
#include "simulator/runtime/rdo_object.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(FuzzySet);
PREDECLARE_POINTER(DefineArea);

PREDECLARE_POINTER(DefineArea);
class DefineArea: public rdo::counter_reference
{
DECLARE_FACTORY(DefineArea);
public:
    typedef  boost::icl::interval<RDOValue> DomainPart;
    typedef  boost::icl::interval<RDOValue>::type IntervalType;
    typedef  boost::icl::interval_set<RDOValue> Domain;

    DefineArea();
    virtual ~DefineArea();
private:
    DefineArea(const RDOValue&);
    DefineArea(const RDOValue& leftBorder, const RDOValue& rightBorder);

    Domain m_domain;
};

PREDECLARE_POINTER(FuzzySet);
class FuzzySet: public rdo::counter_reference
{
DECLARE_FACTORY(FuzzySet);
public:
    typedef  std::pair<RDOValue, double>                              FuzzyItem;
    typedef  std::map<FuzzyItem::first_type, FuzzyItem::second_type>  FuzzySetDefinition;

    LPFuzzySet                                append     (const RDOValue& rdovalue, double appertain);
    LPFuzzySet                                operator() (const RDOValue& rdovalue, double appertain);
    double&                                   operator[] (const RDOValue& rdovalue);
    FuzzySetDefinition::const_iterator        find       (const RDOValue& rdovalue) const;
    FuzzyItem                                 findValue  (const RDOValue& rdovalue) const;
    FuzzySetDefinition::const_iterator        begin      () const;
    FuzzySetDefinition::const_iterator        end        () const;
    FuzzySetDefinition::iterator              begin      ();
    FuzzySetDefinition::iterator              end        ();
    LPFuzzySet                                clone      () const;
    bool                                      empty      () const;
    bool                                      inRange    (const RDOValue& rdovalue);
    void                                      setValues  (const FuzzySetDefinition& values);

                LPFuzzySet operator&& (const LPFuzzySet& pFuzzyValue) const;
                LPFuzzySet operator|| (const LPFuzzySet& pFuzzyValue) const;
    /* 3.102*/  LPFuzzySet operator+  (const LPFuzzySet& pFuzzyValue) const;
    /* 3.104*/  LPFuzzySet operator-  (const LPFuzzySet& pFuzzyValue) const;
    /* 3.106*/  LPFuzzySet operator*  (const LPFuzzySet& pFuzzyValue) const;
    /* 3.108*/  LPFuzzySet operator/  (const LPFuzzySet& pFuzzyValue) const;


    std::string getAsString() const;
private:
    FuzzySet();
    FuzzySet(const LPDefineArea& pDefineArea);
    FuzzySet(const LPFuzzySet& pSet); // KK

    virtual ~FuzzySet();

    FuzzySetDefinition  m_fuzzySet;
    LPDefineArea        m_defineArea;
};

PREDECLARE_POINTER(RDOValue);
PREDECLARE_POINTER(RDOLingvoVariable);

class MemberFunctionProperties
{
public:
    typedef RDOValue (*ExtUnaryFun )(const RDOValue& rdovalue);
    typedef RDOValue (*ExtUnaryFunP)(const RDOValue& rdovalue, void* pParam);
    typedef RDOValue (*ExtBinaryFun)(const RDOValue& rdovalue1, const RDOValue& rdovalue2);

    /* 3.116*/  static LPFuzzySet u_minus(const LPFuzzySet& pSet);
    /* 3.117*/  static LPFuzzySet u_obr  (const LPFuzzySet& pSet);
    /* 3.118*/  static LPFuzzySet u_scale(const LPFuzzySet& pSet, double scale);
    /* 3.119*/  static LPFuzzySet u_log  (const LPFuzzySet& pSet);

    /* 3.39 */  static LPFuzzySet a_mult    (const LPFuzzySet& pSet1, const LPFuzzySet& pSet2); // не работает пока
    /* 3.48 */  static LPFuzzySet alpha     (const LPFuzzySet& pSet,  double appertain);
    /* 3.62 */  static LPFuzzySet supplement(const LPFuzzySet& pSet);
    /* 3.78 */  static LPFuzzySet a_con     (const LPFuzzySet& pSet);
    /* 3.79 */  static LPFuzzySet a_dil     (const LPFuzzySet& pSet);

                static LPRDOLingvoVariable fuzzyfication  (const RDOValue&, const LPRDOLingvoVariable&);
    /* 3.272*/  static RDOValue            defuzzyfication(const LPFuzzySet& pSet);

                static LPFuzzySet a_pow     (LPFuzzySet pSet, double power);
    /* 3.114*/  static LPFuzzySet ext_unary (ExtUnaryFun  fun, const LPFuzzySet& pSet);
    /* 3.114*/  static LPFuzzySet ext_unary (ExtUnaryFunP fun, void* pParam, const LPFuzzySet& pSet);
    /* 3.83 */  static LPFuzzySet ext_binary(ExtBinaryFun fun, const LPFuzzySet& pSet1, const LPFuzzySet& pSet2);

};
// ! Терм

PREDECLARE_POINTER(RDOFuzzyTerm);
class RDOFuzzyTerm: public rdo::counter_reference
{
DECLARE_FACTORY (RDOFuzzyTerm)
public:
    typedef std::string termName;
    typedef std::pair<termName,LPFuzzySet> Term;

    const LPFuzzySet& getFuzzySetDefinition() const;
    termName          getName              () const;

private:
    RDOFuzzyTerm(const termName&, const LPFuzzySet&);
    virtual ~RDOFuzzyTerm();

    Term m_term;
};

// !Лингвистическая переменная

PREDECLARE_POINTER(RDOLingvoVariable);
class RDOLingvoVariable: public rdo::counter_reference
{
DECLARE_FACTORY(RDOLingvoVariable)
public:
    typedef std::map<RDOFuzzyTerm::Term::first_type, RDOFuzzyTerm::Term::second_type> TermSet;
    typedef std::string nameOfVariable;

    TermSet::const_iterator begin() const;
    TermSet::const_iterator end() const;

    LPFuzzySet& operator[] (std::string name);

    void setName(nameOfVariable);
    nameOfVariable getName() {return m_name;};
    void append(std::string name, const LPFuzzySet& fuzzySet);

private:
    TermSet        m_set;
    nameOfVariable m_name;

    RDOLingvoVariable(const LPRDOFuzzyTerm& term, nameOfVariable nameOfVariable);
    RDOLingvoVariable(const RDOValue& pDefineAreaValue, const LPRDOLingvoVariable& variable);
    RDOLingvoVariable(const RDOLingvoVariable&); // KK
    virtual ~RDOLingvoVariable();
};

PREDECLARE_POINTER(Statement);
class Statement: public rdo::counter_reference
{
DECLARE_FACTORY(Statement)
public:
    void setTerm(LPRDOFuzzyTerm term);
    void setVariable(LPRDOLingvoVariable variable);

private:
    LPRDOLingvoVariable m_variable;
    LPRDOFuzzyTerm      m_term;
    Statement();
    ~Statement();
};

CLOSE_RDO_RUNTIME_NAMESPACE
