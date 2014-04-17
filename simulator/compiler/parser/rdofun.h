/*!
  \copyright (c) RDO-Team, 2011
  \file      rdofun.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date
  \brief
  \indent    4T
*/

#ifndef _RDOFUN_H_
#define _RDOFUN_H_

// ----------------------------------------------------------------------- INCLUDES
#include <boost/noncopyable.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdo_object.h"
#include "simulator/compiler/parser/rdo_value.h"
#include "simulator/compiler/parser/rdortp.h"
#include "simulator/compiler/parser/param.h"
#include "simulator/compiler/parser/expression.h"
#include "simulator/compiler/parser/context/context.h"
#include "simulator/compiler/parser/context/memory.h"
#include "simulator/compiler/parser/context/context_find_i.h"
#include "simulator/compiler/parser/type/function_type.h"
#include "simulator/compiler/parser/src/function/function.h"

#include "simulator/runtime/rdo_object.h"
#include "simulator/runtime/rdo_type.h"
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/calc/operation/calc_logic.h"
#include "simulator/runtime/calc/operation/calc_unary.h"
#include "simulator/runtime/calc/calc_sequence.h"
#include "simulator/runtime/calc/resource/calc_select.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOFUNDoubleToIntByResult
// --------------------------------------------------------------------------------
class RDOFUNDoubleToIntByResult
{
public:
	void roundCalc();
	void push_back(const rdo::runtime::LPRDOCalcDoubleToIntByResult& pCalc)
	{
		m_intOrDouble.push_back(pCalc);
	}
	void insert(const RDOFUNDoubleToIntByResult& first)
	{
		m_intOrDouble.insert(m_intOrDouble.end(), first.m_intOrDouble.begin(), first.m_intOrDouble.end());
	}
	void insert(const RDOFUNDoubleToIntByResult& first, const RDOFUNDoubleToIntByResult& pSecond)
	{
		m_intOrDouble.insert(m_intOrDouble.end(), first.m_intOrDouble.begin(), first.m_intOrDouble.end());
		m_intOrDouble.insert(m_intOrDouble.end(), pSecond.m_intOrDouble.begin(), pSecond.m_intOrDouble.end());
	}

private:
	typedef std::vector<rdo::runtime::LPRDOCalcDoubleToIntByResult> CalcList;

	CalcList  m_intOrDouble;
};

// --------------------------------------------------------------------------------
// -------------------- RDOFUNBase
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RDOFUNBase);
class RDOFUNBase
	: public rdo::counter_reference
	, public RDOParserSrcInfo
{
public:
	const LPExpression& expression() const;

protected:
	RDOFUNBase(const RDOParserSrcInfo& srcInfo);
	RDOFUNBase(const LPExpression& pExpression);

	LPExpression               m_pExpression;
	RDOFUNDoubleToIntByResult  m_intOrDouble;
};

// --------------------------------------------------------------------------------
// -------------------- RDOFUNLogic
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RDOFUNArithm);
PREDECLARE_POINTER(RDOFUNLogic);

class RDOFUNLogic: public RDOFUNBase
{
DECLARE_FACTORY(RDOFUNLogic);
friend class RDOFUNArithm;
public:
	rdo::runtime::LPRDOCalc getCalc(rdo::runtime::RDOType::TypeID id = rdo::runtime::RDOType::t_real);

	LPRDOFUNLogic operator && (const LPRDOFUNLogic& pSecond);
	LPRDOFUNLogic operator || (const LPRDOFUNLogic& pSecond);
	LPRDOFUNLogic operator_not(const RDOSrcInfo::Position& position);

	virtual void setSrcInfo(const RDOParserSrcInfo& srcInfo);
	virtual void setSrcPos(const RDOSrcInfo::Position& position);
	virtual void setSrcText(const std::string& value);
	void setSrcPos(const YYLTYPE& error_pos);
	void setSrcPos(const YYLTYPE& pos_begin, const YYLTYPE& pos_end);

