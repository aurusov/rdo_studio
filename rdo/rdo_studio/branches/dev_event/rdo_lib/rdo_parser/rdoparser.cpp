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
#include "rdo_lib/rdo_parser/rdortp.h"
#include "rdo_lib/rdo_parser/rdofrm.h"
#include "rdo_lib/rdo_parser/rdofun.h"
#include "rdo_lib/rdo_parser/rdoopr.h"
#include "rdo_lib/rdo_parser/rdorss.h"
#include "rdo_lib/rdo_parser/rdodpt.h"
#include "rdo_lib/rdo_parser/rdopmd.h"
#include "rdo_common/rdocommon.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOParser
// ----------------------------------------------------------------------------
RDOParser::ParserList RDOParser::s_parserStack;

#define DECLARE_PARSER_OBJECT_CONTAINER_NONAME_NO_LP(NAME) \
void RDOParser::insert##NAME(PTR(RDO##NAME) value) \
{ \
	m_parsing_object = (RDOParserObject*)value; \
	m_all##NAME.push_back(value); \
}

#define DECLARE_PARSER_OBJECT_CONTAINER_NONAME_LP(NAME) \
void RDOParser::insert##NAME(LPRDO##NAME value) \
{ \
	m_parsing_object = (RDOParserObject*)(value.get()); \
	m_all##NAME.push_back(value); \
}

