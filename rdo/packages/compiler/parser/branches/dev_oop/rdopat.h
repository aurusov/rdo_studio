/*!
  \copyright (c) RDO-Team, 2011
  \file      rdopat.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

#ifndef _RDOPAT_H_
#define _RDOPAT_H_

// ----------------------------------------------------------------------- INCLUDES
#include <boost/noncopyable.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdortp_param.h"
#include "simulator/compiler/parser/rdofun.h"
#include "simulator/compiler/parser/rdorss.h"
#include "simulator/compiler/parser/param.h"
#include "simulator/compiler/parser/local_variable.h"
#include "simulator/compiler/parser/context/context.h"
#include "simulator/compiler/parser/context/memory.h"
#include "simulator/compiler/parser/context/context_switch_i.h"

#include "simulator/runtime/rdo_pattern.h"
#include "simulator/runtime/rdo_resource.h"
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/calc/procedural/calc_statement.h"
#include "simulator/runtime/calc/resource/calc_relevant.h"
#include "simulator/runtime/calc/resource/calc_choice_from.h"
#include "simulator/runtime/calc/resource/calc_create_resource.h"

#include "utils/smart_ptr/factory.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

int  patparse(PTR(void) lexer);
int  patlex  (PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer);
void paterror(PTR(char) message);

int  evnparse(PTR(void) lexer);
int  evnlex  (PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer);
void evnerror(PTR(char) message);

int  evn_preparse_parse(PTR(void) lexer);
int  evn_preparse_lex  (PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer);
void evn_preparse_error(PTR(char) message);

// --------------------------------------------------------------------------------
// -------------------- ConvertCmdList
// --------------------------------------------------------------------------------
OBJECT(ConvertCmdList)
{
DECLARE_FACTORY(ConvertCmdList)
public:
	typedef std::vector<rdo::runtime::LPRDOCalc> CalcList;

	void insertCommand(CREF(rdo::runtime::LPRDOCalc) pCalc)
	{
		m_calcList.push_back(pCalc);
	}
	CREF(CalcList) commands() const
	{
		return m_calcList;
	}

private:
	CalcList m_calcList;
};

// --------------------------------------------------------------------------------
// -------------------- RDOPATPattern
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RDOPATChoiceFrom);
PREDECLARE_POINTER(RDOPATChoiceOrder);
PREDECLARE_POINTER(RDORelevantResource);

CLASS(RDOPATPattern):
	    INSTANCE_OF      (RDOParserSrcInfo)
	AND INSTANCE_OF      (Context         )
	AND IMPLEMENTATION_OF(IContextFind    )
{
DECLARE_FACTORY(RDOPATPattern)
friend class RDOOPROperation;
friend class RDODPTActivity;
friend class RDOParserEVNPost;

public:
	enum PatType
	{
		PT_Event,
		PT_Rule,
		PT_Operation,
		PT_Keyboard
	};
	virtual PatType getType() const = 0;

	typedef std::vector<LPRDORelevantResource> RelResList;

	rbool                            isHaveConvertEnd() const { return getType() == PT_Operation || getType() == PT_Keyboard; }

	CREF(rdo::runtime::LPRDOPattern) getPatRuntime   () const { return m_pPatRuntime; }
	template<class T>
	rdo::intrusive_ptr<T>            getPatRuntime   () const
	{
		rdo::intrusive_ptr<T> pPatRuntime = m_pPatRuntime.object_static_cast<T>();
		ASSERT(pPatRuntime);
		return pPatRuntime;
	}

	static tstring StatusToStr(rdo::runtime::RDOResource::ConvertStatus value);
	rdo::runtime::RDOResource::ConvertStatus StrToStatus(CREF(tstring) value, CREF(YYLTYPE) convertor_pos);

	RelResList::const_iterator rel_res_begin () const { return m_relResList.begin(); }
	RelResList::const_iterator rel_res_end   () const { return m_relResList.end();   }
	int                        rel_res_count () const { return m_relResList.size();  }
	virtual void               rel_res_insert(CREF(LPRDORelevantResource) pRelevantResource);

	void beforeRelRensert(CREF(RDOParserSrcInfo) rel_info);

	LPRDORelevantResource m_pCurrRelRes;

	void                  add                    (CREF(LPRDOParam) pParam);
	LPRDOParam            findPATPatternParam    (CREF(tstring) paramName) const;
	int                   findPATPatternParamNum (CREF(tstring) paramName) const;
	LPRDORelevantResource findRelevantResource   (CREF(tstring) resName  ) const;
	ruint                 findRelevantResourceNum(CREF(tstring) resName  ) const;
	virtual void          addRelRes              (CREF(RDOParserSrcInfo) rel_info, CREF(RDOParserSrcInfo) type_info, rdo::runtime::RDOResource::ConvertStatus beg, CREF(YYLTYPE) convertor_pos) = 0;

	CREF(tstring) name() const { return src_text(); }

	void setCommonChoiceFirst  ();
	void setCommonChoiceWithMin(CREF(LPRDOFUNArithm) arithm);
	void setCommonChoiceWithMax(CREF(LPRDOFUNArithm) arithm);
	void setTime               ( REF(LPRDOFUNArithm) arithm);
	void addRelResBody         (CREF(RDOParserSrcInfo) body_name);
	virtual void addRelResUsage(CREF(LPRDOPATChoiceFrom) pChoiceFrom, CREF(LPRDOPATChoiceOrder) pChoiceOrder);
	void addRelResConvert      (rbool trace, CREF(LPExpression) pStatementList, CREF(YYLTYPE) convertor_pos, CREF(YYLTYPE) trace_pos, rdo::runtime::RDOResource::ConvertStatus status);
	void end                   ();

	void         writeModelStructure    (REF(std::ostream) stream) const;
	virtual char getModelStructureLetter() const = 0;
	tstring      getPatternId           () const;

protected:
	RDOPATPattern(CREF(RDOParserSrcInfo) name_src_info);
	virtual ~RDOPATPattern();

	rdo::runtime::LPRDOPattern m_pPatRuntime;


	rdo::runtime::LPRDOCalc createRelRes   (rbool trace) const;
	virtual void            addParamSetCalc(CREF(rdo::runtime::LPRDOCalc) pCalc);

	virtual tstring getErrorMessage_NotNeedConvertor(CREF(tstring) name, rdo::runtime::RDOResource::ConvertStatus status) = 0;
	virtual tstring getWarningMessage_EmptyConvertor(CREF(tstring) name, rdo::runtime::RDOResource::ConvertStatus status) = 0;

private:
	typedef std::vector<LPRDOParam> ParamList;

	ParamList        m_paramList;
	RelResList       m_relResList;

	rbool            m_useCommonChoice;
	rbool            m_useCommonWithMax;
	LPRDOFUNArithm   m_pCommonChoice;

	ruint            m_currentRelResIndex;

	LPContextMemory  m_pContextMemory;

	tstring               typeToString     (PatType type) const;
	void                  addChoiceFromCalc(CREF(rdo::runtime::LPRDOCalc) pCalc);
	LPRDORelevantResource findRelRes       (CREF(LPRDOValue) pValue) const;

	DECLARE_IContextFind;
};
DECLARE_POINTER(RDOPATPattern);

// --------------------------------------------------------------------------------
// -------------------- RDOPatternEvent
// --------------------------------------------------------------------------------
class RDOPatternEvent: public RDOPATPattern
{
DECLARE_FACTORY(RDOPatternEvent)
public:
	virtual void addRelRes     (CREF(RDOParserSrcInfo) rel_info, CREF(RDOParserSrcInfo) type_info, rdo::runtime::RDOResource::ConvertStatus beg, CREF(YYLTYPE) convertor_pos);
	virtual void addRelResUsage(CREF(LPRDOPATChoiceFrom) pChoiceFrom, CREF(LPRDOPATChoiceOrder) pChoiceOrder);

	virtual char getModelStructureLetter() const
	{
		return _T('V');
	}
	virtual PatType getType() const
	{
		return PT_Event;
	}

protected:
	virtual tstring getErrorMessage_NotNeedConvertor(CREF(tstring) name, rdo::runtime::RDOResource::ConvertStatus status);
	virtual tstring getWarningMessage_EmptyConvertor(CREF(tstring) name, rdo::runtime::RDOResource::ConvertStatus status);

private:
	RDOPatternEvent(CREF(RDOParserSrcInfo) name_src_info, rbool trace);
};

// --------------------------------------------------------------------------------
// -------------------- RDOPatternRule
// --------------------------------------------------------------------------------
class RDOPatternRule: public RDOPATPattern
{
DECLARE_FACTORY(RDOPatternRule)
public:
	virtual void addRelRes(CREF(RDOParserSrcInfo) rel_info, CREF(RDOParserSrcInfo) type_info, rdo::runtime::RDOResource::ConvertStatus beg, CREF(YYLTYPE) convertor_pos);

	virtual char getModelStructureLetter() const
	{
		return _T('R');
	};
	virtual PatType getType() const
	{
		return PT_Rule;
	}

protected:
	virtual tstring getErrorMessage_NotNeedConvertor(CREF(tstring) name, rdo::runtime::RDOResource::ConvertStatus status);
	virtual tstring getWarningMessage_EmptyConvertor(CREF(tstring) name, rdo::runtime::RDOResource::ConvertStatus status);

private:
	RDOPatternRule(CREF(RDOParserSrcInfo) name_src_info, rbool trace);
};

// --------------------------------------------------------------------------------
// -------------------- RDOPatternOperation
// --------------------------------------------------------------------------------
class RDOPatternOperation: public RDOPATPattern
{
DECLARE_FACTORY(RDOPatternOperation)
public:
	virtual void addRelRes               (CREF(RDOParserSrcInfo) rel_info, CREF(RDOParserSrcInfo) type_info, rdo::runtime::RDOResource::ConvertStatus beg, CREF(YYLTYPE) convertor_pos);
	        void addRelRes               (CREF(RDOParserSrcInfo) rel_info, CREF(RDOParserSrcInfo) type_info, rdo::runtime::RDOResource::ConvertStatus beg, rdo::runtime::RDOResource::ConvertStatus end, CREF(YYLTYPE) convertor_begin_pos, CREF(YYLTYPE) convertor_end_pos);
	        void addRelResConvertBeginEnd(rbool trace_begin, CREF(LPExpression) pBeginStatementList, rbool trace_end, CREF(LPExpression) pEndStatementList, CREF(YYLTYPE) convertor_begin_pos, CREF(YYLTYPE) convertor_end_pos, CREF(YYLTYPE) trace_begin_pos, CREF(YYLTYPE) trace_end_pos);

	virtual char getModelStructureLetter() const
	{
		return _T('A');
	};
	virtual PatType getType() const
	{
		return PT_Operation;
	}

protected:
	//! Конструктор вызывается из RDOPatternKeyboard
	RDOPatternOperation(rbool trace, CREF(RDOParserSrcInfo) name_src_info);

	virtual void rel_res_insert (CREF(LPRDORelevantResource) pRelevantResource);
	virtual void addParamSetCalc(CREF(rdo::runtime::LPRDOCalc) pCalc            );

	virtual tstring getErrorMessage_NotNeedConvertor(CREF(tstring) name, rdo::runtime::RDOResource::ConvertStatus status);
	virtual tstring getWarningMessage_EmptyConvertor(CREF(tstring) name, rdo::runtime::RDOResource::ConvertStatus status);

private:
	RDOPatternOperation(CREF(RDOParserSrcInfo) name_src_info, rbool trace);

	enum ConvertorType
	{
		convert_unknow,
		convert_begin,
		convert_end
	};
	ConvertorType m_convertorType;
};

// --------------------------------------------------------------------------------
// -------------------- RDOPatternKeyboard
// --------------------------------------------------------------------------------
class RDOPatternKeyboard: public RDOPatternOperation
{
DECLARE_FACTORY(RDOPatternKeyboard)
public:
	virtual char getModelStructureLetter() const
	{
		return _T('K');
	};
	virtual PatType getType() const
	{
		return PT_Keyboard;
	}

private:
	RDOPatternKeyboard(CREF(RDOParserSrcInfo) name_src_info, rbool trace);
};

// --------------------------------------------------------------------------------
// -------------------- RDORelevantResource
// --------------------------------------------------------------------------------
CLASS(RDORelevantResource):
	    INSTANCE_OF      (RDOParserSrcInfo  )
	AND INSTANCE_OF      (Context           )
	AND INSTANCE_OF      (boost::noncopyable)
	AND IMPLEMENTATION_OF(IContextSwitch    )
{
DECLARE_FACTORY(RDORelevantResource)
public:
	int                  m_relResID;
	rbool                m_alreadyHaveConverter;
	LPRDOPATChoiceFrom   m_pChoiceFrom;
	LPRDOPATChoiceOrder  m_pChoiceOrder;
	RDOParserSrcInfo     m_bodySrcInfo;

	const rdo::runtime::RDOResource::ConvertStatus m_statusBegin;
	const rdo::runtime::RDOResource::ConvertStatus m_statusEnd;

	enum
	{
		stateNone = 0,
		choiceEmpty,
		choiceNoCheck,
		choiceFrom,
		choiceOrderEmpty,
		choiceOrderFirst,
		choiceOrderWithMin,
		choiceOrderWithMax,
		convertBegin,
		convertEnd
	} m_currentState;
	rbool isChoiceFromState() const { return m_currentState == choiceEmpty || m_currentState == choiceNoCheck || m_currentState == choiceFrom; }

	        CREF(tstring)   name   () const  { return src_text(); };
	virtual LPRDORTPResType getType() const = 0;

	virtual rdo::runtime::LPRDOCalc                  createPreSelectRelResCalc           () = 0; //! Предварительный выбор ресурсов в самом списке рел. ресурсов
	virtual rdo::runtime::LPRDOCalc                  createSelectResourceChoiceCalc      () = 0; //! Самый обыкновенный choice from + first/with_min/with_max
	virtual rdo::runtime::LPRDOCalc                  createSelectFirstResourceChoiceCalc () = 0; //! common first, который не пашет
	virtual rdo::runtime::LPIRDOSelectResourceCommon createSelectResourceCommonChoiceCalc() = 0; //! common with_min/with_max

	virtual rbool isDirect() const = 0;

	class ParamSetList
	{
	public:
		void reset()
		{
			m_list.clear();
		}
		void insert(CREF(LPRDORTPParam) param)
		{
			m_list.push_back(param);
		}
		rbool find(CREF(tstring) name) const
		{
			STL_FOR_ALL_CONST(m_list, paramIt)
			{
				if ((*paramIt)->name() == name)
					return true;
			}
			return false;
		}
		rbool empty() const
		{
			return m_list.empty();
		}
	private:
		typedef std::vector<LPRDORTPParam> List;

		List m_list;
	};
	CREF(ParamSetList) getParamSetList() const
	{
		return m_paramSetList;
	}
	REF(ParamSetList) getParamSetList()
	{
		return m_paramSetList;
	}

protected:
	RDORelevantResource(CREF(RDOParserSrcInfo) src_info, const int relResID, const rdo::runtime::RDOResource::ConvertStatus statusBegin, const rdo::runtime::RDOResource::ConvertStatus statusEnd);
	virtual ~RDORelevantResource();

	rdo::runtime::LPRDOCalc                   getChoiceCalc();
	rdo::runtime::LPRDOCalc                   getSelectCalc();
	rdo::runtime::RDOSelectResourceCalc::Type getSelectType() const;

private:
	ParamSetList m_paramSetList;

	DECLARE_IContextSwitch;
};
DECLARE_POINTER(RDORelevantResource);

// --------------------------------------------------------------------------------
// -------------------- RDOPATChoiceFrom
// --------------------------------------------------------------------------------
OBJECT(RDOPATChoiceFrom) IS INSTANCE_OF(RDOParserSrcInfo)
{
DECLARE_FACTORY(RDOPATChoiceFrom)
public:
	enum Type
	{
		ch_empty = 0,
		ch_nocheck,
		ch_from
	} m_type;

	LPRDOFUNLogic m_pLogic;

private:
	RDOPATChoiceFrom(CREF(RDOParserSrcInfo) src_info, Type type, CREF(LPRDOFUNLogic) pLogic = NULL)
		: RDOParserSrcInfo(src_info)
		, m_type          (type    )
		, m_pLogic        (pLogic  )
	{}
};

// --------------------------------------------------------------------------------
// -------------------- RDOPATChoiceOrder
// --------------------------------------------------------------------------------
OBJECT(RDOPATChoiceOrder) IS INSTANCE_OF(RDOParserSrcInfo)
{
DECLARE_FACTORY(RDOPATChoiceOrder)
public:
	rdo::runtime::RDOSelectResourceCalc::Type m_type;

	LPRDOFUNArithm m_pArithm;

	tstring asString() const
	{
		switch (m_type)
		{
		case rdo::runtime::RDOSelectResourceCalc::order_empty   : return _T("<правило_выбора_не_указано>");
		case rdo::runtime::RDOSelectResourceCalc::order_first   : return _T("first");
		case rdo::runtime::RDOSelectResourceCalc::order_with_min: return _T("with_min");
		case rdo::runtime::RDOSelectResourceCalc::order_with_max: return _T("with_max");
		default                                                 : NEVER_REACH_HERE;
		}
		return tstring();
	}

private:
	RDOPATChoiceOrder(CREF(RDOParserSrcInfo) src_info, rdo::runtime::RDOSelectResourceCalc::Type type, CREF(LPRDOFUNArithm) pArithm = NULL)
		: RDOParserSrcInfo(src_info)
		, m_type          (type    )
		, m_pArithm       (pArithm )
	{}
};

// --------------------------------------------------------------------------------
// -------------------- RDORelevantResourceDirect - по имени ресурса
// --------------------------------------------------------------------------------
class RDORelevantResourceDirect: public RDORelevantResource
{
DECLARE_FACTORY(RDORelevantResourceDirect)
public:
	LPRDORSSResource getResource() const { return m_pResource; }

	virtual LPRDORTPResType                          getType                             () const;
	virtual rdo::runtime::LPRDOCalc                  createPreSelectRelResCalc           ();
	virtual rdo::runtime::LPRDOCalc                  createSelectFirstResourceChoiceCalc ();
	virtual rdo::runtime::LPRDOCalc                  createSelectResourceChoiceCalc      ();
	virtual rdo::runtime::LPIRDOSelectResourceCommon createSelectResourceCommonChoiceCalc();
	virtual rbool isDirect() const  { return true; }

private:
	RDORelevantResourceDirect(CREF(RDOParserSrcInfo) src_info, const int relResID, CREF(LPRDORSSResource) pResource, const rdo::runtime::RDOResource::ConvertStatus statusBegin, const rdo::runtime::RDOResource::ConvertStatus statusEnd = rdo::runtime::RDOResource::CS_NoChange)
		: RDORelevantResource(src_info, relResID, statusBegin, statusEnd)
		, m_pResource        (pResource)
	{}

	LPRDORSSResource m_pResource;
};

// --------------------------------------------------------------------------------
// -------------------- RDORelevantResourceByType - по имени типа
// --------------------------------------------------------------------------------
class RDORelevantResourceByType: public RDORelevantResource
{
DECLARE_FACTORY(RDORelevantResourceByType)
public:
	virtual LPRDORTPResType getType() const
	{
		return m_pResType;
	}
	virtual rdo::runtime::LPRDOCalc                  createPreSelectRelResCalc           ();
	virtual rdo::runtime::LPRDOCalc                  createSelectFirstResourceChoiceCalc ();
	virtual rdo::runtime::LPRDOCalc                  createSelectResourceChoiceCalc      ();
	virtual rdo::runtime::LPIRDOSelectResourceCommon createSelectResourceCommonChoiceCalc();
	virtual rbool isDirect() const  { return false; }

private:
	RDORelevantResourceByType(CREF(RDOParserSrcInfo) src_info, const int relResID, LPRDORTPResType pResType, const rdo::runtime::RDOResource::ConvertStatus statusBegin, const rdo::runtime::RDOResource::ConvertStatus statusEnd = rdo::runtime::RDOResource::CS_NoChange)
		: RDORelevantResource(src_info, relResID, statusBegin, statusEnd)
		, m_pResType         (pResType)
	{}

	LPRDORTPResType m_pResType;
};

CLOSE_RDO_PARSER_NAMESPACE

#endif // _RDOPAT_H_
