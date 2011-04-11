/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdoparser.cpp
 * author   : Александ Барс, Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_parser/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdoparser.h"
#include "rdo_lib/rdo_parser/rdoparser_rdo.h"
#include "rdo_lib/rdo_parser/rdofun.h"
#include "rdo_lib/rdo_parser/rdorss.h"
#include "rdo_common/rdocommon.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOParser
// ----------------------------------------------------------------------------
RDOParser::ParserList RDOParser::s_parserStack;

#define DECLARE_PARSER_OBJECT_CONTAINER_NONAME(NAME) \
void RDOParser::insert##NAME(LPRDO##NAME value) \
{ \
	howIsIt<LPRDO##NAME>(); \
	m_all##NAME.push_back(value); \
}

#define DECLARE_PARSER_OBJECT_CONTAINER(NAME) \
DECLARE_PARSER_OBJECT_CONTAINER_NONAME(NAME) \
const LPRDO##NAME RDOParser::find##NAME(CREF(tstring) name) const \
{ \
	NAME##List::const_iterator it = std::find_if(m_all##NAME.begin(), m_all##NAME.end(), compareName<RDO##NAME>(name)); \
	return it != m_all##NAME.end() ? *it : NULL; \
} \
rbool RDOParser::remove##NAME(const LPRDO##NAME item) \
{ \
	NAME##List::iterator it = std::find(m_all##NAME.begin(), m_all##NAME.end(), item); \
	if (it == m_all##NAME.end()) \
		return false; \
	m_all##NAME.erase(it); \
	return true; \
}

DECLARE_PARSER_OBJECT_CONTAINER(PATPattern     );
DECLARE_PARSER_OBJECT_CONTAINER(RTPResType     );
DECLARE_PARSER_OBJECT_CONTAINER(RSSResource    );
DECLARE_PARSER_OBJECT_CONTAINER(FRMFrame       );
DECLARE_PARSER_OBJECT_CONTAINER(FUNConstant    );
DECLARE_PARSER_OBJECT_CONTAINER(FUNFunction    );
DECLARE_PARSER_OBJECT_CONTAINER(FUNSequence    );
DECLARE_PARSER_OBJECT_CONTAINER(DPTSearch      );
DECLARE_PARSER_OBJECT_CONTAINER(DPTSome        );
DECLARE_PARSER_OBJECT_CONTAINER(DPTPrior       );
DECLARE_PARSER_OBJECT_CONTAINER(DPTFreeActivity);
DECLARE_PARSER_OBJECT_CONTAINER(Event          );
DECLARE_PARSER_OBJECT_CONTAINER(ResultGroup    );

DECLARE_PARSER_OBJECT_CONTAINER_NONAME(FUNGroup   );
DECLARE_PARSER_OBJECT_CONTAINER_NONAME(DPTFree    );
DECLARE_PARSER_OBJECT_CONTAINER_NONAME(PROCProcess);

rdoModelObjects::RDOFileType RDOParser::getFileToParse()
{
	return !s_parserStack.empty() && s_parserStack.back()->m_parser_item ? s_parserStack.back()->m_parser_item->m_type : rdoModelObjects::PAT;
}

ruint RDOParser::lexer_loc_line()
{
	return !s_parserStack.empty() && s_parserStack.back()->m_parser_item ? s_parserStack.back()->m_parser_item->lexer_loc_line() : ~0;
}

ruint RDOParser::lexer_loc_pos()
{
	return !s_parserStack.empty() && s_parserStack.back()->m_parser_item ? s_parserStack.back()->m_parser_item->lexer_loc_pos() : 0;
}

LPRDOParser RDOParser::s_parser()
{
	return !s_parserStack.empty() ? s_parserStack.back() : NULL;
}

RDOParser::RDOParser()
	: m_parser_item         (NULL )
	, m_have_kw_Resources   (false)
	, m_have_kw_ResourcesEnd(false)
	, m_pattern             (false)
{}

RDOParser::~RDOParser()
{}

void RDOParser::init()
{
	s_parserStack.push_back(this);
	m_runtime.memory_insert(sizeof(RDOParser));
	m_runtime.init();

	m_pContextStack = rdo::Factory<ContextStack>::create();
	ASSERT(m_pContextStack);

	m_pContextStack->push(this);

	LPRDOSMR pSMR = rdo::Factory<RDOSMR>::create();
	ASSERT(pSMR);
	setSMR(pSMR);

	m_resultGeneratorID.get(); //! Для PMD нумерация с 1
}

void RDOParser::deinit()
{
	m_pContextStack->pop();

	m_runtime.deinit();
	rdo::deleteAllObjects(m_allValues);
	m_movementObjectList.clear();
	s_parserStack.remove(this);
}

LPContextStack RDOParser::contextStack()
{
	return m_pContextStack;
}

LPContext RDOParser::context() const
{
	return m_pContextStack->top();
}

LPContext RDOParser::onFindContext(CREF(RDOValue) value) const
{
	if (value->getIdentificator() == _T("Time_now")          || value->getIdentificator() == _T("time_now") || value->getIdentificator() == _T("Системное_время") || value->getIdentificator() == _T("системное_время") ||
		value->getIdentificator() == _T("Seconds")           || value->getIdentificator() == _T("seconds") ||
		value->getIdentificator() == _T("Terminate_counter") || value->getIdentificator() == _T("terminate_counter"))
	{
		return const_cast<PTR(RDOParser)>(this);
	}

	//! Ресурсы
	LPRDORSSResource pResource = findRSSResource(value->getIdentificator());
	if (pResource)
	{
		//! Это ресурс с закладки RSS
		return pResource.object_static_cast<Context>();
	}

	//! Константы
	LPRDOFUNConstant pConstant = findFUNConstant(value->getIdentificator());
	if (pConstant)
	{
		return const_cast<PTR(RDOParser)>(this);
	}

	//! Последовательности
	LPRDOFUNSequence pSequence = findFUNSequence(value->getIdentificator());
	if (pSequence)
	{
		return const_cast<PTR(RDOParser)>(this);
	}

	//! Возможно, что это значение перечислимого типа, только одно и тоже значение может встречаться в разных
	//! перечислимых типах, поэтому какой именно из них выбрать - вопрос
	{ErrorBlockMonicker errorBlockMonicker;
		CREF(PreCastTypeList) typeList = getPreCastTypeList();
		STL_FOR_ALL_CONST(typeList, it)
		{
			RDOValue try_cast_value = (*it)->value_cast(value);
			if (try_cast_value.defined())
			{
				return const_cast<PTR(RDOParser)>(this);
			}
		}
	}

	const_cast<PTR(RDOParser)>(this)->error().error(value.src_info(), rdo::format(_T("Неизвестный идентификатор: %s"), value->getIdentificator().c_str()));
	return NULL;
}

LPExpression RDOParser::onCreateExpression(CREF(RDOValue) value)
{
	if (value->getIdentificator() == _T("Time_now") || value->getIdentificator() == _T("time_now") || value->getIdentificator() == _T("Системное_время") || value->getIdentificator() == _T("системное_время"))
	{
		LPExpression pExpression = rdo::Factory<Expression>::create(
			rdo::Factory<TypeInfo>::create(
				rdo::Factory<RDOType__real>::create(),
				value.src_info()
			),
			rdo::Factory<rdoRuntime::RDOCalcGetTimeNow>::create(),
			value.src_info()
		);
		ASSERT(pExpression);
		return pExpression;
	}
	else if (value->getIdentificator() == _T("Seconds") || value->getIdentificator() == _T("seconds"))
	{
		LPExpression pExpression = rdo::Factory<Expression>::create(
			rdo::Factory<TypeInfo>::create(
				rdo::Factory<RDOType__real>::create(),
				value.src_info()
			),
			rdo::Factory<rdoRuntime::RDOCalcGetSeconds>::create(),
			value.src_info()
		);
		ASSERT(pExpression);
		return pExpression;
	}
	else if (value->getIdentificator() == _T("Terminate_counter") || value->getIdentificator() == _T("terminate_counter"))
	{
		LPExpression pExpression = rdo::Factory<Expression>::create(
			rdo::Factory<TypeInfo>::create(
				rdo::Factory<RDOType__int>::create(),
				value.src_info()
			),
			rdo::Factory<rdoRuntime::RDOCalcGetTermNow>::create(),
			value.src_info()
		);
		ASSERT(pExpression);
		return pExpression;
	}

	//! Константы
	LPRDOFUNConstant pConstant = findFUNConstant(value->getIdentificator());
	if (pConstant)
	{
		LPExpression pExpression = rdo::Factory<Expression>::create(
			pConstant->getTypeInfo(),
			rdo::Factory<rdoRuntime::RDOCalcGetConst>::create(pConstant->getNumber()),
			value.src_info()
		);
		ASSERT(pExpression);
		return pExpression;
	}

	//! Последовательности
	LPRDOFUNSequence pSequence = findFUNSequence(value->getIdentificator());
	if (pSequence)
	{
		LPRDOFUNParams pParams = rdo::Factory<RDOFUNParams>::create();
		ASSERT(pParams);
		LPRDOFUNArithm pArithm = pParams->createSeqCall(value->getIdentificator());
		ASSERT(pArithm);
		pArithm->setSrcInfo(value.src_info());
		LPExpression pExpression = rdo::Factory<Expression>::create(
			pArithm->typeInfo(),
			pArithm->calc(),
			value.src_info()
		);
		ASSERT(pExpression);
		return pExpression;
	}

	//! Возможно, что это значение перечислимого типа, только одно и тоже значение может встречаться в разных
	//! перечислимых типах, поэтому какой именно из них выбрать - вопрос
	{ErrorBlockMonicker errorBlockMonicker;
		CREF(PreCastTypeList) typeList = getPreCastTypeList();
		STL_FOR_ALL_CONST(typeList, it)
		{
			RDOValue try_cast_value = (*it)->value_cast(value);
			if (try_cast_value.defined())
			{
				LPExpression pExpression = rdo::Factory<Expression>::create(
					rdo::Factory<TypeInfo>::create(
						rdo::Factory<RDOType__identificator>::create(),
						value.src_info()
					),
					rdo::Factory<rdoRuntime::RDOCalcConst>::create(value.value()),
					value.src_info()
				);
				ASSERT(pExpression);
				return pExpression;
			}
		}
	}

	return NULL;
}

rbool RDOParser::isCurrentDPTSearch()
{
	return getLastDPTSearch() && !getLastDPTSearch()->closed() ? true : false;
}

rbool RDOParser::isCurrentDPTPrior()
{
	return getLastDPTPrior() ? true : false;
}

void RDOParser::insertChanges(CREF(tstring) name, CREF(tstring) value)
{
	m_changes.push_back(Changes(name, value));
}

tstring RDOParser::getChanges() const
{
	rdo::textstream stream;
	stream << _T("$Changes") << std::endl;
	ruint changes_max_length = 0;
	STL_FOR_ALL_CONST(m_changes, change_it)
	{
		if (change_it->m_name.length() > changes_max_length)
		{
			changes_max_length = change_it->m_name.length();
		}
	}
	STL_FOR_ALL_CONST(m_changes, change_it)
	{
		stream << _T("  ") << change_it->m_name;
		for (ruint i = change_it->m_name.length(); i < changes_max_length; i++)
		{
			stream << _T(" ");
		}
		stream << _T("  = ") << change_it->m_value << std::endl;
	}
	return stream.str();
}

tstring RDOParser::getModelStructure()
{
	rdo::textstream modelStructure;

	// $Changes
	modelStructure << getChanges();

	// RTP
	modelStructure << std::endl << std::endl << _T("$Resource_type") << std::endl;
	STL_FOR_ALL_CONST(m_allRTPResType, rtp_it)
	{
		(*rtp_it)->writeModelStructure(modelStructure);
	}

	// RSS
	modelStructure << std::endl << _T("$Resources") << std::endl;
	STL_FOR_ALL_CONST(m_allRSSResource, rss_it)
	{
		(*rss_it)->writeModelStructure(modelStructure);
	}

	// PAT
	modelStructure << std::endl << _T("$Pattern") << std::endl;
	STL_FOR_ALL_CONST(m_allPATPattern, pat_it)
	{
		(*pat_it)->writeModelStructure(modelStructure);
	}

	// OPR/DPT
	ruint counter = 1;
	modelStructure << std::endl << _T("$Activities") << std::endl;
	modelStructure << m_runtime.writeActivitiesStructure(counter);

	// DPT only
	for (ruint i = 0; i < m_allDPTSearch.size(); i++)
	{
		for (ruint j = 0; j < m_allDPTSearch.at(i)->getActivities().size(); j++)
		{
			LPRDODPTSearchActivity pSearchActivity = m_allDPTSearch.at(i)->getActivities().at(j);
			ASSERT(pSearchActivity);
			modelStructure << counter++ << _T(" ") << pSearchActivity->name() << _T(" ") << pSearchActivity->pattern()->getPatternId() << std::endl;
		}
	}

	// PMD
	modelStructure << std::endl << _T("$Watching") << std::endl;
	ruint watching_max_length = 0;
	STL_FOR_ALL_CONST(m_runtime.getPokaz(), watching_it)
	{
		LPITrace          trace     = *watching_it;
		LPIName           name      = trace;
		LPIModelStructure structure = trace;
		if (trace && name && structure)
		{
			if (trace->traceable() && name->name().length() > watching_max_length)
			{
				watching_max_length = name->name().length();
			}
		}
	}
	STL_FOR_ALL_CONST(m_runtime.getPokaz(), watching_it)
	{
		LPITrace          trace     = *watching_it;
		LPIName           name      = trace;
		LPIModelStructure structure = trace;
		if (trace && name && structure)
		{
			if (trace->traceable())
			{
				modelStructure << _T("  ") << name->name();
				for (ruint i = name->name().length(); i < watching_max_length + 2; i++)
					modelStructure << _T(" ");

				structure->writeModelStructure(modelStructure);
			}
		}
	}

	return modelStructure.str();
}

void RDOParser::parse()
{
	parse(rdoModelObjects::obPRE);

	RDOParserContainer::Iterator it = begin();
	while (it != end())
	{
		m_parser_item = it->second;
		it->second->parse(this);
		m_parser_item = NULL;
		it++;
	}

	parse(rdoModelObjects::obPOST);
}

void RDOParser::parse(rdoModelObjects::RDOParseType file)
{
	ruint min, max;
	RDOParserContainer::getMinMax(file, min, max);
	if (min == RDOParserContainer::UNDEFINED_ID || max == RDOParserContainer::UNDEFINED_ID)
		return;

	RDOParserContainer::Iterator it = find(min);
	while (it != end())
	{
		if (it->first <= max)
		{
			m_parser_item = it->second;
			it->second->parse(this);
			m_parser_item = NULL;
		}
		else
		{
			break;
		}
		it++;
	}
}

void RDOParser::parse(REF(std::istream) stream)
{
	RDOParserContainer::Iterator it = begin();
	while (it != end())
	{
		m_parser_item = it->second;
		it->second->parse(this, stream);
		m_parser_item = NULL;
		it++;
	}
}

void RDOParser::checkFunctionName(CREF(RDOParserSrcInfo) src_info)
{
	LPRDOFUNConstant pConstant = findFUNConstant(src_info.src_text());
	if (pConstant)
	{
		error().push_only(src_info, rdo::format(_T("Константа '%s' уже существует"), src_info.src_text().c_str()));
//		parser->error(_T("Second appearance of the same constant name: ") + *(_cons->getName()));
		error().push_only(pConstant->src_info(), _T("См. первое определение"));
		error().push_done();
	}
	LPRDOFUNSequence pSequence = findFUNSequence(src_info.src_text());
	if (pSequence)
	{
		error().push_only(src_info, rdo::format(_T("Последовательность '%s' уже существует"), src_info.src_text().c_str()));
		error().push_only(pSequence->src_info(), _T("См. первое определение"));
		error().push_done();
	}
	LPRDOFUNFunction pFunction = findFUNFunction(src_info.src_text());
	if (pFunction)
	{
		error().push_only(src_info, rdo::format(_T("Функция '%s' уже существует"), src_info.src_text().c_str()));
		error().push_only(pFunction->src_info(), _T("См. первое определение"));
		error().push_done();
	}
}

void RDOParser::checkActivityName(CREF(RDOParserSrcInfo) src_info)
{
	STL_FOR_ALL_CONST(getDPTSearchs(), it_search)
	{
		RDODPTSearch::ActivityList::const_iterator it_search_act = std::find_if((*it_search)->getActivities().begin(), (*it_search)->getActivities().end(), compareName<RDODPTSearchActivity>(src_info.src_text()));
		if (it_search_act != (*it_search)->getActivities().end())
		{
			error().push_only(src_info, rdo::format(_T("Активность '%s' уже существует"), src_info.src_text().c_str()));
			error().push_only((*it_search_act)->src_info(), _T("См. первое определение"));
			error().push_done();
//			error(_T("Activity name: ") + *_name + _T(" already defined"));
		}
	}
	STL_FOR_ALL_CONST(getDPTSomes(), it_some)
	{
		RDODPTSome::ActivityList::const_iterator it_some_act = std::find_if((*it_some)->getActivities().begin(), (*it_some)->getActivities().end(), compareName<RDODPTSomeActivity>(src_info.src_text()));
		if (it_some_act != (*it_some)->getActivities().end())
		{
			error().push_only(src_info, rdo::format(_T("Активность '%s' уже существует"), src_info.src_text().c_str()));
			error().push_only((*it_some_act)->src_info(), _T("См. первое определение"));
			error().push_done();
		}
	}
	STL_FOR_ALL_CONST(getDPTPriors(), it_prior)
	{
		RDODPTPrior::ActivityList::const_iterator it_prior_act = std::find_if((*it_prior)->getActivities().begin(), (*it_prior)->getActivities().end(), compareName<RDODPTPriorActivity>(src_info.src_text()));
		if (it_prior_act != (*it_prior)->getActivities().end())
		{
			error().push_only(src_info, rdo::format(_T("Активность '%s' уже существует"), src_info.src_text().c_str()));
			error().push_only((*it_prior_act)->src_info(), _T("См. первое определение"));
			error().push_done();
		}
	}
	LPRDODPTFreeActivity pFreeActivity = findDPTFreeActivity(src_info.src_text());
	if (pFreeActivity)
	{
		error().push_only(src_info, rdo::format(_T("Активность '%s' уже существует"), src_info.src_text().c_str()));
		error().push_only(pFreeActivity->src_info(), _T("См. первое определение"));
		error().push_done();
	}
}

void RDOParser::checkDPTName(CREF(RDOParserSrcInfo) src_info)
{
	if (src_info.src_text().empty())
	{
		// Актуально для операций и свободных блоков активностей
		return;
	}
	DPTSearchList::const_iterator search_it = std::find_if(getDPTSearchs().begin(), getDPTSearchs().end(), compareName<RDODPTSearch>(src_info.src_text()));
	if (search_it != getDPTSearchs().end())
	{
		error().push_only(src_info, rdo::format(_T("Точка '%s' уже существует"), src_info.src_text().c_str()));
		error().push_only((*search_it)->src_info(), _T("См. первое определение"));
		error().push_done();
//		error(src_info, _T("DPT name: ") + src_info.src_text() + _T(" already defined"));
	}
	DPTSomeList::const_iterator some_it = std::find_if(getDPTSomes().begin(), getDPTSomes().end(), compareName<RDODPTSome>(src_info.src_text()));
	if (some_it != getDPTSomes().end())
	{
		error().push_only(src_info, rdo::format(_T("Точка '%s' уже существует"), src_info.src_text().c_str()));
		error().push_only((*some_it)->src_info(), _T("См. первое определение"));
		error().push_done();
	}
}

CLOSE_RDO_PARSER_NAMESPACE