#define DECLARE_PARSER_OBJECT_CONTAINER_NO_LP(NAME) \
DECLARE_PARSER_OBJECT_CONTAINER_NONAME_NO_LP(NAME) \
const RDO##NAME* RDOParser::find##NAME(CREF(tstring) name) const \
{ \
	NAME##List::const_iterator it = std::find_if(m_all##NAME.begin(), m_all##NAME.end(), compareName<RDO##NAME>(name)); \
	return it != m_all##NAME.end() ? *it : NULL; \
} \
rbool RDOParser::remove##NAME(const PTR(RDO##NAME) item) \
{ \
	NAME##List::iterator it = std::find(m_all##NAME.begin(), m_all##NAME.end(), item); \
	if (it == m_all##NAME.end()) \
		return false; \
	m_all##NAME.erase(it); \
	return true; \
}

#define DECLARE_PARSER_OBJECT_CONTAINER_LP(NAME) \
DECLARE_PARSER_OBJECT_CONTAINER_NONAME_LP(NAME) \
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
//DECLARE_PARSER_OBJECT_CONTAINER(Event          );

DECLARE_PARSER_OBJECT_CONTAINER_NO_LP(PATPattern     );
DECLARE_PARSER_OBJECT_CONTAINER_LP   (RTPResType     );
DECLARE_PARSER_OBJECT_CONTAINER_NO_LP(RSSResource    );
DECLARE_PARSER_OBJECT_CONTAINER_NO_LP(OPROperation   );
DECLARE_PARSER_OBJECT_CONTAINER_NO_LP(FRMFrame       );
DECLARE_PARSER_OBJECT_CONTAINER_NO_LP(FUNConstant    );
DECLARE_PARSER_OBJECT_CONTAINER_NO_LP(FUNFunction    );
DECLARE_PARSER_OBJECT_CONTAINER_NO_LP(FUNSequence    );
DECLARE_PARSER_OBJECT_CONTAINER_NO_LP(DPTSearch      );
DECLARE_PARSER_OBJECT_CONTAINER_NO_LP(DPTSome        );
DECLARE_PARSER_OBJECT_CONTAINER_NO_LP(DPTPrior       );
DECLARE_PARSER_OBJECT_CONTAINER_NO_LP(DPTFreeActivity);
DECLARE_PARSER_OBJECT_CONTAINER_NO_LP(PMDPokaz       );

DECLARE_PARSER_OBJECT_CONTAINER_NONAME_NO_LP(FUNGroup   );
DECLARE_PARSER_OBJECT_CONTAINER_NONAME_NO_LP(DPTFree    );
DECLARE_PARSER_OBJECT_CONTAINER_NONAME_NO_LP(PROCProcess);
DECLARE_PARSER_OBJECT_CONTAINER_NONAME_NO_LP(Operations );

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

PTR(RDOParser) RDOParser::s_parser()
{
	return !s_parserStack.empty() ? s_parserStack.back() : NULL;
}

RDOParser::RDOParser()
	: m_parsing_object      (NULL )
	, m_parser_item         (NULL )
	, m_have_kw_Resources   (false)
	, m_have_kw_ResourcesEnd(false)
	, m_smr                 (NULL )
{
	s_parserStack.push_back(this);
	m_runtime.memory_insert(sizeof(RDOParser));
	m_runtime.init();
}

RDOParser::~RDOParser()
{
	m_runtime.deinit();
	rdo::deleteAllObjects(m_allValues);
	m_movementObjectList.clear();

	DeletableList::reverse_iterator it = m_allDeletables.rbegin();
	while (it != m_allDeletables.rend())
	{
		delete *it;
		it = m_allDeletables.rbegin();
	}
	TRACE1(_T("PARSER : m_allDeletables.size() = %d\n"), m_allDeletables.size());
	s_parserStack.remove(this);
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
	STL_FOR_ALL_CONST(ChangesList, m_changes, change_it)
	{
		if (change_it->m_name.length() > changes_max_length)
		{
			changes_max_length = change_it->m_name.length();
		}
	}
	STL_FOR_ALL_CONST(ChangesList, m_changes, change_it)
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
	STL_FOR_ALL_CONST(RTPResTypeList, m_allRTPResType, rtp_it)
	{
		(*rtp_it)->writeModelStructure(modelStructure);
	}

	// RSS
	modelStructure << std::endl << _T("$Resources") << std::endl;
	STL_FOR_ALL_CONST(RSSResourceList, m_allRSSResource, rss_it)
	{
		(*rss_it)->writeModelStructure(modelStructure);
	}

	// PAT
	modelStructure << std::endl << _T("$Pattern") << std::endl;
	STL_FOR_ALL_CONST(PATPatternList, m_allPATPattern, pat_it)
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
			PTR(RDODPTSearchActivity) curr = m_allDPTSearch.at(i)->getActivities().at(j);
			modelStructure << counter++ << _T(" ") << curr->name() << _T(" ") << curr->pattern()->getPatternId() << std::endl;
		}
	}

	// PMD
	modelStructure << std::endl << _T("$Watching") << std::endl;
	ruint watching_max_length = 0;
	STL_FOR_ALL_CONST(rdoRuntime::RDORuntime::LPIPokazList, m_runtime.getPokaz(), watching_it)
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
	STL_FOR_ALL_CONST(rdoRuntime::RDORuntime::LPIPokazList, m_runtime.getPokaz(), watching_it)
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

	RDOParserContainer::CIterator it = begin();
	while (it != end())
	{
		m_parser_item = it->second;
		it->second->parse();
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

	RDOParserContainer::CIterator it = find(min);
	while (it != end())
	{
		if (it->first <= max)
		{
			m_parser_item = it->second;
			it->second->parse();
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
	RDOParserContainer::CIterator it = begin();
	while (it != end())
	{
		m_parser_item = it->second;
		it->second->parse(stream);
		m_parser_item = NULL;
		it++;
	}
}

void RDOParser::checkFunctionName(CREF(RDOParserSrcInfo) src_info)
{
	CPTR(RDOFUNConstant) pConst = findFUNConstant(src_info.src_text());
	if (pConst)
	{
		error().push_only(src_info, rdo::format(_T("Константа '%s' уже существует"), src_info.src_text().c_str()));
//		parser->error(_T("Second appearance of the same constant name: ") + *(_cons->getName()));
		error().push_only(pConst->src_info(), _T("См. первое определение"));
		error().push_done();
	}
	CPTR(RDOFUNSequence) pSequence = findFUNSequence(src_info.src_text());
	if (pSequence)
	{
		error().push_only(src_info, rdo::format(_T("Последовательность '%s' уже существует"), src_info.src_text().c_str()));
		error().push_only(pSequence->src_info(), _T("См. первое определение"));
		error().push_done();
	}
	CPTR(RDOFUNFunction) pFunction = findFUNFunction(src_info.src_text());
	if (pFunction)
	{
		error().push_only(src_info, rdo::format(_T("Функция '%s' уже существует"), src_info.src_text().c_str()));
		error().push_only(pFunction->src_info(), _T("См. первое определение"));
		error().push_done();
	}
}

void RDOParser::checkActivityName(CREF(RDOParserSrcInfo) src_info)
{
	STL_FOR_ALL_CONST(DPTSearchList, getDPTSearchs(), it_search)
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
	STL_FOR_ALL_CONST(DPTSomeList, getDPTSomes(), it_some)
	{
		RDODPTSome::ActivityList::const_iterator it_some_act = std::find_if((*it_some)->getActivities().begin(), (*it_some)->getActivities().end(), compareName<RDODPTSomeActivity>(src_info.src_text()));
		if (it_some_act != (*it_some)->getActivities().end())
		{
			error().push_only(src_info, rdo::format(_T("Активность '%s' уже существует"), src_info.src_text().c_str()));
			error().push_only((*it_some_act)->src_info(), _T("См. первое определение"));
			error().push_done();
		}
	}
	CPTR(RDODPTFreeActivity) pFreeAct = findDPTFreeActivity(src_info.src_text());
	if (pFreeAct)
	{
		error().push_only(src_info, rdo::format(_T("Активность '%s' уже существует"), src_info.src_text().c_str()));
		error().push_only(pFreeAct->src_info(), _T("См. первое определение"));
		error().push_done();
//		error(_T("Free activity name: ") + *_name + _T(" already defined"));
	}
	CPTR(RDOOPROperation) pOpr = findOPROperation(src_info.src_text());
	if (pOpr)
	{
		error().push_only(src_info, rdo::format(_T("Операция '%s' уже существует"), src_info.src_text().c_str()));
		error().push_only(pOpr->src_info(), _T("См. первое определение"));
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
