#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include <boost/noncopyable.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/rdortp_param.h"
#include "converter/smr2rdox/rdofun.h"
#include "converter/smr2rdox/rdorss.h"
#include "converter/smr2rdox/param.h"

#include "simulator/runtime/rdo_pattern.h"
#include "simulator/runtime/rdo_resource.h"
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/calc/resource/calc_relevant.h"
#include "simulator/runtime/calc/resource/calc_choice_from.h"

#include "utils/src/smart_ptr/factory/factory.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

int cnv_patparse(void* lexer);
int cnv_patlex(YYSTYPE* lpval, YYLTYPE* llocp, void* lexer);
void cnv_paterror(YYLTYPE* llocp, void* lexer, const char* message);

// --------------------------------------------------------------------------------
// -------------------- ConvertCmdList
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(ConvertCmdList);
class ConvertCmdList: public rdo::counter_reference
{
DECLARE_FACTORY(ConvertCmdList)
public:
	typedef std::vector<rdo::runtime::LPRDOCalc> CalcList;

	void insertCommand(const rdo::runtime::LPRDOCalc& pCalc)
	{
		m_calcList.push_back(pCalc);
	}
	const CalcList& commands() const
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

PREDECLARE_POINTER(RDOPATPattern);
class RDOPATPattern
	: public rdo::counter_reference
	, public RDOParserSrcInfo
{
DECLARE_FACTORY(RDOPATPattern)
friend class RDOOPROperation;
friend class RDODPTActivity;

public:
	enum PatType
	{
		PT_IE,
		PT_Rule,
		PT_Operation,
		PT_Keyboard
	};
	virtual PatType getType() const = 0;

	typedef std::vector<LPRDORelevantResource> RelResList;

	bool isHaveConvertEnd() const { return getType() == PT_Operation || getType() == PT_Keyboard; }

	const rdo::runtime::LPRDOPattern& getPatRuntime() const { return m_pPatRuntime; }
	template<class T>
	rdo::intrusive_ptr<T> getPatRuntime() const
	{
		rdo::intrusive_ptr<T> pPatRuntime = m_pPatRuntime.object_static_cast<T>();
		ASSERT(pPatRuntime);
		return pPatRuntime;
	}

	static std::string StatusToStr(rdo::runtime::RDOResource::ConvertStatus value);
	rdo::runtime::RDOResource::ConvertStatus StrToStatus(const std::string& value, const YYLTYPE& convertor_pos);

	RelResList::const_iterator rel_res_begin () const { return m_relResList.begin(); }
	RelResList::const_iterator rel_res_end   () const { return m_relResList.end();   }
	int                        rel_res_count () const { return m_relResList.size();  }
	virtual void               rel_res_insert(const LPRDORelevantResource& pRelevantResource);

	void beforeRelRensert(const RDOParserSrcInfo& rel_info);

	LPRDORelevantResource m_pCurrRelRes;

	LPRDOFUNArithm        time;

	void add(const LPRDOParam& pParam);
	LPRDOParam findPATPatternParam(const std::string& paramName) const;
	int findPATPatternParamNum(const std::string& paramName) const;
	LPRDORelevantResource findRelevantResource(const std::string& resName) const;
	std::size_t findRelevantResourceNum(const std::string& resName) const;
	virtual void addRelRes (const RDOParserSrcInfo& rel_info, const RDOParserSrcInfo& type_info, rdo::runtime::RDOResource::ConvertStatus beg, const YYLTYPE& convertor_pos) = 0;

	const std::string& name() const { return src_text(); }

	typedef std::vector<LPRDOParam> ParamList;
	const ParamList& getParamList() const;

	void setCommonChoiceFirst();
	void setCommonChoiceWithMin(const LPRDOFUNArithm& arithm);
	void setCommonChoiceWithMax(const LPRDOFUNArithm& arithm);
	void setTime (LPRDOFUNArithm& arithm);
	void addRelResBody (const RDOParserSrcInfo& body_name);
	virtual void addRelResUsage(const LPRDOPATChoiceFrom& pChoiceFrom, const LPRDOPATChoiceOrder& pChoiceOrder);
	void addRelResConvert(bool trace, const LPConvertCmdList& commands, const YYLTYPE& convertor_pos, const YYLTYPE& trace_pos, rdo::runtime::RDOResource::ConvertStatus status);
	void end ();

	void writeModelStructure(std::ostream& stream) const;
	virtual char getModelStructureLetter() const = 0;
	std::string getPatternId() const;

protected:
	RDOPATPattern(const RDOParserSrcInfo& name_src_info);
	virtual ~RDOPATPattern()
	{}

	rdo::runtime::LPRDOPattern m_pPatRuntime;

	rdo::runtime::LPRDOCalc createRelRes (bool trace) const;
	virtual void addParamSetCalc(const rdo::runtime::LPRDOCalc& pCalc);

	virtual std::string getErrorMessage_NotNeedConvertor(const std::string& name, rdo::runtime::RDOResource::ConvertStatus status) = 0;
	virtual std::string getWarningMessage_EmptyConvertor(const std::string& name, rdo::runtime::RDOResource::ConvertStatus status) = 0;

private:
	ParamList m_paramList;
	RelResList m_relResList;
	bool m_useCommonChoice;
	bool m_useCommonWithMax;
	LPRDOFUNArithm m_pCommonChoice;
	std::size_t m_currentRelResIndex;

	std::string typeToString(PatType type)
	{
		switch (type)
		{
		case PT_IE       : return "нерегулярное событие";
		case PT_Rule     : return "продукционное правило";
		case PT_Operation: return "операция";
		case PT_Keyboard : return "клавиатурная операция";
		default          : return "неизвестный";
		}
	}

	void addChoiceFromCalc(const rdo::runtime::LPRDOCalc& pCalc);
};

// --------------------------------------------------------------------------------
// -------------------- RDOPatternIrregEvent
// --------------------------------------------------------------------------------
class RDOPatternIrregEvent: public RDOPATPattern
{
DECLARE_FACTORY(RDOPatternIrregEvent)
public:
	virtual void addRelRes     (const RDOParserSrcInfo& rel_info, const RDOParserSrcInfo& type_info, rdo::runtime::RDOResource::ConvertStatus beg, const YYLTYPE& convertor_pos);
	virtual void addRelResUsage(const LPRDOPATChoiceFrom& pChoiceFrom, const LPRDOPATChoiceOrder& pChoiceOrder);