	static LPRDOFUNLogic generateTrue(const RDOParserSrcInfo& srcInfo);

private:
	RDOFUNLogic(const LPRDOFUNArithm& pArithm);
	RDOFUNLogic(const LPExpression& pExpression, bool hideWarning);
	virtual ~RDOFUNLogic();

	LPRDOFUNLogic createLogic(const rdo::runtime::LPRDOCalc& pCalc);

	template <class T>
	LPRDOFUNLogic generateLogic(const LPRDOFUNLogic& pSecond);

	template <class T>
	LPRDOFUNLogic generateLogic(const RDOSrcInfo::Position& position);
};
DECLARE_POINTER(LPRDOFUNLogic);

// --------------------------------------------------------------------------------
// -------------------- RDOFUNArithm
// --------------------------------------------------------------------------------
class RDOFUNArithm: public RDOFUNBase
{
DECLARE_FACTORY(RDOFUNArithm);
public:
	static const std::string CONTEXT_PARAM_SET_ARITHM;

	LPRDOFUNArithm operator +(const LPRDOFUNArithm& pSecond);
	LPRDOFUNArithm operator -(const LPRDOFUNArithm& pSecond);
	LPRDOFUNArithm operator *(const LPRDOFUNArithm& pSecond);
	LPRDOFUNArithm operator /(const LPRDOFUNArithm& pSecond);
	LPRDOFUNArithm setEqual  (const LPRDOFUNArithm& pSecond);
	LPRDOFUNArithm uminus    (const rdo::runtime::RDOSrcInfo::Position& position);

	LPRDOFUNLogic operator ==(const LPRDOFUNArithm& pSecond);
	LPRDOFUNLogic operator !=(const LPRDOFUNArithm& pSecond);
	LPRDOFUNLogic operator < (const LPRDOFUNArithm& pSecond);
	LPRDOFUNLogic operator > (const LPRDOFUNArithm& pSecond);
	LPRDOFUNLogic operator <=(const LPRDOFUNArithm& pSecond);
	LPRDOFUNLogic operator >=(const LPRDOFUNArithm& pSecond);

	rdo::runtime::LPRDOCalc       createCalc(const LPTypeInfo& pForType = NULL);

	const LPTypeInfo&             typeInfo   () const { return m_pExpression->typeInfo();  }
	rdo::runtime::LPRDOCalc       calc       () const { return m_pExpression->calc();      }
	rdo::runtime::RDOValue        const_value() const;

	LPRDOEnumType                 enumType  () const { return typeInfo()->type().object_static_cast<RDOEnumType>(); }
	rdo::runtime::RDOType::TypeID typeID    () const { return typeInfo()->type()->typeID();                         }

	virtual void setSrcInfo(const RDOParserSrcInfo& srcInfo );
	virtual void setSrcPos (const RDOSrcInfo::Position& position);
	virtual void setSrcText(const std::string& value);
	void setSrcInfo(const RDOParserSrcInfo& begin, const std::string& delim, const RDOParserSrcInfo& end);
	void setSrcPos (const YYLTYPE& error_pos);
	void setSrcPos (const YYLTYPE& pos_begin, const YYLTYPE& pos_end);

	void checkParamType(const LPTypeInfo& pType);

	static void wrongVarInitialization(const LPRDOValue& name);

	static LPRDOFUNArithm generateByConst        (const LPRDOValue& pValue);
	static LPRDOFUNArithm generateByIdentificator(const LPRDOValue& pValue);
	static LPRDOFUNArithm generateByIdentificator(const LPRDOValue& pValue1, const LPRDOValue& pValue2);

private:
	RDOFUNArithm(const LPExpression& pExpression);
	virtual ~RDOFUNArithm();

	LPTypeInfo getPreType(const LPRDOFUNArithm& pSecond);

	template <class T>
	rdo::runtime::LPRDOCalc generateCalc(const rdo::runtime::RDOSrcInfo::Position& position, const std::string& error);

	template <class T>
	rdo::runtime::LPRDOCalc generateCalc(const LPRDOFUNArithm& pSecond, const std::string& error);

