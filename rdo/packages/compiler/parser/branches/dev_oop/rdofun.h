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

#include "simulator/runtime/rdo_object.h"
#include "simulator/runtime/rdo_type.h"
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/calc/operation/calc_logic.h"
#include "simulator/runtime/calc/operation/calc_unary.h"
#include "simulator/runtime/calc/calc_sequence.h"
#include "simulator/runtime/calc/resource/calc_select.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

int  funparse(PTR(void) lexer);
int  funlex  (PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer);
void funerror(PTR(char) message);

// --------------------------------------------------------------------------------
// -------------------- RDOFUNDoubleToIntByResult
// --------------------------------------------------------------------------------
class RDOFUNDoubleToIntByResult
{
public:
	void roundCalc();
	void push_back(CREF(rdo::runtime::LPRDOCalcDoubleToIntByResult) pCalc)
	{
		m_intOrDouble.push_back(pCalc);
	}
	void insert(CREF(RDOFUNDoubleToIntByResult) first)
	{
		m_intOrDouble.insert(m_intOrDouble.end(), first.m_intOrDouble.begin(), first.m_intOrDouble.end());
	}
	void insert(CREF(RDOFUNDoubleToIntByResult) first, CREF(RDOFUNDoubleToIntByResult) pSecond)
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
OBJECT(RDOFUNBase) IS INSTANCE_OF(RDOParserSrcInfo)
{
public:
	CREF(LPExpression) expression() const;

protected:
	RDOFUNBase(CREF(RDOParserSrcInfo) srcInfo);
	RDOFUNBase(CREF(LPExpression) pExpression);

	LPExpression               m_pExpression;
	RDOFUNDoubleToIntByResult  m_intOrDouble;
};

// --------------------------------------------------------------------------------
// -------------------- RDOFUNLogic
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RDOFUNArithm);
PREDECLARE_POINTER(RDOFUNLogic);

CLASS(RDOFUNLogic): INSTANCE_OF(RDOFUNBase)
{
DECLARE_FACTORY(RDOFUNLogic);
friend class RDOFUNArithm;
public:
	rdo::runtime::LPRDOCalc getCalc(rdo::runtime::RDOType::TypeID id = rdo::runtime::RDOType::t_real);

	LPRDOFUNLogic operator && (CREF(LPRDOFUNLogic) pSecond);
	LPRDOFUNLogic operator || (CREF(LPRDOFUNLogic) pSecond);
	LPRDOFUNLogic operator_not(CREF(RDOSrcInfo::Position) position);

	virtual void setSrcInfo(CREF(RDOParserSrcInfo)     srcInfo );
	virtual void setSrcPos (CREF(RDOSrcInfo::Position) position);
	virtual void setSrcText(CREF(tstring)              value   );
	        void setSrcPos (CREF(YYLTYPE) error_pos);
	        void setSrcPos (CREF(YYLTYPE) pos_begin, CREF(YYLTYPE) pos_end);

	static LPRDOFUNLogic generateTrue(CREF(RDOParserSrcInfo) srcInfo);

private:
	RDOFUNLogic(CREF(LPRDOFUNArithm) pArithm);
	RDOFUNLogic(CREF(LPExpression) pExpression, rbool hideWarning);
	virtual ~RDOFUNLogic();

	LPRDOFUNLogic createLogic(CREF(rdo::runtime::LPRDOCalc) pCalc);

	template <class T>
	LPRDOFUNLogic generateLogic(CREF(LPRDOFUNLogic) pSecond);

	template <class T>
	LPRDOFUNLogic generateLogic(CREF(RDOSrcInfo::Position) position);
};
DECLARE_POINTER(LPRDOFUNLogic);

