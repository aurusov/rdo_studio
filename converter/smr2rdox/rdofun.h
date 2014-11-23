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

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

int  cnv_funparse(void* lexer);
int  cnv_funlex  (YYSTYPE* lpval, YYLTYPE* llocp, void* lexer);
void cnv_funerror(YYLTYPE* llocp, void* lexer, const char* message);

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
// -------------------- RDOFUNLogic
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RDOFUNArithm);

PREDECLARE_POINTER(RDOFUNLogic);
class RDOFUNLogic
	: public rdo::counter_reference
	, public RDOParserSrcInfo
{
DECLARE_FACTORY(RDOFUNLogic);
friend class RDOFUNArithm;
public:
	rdo::runtime::LPRDOCalc getCalc(rdo::runtime::RDOType::TypeID id = rdo::runtime::RDOType::t_real);

	LPRDOFUNLogic operator && (const LPRDOFUNLogic& pSecond);
	LPRDOFUNLogic operator || (const LPRDOFUNLogic& pSecond);
	LPRDOFUNLogic operator_not();

	virtual void setSrcInfo(const RDOParserSrcInfo& src_info);
	virtual void setSrcPos (const RDOSrcInfo::Position& position);
	virtual void setSrcText(const std::string& value);
	        void setSrcPos (const YYLTYPE& error_pos);
	        void setSrcPos (const YYLTYPE& pos_begin, const YYLTYPE& pos_end);

private:
	RDOFUNLogic(const LPRDOFUNArithm& pArithm);
	RDOFUNLogic(const rdo::runtime::LPRDOCalc& pCalc, bool hideWarning);
	virtual ~RDOFUNLogic();

	rdo::runtime::LPRDOCalc    m_pCalc;
	RDOFUNDoubleToIntByResult  m_intOrDouble;
};

// --------------------------------------------------------------------------------
// -------------------- RDOFUNArithm
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RDOFUNArithm);
class RDOFUNArithm
	: public rdo::counter_reference
	, public RDOParserSrcInfo
{
DECLARE_FACTORY(RDOFUNArithm);
public:
	LPRDOFUNArithm operator +(const LPRDOFUNArithm& pSecond);
	LPRDOFUNArithm operator -(const LPRDOFUNArithm& pSecond);
	LPRDOFUNArithm operator *(const LPRDOFUNArithm& pSecond);
	LPRDOFUNArithm operator /(const LPRDOFUNArithm& pSecond);

	LPRDOFUNLogic operator ==(const LPRDOFUNArithm& pSecond);
	LPRDOFUNLogic operator !=(const LPRDOFUNArithm& pSecond);
	LPRDOFUNLogic operator < (const LPRDOFUNArithm& pSecond);
	LPRDOFUNLogic operator > (const LPRDOFUNArithm& pSecond);
	LPRDOFUNLogic operator <=(const LPRDOFUNArithm& pSecond);
	LPRDOFUNLogic operator >=(const LPRDOFUNArithm& pSecond);

	rdo::runtime::LPRDOCalc       createCalc(const LPRDOTypeParam& pForType = NULL);
	rdo::runtime::LPRDOCalc       calc      () const { return m_pCalc;                    }
	const LPRDOValue&             value     () const { return m_pValue;                   }
	LPRDOType                     type      () const { return m_pValue->type();           }
	LPRDOEnumType                 enumType  () const { return type().object_static_cast<RDOEnumType>(); }
	rdo::runtime::RDOType::TypeID typeID    () const { return type()->type()->typeID();   }

	virtual void setSrcInfo(const RDOParserSrcInfo& src_info);
	virtual void setSrcPos (const RDOSrcInfo::Position& position);
	virtual void setSrcText(const std::string& value);
	        void setSrcInfo(const RDOParserSrcInfo& begin, const std::string& delim, const RDOParserSrcInfo& end);
	        void setSrcPos (const YYLTYPE& error_pos);
	        void setSrcPos (const YYLTYPE& pos_begin, const YYLTYPE& pos_end);

	void checkParamType(const LPRDOTypeParam& pType);

private:
	RDOFUNArithm(const LPRDOValue& pValue);
	RDOFUNArithm(const LPRDOValue& pValue, const rdo::runtime::LPRDOCalc& pCalc);
	RDOFUNArithm(const LPRDOValue& pResName, const LPRDOValue& pParName);
	virtual ~RDOFUNArithm();

	LPRDOValue                 m_pValue;
	rdo::runtime::LPRDOCalc    m_pCalc;
	RDOFUNDoubleToIntByResult  m_intOrDouble;

	void init(const LPRDOValue& pValue);
	void init(const LPRDOValue& pResName, const LPRDOValue& pParName);

	enum CastResult
	{
		CR_DONE,
		CR_CONTINUE
	};
	CastResult beforeCastValue(LPRDOFUNArithm        pSecond);
	LPRDOType  getPreType     (const LPRDOFUNArithm& pSecond);
};