	template <class T>
	LPRDOFUNArithm generateArithm(const rdo::runtime::RDOSrcInfo::Position& position, const std::string& error);

	template <class T>
	LPRDOFUNArithm generateArithm(const LPRDOFUNArithm& pSecond, const std::string& error);

	template <class T>
	LPRDOFUNLogic generateLogic(const LPRDOFUNArithm& pSecond, const std::string& error);

	void castType (const LPRDOFUNArithm& pSecond, const std::string& error);
	void castValue(const LPRDOFUNArithm& pSecond, const std::string& error);
};
DECLARE_POINTER(LPRDOFUNArithm);

// --------------------------------------------------------------------------------
// -------------------- RDOFUNConstant
// --------------------------------------------------------------------------------
class RDOFUNConstant: public RDOParam
{
DECLARE_FACTORY(RDOFUNConstant);
public:
	int getNumber() const { return m_number; }

private:
	RDOFUNConstant(const RDOParserSrcInfo& srcInfo, const LPTypeInfo& pType, const LPRDOValue& pDefault);
	virtual ~RDOFUNConstant();

	int m_number;
};
DECLARE_POINTER(RDOFUNConstant);

//! Список арифметических выражений
//! \details Используется для передачи параметров при вызове событий и функций
PREDECLARE_POINTER(ArithmContainer);
class ArithmContainer
	: public rdo::counter_reference
	, public RDOParserSrcInfo
{
DECLARE_FACTORY(ArithmContainer);
public:
	typedef std::vector<LPRDOFUNArithm> Container;

	const Container&    getContainer() const { return m_arithmList; }
	void                addItem     (const LPRDOFUNArithm& pArithm);
	LPFunctionParamType getType     () const;

private:
	ArithmContainer();
	virtual ~ArithmContainer();

	Container m_arithmList;
};

// --------------------------------------------------------------------------------
// -------------------- RDOFUNParams
// --------------------------------------------------------------------------------
// Параметры, с которыми функция вызывается
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RDOFUNParams);
class RDOFUNParams
	: public rdo::counter_reference
	, public RDOParserSrcInfo
{
DECLARE_FACTORY(RDOFUNParams);
public:
	typedef std::vector<LPRDOFUNArithm> ParamList;

	RDOParserSrcInfo& getFunseqName() { return m_funseqName; }
	const LPArithmContainer& getParamList() const { return m_pArithmContainer ; }
	rdo::runtime::LPRDOCalc getCalc(std::size_t paramID, const LPTypeInfo& pType);

	LPExpression createCallExpression(const LPExpression& pFunction);
	LPRDOFUNArithm createCall(const std::string& funName);
	LPRDOFUNArithm createSeqCall(const std::string& seqName);

private:
	RDOFUNParams(const LPArithmContainer& pArithmContainer);
	virtual ~RDOFUNParams();

	RDOParserSrcInfo  m_funseqName;
	LPArithmContainer m_pArithmContainer;
};
// --------------------------------------------------------------------------------
// -------------------- Последовательности
// --------------------------------------------------------------------------------
// -------------------- RDOFUNSequence
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RDOFUNSequence);
class RDOFUNSequence
	: public rdo::counter_reference
	, public RDOParserSrcInfo
{
DECLARE_FACTORY(RDOFUNSequence);
public:
	PREDECLARE_POINTER(RDOFUNSequenceHeader);
	class RDOFUNSequenceHeader
		: public rdo::counter_reference
		, public RDOParserSrcInfo
	{
	DECLARE_FACTORY(RDOFUNSequenceHeader);
	public:
		const LPTypeInfo& getTypeInfo() const { return m_pType; }

	private:
		RDOFUNSequenceHeader(const LPTypeInfo& pType, const RDOParserSrcInfo& srcInfo)
			: RDOParserSrcInfo(srcInfo)
			, m_pType         (pType  )
		{}

		LPTypeInfo m_pType;
	};

	const std::string& name() const { return m_pHeader->src_text(); }
	const LPRDOFUNSequenceHeader& getHeader() const { return m_pHeader; }
	rdo::runtime::LPRDOCalcSeqInit& getInitCalc() { return m_pInitCalc; }
	rdo::runtime::LPRDOCalcSeqNext& getNextCalc() { return m_pNextCalc; }

	virtual void           createCalcs   () = 0;
	virtual LPRDOFUNArithm createCallCalc(LPRDOFUNParams& pParamList, const RDOParserSrcInfo& srcInfo) const = 0;

protected:
	RDOFUNSequence(const LPRDOFUNSequenceHeader& pHeader, int seed);
	virtual ~RDOFUNSequence();

	LPRDOFUNSequenceHeader         m_pHeader;
	int                            m_seed;
	rdo::runtime::LPRDOCalcSeqInit m_pInitCalc;
	rdo::runtime::LPRDOCalcSeqNext m_pNextCalc;

	void initResult     ();
	void initCalcSrcInfo();
};