// --------------------------------------------------------------------------------
// -------------------- RDOFUNArithm
// --------------------------------------------------------------------------------
CLASS(RDOFUNArithm): INSTANCE_OF(RDOFUNBase)
{
DECLARE_FACTORY(RDOFUNArithm);
public:
	LPRDOFUNArithm operator +(CREF(LPRDOFUNArithm) pSecond);
	LPRDOFUNArithm operator -(CREF(LPRDOFUNArithm) pSecond);
	LPRDOFUNArithm operator *(CREF(LPRDOFUNArithm) pSecond);
	LPRDOFUNArithm operator /(CREF(LPRDOFUNArithm) pSecond);
	LPRDOFUNArithm setEqual  (CREF(LPRDOFUNArithm) pSecond);
	LPRDOFUNArithm uminus    (CREF(rdo::runtime::RDOSrcInfo::Position) position);

	LPRDOFUNLogic operator ==(CREF(LPRDOFUNArithm) pSecond);
	LPRDOFUNLogic operator !=(CREF(LPRDOFUNArithm) pSecond);
	LPRDOFUNLogic operator < (CREF(LPRDOFUNArithm) pSecond);
	LPRDOFUNLogic operator > (CREF(LPRDOFUNArithm) pSecond);
	LPRDOFUNLogic operator <=(CREF(LPRDOFUNArithm) pSecond);
	LPRDOFUNLogic operator >=(CREF(LPRDOFUNArithm) pSecond);

	rdo::runtime::LPRDOCalc       createCalc(CREF(LPTypeInfo) pForType = NULL);

	CREF(LPTypeInfo)              typeInfo   () const { return m_pExpression->typeInfo();  }
	rdo::runtime::LPRDOCalc       calc       () const { return m_pExpression->calc();      }
	rdo::runtime::RDOValue        const_value() const;

	LPRDOEnumType                 enumType  () const { return typeInfo()->type().object_static_cast<RDOEnumType>(); }
	rdo::runtime::RDOType::TypeID typeID    () const { return typeInfo()->type()->type()->typeID();                 }

	virtual void setSrcInfo(CREF(RDOParserSrcInfo)     srcInfo );
	virtual void setSrcPos (CREF(RDOSrcInfo::Position) position);
	virtual void setSrcText(CREF(tstring) value);
	        void setSrcInfo(CREF(RDOParserSrcInfo) begin, CREF(tstring) delim, CREF(RDOParserSrcInfo) end);
	        void setSrcPos (CREF(YYLTYPE) error_pos);
	        void setSrcPos (CREF(YYLTYPE) pos_begin, CREF(YYLTYPE) pos_end);

	void checkParamType(CREF(LPTypeInfo) pType);
	
	static void wrongVarInit(CREF(LPRDOValue) pParamName ,CREF(tstring) paramName);

	static LPRDOFUNArithm generateByConst        (CREF(LPRDOValue) pValue);
	static LPRDOFUNArithm generateByIdentificator(CREF(LPRDOValue) pValue);
	static LPRDOFUNArithm generateByIdentificator(CREF(LPRDOValue) pValue1, CREF(LPRDOValue) pValue2);

private:
	RDOFUNArithm(CREF(LPExpression) pExpression);
	virtual ~RDOFUNArithm();

	LPTypeInfo getPreType(CREF(LPRDOFUNArithm) pSecond);

	template <class T>
	rdo::runtime::LPRDOCalc generateCalc(CREF(rdo::runtime::RDOSrcInfo::Position) position, CREF(tstring) error);

	template <class T>
	rdo::runtime::LPRDOCalc generateCalc(CREF(LPRDOFUNArithm) pSecond, CREF(tstring) error);

	template <class T>
	LPRDOFUNArithm generateArithm(CREF(rdo::runtime::RDOSrcInfo::Position) position, CREF(tstring) error);

	template <class T>
	LPRDOFUNArithm generateArithm(CREF(LPRDOFUNArithm) pSecond, CREF(tstring) error);

	template <class T>
	LPRDOFUNLogic generateLogic(CREF(LPRDOFUNArithm) pSecond, CREF(tstring) error);

	void castType (CREF(LPRDOFUNArithm) pSecond, CREF(tstring) error);
	void castValue(CREF(LPRDOFUNArithm) pSecond, CREF(tstring) error);
};
DECLARE_POINTER(LPRDOFUNArithm);

// --------------------------------------------------------------------------------
// -------------------- RDOFUNConstant
// --------------------------------------------------------------------------------
CLASS(RDOFUNConstant): INSTANCE_OF(RDOParam)
{
DECLARE_FACTORY(RDOFUNConstant);
public:
	int getNumber() const { return m_number; }

private:
	RDOFUNConstant(CREF(RDOParserSrcInfo) srcInfo, CREF(LPTypeInfo) pType, CREF(LPRDOValue) pDefault);
	virtual ~RDOFUNConstant();

	int m_number;
};
DECLARE_POINTER(RDOFUNConstant);

