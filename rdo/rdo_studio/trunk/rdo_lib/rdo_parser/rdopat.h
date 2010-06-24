/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdopat.h
 * author   : Александ Барс, Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOPAT_H_
#define _RDOPAT_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdo_object.h"
#include "rdo_lib/rdo_parser/rdortp_param.h"
#include "rdo_lib/rdo_parser/rdofun.h"
#include "rdo_lib/rdo_parser/rdorss.h"
#include "rdo_lib/rdo_runtime/rdo_resource.h"
#include "rdo_lib/rdo_runtime/rdocalc.h"
#include "rdo_lib/rdo_runtime/rdocalc_relres.h"
#include "rdo_common/smart_ptr/intrusive_ptr.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE
class RDOPattern;
CLOSE_RDO_RUNTIME_NAMESPACE

OPEN_RDO_PARSER_NAMESPACE

int  patparse(PTR(void) lexer);
int  patlex  (PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer);
void paterror(PTR(char) mes);

int  pat_preparse_parse(PTR(void) lexer);
int  pat_preparse_lex  (PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer);
void pat_preparse_error(PTR(char) mes);

class RDOPATChoiceFrom;
class RDOPATChoiceOrder;
class RDORelevantResource;

// ----------------------------------------------------------------------------
// ---------- ConvertCmdList
// ----------------------------------------------------------------------------
OBJECT(ConvertCmdList)
{
DECLARE_FACTORY(ConvertCmdList);
public:
	typedef std::vector<PTR(rdoRuntime::RDOCalc)> CalcList;

	void insertCommand(PTR(rdoRuntime::RDOCalc) pCalc)
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

// ----------------------------------------------------------------------------
// ---------- RDOPATPattern
// ----------------------------------------------------------------------------
class RDOPATPattern: public RDOParserObject, public RDOParserSrcInfo
{
friend class RDOOPROperation;
friend class RDODPTActivity;

public:
	enum PatType
	{
		PT_IE,
		PT_Event,
		PT_Rule,
		PT_Operation,
		PT_Keyboard
	};
	virtual PatType getType() const = 0;

	typedef std::vector<PTR(RDORelevantResource)> RelResList;

	rbool                       isHaveConvertEnd() const { return getType() == PT_Operation || getType() == PT_Keyboard; }
	PTR(rdoRuntime::RDOPattern) getPatRuntime   () const { return m_pPatRuntime; }

	static tstring StatusToStr(rdoRuntime::RDOResource::ConvertStatus value);
	rdoRuntime::RDOResource::ConvertStatus StrToStatus(CREF(tstring) value, CREF(YYLTYPE) convertor_pos);

	RelResList::const_iterator rel_res_begin () const { return m_relResList.begin(); }
	RelResList::const_iterator rel_res_end   () const { return m_relResList.end();   }
	int                        rel_res_count () const { return m_relResList.size();  }
	virtual void               rel_res_insert(PTR(RDORelevantResource) pRelevantResource);

	void beforeRelRensert(CREF(RDOParserSrcInfo) rel_info);
	
	PTR(RDORelevantResource) m_pCurrRelRes;

	void                      add                    (PTR(RDOFUNFunctionParam) const _param);
	CPTR(RDOFUNFunctionParam) findPATPatternParam    (CREF(tstring) paramName) const;
	int                       findPATPatternParamNum (CREF(tstring) paramName) const;
	CPTR(RDORelevantResource) findRelevantResource   (CREF(tstring) resName  ) const;
	int                       findRelevantResourceNum(CREF(tstring) resName  ) const;
	virtual void              addRelRes              (CREF(RDOParserSrcInfo) rel_info, CREF(RDOParserSrcInfo) type_info, rdoRuntime::RDOResource::ConvertStatus beg, CREF(YYLTYPE) convertor_pos) = 0;

	CREF(tstring) name() const { return src_text(); }

	void setCommonChoiceFirst  ();
	void setCommonChoiceWithMin(PTR(RDOFUNArithm) arithm);
	void setCommonChoiceWithMax(PTR(RDOFUNArithm) arithm);
	void setTime               (PTR(RDOFUNArithm) arithm);
	void addRelResBody         (CREF(RDOParserSrcInfo) body_name);
	virtual void addRelResUsage(PTR(RDOPATChoiceFrom) choice_from, PTR(RDOPATChoiceOrder) choice_order);
	void addRelResConvert      (rbool trace, CREF(LPConvertCmdList) commands, CREF(YYLTYPE) convertor_pos, CREF(YYLTYPE) trace_pos, rdoRuntime::RDOResource::ConvertStatus status);
	void end                   ();

	void         writeModelStructure    (REF(std::ostream) stream) const;
	virtual char getModelStructureLetter() const = 0;
	tstring      getPatternId           () const;

protected:
	RDOPATPattern(PTR(RDOParser) pParser, CREF(RDOParserSrcInfo) name_src_info);
	virtual ~RDOPATPattern()
	{}

	PTR(rdoRuntime::RDOPattern) m_pPatRuntime;

	rdoRuntime::LPRDOCalc createRelRes   (rbool trace) const;
	virtual void          addParamSetCalc(CREF(rdoRuntime::LPRDOCalc) pCalc);

	virtual tstring getErrorMessage_NotNeedConvertor(CREF(tstring) name, rdoRuntime::RDOResource::ConvertStatus status) = 0;
	virtual tstring getWarningMessage_EmptyConvertor(CREF(tstring) name, rdoRuntime::RDOResource::ConvertStatus status) = 0;

private:
	typedef std::vector<PTR(RDOFUNFunctionParam)> ParamList;

	ParamList         m_paramList;
	RelResList        m_relResList;

	rbool             m_useCommonChoice;
	rbool             m_useCommonWithMax;
	PTR(RDOFUNArithm) m_pCommonChoice;

	ruint             m_currentRelResIndex;

	tstring typeToString(PatType type)
	{
		switch (type)
		{
		case PT_IE       : return _T("нерегулярное событие");
		case PT_Event    : return _T("событие");
		case PT_Rule     : return _T("продукционное правило");
		case PT_Operation: return _T("операция");
		case PT_Keyboard : return _T("клавиатурная операция");
		default          : return _T("неизвестный");
		}
	}

	void addChoiceFromCalc(CREF(rdoRuntime::LPRDOCalc) pCalc);
};

// ----------------------------------------------------------------------------
// ---------- RDOPatternIrregEvent
// ----------------------------------------------------------------------------
class RDOPatternIrregEvent: public RDOPATPattern
{
public:
	RDOPatternIrregEvent(PTR(RDOParser) pParser, CREF(RDOParserSrcInfo) name_src_info, rbool trace);

	virtual void addRelRes     (CREF(RDOParserSrcInfo) rel_info, CREF(RDOParserSrcInfo) type_info, rdoRuntime::RDOResource::ConvertStatus beg, CREF(YYLTYPE) convertor_pos);
	virtual void addRelResUsage(PTR(RDOPATChoiceFrom) choice_from, PTR(RDOPATChoiceOrder) choice_order);

	virtual char getModelStructureLetter() const
	{
		return _T('I');
	};
	virtual PatType getType() const
	{
		return PT_IE;
	}

protected:
	virtual tstring getErrorMessage_NotNeedConvertor(CREF(tstring) name, rdoRuntime::RDOResource::ConvertStatus status);
	virtual tstring getWarningMessage_EmptyConvertor(CREF(tstring) name, rdoRuntime::RDOResource::ConvertStatus status);
};

// ----------------------------------------------------------------------------
// ---------- RDOPatternEvent
// ----------------------------------------------------------------------------
class RDOPatternEvent: public RDOPATPattern
{
public:
	RDOPatternEvent(PTR(RDOParser) pParser, CREF(RDOParserSrcInfo) name_src_info, rbool trace);

	virtual void addRelRes     (CREF(RDOParserSrcInfo) rel_info, CREF(RDOParserSrcInfo) type_info, rdoRuntime::RDOResource::ConvertStatus beg, CREF(YYLTYPE) convertor_pos);
	virtual void addRelResUsage(PTR(RDOPATChoiceFrom) choice_from, PTR(RDOPATChoiceOrder) choice_order);

	virtual char getModelStructureLetter() const
	{
		return _T('V');
	}
	virtual PatType getType() const
	{
		return PT_Event;
	}

protected:
	virtual tstring getErrorMessage_NotNeedConvertor(CREF(tstring) name, rdoRuntime::RDOResource::ConvertStatus status);
	virtual tstring getWarningMessage_EmptyConvertor(CREF(tstring) name, rdoRuntime::RDOResource::ConvertStatus status);
};

// ----------------------------------------------------------------------------
// ---------- RDOPatternRule
// ----------------------------------------------------------------------------
class RDOPatternRule: public RDOPATPattern
{
public:
	RDOPatternRule(PTR(RDOParser) pParser, CREF(RDOParserSrcInfo) name_src_info, rbool trace);

	virtual void addRelRes(CREF(RDOParserSrcInfo) rel_info, CREF(RDOParserSrcInfo) type_info, rdoRuntime::RDOResource::ConvertStatus beg, CREF(YYLTYPE) convertor_pos);

	virtual char getModelStructureLetter() const
	{
		return _T('R');
	};
	virtual PatType getType() const
	{
		return PT_Rule;
	}

protected:
	virtual tstring getErrorMessage_NotNeedConvertor(CREF(tstring) name, rdoRuntime::RDOResource::ConvertStatus status);
	virtual tstring getWarningMessage_EmptyConvertor(CREF(tstring) name, rdoRuntime::RDOResource::ConvertStatus status);
};

// ----------------------------------------------------------------------------
// ---------- RDOPatternOperation
// ----------------------------------------------------------------------------
class RDOPatternOperation: public RDOPATPattern
{
public:
	RDOPatternOperation(PTR(RDOParser) pParser, CREF(RDOParserSrcInfo) name_src_info, rbool trace);

	virtual void addRelRes               (CREF(RDOParserSrcInfo) rel_info, CREF(RDOParserSrcInfo) type_info, rdoRuntime::RDOResource::ConvertStatus beg, CREF(YYLTYPE) convertor_pos);
	        void addRelRes               (CREF(RDOParserSrcInfo) rel_info, CREF(RDOParserSrcInfo) type_info, rdoRuntime::RDOResource::ConvertStatus beg, rdoRuntime::RDOResource::ConvertStatus end, CREF(YYLTYPE) convertor_begin_pos, CREF(YYLTYPE) convertor_end_pos);
	        void addRelResConvertBeginEnd(rbool trace_begin, CREF(LPConvertCmdList) cmd_begin, rbool trace_end, CREF(LPConvertCmdList) cmd_end, CREF(YYLTYPE) convertor_begin_pos, CREF(YYLTYPE) convertor_end_pos, CREF(YYLTYPE) trace_begin_pos, CREF(YYLTYPE) trace_end_pos);

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
	RDOPatternOperation(PTR(RDOParser) pParser, rbool trace, CREF(RDOParserSrcInfo) name_src_info);

	virtual void rel_res_insert (PTR(RDORelevantResource)    pRelevantResource);
	virtual void addParamSetCalc(CREF(rdoRuntime::LPRDOCalc) pCalc            );

	virtual tstring getErrorMessage_NotNeedConvertor(CREF(tstring) name, rdoRuntime::RDOResource::ConvertStatus status);
	virtual tstring getWarningMessage_EmptyConvertor(CREF(tstring) name, rdoRuntime::RDOResource::ConvertStatus status);

private:
	enum ConvertorType
	{
		convert_unknow,
		convert_begin,
		convert_end
	};
	ConvertorType m_convertorType;
};

// ----------------------------------------------------------------------------
// ---------- RDOPatternKeyboard
// ----------------------------------------------------------------------------
class RDOPatternKeyboard: public RDOPatternOperation
{
public:
	RDOPatternKeyboard(PTR(RDOParser) pParser, CREF(RDOParserSrcInfo) name_src_info, rbool trace);

	virtual char getModelStructureLetter() const
	{
		return _T('K');
	};
	virtual PatType getType() const
	{
		return PT_Keyboard;
	}
};

// ----------------------------------------------------------------------------
// ---------- RDORelevantResource
// ----------------------------------------------------------------------------
class RDORelevantResource: public RDOParserObject, public RDOParserSrcInfo
{
public:
	int                    m_relResID;
	rbool                  m_alreadyHaveConverter;
	PTR(RDOPATChoiceFrom)  m_pChoiceFrom;
	PTR(RDOPATChoiceOrder) m_pChoiceOrder;
	RDOParserSrcInfo       m_bodySrcInfo;

	const rdoRuntime::RDOResource::ConvertStatus m_statusBegin;
	const rdoRuntime::RDOResource::ConvertStatus m_statusEnd;

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

	RDORelevantResource(CPTR(RDOPATPattern) parent, CREF(RDOParserSrcInfo) src_info, const int relResID, const rdoRuntime::RDOResource::ConvertStatus statusBegin, const rdoRuntime::RDOResource::ConvertStatus statusEnd)
		: RDOParserObject       (parent    )
		, RDOParserSrcInfo      (src_info  )
		, m_relResID            (relResID   )
		, m_statusBegin         (statusBegin)
		, m_statusEnd           (statusEnd  )
		, m_alreadyHaveConverter(false      )
		, m_pChoiceFrom         (NULL       )
		, m_pChoiceOrder        (NULL       )
		, m_currentState        (stateNone  )
	{}

	        CREF(tstring)   name   () const  { return src_text(); };
	virtual LPRDORTPResType getType() const = 0;

	virtual rdoRuntime::LPRDOCalc                  createPreSelectRelResCalc           () = 0; //! Предварительный выбор ресурсов в самом списке рел. ресурсов
	virtual rdoRuntime::LPRDOCalc                  createSelectResourceChoiceCalc      () = 0; //! Самый обыкновенный choice from + first/with_min/with_max
	virtual rdoRuntime::LPRDOCalc                  createSelectFirstResourceChoiceCalc () = 0; //! common first, который не пашет
	virtual rdoRuntime::LPIRDOSelectResourceCommon createSelectResourceCommonChoiceCalc() = 0; //! common with_min/with_max

	virtual rbool isDirect() const                                                        = 0;

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
			STL_FOR_ALL_CONST(List, m_list, paramIt)
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
	REF(ParamSetList) getParamSetList()
	{
		return m_paramSetList;
	}

protected:
	rdoRuntime::LPRDOCalc                   getChoiceCalc() const;
	rdoRuntime::LPRDOCalc                   getSelectCalc() const;
	rdoRuntime::RDOSelectResourceCalc::Type getSelectType() const;

private:
	ParamSetList m_paramSetList;
};

// ----------------------------------------------------------------------------
// ---------- RDOPATChoiceFrom
// ----------------------------------------------------------------------------
class RDOPATChoiceFrom: public RDOParserObject, public RDOParserSrcInfo
{
public:
	enum Type
	{
		ch_empty = 0,
		ch_nocheck,
		ch_from
	} m_type;

	PTR(RDOFUNLogic) m_pLogic;

	RDOPATChoiceFrom(CPTR(RDORelevantResource) pRelevantResource, CREF(RDOParserSrcInfo) src_info, Type type, PTR(RDOFUNLogic) pLogic = NULL)
		: RDOParserObject (pRelevantResource)
		, RDOParserSrcInfo(src_info         )
		, m_type          (type             )
		, m_pLogic        (pLogic           )
	{}
};

// ----------------------------------------------------------------------------
// ---------- RDOPATChoiceOrder
// ----------------------------------------------------------------------------
class RDOPATChoiceOrder: public RDOParserObject, public RDOParserSrcInfo
{
public:
	rdoRuntime::RDOSelectResourceCalc::Type m_type;

	PTR(RDOFUNArithm) m_pArithm;

	RDOPATChoiceOrder(CPTR(RDORelevantResource) pRelevantResource, CREF(RDOParserSrcInfo) src_info, rdoRuntime::RDOSelectResourceCalc::Type type, PTR(RDOFUNArithm) pArithm = NULL)
		: RDOParserObject (pRelevantResource)
		, RDOParserSrcInfo(src_info         )
		, m_type          (type             )
		, m_pArithm       (pArithm          )
	{}

	tstring asString() const
	{
		switch (m_type)
		{
		case rdoRuntime::RDOSelectResourceCalc::order_empty   : return _T("<правило_выбора_не_указано>");
		case rdoRuntime::RDOSelectResourceCalc::order_first   : return _T("first");
		case rdoRuntime::RDOSelectResourceCalc::order_with_min: return _T("with_min");
		case rdoRuntime::RDOSelectResourceCalc::order_with_max: return _T("with_max");
		default                                               : NEVER_REACH_HERE;
		}
		return tstring();
	}
};

// ----------------------------------------------------------------------------
// ---------- RDORelevantResourceDirect - по имени ресурса
// ----------------------------------------------------------------------------
class RDORelevantResourceDirect: public RDORelevantResource
{
public:
	RDORelevantResourceDirect(CPTR(RDOPATPattern) parent, CREF(RDOParserSrcInfo) src_info, const int relResID, CPTRC(RDORSSResource) pResource, const rdoRuntime::RDOResource::ConvertStatus statusBegin, const rdoRuntime::RDOResource::ConvertStatus statusEnd = rdoRuntime::RDOResource::CS_NoChange)
		: RDORelevantResource(parent, src_info, relResID, statusBegin, statusEnd)
		, m_pResource        (pResource)
	{}

	CPTRC(RDORSSResource) getResource() const { return m_pResource; }

	virtual LPRDORTPResType                        getType                             () const;
	virtual rdoRuntime::LPRDOCalc                  createPreSelectRelResCalc           ();
	virtual rdoRuntime::LPRDOCalc                  createSelectFirstResourceChoiceCalc ();
	virtual rdoRuntime::LPRDOCalc                  createSelectResourceChoiceCalc      ();
	virtual rdoRuntime::LPIRDOSelectResourceCommon createSelectResourceCommonChoiceCalc();
	virtual rbool isDirect() const  { return true; }

private:
	CPTRC(RDORSSResource) m_pResource;
};

// ----------------------------------------------------------------------------
// ---------- RDORelevantResourceByType - по имени типа
// ----------------------------------------------------------------------------
class RDORelevantResourceByType: public RDORelevantResource
{
public:
	RDORelevantResourceByType(CPTR(RDOPATPattern) parent, CREF(RDOParserSrcInfo) src_info, const int relResID, LPRDORTPResType pResType, const rdoRuntime::RDOResource::ConvertStatus statusBegin, const rdoRuntime::RDOResource::ConvertStatus statusEnd = rdoRuntime::RDOResource::CS_NoChange)
		: RDORelevantResource(parent, src_info, relResID, statusBegin, statusEnd)
		, m_pResType         (pResType)
	{}

	virtual LPRDORTPResType getType() const
	{
		return m_pResType;
	}
	virtual rdoRuntime::LPRDOCalc                  createPreSelectRelResCalc           ();
	virtual rdoRuntime::LPRDOCalc                  createSelectFirstResourceChoiceCalc ();
	virtual rdoRuntime::LPRDOCalc                  createSelectResourceChoiceCalc      ();
	virtual rdoRuntime::LPIRDOSelectResourceCommon createSelectResourceCommonChoiceCalc();
	virtual rbool isDirect() const  { return false; }

private:
	LPRDORTPResType m_pResType;
};

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _RDOPAT_H_