// --------------------------------------------------------------------------------
// -------------------- Датчики случайных чисел
// --------------------------------------------------------------------------------
// -------------------- RDOFUNSequenceUniform
// --------------------------------------------------------------------------------
class RDOFUNSequenceUniform: public RDOFUNSequence
{
DECLARE_FACTORY(RDOFUNSequenceUniform);
private:
	RDOFUNSequenceUniform(const LPRDOFUNSequenceHeader& pHeader, int seed = 123456789);

	virtual void           createCalcs   ();
	virtual LPRDOFUNArithm createCallCalc(LPRDOFUNParams& pParamList, const RDOParserSrcInfo& seqSrcInfo) const;
};

// --------------------------------------------------------------------------------
// -------------------- RDOFUNSequenceExponential
// --------------------------------------------------------------------------------
class RDOFUNSequenceExponential: public RDOFUNSequence
{
DECLARE_FACTORY(RDOFUNSequenceExponential);
private:
	RDOFUNSequenceExponential(const LPRDOFUNSequenceHeader& pHeader, int seed = 123456789);

	virtual void           createCalcs   ();
	virtual LPRDOFUNArithm createCallCalc(LPRDOFUNParams& pParamList, const RDOParserSrcInfo& seqSrcInfo) const;
};

// --------------------------------------------------------------------------------
// -------------------- RDOFUNSequenceNormal
// --------------------------------------------------------------------------------
class RDOFUNSequenceNormal: public RDOFUNSequence
{
DECLARE_FACTORY(RDOFUNSequenceNormal);
private:
	RDOFUNSequenceNormal(const LPRDOFUNSequenceHeader& pHeader, int seed = 123456789);

	virtual void           createCalcs   ();
	virtual LPRDOFUNArithm createCallCalc(LPRDOFUNParams& pParamList, const RDOParserSrcInfo& seqSrcInfo) const;
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceTriangular
// ----------------------------------------------------------------------------
class RDOFUNSequenceTriangular: public RDOFUNSequence
{
DECLARE_FACTORY(RDOFUNSequenceTriangular);
private:
	RDOFUNSequenceTriangular(const LPRDOFUNSequenceHeader& pHeader, int seed = 123456789);