	virtual char getModelStructureLetter() const
	{
		return 'I';
	};
	virtual PatType getType() const
	{
		return PT_IE;
	}

protected:
	virtual std::string getErrorMessage_NotNeedConvertor(const std::string& name, rdo::runtime::RDOResource::ConvertStatus status);
	virtual std::string getWarningMessage_EmptyConvertor(const std::string& name, rdo::runtime::RDOResource::ConvertStatus status);

private:
	RDOPatternIrregEvent(const RDOParserSrcInfo& name_src_info, bool trace);
};

// --------------------------------------------------------------------------------
// -------------------- RDOPatternRule
// --------------------------------------------------------------------------------
class RDOPatternRule: public RDOPATPattern
{
DECLARE_FACTORY(RDOPatternRule)
public:
	virtual void addRelRes(const RDOParserSrcInfo& rel_info, const RDOParserSrcInfo& type_info, rdo::runtime::RDOResource::ConvertStatus beg, const YYLTYPE& convertor_pos);

	virtual char getModelStructureLetter() const
	{
		return 'R';
	};
	virtual PatType getType() const
	{
		return PT_Rule;
	}

protected:
	virtual std::string getErrorMessage_NotNeedConvertor(const std::string& name, rdo::runtime::RDOResource::ConvertStatus status);
	virtual std::string getWarningMessage_EmptyConvertor(const std::string& name, rdo::runtime::RDOResource::ConvertStatus status);

private:
	RDOPatternRule(const RDOParserSrcInfo& name_src_info, bool trace);
};

// --------------------------------------------------------------------------------
// -------------------- RDOPatternOperation
// --------------------------------------------------------------------------------
class RDOPatternOperation: public RDOPATPattern
{
DECLARE_FACTORY(RDOPatternOperation)
public:
	virtual void addRelRes(const RDOParserSrcInfo& rel_info, const RDOParserSrcInfo& type_info, rdo::runtime::RDOResource::ConvertStatus beg, const YYLTYPE& convertor_pos);
	void addRelRes(const RDOParserSrcInfo& rel_info, const RDOParserSrcInfo& type_info, rdo::runtime::RDOResource::ConvertStatus beg, rdo::runtime::RDOResource::ConvertStatus end, const YYLTYPE& convertor_begin_pos, const YYLTYPE& convertor_end_pos);
	void addRelResConvertBeginEnd(bool trace_begin, const LPConvertCmdList& cmd_begin, bool trace_end, const LPConvertCmdList& cmd_end, const YYLTYPE& convertor_begin_pos, const YYLTYPE& convertor_end_pos, const YYLTYPE& trace_begin_pos, const YYLTYPE& trace_end_pos);