// --------------------------------------------------------------------------------
// -------------------- RDOFUNConstant
// --------------------------------------------------------------------------------
class RDOFUNConstant: public RDOParam
{
DECLARE_FACTORY(RDOFUNConstant);
public:
	int getNumber() const { return m_number; }

private:
	RDOFUNConstant(const RDOParserSrcInfo& src_info, const LPRDOTypeParam& pType, const LPRDOValue& pDefault);
	virtual ~RDOFUNConstant();

	int m_number;
};
DECLARE_POINTER(RDOFUNConstant);

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
	const ParamList& getParamList() const { return m_paramList ; }
	rdo::runtime::LPRDOCalc getCalc(std::size_t paramID, const LPRDOTypeParam& pType);

	void addParameter(const LPRDOFUNArithm& pParam);
	LPRDOFUNArithm createCall(const std::string& funName);
	LPRDOFUNArithm createSeqCall(const std::string& seqName);

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
		const LPRDOTypeParam& getType() const { return m_pType; }

	private:
		RDOFUNSequenceHeader(const LPRDOTypeParam& pType, const RDOParserSrcInfo& src_info)
			: RDOParserSrcInfo(src_info)
			, m_pType         (pType   )
		{}

		LPRDOTypeParam m_pType;
	};

	const std::string& name() const { return m_pHeader->src_text(); }
	const LPRDOFUNSequenceHeader& getHeader() const { return m_pHeader; }
	 rdo::runtime::LPRDOCalcSeqInit& getInitCalc() { return m_pInitCalc; }
	 rdo::runtime::LPRDOCalcSeqNext& getNextCalc() { return m_pNextCalc; }

	virtual void           createCalcs   () = 0;
	virtual LPRDOFUNArithm createCallCalc(LPRDOFUNParams& pParamList, const RDOParserSrcInfo& src_info) const = 0;

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
	virtual LPRDOFUNArithm createCallCalc(LPRDOFUNParams& pParamList, const RDOParserSrcInfo& seq_src_info) const;
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
	virtual LPRDOFUNArithm createCallCalc(LPRDOFUNParams& pParamList, const RDOParserSrcInfo& seq_src_info) const;
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
	virtual LPRDOFUNArithm createCallCalc(LPRDOFUNParams& pParamList, const RDOParserSrcInfo& seq_src_info) const;
};

// --------------------------------------------------------------------------------
// -------------------- Гистограмма
// --------------------------------------------------------------------------------
// -------------------- RDOFUNSequenceByHist
// --------------------------------------------------------------------------------
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
	virtual LPRDOFUNArithm createCallCalc(LPRDOFUNParams& pParamList, const RDOParserSrcInfo& src_info) const;
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
		m_valueList.push_back(m_pHeader->getType()->value_cast(pValue));
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
	virtual LPRDOFUNArithm createCallCalc(LPRDOFUNParams& pParamList, const RDOParserSrcInfo& src_info) const;
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
	virtual rdo::runtime::LPRDOCalcIsEqual createIsEqualCalc(const LPRDOTypeParam& pRetType, const rdo::runtime::LPRDOCalcFuncParam& pFuncParam, const RDOParserSrcInfo& src_pos) const;
	virtual rdo::runtime::LPRDOCalcConst createResultCalc(const LPRDOTypeParam& pRetType, const RDOParserSrcInfo& src_pos) const = 0;
	virtual bool isEquivalence() const { return false; }

protected:
	RDOFUNFunctionListElement(const RDOParserSrcInfo& src_info);
	virtual ~RDOFUNFunctionListElement();
};

// --------------------------------------------------------------------------------
// -------------------- RDOFUNFunctionListElementIdentif
// --------------------------------------------------------------------------------
class RDOFUNFunctionListElementIdentif: public RDOFUNFunctionListElement
{
DECLARE_FACTORY(RDOFUNFunctionListElementIdentif)
private:
	RDOFUNFunctionListElementIdentif(const RDOParserSrcInfo& src_info);

	virtual rdo::runtime::LPRDOCalcConst createResultCalc(const LPRDOTypeParam& pRetType, const RDOParserSrcInfo& src_pos) const;
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
	virtual rdo::runtime::LPRDOCalcConst createResultCalc(const LPRDOTypeParam& pRetType, const RDOParserSrcInfo& src_pos) const;

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
	virtual rdo::runtime::LPRDOCalcConst createResultCalc(const LPRDOTypeParam& pRetType, const RDOParserSrcInfo& src_pos) const;

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