	virtual void           createCalcs   ();
	virtual LPRDOFUNArithm createCallCalc(LPRDOFUNParams& pParamList, const RDOParserSrcInfo& seqSrcInfo) const;
};

// ----------------------------------------------------------------------------
// ---------- Гистограмма
// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceByHist
// ----------------------------------------------------------------------------
class RDOFUNSequenceByHist: public RDOFUNSequence
{
public:
	PREDECLARE_POINTER(RDOFUNSequenceByHistHeader);
	class RDOFUNSequenceByHistHeader
		: public rdo::counter_reference
		, public RDOParserSrcInfo
	{
	DECLARE_FACTORY(RDOFUNSequenceByHistHeader);
	public:
		const RDOFUNSequence::LPRDOFUNSequenceHeader& getHeader() const { return m_pHeader; }
		int                                           getSeed  () const { return m_seed;    }

	private:
		RDOFUNSequenceByHistHeader(const RDOFUNSequence::LPRDOFUNSequenceHeader& pHeader, int seed = 123456789)
			: RDOParserSrcInfo(pHeader->src_info())
			, m_pHeader       (pHeader            )
			, m_seed          (seed               )
		{}

		RDOFUNSequence::LPRDOFUNSequenceHeader m_pHeader;
		int                                    m_seed;
	};

protected:
	RDOFUNSequenceByHist(const LPRDOFUNSequenceByHistHeader& pHeader);

private:
	virtual LPRDOFUNArithm createCallCalc(LPRDOFUNParams& pParamList, const RDOParserSrcInfo& srcInfo) const;
};

// --------------------------------------------------------------------------------
// -------------------- RDOFUNSequenceByHistReal
// --------------------------------------------------------------------------------
class RDOFUNSequenceByHistReal: public RDOFUNSequenceByHist
{
DECLARE_FACTORY(RDOFUNSequenceByHistReal);
public:
	typedef std::vector<rdo::runtime::RDOValue> ValueList;

	void addReal(const LPRDOValue& pFrom, const LPRDOValue& pTo, const LPRDOValue& pFreq);

private:
	RDOFUNSequenceByHistReal(const LPRDOFUNSequenceByHistHeader& pHeader, const LPRDOValue& pFrom, const LPRDOValue& pTo, const LPRDOValue& pFreq);
	virtual void createCalcs();

	ValueList m_from;
	ValueList m_to;
	ValueList m_freq;
};
DECLARE_POINTER(RDOFUNSequenceByHistReal);

// --------------------------------------------------------------------------------
// -------------------- RDOFUNSequenceByHistEnum
// --------------------------------------------------------------------------------
class RDOFUNSequenceByHistEnum: public RDOFUNSequenceByHist
{
DECLARE_FACTORY(RDOFUNSequenceByHistEnum);
public:
	typedef std::vector<rdo::runtime::RDOValue> ValueList;

	void addEnum(const LPRDOValue& pValue, const LPRDOValue& pFreq);

private:
	RDOFUNSequenceByHistEnum(const LPRDOFUNSequenceByHistHeader& pHeader, const LPRDOValue& pValue, const LPRDOValue& pFreq);
	virtual void createCalcs();

	ValueList m_values;
	ValueList m_freq;
};
DECLARE_POINTER(RDOFUNSequenceByHistEnum);

// --------------------------------------------------------------------------------
// -------------------- Перечень значений
// --------------------------------------------------------------------------------
// -------------------- RDOFUNSequenceEnumerative
// --------------------------------------------------------------------------------
class RDOFUNSequenceEnumerative: public RDOFUNSequence
{
DECLARE_FACTORY(RDOFUNSequenceEnumerative);
public:
	void addValue(const LPRDOValue& pValue)
	{
		m_valueList.push_back(m_pHeader->getTypeInfo()->value_cast(pValue));
	}

private:
	RDOFUNSequenceEnumerative(const LPRDOFUNSequenceHeader& pHeader, const LPRDOValue& pValue)
		: RDOFUNSequence(pHeader, 0)
	{
		addValue(pValue);
	}
	virtual ~RDOFUNSequenceEnumerative()
	{}

	typedef std::vector<LPRDOValue> ValueList;
	ValueList m_valueList;

	virtual void           createCalcs   ();
	virtual LPRDOFUNArithm createCallCalc(LPRDOFUNParams& pParamList, const RDOParserSrcInfo& srcInfo) const;
};
DECLARE_POINTER(RDOFUNSequenceEnumerative);

// --------------------------------------------------------------------------------
// -------------------- Функции
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOFUNFunctionListElement
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RDOFUNFunctionListElement);
class RDOFUNFunctionListElement
	: public rdo::counter_reference
	, public RDOParserSrcInfo
{
DECLARE_FACTORY(RDOFUNFunctionListElement)
public:
	virtual rdo::runtime::LPRDOCalcIsEqual createIsEqualCalc(const LPTypeInfo& pRetType, const rdo::runtime::LPRDOCalcFuncParam& pFuncParam, const RDOParserSrcInfo& src_pos) const;
	virtual rdo::runtime::LPRDOCalcConst createResultCalc (const LPTypeInfo& pRetType, const RDOParserSrcInfo& src_pos) const = 0;
	virtual bool isEquivalence() const { return false; }

protected:
	RDOFUNFunctionListElement(const RDOParserSrcInfo& srcInfo);
	virtual ~RDOFUNFunctionListElement();
};

// --------------------------------------------------------------------------------
// -------------------- RDOFUNFunctionListElementIdentif
// --------------------------------------------------------------------------------
class RDOFUNFunctionListElementIdentif: public RDOFUNFunctionListElement
{
DECLARE_FACTORY(RDOFUNFunctionListElementIdentif)
private:
	RDOFUNFunctionListElementIdentif(const RDOParserSrcInfo& srcInfo);

	virtual rdo::runtime::LPRDOCalcConst createResultCalc(const LPTypeInfo& pRetType, const RDOParserSrcInfo& src_pos) const;
};
DECLARE_POINTER(RDOFUNFunctionListElementIdentif);

// --------------------------------------------------------------------------------
// -------------------- RDOFUNFunctionListElementReal
// --------------------------------------------------------------------------------
class RDOFUNFunctionListElementReal: public RDOFUNFunctionListElement
{
DECLARE_FACTORY(RDOFUNFunctionListElementReal)
public:
	double getValue() const { return m_value; }

private:
	RDOFUNFunctionListElementReal(const YYLTYPE& position, double value);
	virtual rdo::runtime::LPRDOCalcConst createResultCalc(const LPTypeInfo& pRetType, const RDOParserSrcInfo& src_pos) const;

	double m_value;
};
DECLARE_POINTER(RDOFUNFunctionListElementReal);

// --------------------------------------------------------------------------------
// -------------------- RDOFUNFunctionListElementInt
// --------------------------------------------------------------------------------
class RDOFUNFunctionListElementInt: public RDOFUNFunctionListElement
{
DECLARE_FACTORY(RDOFUNFunctionListElementInt)
public:
	int getValue() const { return m_value; }

private:
	RDOFUNFunctionListElementInt(const YYLTYPE& position, int value);
	virtual rdo::runtime::LPRDOCalcConst createResultCalc(const LPTypeInfo& pRetType, const RDOParserSrcInfo& src_pos) const;

	int m_value;
};
DECLARE_POINTER(RDOFUNFunctionListElementInt);

// --------------------------------------------------------------------------------
// -------------------- RDOFUNFunctionListElementEq
// --------------------------------------------------------------------------------
class RDOFUNFunctionListElementEq: public RDOFUNFunctionListElement
{
DECLARE_FACTORY(RDOFUNFunctionListElementEq)
private:
	RDOFUNFunctionListElementEq(const YYLTYPE& position);

	virtual rdo::runtime::LPRDOCalcConst createResultCalc(const LPTypeInfo& pRetType, const RDOParserSrcInfo& src_pos) const;
	virtual bool isEquivalence() const { return true; }
};
DECLARE_POINTER(RDOFUNFunctionListElementEq);

// --------------------------------------------------------------------------------
// -------------------- RDOFUNCalculateIf
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RDOFUNCalculateIf);
class RDOFUNCalculateIf
	: public rdo::counter_reference
	, public RDOParserSrcInfo
{
DECLARE_FACTORY(RDOFUNCalculateIf)
public:
	LPRDOFUNLogic& getCondition() { return m_pCondition; }
	LPRDOFUNArithm& getAction() { return m_pAction; }

private:
	RDOFUNCalculateIf(const LPRDOFUNLogic& pCondition, const LPRDOFUNArithm& pAction);
	virtual ~RDOFUNCalculateIf();

	LPRDOFUNLogic  m_pCondition;
	LPRDOFUNArithm m_pAction;
};