	virtual char getModelStructureLetter() const
	{
		return 'A';
	};
	virtual PatType getType() const
	{
		return PT_Operation;
	}

protected:
	// Конструктор вызывается из RDOPatternKeyboard
	RDOPatternOperation(bool trace, const RDOParserSrcInfo& name_src_info);

	virtual void rel_res_insert (const LPRDORelevantResource& pRelevantResource);
	virtual void addParamSetCalc(const rdo::runtime::LPRDOCalc& pCalc            );

	virtual std::string getErrorMessage_NotNeedConvertor(const std::string& name, rdo::runtime::RDOResource::ConvertStatus status);
	virtual std::string getWarningMessage_EmptyConvertor(const std::string& name, rdo::runtime::RDOResource::ConvertStatus status);

private:
	RDOPatternOperation(const RDOParserSrcInfo& name_src_info, bool trace);

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
		return 'K';
	};
	virtual PatType getType() const
	{
		return PT_Keyboard;
	}

private:
	RDOPatternKeyboard(const RDOParserSrcInfo& name_src_info, bool trace);
};

// --------------------------------------------------------------------------------
// -------------------- RDORelevantResource
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RDORelevantResource);
class RDORelevantResource
	: public rdo::counter_reference
	, public RDOParserSrcInfo
	, public boost::noncopyable
{
DECLARE_FACTORY(RDORelevantResource)
public:
	int m_relResID;
	bool m_alreadyHaveConverter;
	LPRDOPATChoiceFrom m_pChoiceFrom;
	LPRDOPATChoiceOrder m_pChoiceOrder;
	RDOParserSrcInfo m_bodySrcInfo;

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
	bool isChoiceFromState() const { return m_currentState == choiceEmpty || m_currentState == choiceNoCheck || m_currentState == choiceFrom; }

	const std::string& name() const { return src_text(); };
	virtual LPRDORTPResType getType() const = 0;

	virtual rdo::runtime::LPRDOCalc                  createPreSelectRelResCalc           () = 0; // Предварительный выбор ресурсов в самом списке рел. ресурсов
	virtual rdo::runtime::LPRDOCalc                  createSelectResourceChoiceCalc      () = 0; // Самый обыкновенный choice from + first/with_min/with_max
	virtual rdo::runtime::LPRDOCalc                  createSelectFirstResourceChoiceCalc () = 0; // common first, который не пашет
	virtual rdo::runtime::LPIRDOSelectResourceCommon createSelectResourceCommonChoiceCalc() = 0; // common with_min/with_max

	virtual bool isDirect() const = 0;

	class ParamSetList
	{
	public:
		void reset()
		{
			m_list.clear();
		}
		void insert(const LPRDORTPParam& param)
		{
			m_list.push_back(param);
		}
		bool find(const std::string& name) const
		{
			for (const auto& param: m_list)
			{
				if (param->name() == name)
					return true;
			}
			return false;
		}
		bool empty() const
		{
			return m_list.empty();
		}
	private:
		typedef std::vector<LPRDORTPParam> List;

		List m_list;
	};
	const ParamSetList& getParamSetList() const
	{
		return m_paramSetList;
	}
	ParamSetList& getParamSetList()
	{
		return m_paramSetList;
	}

protected:
	RDORelevantResource(const RDOParserSrcInfo& src_info, const int relResID, const rdo::runtime::RDOResource::ConvertStatus statusBegin, const rdo::runtime::RDOResource::ConvertStatus statusEnd)
		: RDOParserSrcInfo      (src_info   )
		, m_relResID            (relResID   )
		, m_alreadyHaveConverter(false      )
		, m_statusBegin         (statusBegin)
		, m_statusEnd           (statusEnd  )
		, m_currentState        (stateNone  )
	{}

	rdo::runtime::LPRDOCalc                   getChoiceCalc();
	rdo::runtime::LPRDOCalc                   getSelectCalc();
	rdo::runtime::RDOSelectResourceCalc::Type getSelectType() const;

private:
	ParamSetList m_paramSetList;
};