	virtual rdo::runtime::LPRDOCalcConst createResultCalc(const LPRDOTypeParam& pRetType, const RDOParserSrcInfo& src_pos) const;
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
	LPRDOFUNLogic&  getCondition() { return m_pCondition; }
	LPRDOFUNArithm& getAction   () { return m_pAction;    }

private:
	RDOFUNCalculateIf(const LPRDOFUNLogic& pCondition, const LPRDOFUNArithm& pAction);
	virtual ~RDOFUNCalculateIf();

	LPRDOFUNLogic  m_pCondition;
	LPRDOFUNArithm m_pAction;
};

// --------------------------------------------------------------------------------
// -------------------- RDOFUNFunction
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RDOFUNFunction);
class RDOFUNFunction
	: public rdo::counter_reference
	, public RDOParserSrcInfo
{
DECLARE_FACTORY(RDOFUNFunction)
friend class Converter;
public:
	typedef std::vector<LPRDOParam> ParamList;

	void add(const LPRDOParam& pParam);
	void add(const LPRDOFUNFunctionListElement& pListElement);
	void add(const LPRDOFUNCalculateIf& pCalculateIf);

	LPRDOParam findFUNFunctionParam(const std::string& paramName) const;
	int findFUNFunctionParamNum(const std::string& paramName) const;

	void createListCalc();
	void createTableCalc(const YYLTYPE& elements_pos);
	void createAlgorithmicCalc(const RDOParserSrcInfo& body_src_info);

	const std::string& name() const { return src_info().src_text(); }
	const LPRDOParam& getReturn() const { return m_pReturn; }
	const ParamList getParams() const { return m_paramList; }

	void                       setFunctionCalc(const rdo::runtime::LPRDOFunCalc& pCalc);
	rdo::runtime::LPRDOFunCalc getFunctionCalc() const { return m_pFunctionCalc; }

	void insertPostLinked(const rdo::runtime::LPRDOCalcFunctionCaller& pCalc)
	{
		ASSERT(pCalc);
		m_postLinkedList.push_back(pCalc);
	}

private:
	RDOFUNFunction(const RDOParserSrcInfo& src_info, const LPRDOParam& pReturn);
	RDOFUNFunction(const std::string& name, const LPRDOParam& pReturn);
	virtual ~RDOFUNFunction();

	typedef std::vector<LPRDOFUNFunctionListElement>           ElementList;
	typedef std::vector<LPRDOFUNCalculateIf>                   CalculateIfList;
	typedef std::vector<rdo::runtime::LPRDOCalcFunctionCaller> PostLinkedList;

	LPRDOParam                 m_pReturn;
	ParamList                  m_paramList;
	ElementList                m_elementList;     //! for list and table
	CalculateIfList            m_calculateIfList; //! for algorithmic
	PostLinkedList             m_postLinkedList;  //! для рекурсивного вызова
	rdo::runtime::LPRDOFunCalc m_pFunctionCalc;
};

// --------------------------------------------------------------------------------
// -------------------- Групповые выражения
// --------------------------------------------------------------------------------
// -------------------- RDOFUNGroup
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RDOFUNGroup);
class RDOFUNGroup
	: public rdo::counter_reference
	, public RDOParserSrcInfo
{
DECLARE_FACTORY(RDOFUNGroup);
public:
	const LPRDORTPResType& getResType()const { return m_pResType; }

protected:
	RDOFUNGroup(const RDOParserSrcInfo& res_info);
	virtual ~RDOFUNGroup();

private:
	void init(const RDOParserSrcInfo& res_info);

	LPRDORTPResType m_pResType;
};

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
		fgt_notforall = 4
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
	void           initSelect          (LPRDOFUNLogic pCondition = NULL);
	LPRDOFUNLogic  createFunSelectGroup(RDOFUNGroupLogic::FunGroupType funType, LPRDOFUNLogic& pCondition);
	LPRDOFUNLogic  createFunSelectEmpty(const RDOParserSrcInfo& empty_info);
	LPRDOFUNArithm createFunSelectSize (const RDOParserSrcInfo& size_info );

private:
	RDOFUNSelect(const RDOParserSrcInfo& res_info);

	rdo::runtime::LPRDOFunCalcSelect m_pCalcSelect;
};
DECLARE_POINTER(RDOFUNSelect);

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE

#endif // _CONVERTOR_RDOFUN_H_