//! Список арифметических выражений
//! \details Используется для передачи параметров при вызове событий и функций
OBJECT(ArithmContainer) IS INSTANCE_OF(RDOParserSrcInfo)
{
DECLARE_FACTORY(ArithmContainer);
public:
	typedef std::vector<LPRDOFUNArithm> Container;

	CREF(Container) getContainer() const { return m_arithmList; }
	void            addItem     (CREF(LPRDOFUNArithm) pArithm);

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
OBJECT(RDOFUNParams) IS INSTANCE_OF(RDOParserSrcInfo)
{
DECLARE_FACTORY(RDOFUNParams);
public:
	typedef std::vector<LPRDOFUNArithm> ParamList;

	REF(RDOParserSrcInfo)    getFunseqName()       { return m_funseqName;        }
	CREF(LPArithmContainer)  getParamList () const { return m_pArithmContainer ; }
	rdo::runtime::LPRDOCalc  getCalc      (ruint paramID, CREF(LPTypeInfo) pType);

	LPRDOFUNArithm createCall   (CREF(tstring) funName);
	LPExpression   createCallExpression   (CREF(tstring) funName);
	LPRDOFUNArithm createSeqCall(CREF(tstring) seqName);

private:
	RDOFUNParams(CREF(LPArithmContainer) pArithmContainer);
	virtual ~RDOFUNParams();

	RDOParserSrcInfo  m_funseqName;
	LPArithmContainer m_pArithmContainer;
};
// --------------------------------------------------------------------------------
// -------------------- Последовательности
// --------------------------------------------------------------------------------
// -------------------- RDOFUNSequence
// --------------------------------------------------------------------------------
OBJECT(RDOFUNSequence) IS INSTANCE_OF(RDOParserSrcInfo)
{
DECLARE_FACTORY(RDOFUNSequence);
public:
	OBJECT(RDOFUNSequenceHeader) IS INSTANCE_OF(RDOParserSrcInfo)
	{
	DECLARE_FACTORY(RDOFUNSequenceHeader);
	public:
		CREF(LPTypeInfo) getTypeInfo() const { return m_pType; }

	private:
		RDOFUNSequenceHeader(CREF(LPTypeInfo) pType, CREF(RDOParserSrcInfo) srcInfo)
			: RDOParserSrcInfo(srcInfo)
			, m_pType         (pType  )
		{}

		LPTypeInfo m_pType;
	};

	CREF(tstring)                        name       () const { return m_pHeader->src_text(); }
	CREF(LPRDOFUNSequenceHeader)         getHeader  () const { return m_pHeader;             }
	 REF(rdo::runtime::LPRDOCalcSeqInit) getInitCalc()       { return m_pInitCalc;           }
	 REF(rdo::runtime::LPRDOCalcSeqNext) getNextCalc()       { return m_pNextCalc;           }

	virtual void           createCalcs   () = 0;
	virtual LPRDOFUNArithm createCallCalc(REF(LPRDOFUNParams) pParamList, CREF(RDOParserSrcInfo) srcInfo) const = 0;

protected:
	RDOFUNSequence(CREF(LPRDOFUNSequenceHeader) pHeader, int seed);
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
	RDOFUNSequenceUniform(CREF(LPRDOFUNSequenceHeader) pHeader, int seed = 123456789);

	virtual void           createCalcs   ();
	virtual LPRDOFUNArithm createCallCalc(REF(LPRDOFUNParams) pParamList, CREF(RDOParserSrcInfo) seqSrcInfo) const;
};

// --------------------------------------------------------------------------------
// -------------------- RDOFUNSequenceExponential
// --------------------------------------------------------------------------------
class RDOFUNSequenceExponential: public RDOFUNSequence
{
DECLARE_FACTORY(RDOFUNSequenceExponential);
private:
	RDOFUNSequenceExponential(CREF(LPRDOFUNSequenceHeader) pHeader, int seed = 123456789);

	virtual void           createCalcs   ();
	virtual LPRDOFUNArithm createCallCalc(REF(LPRDOFUNParams) pParamList, CREF(RDOParserSrcInfo) seqSrcInfo) const;
};

// --------------------------------------------------------------------------------
// -------------------- RDOFUNSequenceNormal
// --------------------------------------------------------------------------------
class RDOFUNSequenceNormal: public RDOFUNSequence
{
DECLARE_FACTORY(RDOFUNSequenceNormal);
private:
	RDOFUNSequenceNormal(CREF(LPRDOFUNSequenceHeader) pHeader, int seed = 123456789);

	virtual void           createCalcs   ();
	virtual LPRDOFUNArithm createCallCalc(REF(LPRDOFUNParams) pParamList, CREF(RDOParserSrcInfo) seqSrcInfo) const;
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceTriangular
// ----------------------------------------------------------------------------
class RDOFUNSequenceTriangular: public RDOFUNSequence
{
DECLARE_FACTORY(RDOFUNSequenceTriangular);
private:
	RDOFUNSequenceTriangular(CREF(LPRDOFUNSequenceHeader) pHeader, int seed = 123456789);

	virtual void           createCalcs   ();
	virtual LPRDOFUNArithm createCallCalc(REF(LPRDOFUNParams) pParamList, CREF(RDOParserSrcInfo) seqSrcInfo) const;
};

// ----------------------------------------------------------------------------
// ---------- Гистограмма
// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceByHist
// ----------------------------------------------------------------------------
class RDOFUNSequenceByHist: public RDOFUNSequence
{
public:
	OBJECT(RDOFUNSequenceByHistHeader) IS INSTANCE_OF(RDOParserSrcInfo)
	{
	DECLARE_FACTORY(RDOFUNSequenceByHistHeader);
	public:
		CREF(RDOFUNSequence::LPRDOFUNSequenceHeader) getHeader() const { return m_pHeader; }
		int                                          getSeed  () const { return m_seed;    }

	private:
		RDOFUNSequenceByHistHeader(CREF(RDOFUNSequence::LPRDOFUNSequenceHeader) pHeader, int seed = 123456789)
			: RDOParserSrcInfo(pHeader->src_info())
			, m_pHeader       (pHeader            )
			, m_seed          (seed               )
		{}

		RDOFUNSequence::LPRDOFUNSequenceHeader m_pHeader;
		int                                    m_seed;
	};

protected:
	RDOFUNSequenceByHist(CREF(LPRDOFUNSequenceByHistHeader) pHeader);

private:
	virtual LPRDOFUNArithm createCallCalc(REF(LPRDOFUNParams) pParamList, CREF(RDOParserSrcInfo) srcInfo) const;
};

// --------------------------------------------------------------------------------
// -------------------- RDOFUNSequenceByHistReal
// --------------------------------------------------------------------------------
class RDOFUNSequenceByHistReal: public RDOFUNSequenceByHist
{
DECLARE_FACTORY(RDOFUNSequenceByHistReal);
public:
	typedef std::vector<rdo::runtime::RDOValue> ValueList;

	void addReal(CREF(LPRDOValue) pFrom, CREF(LPRDOValue) pTo, CREF(LPRDOValue) pFreq);

private:
	RDOFUNSequenceByHistReal(CREF(LPRDOFUNSequenceByHistHeader) pHeader, CREF(LPRDOValue) pFrom, CREF(LPRDOValue) pTo, CREF(LPRDOValue) pFreq);
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

	void addEnum(CREF(LPRDOValue) pValue, CREF(LPRDOValue) pFreq);

private:
	RDOFUNSequenceByHistEnum(CREF(LPRDOFUNSequenceByHistHeader) pHeader, CREF(LPRDOValue) pValue, CREF(LPRDOValue) pFreq);
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
	void addValue(CREF(LPRDOValue) pValue)
	{
		m_valueList.push_back(m_pHeader->getTypeInfo()->value_cast(pValue));
	}

private:
	RDOFUNSequenceEnumerative(CREF(LPRDOFUNSequenceHeader) pHeader, CREF(LPRDOValue) pValue)
		: RDOFUNSequence(pHeader, 0)
	{
		addValue(pValue);
	}
	virtual ~RDOFUNSequenceEnumerative()
	{}

	typedef std::vector<LPRDOValue> ValueList;
	ValueList m_valueList;

	virtual void           createCalcs   ();
	virtual LPRDOFUNArithm createCallCalc(REF(LPRDOFUNParams) pParamList, CREF(RDOParserSrcInfo) srcInfo) const;
};
DECLARE_POINTER(RDOFUNSequenceEnumerative);

// --------------------------------------------------------------------------------
// -------------------- Функции
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOFUNFunctionListElement
// --------------------------------------------------------------------------------
OBJECT(RDOFUNFunctionListElement) IS INSTANCE_OF(RDOParserSrcInfo)
{
DECLARE_FACTORY(RDOFUNFunctionListElement)
public:
	virtual rdo::runtime::LPRDOCalcIsEqual createIsEqualCalc(CREF(LPTypeInfo) pRetType, CREF(rdo::runtime::LPRDOCalcFuncParam) pFuncParam, CREF(RDOParserSrcInfo) src_pos) const;
	virtual rdo::runtime::LPRDOCalcConst   createResultCalc (CREF(LPTypeInfo) pRetType, CREF(RDOParserSrcInfo) src_pos) const = 0;
	virtual rbool                          isEquivalence    () const { return false; }

protected:
	RDOFUNFunctionListElement(CREF(RDOParserSrcInfo) srcInfo);
	virtual ~RDOFUNFunctionListElement();
};

// --------------------------------------------------------------------------------
// -------------------- RDOFUNFunctionListElementIdentif
// --------------------------------------------------------------------------------
class RDOFUNFunctionListElementIdentif: public RDOFUNFunctionListElement
{
DECLARE_FACTORY(RDOFUNFunctionListElementIdentif)
private:
	RDOFUNFunctionListElementIdentif(CREF(RDOParserSrcInfo) srcInfo);

	virtual rdo::runtime::LPRDOCalcConst createResultCalc(CREF(LPTypeInfo) pRetType, CREF(RDOParserSrcInfo) src_pos) const;
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
	RDOFUNFunctionListElementReal(CREF(YYLTYPE) position, double value);
	virtual rdo::runtime::LPRDOCalcConst createResultCalc(CREF(LPTypeInfo) pRetType, CREF(RDOParserSrcInfo) src_pos) const;

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
	RDOFUNFunctionListElementInt(CREF(YYLTYPE) position, int value);
	virtual rdo::runtime::LPRDOCalcConst createResultCalc(CREF(LPTypeInfo) pRetType, CREF(RDOParserSrcInfo) src_pos) const;

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
	RDOFUNFunctionListElementEq(CREF(YYLTYPE) position);

	virtual rdo::runtime::LPRDOCalcConst createResultCalc(CREF(LPTypeInfo) pRetType, CREF(RDOParserSrcInfo) src_pos) const;
	virtual rbool                        isEquivalence   () const { return true; }
};
DECLARE_POINTER(RDOFUNFunctionListElementEq);

// --------------------------------------------------------------------------------
// -------------------- RDOFUNCalculateIf
// --------------------------------------------------------------------------------
OBJECT(RDOFUNCalculateIf) IS INSTANCE_OF(RDOParserSrcInfo)
{
DECLARE_FACTORY(RDOFUNCalculateIf)
public:
	REF(LPRDOFUNLogic)  getCondition() { return m_pCondition; }
	REF(LPRDOFUNArithm) getAction   () { return m_pAction;    }

private:
	RDOFUNCalculateIf(CREF(LPRDOFUNLogic) pCondition, CREF(LPRDOFUNArithm) pAction);
	virtual ~RDOFUNCalculateIf();

	LPRDOFUNLogic  m_pCondition;
	LPRDOFUNArithm m_pAction;
};

// --------------------------------------------------------------------------------
// -------------------- RDOFUNFunction
// --------------------------------------------------------------------------------
CLASS(RDOFUNFunction):
	    INSTANCE_OF      (RDOParserSrcInfo)
	AND INSTANCE_OF      (Context         )
	AND IMPLEMENTATION_OF(IContextFind    )
{
DECLARE_FACTORY(RDOFUNFunction)
friend class Converter;
public:
	typedef std::vector<LPRDOParam> ParamList;

	CREF(tstring)    name                   () const;
	void             add                    (CREF(LPRDOParam)                  pParam       );
	void             add                    (CREF(LPRDOFUNFunctionListElement) pListElement );
	void             add                    (CREF(LPRDOFUNCalculateIf)         pCalculateIf );
	LPRDOParam       findFUNFunctionParam   (CREF(tstring)                     paramName    ) const;
	int              findFUNFunctionParamNum(CREF(tstring)                     paramName    ) const;
	void             createListCalc         ();
	void             createTableCalc        (CREF(YYLTYPE)                     elements_pos );
	void             createAlgorithmicCalc  ();
	CREF(LPRDOParam) getReturn              () const;
	const ParamList  getParams              () const;
	rbool            getReturnFlag          () const;
	void             setReturnFlag          (rbool flag);
	void             end                    ();

	rdo::runtime::LPRDOCalc  getFunctionCalc() const;
	void                     setFunctionCalc(CREF(rdo::runtime::LPRDOCalc) pCalc);

private:
	RDOFUNFunction(CREF(RDOParserSrcInfo) srcInfo, CREF(LPRDOParam) pReturn);
	RDOFUNFunction(CREF(tstring) name,             CREF(LPRDOParam) pReturn);
	virtual ~RDOFUNFunction();

	typedef std::vector<LPRDOFUNFunctionListElement>           ElementList;
	typedef std::vector<LPRDOFUNCalculateIf>                   CalculateIfList;

	LPRDOParam               m_pReturn;
	ParamList                m_paramList;
	ElementList              m_elementList;     //! for list and table
	CalculateIfList          m_calculateIfList; //! for algorithmic
	rdo::runtime::LPRDOCalc  m_pFunctionCalc;
	LPContextMemory          m_pContextMemory;
	rbool                    m_returnFlag;

	void init();

	DECLARE_IContextFind;
};
DECLARE_POINTER(RDOFUNFunction);

// --------------------------------------------------------------------------------
// -------------------- Групповые выражения
// --------------------------------------------------------------------------------
// -------------------- RDOFUNGroup
// --------------------------------------------------------------------------------
CLASS(RDOFUNGroup):
	    INSTANCE_OF      (RDOParserSrcInfo)
	AND INSTANCE_OF      (Context         )
	AND IMPLEMENTATION_OF(IContextFind    )
{
DECLARE_FACTORY(RDOFUNGroup);
public:
	CREF(LPRDORTPResType) getResType()const { return m_pResType; }

protected:
	RDOFUNGroup(CREF(RDOParserSrcInfo) res_info);
	virtual ~RDOFUNGroup();

	void end();

private:
	void init(CREF(RDOParserSrcInfo) res_info);

	LPRDORTPResType m_pResType;

	DECLARE_IContextFind;
};
DECLARE_POINTER(RDOFUNGroup);

// --------------------------------------------------------------------------------
// -------------------- RDOFUNGroupLogic
// --------------------------------------------------------------------------------
CLASS(RDOFUNGroupLogic):
	    INSTANCE_OF(RDOFUNGroup       )
	AND INSTANCE_OF(boost::noncopyable)
{
DECLARE_FACTORY(RDOFUNGroupLogic)
public:
	enum FunGroupType
	{
		fgt_unknow    = 0,
		fgt_exist     = 1,
		fgt_notexist  = 2,
		fgt_forall    = 3,
		fgt_notforall = 4
	};

	LPRDOFUNLogic createFunLogic(REF(LPRDOFUNLogic) pCondition);

private:
	RDOFUNGroupLogic(FunGroupType funType, CREF(RDOParserSrcInfo) res_info);

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
	void           initSelect          (LPRDOFUNLogic pCondition = NULL);
	LPRDOFUNLogic  createFunSelectGroup(RDOFUNGroupLogic::FunGroupType funType, REF(LPRDOFUNLogic) pCondition);
	LPRDOFUNLogic  createFunSelectEmpty(CREF(RDOParserSrcInfo) empty_info);
	LPRDOFUNArithm createFunSelectSize (CREF(RDOParserSrcInfo) size_info );

private:
	RDOFUNSelect(CREF(RDOParserSrcInfo) res_info);

	rdo::runtime::LPRDOFunCalcSelect m_pCalcSelect;
};
DECLARE_POINTER(RDOFUNSelect);

CLOSE_RDO_PARSER_NAMESPACE

#endif // _RDOFUN_H_