// --------------------------------------------------------------------------------
// -------------------- RDOPATChoiceFrom
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RDOPATChoiceFrom);
class RDOPATChoiceFrom
	: public rdo::counter_reference
	, public RDOParserSrcInfo
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
	RDOPATChoiceFrom(const RDOParserSrcInfo& src_info, Type type, const LPRDOFUNLogic& pLogic = NULL)
		: RDOParserSrcInfo(src_info)
		, m_type          (type    )
		, m_pLogic        (pLogic  )
	{}
};

// --------------------------------------------------------------------------------
// -------------------- RDOPATChoiceOrder
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RDOPATChoiceOrder);
class RDOPATChoiceOrder
	: public rdo::counter_reference
	, public RDOParserSrcInfo
{
DECLARE_FACTORY(RDOPATChoiceOrder)
public:
	rdo::runtime::RDOSelectResourceCalc::Type m_type;

	LPRDOFUNArithm m_pArithm;

	std::string asString() const
	{
		switch (m_type)
		{
		case rdo::runtime::RDOSelectResourceCalc::order_empty   : return "<правило_выбора_не_указано>";
		case rdo::runtime::RDOSelectResourceCalc::order_first   : return "first";
		case rdo::runtime::RDOSelectResourceCalc::order_with_min: return "with_min";
		case rdo::runtime::RDOSelectResourceCalc::order_with_max: return "with_max";
		default                                               : NEVER_REACH_HERE;
		}
		return std::string();
	}

private:
	RDOPATChoiceOrder(const RDOParserSrcInfo& src_info, rdo::runtime::RDOSelectResourceCalc::Type type, const LPRDOFUNArithm& pArithm = NULL)
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
	virtual bool isDirect() const { return true; }

private:
	RDORelevantResourceDirect(const RDOParserSrcInfo& src_info, const int relResID, const LPRDORSSResource& pResource, const rdo::runtime::RDOResource::ConvertStatus statusBegin, const rdo::runtime::RDOResource::ConvertStatus statusEnd = rdo::runtime::RDOResource::CS_NoChange)
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
	virtual bool isDirect() const { return false; }

private:
	RDORelevantResourceByType(const RDOParserSrcInfo& src_info, const int relResID, LPRDORTPResType pResType, const rdo::runtime::RDOResource::ConvertStatus statusBegin, const rdo::runtime::RDOResource::ConvertStatus statusEnd = rdo::runtime::RDOResource::CS_NoChange)
		: RDORelevantResource(src_info, relResID, statusBegin, statusEnd)
		, m_pResType         (pResType)
	{}

	LPRDORTPResType m_pResType;
};

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE
