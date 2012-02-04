/*!
  \copyright (c) RDO-Team, 2011
  \file      rdofun.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

#ifndef _CONVERTOR_RDOFUN_H_
#define _CONVERTOR_RDOFUN_H_

// ----------------------------------------------------------------------- INCLUDES
#include <boost/noncopyable.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/rdo_object.h"
#include "converter/smr2rdox/rdo_value.h"
#include "converter/smr2rdox/rdortp.h"
#include "converter/smr2rdox/param.h"
#include "simulator/runtime/rdo_object.h"
#include "simulator/runtime/rdo_type.h"
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/calc/operation/calc_logic.h"
#include "simulator/runtime/calc/operation/calc_unary.h"
#include "simulator/runtime/calc/calc_sequence.h"
#include "simulator/runtime/calc/resource/calc_select.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_NAMESPACE

int  cnv_funparse(PTR(void) lexer);
int  cnv_funlex  (PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer);
void cnv_funerror(PTR(char) message);

// --------------------------------------------------------------------------------
// -------------------- RDOFUNDoubleToIntByResult
// --------------------------------------------------------------------------------
class RDOFUNDoubleToIntByResult
{
public:
	void roundCalc();
	void push_back(CREF(rdoRuntime::LPRDOCalcDoubleToIntByResult) pCalc)
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
	typedef std::vector<rdoRuntime::LPRDOCalcDoubleToIntByResult> CalcList;

	CalcList  m_intOrDouble;
};

// --------------------------------------------------------------------------------
// -------------------- RDOFUNLogic
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RDOFUNArithm);

OBJECT(RDOFUNLogic) IS INSTANCE_OF(RDOParserSrcInfo)
{
DECLARE_FACTORY(RDOFUNLogic);
friend class RDOFUNArithm;
public:
	rdoRuntime::LPRDOCalc getCalc(rdoRuntime::RDOType::TypeID id = rdoRuntime::RDOType::t_real);

	LPRDOFUNLogic operator && (CREF(LPRDOFUNLogic) pSecond);
	LPRDOFUNLogic operator || (CREF(LPRDOFUNLogic) pSecond);
	LPRDOFUNLogic operator_not();

	virtual void setSrcInfo(CREF(RDOParserSrcInfo)     src_info);
	virtual void setSrcPos (CREF(RDOSrcInfo::Position) position);
	virtual void setSrcText(CREF(tstring)              value   );
	        void setSrcPos (CREF(YYLTYPE) error_pos);
	        void setSrcPos (CREF(YYLTYPE) pos_begin, CREF(YYLTYPE) pos_end);

private:
	RDOFUNLogic(CREF(LPRDOFUNArithm) pArithm);
	RDOFUNLogic(CREF(rdoRuntime::LPRDOCalc) pCalc, rbool hideWarning);
	virtual ~RDOFUNLogic();

	rdoRuntime::LPRDOCalc      m_pCalc;
	RDOFUNDoubleToIntByResult  m_intOrDouble;
};

// --------------------------------------------------------------------------------
// -------------------- RDOFUNArithm
// --------------------------------------------------------------------------------
OBJECT(RDOFUNArithm) IS INSTANCE_OF(RDOParserSrcInfo)
{
DECLARE_FACTORY(RDOFUNArithm);
public:
	LPRDOFUNArithm operator +(CREF(LPRDOFUNArithm) pSecond);
	LPRDOFUNArithm operator -(CREF(LPRDOFUNArithm) pSecond);
	LPRDOFUNArithm operator *(CREF(LPRDOFUNArithm) pSecond);
	LPRDOFUNArithm operator /(CREF(LPRDOFUNArithm) pSecond);

	LPRDOFUNLogic operator ==(CREF(LPRDOFUNArithm) pSecond);
	LPRDOFUNLogic operator !=(CREF(LPRDOFUNArithm) pSecond);
	LPRDOFUNLogic operator < (CREF(LPRDOFUNArithm) pSecond);
	LPRDOFUNLogic operator > (CREF(LPRDOFUNArithm) pSecond);
	LPRDOFUNLogic operator <=(CREF(LPRDOFUNArithm) pSecond);
	LPRDOFUNLogic operator >=(CREF(LPRDOFUNArithm) pSecond);

	rdoRuntime::LPRDOCalc       createCalc(CREF(LPRDOTypeParam) pForType = NULL);
	rdoRuntime::LPRDOCalc       calc      () const { return m_pCalc;                    }
	CREF(RDOValue)              value     () const { return m_value;                    }
	LPRDOType                   type      () const { return m_value.type();             }
	LPRDOEnumType               enumType  () const { return type().object_static_cast<RDOEnumType>(); }
	rdoRuntime::RDOType::TypeID typeID    () const { return type()->type()->typeID();   }

	virtual void setSrcInfo(CREF(RDOParserSrcInfo) src_info);
	virtual void setSrcPos (CREF(RDOSrcInfo::Position) position);
	virtual void setSrcText(CREF(tstring) value);
	        void setSrcInfo(CREF(RDOParserSrcInfo) begin, CREF(tstring) delim, CREF(RDOParserSrcInfo) end);
	        void setSrcPos (CREF(YYLTYPE) error_pos);
	        void setSrcPos (CREF(YYLTYPE) pos_begin, CREF(YYLTYPE) pos_end);

	void checkParamType(CREF(LPRDOTypeParam) pType);

private:
	RDOFUNArithm(CREF(RDOValue) value);
	RDOFUNArithm(CREF(RDOValue) value, CREF(rdoRuntime::LPRDOCalc) pCalc);
	RDOFUNArithm(CREF(RDOValue) resName, CREF(RDOValue) parName);
	virtual ~RDOFUNArithm();

	RDOValue                   m_value;
	rdoRuntime::LPRDOCalc      m_pCalc;
	RDOFUNDoubleToIntByResult  m_intOrDouble;

	void init(CREF(RDOValue) value);
	void init(CREF(RDOValue) resName, CREF(RDOValue) parName);

	enum CastResult
	{
		CR_DONE,
		CR_CONTINUE
	};
	CastResult beforeCastValue(LPRDOFUNArithm       pSecond);
	LPRDOType  getPreType     (CREF(LPRDOFUNArithm) pSecond);
};

// --------------------------------------------------------------------------------
// -------------------- RDOFUNConstant
// --------------------------------------------------------------------------------
CLASS(RDOFUNConstant): INSTANCE_OF(RDOParam)
{
DECLARE_FACTORY(RDOFUNConstant);
public:
	int getNumber() const { return m_number; }

private:
	RDOFUNConstant(CREF(RDOParserSrcInfo) src_info, CREF(LPRDOTypeParam) pType, CREF(RDOValue) defaultValue);
	virtual ~RDOFUNConstant();

	int m_number;
};
DECLARE_POINTER(RDOFUNConstant);

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

	REF(RDOParserSrcInfo) getFunseqName()       { return m_funseqName; }
	CREF(ParamList)       getParamList () const { return m_paramList ; }
	rdoRuntime::LPRDOCalc getCalc      (ruint paramID, CREF(LPRDOTypeParam) pType);

	void           addParameter (CREF(LPRDOFUNArithm) pParam );
	LPRDOFUNArithm createCall   (CREF(tstring)        funName);
	LPRDOFUNArithm createSeqCall(CREF(tstring)        seqName);

private:
	RDOFUNParams();
	virtual ~RDOFUNParams();

	RDOParserSrcInfo m_funseqName;
	ParamList        m_paramList;
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
		CREF(LPRDOTypeParam) getType() const { return m_pType; }

	private:
		RDOFUNSequenceHeader(CREF(LPRDOTypeParam) pType, CREF(RDOParserSrcInfo) src_info)
			: RDOParserSrcInfo(src_info)
			, m_pType         (pType   )
		{}

		LPRDOTypeParam m_pType;
	};

	CREF(tstring)                      name       () const { return m_pHeader->src_text(); }
	CREF(LPRDOFUNSequenceHeader)       getHeader  () const { return m_pHeader;             }
	 REF(rdoRuntime::LPRDOCalcSeqInit) getInitCalc()       { return m_pInitCalc;           }
	 REF(rdoRuntime::LPRDOCalcSeqNext) getNextCalc()       { return m_pNextCalc;           }

	virtual void           createCalcs   () = 0;
	virtual LPRDOFUNArithm createCallCalc(REF(LPRDOFUNParams) pParamList, CREF(RDOParserSrcInfo) src_info) const = 0;

protected:
	RDOFUNSequence(CREF(LPRDOFUNSequenceHeader) pHeader, int seed);
	virtual ~RDOFUNSequence();

	LPRDOFUNSequenceHeader       m_pHeader;
	int                          m_seed;
	rdoRuntime::LPRDOCalcSeqInit m_pInitCalc;
	rdoRuntime::LPRDOCalcSeqNext m_pNextCalc;

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
	virtual LPRDOFUNArithm createCallCalc(REF(LPRDOFUNParams) pParamList, CREF(RDOParserSrcInfo) seq_src_info) const;
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
	virtual LPRDOFUNArithm createCallCalc(REF(LPRDOFUNParams) pParamList, CREF(RDOParserSrcInfo) seq_src_info) const;
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
	virtual LPRDOFUNArithm createCallCalc(REF(LPRDOFUNParams) pParamList, CREF(RDOParserSrcInfo) seq_src_info) const;
};

// --------------------------------------------------------------------------------
// -------------------- Гистограмма
// --------------------------------------------------------------------------------
// -------------------- RDOFUNSequenceByHist
// --------------------------------------------------------------------------------
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
	virtual LPRDOFUNArithm createCallCalc(REF(LPRDOFUNParams) pParamList, CREF(RDOParserSrcInfo) src_info) const;
};

// --------------------------------------------------------------------------------
// -------------------- RDOFUNSequenceByHistReal
// --------------------------------------------------------------------------------
class RDOFUNSequenceByHistReal: public RDOFUNSequenceByHist
{
DECLARE_FACTORY(RDOFUNSequenceByHistReal);
public:
	typedef std::vector<rdoRuntime::RDOValue> ValueList;

	void addReal(CREF(RDOValue) from, CREF(RDOValue) to, CREF(RDOValue) freq);

private:
	RDOFUNSequenceByHistReal(CREF(LPRDOFUNSequenceByHistHeader) pHeader, CREF(RDOValue) from, CREF(RDOValue) to, CREF(RDOValue) freq);
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
	typedef std::vector<rdoRuntime::RDOValue> ValueList;

	void addEnum(CREF(RDOValue) value, CREF(RDOValue) freq);

private:
	RDOFUNSequenceByHistEnum(CREF(LPRDOFUNSequenceByHistHeader) pHeader, CREF(RDOValue) value, CREF(RDOValue) freq);
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
	void addValue(CREF(RDOValue) value)
	{
		m_valueList.push_back(m_pHeader->getType()->value_cast(value));
	}

private:
	RDOFUNSequenceEnumerative(CREF(LPRDOFUNSequenceHeader) pHeader, CREF(RDOValue) value)
		: RDOFUNSequence(pHeader, 0)
	{
		addValue(value);
	}
	virtual ~RDOFUNSequenceEnumerative()
	{}

	typedef std::vector<RDOValue> ValueList;
	ValueList m_valueList;

	virtual void           createCalcs   ();
	virtual LPRDOFUNArithm createCallCalc(REF(LPRDOFUNParams) pParamList, CREF(RDOParserSrcInfo) src_info) const;
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
	virtual rdoRuntime::LPRDOCalcIsEqual createIsEqualCalc(CREF(LPRDOTypeParam) pRetType, CREF(rdoRuntime::LPRDOCalcFuncParam) pFuncParam, CREF(RDOParserSrcInfo) src_pos) const;
	virtual rdoRuntime::LPRDOCalcConst   createResultCalc (CREF(LPRDOTypeParam) pRetType, CREF(RDOParserSrcInfo) src_pos) const = 0;
	virtual rbool                        isEquivalence    () const { return false; }

protected:
	RDOFUNFunctionListElement(CREF(RDOParserSrcInfo) src_info);
	virtual ~RDOFUNFunctionListElement();
};

// --------------------------------------------------------------------------------
// -------------------- RDOFUNFunctionListElementIdentif
// --------------------------------------------------------------------------------
class RDOFUNFunctionListElementIdentif: public RDOFUNFunctionListElement
{
DECLARE_FACTORY(RDOFUNFunctionListElementIdentif)
private:
	RDOFUNFunctionListElementIdentif(CREF(RDOParserSrcInfo) src_info);

	virtual rdoRuntime::LPRDOCalcConst createResultCalc(CREF(LPRDOTypeParam) pRetType, CREF(RDOParserSrcInfo) src_pos) const;
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
	virtual rdoRuntime::LPRDOCalcConst createResultCalc(CREF(LPRDOTypeParam) pRetType, CREF(RDOParserSrcInfo) src_pos) const;

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
	virtual rdoRuntime::LPRDOCalcConst createResultCalc(CREF(LPRDOTypeParam) pRetType, CREF(RDOParserSrcInfo) src_pos) const;

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

	virtual rdoRuntime::LPRDOCalcConst createResultCalc(CREF(LPRDOTypeParam) pRetType, CREF(RDOParserSrcInfo) src_pos) const;
	virtual rbool                      isEquivalence   () const { return true; }
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
OBJECT(RDOFUNFunction) IS INSTANCE_OF(RDOParserSrcInfo)
{
DECLARE_FACTORY(RDOFUNFunction)
friend class Converter;
public:
	typedef std::vector<LPRDOParam> ParamList;

	void       add                    (CREF(LPRDOParam)                  pParam       );
	void       add                    (CREF(LPRDOFUNFunctionListElement) pListElement );
	void       add                    (CREF(LPRDOFUNCalculateIf)         pCalculateIf );
	LPRDOParam findFUNFunctionParam   (CREF(tstring)                     paramName    ) const;
	int        findFUNFunctionParamNum(CREF(tstring)                     paramName    ) const;
	void       createListCalc         ();
	void       createTableCalc        (CREF(YYLTYPE)                     elements_pos );
	void       createAlgorithmicCalc  (CREF(RDOParserSrcInfo)            body_src_info);

	CREF(tstring)    name     () const { return src_info().src_text(); }
	CREF(LPRDOParam) getReturn() const { return m_pReturn;             }
	const ParamList  getParams() const { return m_paramList;           }

	void                     setFunctionCalc(CREF(rdoRuntime::LPRDOFunCalc) pCalc);
	rdoRuntime::LPRDOFunCalc getFunctionCalc() const { return m_pFunctionCalc; }

	void insertPostLinked(CREF(rdoRuntime::LPRDOCalcFunctionCaller) pCalc)
	{
		ASSERT(pCalc);
		m_postLinkedList.push_back(pCalc);
	}

private:
	RDOFUNFunction(CREF(RDOParserSrcInfo) src_info, CREF(LPRDOParam) pReturn);
	RDOFUNFunction(CREF(tstring) name,              CREF(LPRDOParam) pReturn);
	virtual ~RDOFUNFunction();

	typedef std::vector<LPRDOFUNFunctionListElement>         ElementList;
	typedef std::vector<LPRDOFUNCalculateIf>                 CalculateIfList;
	typedef std::vector<rdoRuntime::LPRDOCalcFunctionCaller> PostLinkedList;

	LPRDOParam               m_pReturn;
	ParamList                m_paramList;
	ElementList              m_elementList;     //! for list and table
	CalculateIfList          m_calculateIfList; //! for algorithmic
	PostLinkedList           m_postLinkedList;  //! для рекурсивного вызова
	rdoRuntime::LPRDOFunCalc m_pFunctionCalc;
};

// --------------------------------------------------------------------------------
// -------------------- Групповые выражения
// --------------------------------------------------------------------------------
// -------------------- RDOFUNGroup
// --------------------------------------------------------------------------------
OBJECT(RDOFUNGroup) IS INSTANCE_OF(RDOParserSrcInfo)
{
DECLARE_FACTORY(RDOFUNGroup);
public:
	CREF(LPRDORTPResType) getResType()const { return m_pResType; }

protected:
	RDOFUNGroup(CREF(RDOParserSrcInfo) res_info);
	virtual ~RDOFUNGroup();

private:
	void init(CREF(RDOParserSrcInfo) res_info);

	LPRDORTPResType m_pResType;
};

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

	rdoRuntime::LPRDOFunCalcSelect m_pCalcSelect;
};
DECLARE_POINTER(RDOFUNSelect);

CLOSE_RDO_CONVERTER_NAMESPACE

#endif // _CONVERTOR_RDOFUN_H_