// --------------------------------------------------------------------------------
// -------------------- RDOFUNFunction
// --------------------------------------------------------------------------------
class RDOFUNFunction: public Function
{
DECLARE_FACTORY(RDOFUNFunction)
public:
	const std::string& name() const;

	void add(const LPRDOFUNFunctionListElement& pListElement);
	void add(const LPRDOFUNCalculateIf& pCalculateIf);

	void createListCalc();
	void createTableCalc(const YYLTYPE& elements_pos);
	const LPRDOParam& getReturn() const;
	void end();

	rdo::runtime::LPRDOCalc getFunctionCalc() const;
	void setFunctionCalc(const rdo::runtime::LPRDOCalc& pCalc);

private:
	RDOFUNFunction(const RDOParserSrcInfo& srcInfo, const LPRDOParam& pReturn);
	RDOFUNFunction(const std::string& name, const LPRDOParam& pReturn);
	virtual ~RDOFUNFunction();

	typedef  std::vector<LPRDOFUNFunctionListElement>  ElementList;
	typedef  std::vector<LPRDOFUNCalculateIf>          CalculateIfList;

	LPRDOParam               m_pReturn;
	ElementList              m_elementList;     //! for list and table
	CalculateIfList          m_calculateIfList; //! for algorithmic
	rdo::runtime::LPRDOCalc  m_pFunctionCalc;
	LPContextMemory          m_pContextMemory;

	void init();
};
DECLARE_POINTER(RDOFUNFunction);

// --------------------------------------------------------------------------------
// -------------------- Групповые выражения
// --------------------------------------------------------------------------------
// -------------------- RDOFUNGroup
// --------------------------------------------------------------------------------
class RDOFUNGroup
	: public RDOParserSrcInfo
	, public Context
	, public IContextFind
{
DECLARE_FACTORY(RDOFUNGroup);
public:
	const LPRDORTPResType& getResType()const { return m_pResType; }

	void end();

protected:
	RDOFUNGroup(const RDOParserSrcInfo& res_info);
	virtual ~RDOFUNGroup();

	virtual Context::FindResult onFindContext(const std::string& method, const Context::Params& params, const RDOParserSrcInfo& srcInfo) const;

private:
	void init(const RDOParserSrcInfo& res_info);

	LPRDORTPResType m_pResType;
};
DECLARE_POINTER(RDOFUNGroup);

// --------------------------------------------------------------------------------
// -------------------- RDOFUNGroupLogic
// --------------------------------------------------------------------------------
class RDOFUNGroupLogic
	: public RDOFUNGroup
	, public boost::noncopyable
{
DECLARE_FACTORY(RDOFUNGroupLogic)
public:
	enum FunGroupType
	{
		fgt_unknow    = 0,
		fgt_exist     = 1,
		fgt_notexist  = 2,
		fgt_forall    = 3,
		fgt_notforall = 4,
		fgt_empty     = 5,
		fgt_size      = 6,
		fgt_array     = 7
	};

	LPRDOFUNLogic createFunLogic(LPRDOFUNLogic& pCondition);

private:
	RDOFUNGroupLogic(FunGroupType funType, const RDOParserSrcInfo& res_info);

	const int m_funType;
};
DECLARE_POINTER(RDOFUNGroupLogic);

// --------------------------------------------------------------------------------
// -------------------- RDOFUNSelect
// --------------------------------------------------------------------------------
class RDOFUNSelect: public RDOFUNGroup
{
DECLARE_FACTORY(RDOFUNSelect)
public:
	void initSelect(LPRDOFUNLogic pCondition = NULL);

private:
	RDOFUNSelect(const RDOParserSrcInfo& res_info);

	rdo::runtime::LPRDOFunCalcSelect m_pCalcSelect;

	virtual Context::FindResult onFindContext(const std::string& method, const Context::Params& params, const RDOParserSrcInfo& srcInfo) const;
};
DECLARE_POINTER(RDOFUNSelect);

CLOSE_RDO_PARSER_NAMESPACE

#endif // _RDOFUN_H_
