/*!
  \copyright (c) RDO-Team, 2011
  \file      rdoparser.cpp
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_pattern.h"
#include "simulator/runtime/rdo_pattern.h"

#include "simulator/compiler/parser/rdoparser.h"
#include "simulator/compiler/parser/rdoparser_rdo.h"
#include "simulator/compiler/parser/rdofun.h"
#include "simulator/compiler/parser/rdorss.h"
#include "simulator/compiler/parser/parser/std_fun.h"
#include "simulator/runtime/calc/calc_process.h"
#include "simulator/runtime/calc/resource/calc_resource.h"
#include "simulator/runtime/calc/function/calc_function_system.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOParser
// --------------------------------------------------------------------------------
RDOParser::ParserList RDOParser::s_parserStack;

const std::string RDOParser::METHOD_TIME_NOW = "time_now()";
const std::string RDOParser::METHOD_SECONDS = "seconds()";
const std::string RDOParser::METHOD_TERMINATE_COUNTER = "terminate_counter()";

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
	return it != m_all##NAME.end() ? *it : LPRDO##NAME(NULL); \
} \
rbool RDOParser::remove##NAME(const LPRDO##NAME item) \
{ \
	NAME##List::iterator it = std::find(m_all##NAME.begin(), m_all##NAME.end(), item); \
	if (it == m_all##NAME.end()) \
		return false; \
	m_all##NAME.erase(it); \
	return true; \
}

DECLARE_PARSER_OBJECT_CONTAINER(PATPattern    );
DECLARE_PARSER_OBJECT_CONTAINER(RTPResType    );
DECLARE_PARSER_OBJECT_CONTAINER(RSSResource   );
DECLARE_PARSER_OBJECT_CONTAINER(FRMFrame      );
DECLARE_PARSER_OBJECT_CONTAINER(FRMSprite     );
DECLARE_PARSER_OBJECT_CONTAINER(FRMCommandList);
DECLARE_PARSER_OBJECT_CONTAINER(FUNConstant   );
DECLARE_PARSER_OBJECT_CONTAINER(FUNFunction   );
DECLARE_PARSER_OBJECT_CONTAINER(FUNSequence   );
DECLARE_PARSER_OBJECT_CONTAINER(DPTSearch     );
DECLARE_PARSER_OBJECT_CONTAINER(DPTSome       );
DECLARE_PARSER_OBJECT_CONTAINER(DPTPrior      );
DECLARE_PARSER_OBJECT_CONTAINER(DPTActivity   );
DECLARE_PARSER_OBJECT_CONTAINER(ResultGroup   );
DECLARE_PARSER_OBJECT_CONTAINER(PROCProcess   );

DECLARE_PARSER_OBJECT_CONTAINER_NONAME(FUNGroup);

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

tstring RDOParser::lexer_text()
{
	LPRDOParserRDOItem pParserItem = !s_parserStack.empty()
		? s_parserStack.back()->m_parser_item.object_dynamic_cast<RDOParserRDOItem>()
		: LPRDOParserRDOItem();

	return pParserItem
		? pParserItem->text()
		: tstring();
}

LPRDOParser RDOParser::s_parser()
{
	return !s_parserStack.empty() ? s_parserStack.back() : LPRDOParser(NULL);
}

RDOParser::RDOParser()
	: m_parser_item(NULL )
	, m_pattern    (false)
{
	m_compilers.push_back(rdo::Factory<RDOParserSTDFUN> ::create());
	m_compilers.push_back(rdo::Factory<RDOParserRDOItem>::create(rdoModelObjects::SMR, smr_file_parse, smr_file_error, smr_file_lex));

	m_compilers.push_back(rdo::Factory<RDOParserRDOItem>::create(rdoModelObjects::RTP, evn_preparse_parse, evn_preparse_error, evn_preparse_lex));
	m_compilers.push_back(rdo::Factory<RDOParserRDOItem>::create(rdoModelObjects::RTP, evnparse, evnerror, evnlex));
	m_compilers.push_back(rdo::Factory<RDOParserRDOItem>::create(rdoModelObjects::RSS, evn_preparse_parse, evn_preparse_error, evn_preparse_lex));
	m_compilers.push_back(rdo::Factory<RDOParserRDOItem>::create(rdoModelObjects::RSS, evnparse, evnerror, evnlex));
	m_compilers.push_back(rdo::Factory<RDOParserRDOItem>::create(rdoModelObjects::PRC, evn_preparse_parse, evn_preparse_error, evn_preparse_lex));
	m_compilers.push_back(rdo::Factory<RDOParserRDOItem>::create(rdoModelObjects::PRC, evnparse, evnerror, evnlex));
#ifdef CORBA_ENABLE
	m_compilers.push_back(rdo::Factory<RDOParserCorbaRTP>::create());
	m_compilers.push_back(rdo::Factory<RDOParserCorbaRSS>::create());
#endif
	m_compilers.push_back(rdo::Factory<RDOParserRDOItem>::create(rdoModelObjects::FUN, evn_preparse_parse, evn_preparse_error, evn_preparse_lex));
	m_compilers.push_back(rdo::Factory<RDOParserRDOItem>::create(rdoModelObjects::FUN, evnparse, evnerror, evnlex));
	m_compilers.push_back(rdo::Factory<RDOParserRDOItem>::create(rdoModelObjects::EVN, evn_preparse_parse, evn_preparse_error, evn_preparse_lex));
	m_compilers.push_back(rdo::Factory<RDOParserRDOItem>::create(rdoModelObjects::EVN, evnparse, evnerror, evnlex));
	m_compilers.push_back(rdo::Factory<RDOParserRDOItem>::create(rdoModelObjects::PAT, evn_preparse_parse, evn_preparse_error, evn_preparse_lex));
	m_compilers.push_back(rdo::Factory<RDOParserRDOItem>::create(rdoModelObjects::PAT, evnparse, evnerror, evnlex));
	m_compilers.push_back(rdo::Factory<RDOParserRDOItem>::create(rdoModelObjects::DPT, evn_preparse_parse, evn_preparse_error, evn_preparse_lex));
	m_compilers.push_back(rdo::Factory<RDOParserRDOItem>::create(rdoModelObjects::DPT, evnparse, evnerror, evnlex));
	m_compilers.push_back(rdo::Factory<RDOParserRDOItem>::create(rdoModelObjects::FRM, evn_preparse_parse, evn_preparse_error, evn_preparse_lex));
	m_compilers.push_back(rdo::Factory<RDOParserRDOItem>::create(rdoModelObjects::FRM, evnparse, evnerror, evnlex));
	m_compilers.push_back(rdo::Factory<RDOParserRDOItem>::create(rdoModelObjects::PMD, evn_preparse_parse, evn_preparse_error, evn_preparse_lex));
	m_compilers.push_back(rdo::Factory<RDOParserRDOItem>::create(rdoModelObjects::PMD, evnparse, evnerror, evnlex));
	m_compilers.push_back(rdo::Factory<RDOParserRDOItem>::create(rdoModelObjects::SMR, smr_sim_parse, smr_sim_error, smr_sim_lex));
}

RDOParser::~RDOParser()
{}

void RDOParser::init()
{
	s_parserStack.push_back(this);

	m_pRuntime = rdo::Factory<rdo::runtime::RDORuntime>::create(new rdo::runtime::Error());
	ASSERT(m_pRuntime);
	m_pRuntime->memory_insert(sizeof(RDOParser));
	m_pRuntime->init();

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
	while (!m_pContextStack->top().object_dynamic_cast<RDOParser>())
	{
		m_pContextStack->pop_not_safed();
	}
	m_pContextStack->pop<RDOParser>();

	m_allPATPattern .clear();
	m_allRTPResType .clear();
	m_allRSSResource.clear();
	m_allFRMFrame   .clear();
	m_allFRMSprite  .clear();
	m_allFRMCommandList.clear();
	m_allFUNConstant.clear();
	m_allFUNFunction.clear();
	m_allFUNSequence.clear();
	m_allDPTSearch  .clear();
	m_allDPTSome    .clear();
	m_allDPTPrior   .clear();
	m_allDPTActivity.clear();
	m_allResultGroup.clear();
	m_allPROCProcess.clear();
	m_allFUNGroup   .clear();

	m_parser_item   = NULL;
	m_pSMR          = NULL;
	m_pContextStack = NULL;

	m_preCastTypeList   .clear();
	m_movementObjectList.clear();

	m_pRuntime->deinit();
	m_pRuntime = NULL;

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

namespace
{

LPExpression contextTimeNow(const RDOParserSrcInfo& srcInfo)
{
	return rdo::Factory<Expression>::create(
		rdo::Factory<TypeInfo>::delegate<RDOType__real>(srcInfo),
		rdo::Factory<rdo::runtime::RDOCalcGetTimeNow>::create(),
		srcInfo
	);
}

LPExpression contextSeconds(const RDOParserSrcInfo& srcInfo)
{
	return rdo::Factory<Expression>::create(
		rdo::Factory<TypeInfo>::delegate<RDOType__real>(srcInfo),
		rdo::Factory<rdo::runtime::RDOCalcGetSeconds>::create(),
		srcInfo
	);
}

LPExpression contextTerminateCounter(const RDOParserSrcInfo& srcInfo)
{
	return rdo::Factory<Expression>::create(
		rdo::Factory<TypeInfo>::delegate<RDOType__int>(srcInfo),
		rdo::Factory<rdo::runtime::RDOCalcGetTermNow>::create(),
		srcInfo
	);
}

LPExpression contextGetResource(const LPRDORSSResource& resource, const RDOParserSrcInfo& srcInfo)
{
	return resource->createGetResourceExpression(srcInfo);
}

LPExpression contextConstant(const LPRDOFUNConstant& constant, const RDOParserSrcInfo& srcInfo)
{
	return rdo::Factory<Expression>::create(
		constant->getTypeInfo(),
		rdo::Factory<rdo::runtime::RDOCalcGetConst>::create(constant->getNumber()),
		srcInfo
	);
}

LPExpression contextSequence(const std::string& name, const RDOParserSrcInfo& srcInfo)
{
	LPRDOFUNParams pParams = rdo::Factory<RDOFUNParams>::create(
		rdo::Factory<ArithmContainer>::create()
	);
	LPRDOFUNArithm pArithm = pParams->createSeqCall(name);
	pArithm->setSrcInfo(srcInfo);

	return rdo::Factory<Expression>::create(
		pArithm->typeInfo(),
		pArithm->calc(),
		srcInfo
	);
}

LPExpression contextUnknownEnum(const rdo::runtime::LPRDOEnumType& enumType, ruint index, const RDOParserSrcInfo& srcInfo)
{
	LPTypeInfo typeInfo = rdo::Factory<TypeInfo>::delegate<RDOType__identificator>(srcInfo);
	return rdo::Factory<Expression>::create(
		typeInfo,
		rdo::Factory<rdo::runtime::RDOCalcConst>::create(rdo::runtime::RDOValue(enumType->getValues()[index], typeInfo->type()->type())),
		srcInfo
	);
}

}

Context::FindResult RDOParser::onFindContext(const std::string& method, const Context::Params& params, const RDOParserSrcInfo& srcInfo) const
{
	const std::string identifier = params.identifier();

	if (method == Context::METHOD_GET)
	{
		if (identifier == "Time_now" || identifier == "time_now" || identifier == "Системное_время" || identifier == "системное_время")
		{
			return FindResult(CreateExpression(boost::bind(&contextTimeNow, srcInfo)));
		}
		else if (identifier == "Seconds" || identifier == "seconds")
		{
			return FindResult(CreateExpression(boost::bind(&contextSeconds, srcInfo)));
		}
		else if (identifier == "Terminate_counter" || identifier == "terminate_counter")
		{
			return FindResult(CreateExpression(boost::bind(&contextTerminateCounter, srcInfo)));
		}
	}

	if (method == Context::METHOD_TYPE_OF)
	{
		LPRDORTPResType pResType = findRTPResType(identifier);
		if (pResType)
		{
			return pResType->find(method, params, srcInfo);
		}
	}

	if (method == Context::METHOD_OPERATOR_DOT)
	{
		//! Типы ресурсов
		LPRDORTPResType pResType = findRTPResType(identifier);
		if (pResType)
		{
			return FindResult(SwitchContext(pResType));
		}

		//! Ресурсы
		LPRDORSSResource pResource = findRSSResource(identifier);
		if (pResource)
		{
			return FindResult(SwitchContext(pResource));
		}
	}

	if (method == Context::METHOD_GET)
	{
		//! Ресурсы
		LPRDORSSResource pResource = findRSSResource(identifier);
		if (pResource)
		{
			return FindResult(CreateExpression(boost::bind(&contextGetResource, pResource, srcInfo)));
		}

		//! Константы
		LPRDOFUNConstant pConstant = findFUNConstant(identifier);
		if (pConstant)
		{
			return FindResult(CreateExpression(boost::bind(&contextConstant, pConstant, srcInfo)));
		}

		//! Последовательности
		LPRDOFUNSequence pSequence = findFUNSequence(identifier);
		if (pSequence)
		{
			return FindResult(CreateExpression(boost::bind(&contextSequence, identifier, srcInfo)));
		}

		//! Возможно, что это значение перечислимого типа, только одно и тоже значение может встречаться в разных
		//! перечислимых типах, поэтому какой именно из них выбрать - вопрос
		{
			ErrorBlockMonicker errorBlockMonicker;
			BOOST_FOREACH(const LPTypeInfo& type, m_preCastTypeList)
			{
				LPRDOEnumType enumType = type->type().object_dynamic_cast<RDOEnumType>();
				ASSERT(enumType);

				ruint index = enumType->getEnums()->findEnum(identifier);
				if (index != rdo::runtime::RDOEnumType::END)
				{
					return FindResult(CreateExpression(boost::bind(&contextUnknownEnum, enumType->getEnums(), index, srcInfo)));
				}
			}
		}
	}

	return Context::FindResult();
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
	stream << "$Changes" << std::endl;
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
		stream << "  " << change_it->m_name;
		for (ruint i = change_it->m_name.length(); i < changes_max_length; i++)
		{
			stream << " ";
		}
		stream << "  = " << change_it->m_value << std::endl;
	}
	return stream.str();
}

tstring RDOParser::getModelStructure()
{
	rdo::textstream modelStructure;

	// $Changes
	modelStructure << getChanges();

	// RTP
	modelStructure << std::endl << std::endl << "$Resource_type" << std::endl;
	STL_FOR_ALL_CONST(m_allRTPResType, rtp_it)
	{
		(*rtp_it)->writeModelStructure(modelStructure);
	}

	// RSS
	modelStructure << std::endl << "$Resources" << std::endl;
	STL_FOR_ALL_CONST(m_allRSSResource, rss_it)
	{
		(*rss_it)->writeModelStructure(modelStructure);
	}

	// PAT
	modelStructure << std::endl << "$Pattern" << std::endl;
	STL_FOR_ALL_CONST(m_allPATPattern, pat_it)
	{
		(*pat_it)->writeModelStructure(modelStructure);
	}

	// OPR/DPT
	ruint counter = 1;
	modelStructure << std::endl << "$Activities" << std::endl;
	modelStructure << m_pRuntime->writeActivitiesStructure(counter);

	// DPT only
	for (ruint i = 0; i < m_allDPTSearch.size(); i++)
	{
		for (ruint j = 0; j < m_allDPTSearch.at(i)->getActivities().size(); j++)
		{
			LPRDODPTSearchActivity pSearchActivity = m_allDPTSearch.at(i)->getActivities().at(j);
			ASSERT(pSearchActivity);
			modelStructure << counter++ << " " << pSearchActivity->name() << " " << pSearchActivity->pattern()->getPatternId() << std::endl;
		}
	}

	// PMD
	modelStructure << std::endl << "$Watching" << std::endl;
	ruint watching_max_length = 0;
	STL_FOR_ALL_CONST(m_pRuntime->getResult(), watching_it)
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
	STL_FOR_ALL_CONST(m_pRuntime->getResult(), watching_it)
	{
		LPITrace          trace     = *watching_it;
		LPIName           name      = trace;
		LPIModelStructure structure = trace;
		if (trace && name && structure)
		{
			if (trace->traceable())
			{
				modelStructure << "  " << name->name();
				for (ruint i = name->name().length(); i < watching_max_length + 2; i++)
					modelStructure << " ";

				structure->writeModelStructure(modelStructure);
			}
		}
	}

	return modelStructure.str();
}

void RDOParser::parse()
{
	BOOST_FOREACH(const LPRDOParserItem& compiler, m_compilers)
	{
		m_parser_item = compiler;
		m_parser_item->parse(this);
		m_parser_item = NULL;
	}
	runRTPPost();
}

void RDOParser::parse(REF(std::istream) stream)
{
	BOOST_FOREACH(const LPRDOParserItem& compiler, m_compilers)
	{
		m_parser_item = compiler;
		m_parser_item->parse(this, stream);
		m_parser_item = NULL;
	}
}

void RDOParser::beforeRun()
{
	runRSSPost();
	runSMRPost();
}

void RDOParser::runRSSPost()
{
	//! В режиме совместимости со старым РДО создаем ресурсы по номерам их типов, а не по номерам самих ресурсов из RSS
#ifdef RDOSIM_COMPATIBLE
	STL_FOR_ALL_CONST(getRTPResTypes(), rtp_it)
	{
#endif
		STL_FOR_ALL_CONST(getRSSResources(), rss_it)
		{
#ifdef RDOSIM_COMPATIBLE
			if ((*rss_it)->getType() == *rtp_it)
			{
#endif
				rdo::runtime::LPRDOCalc calc = (*rss_it)->createCalc();
				runtime()->addInitCalc(calc);
#ifdef RDOSIM_COMPATIBLE
			}
#endif
		}
#ifdef RDOSIM_COMPATIBLE
	}
#endif
}

void RDOParser::runSMRPost()
{
	//! Планирование событий, описанных в SMR
	BOOST_FOREACH(const LPRDOPATPattern& pattern, getPATPatterns())
	{
		LPRDOPatternEvent event = pattern.object_dynamic_cast<RDOPatternEvent>();
		if (!event)
			continue;

		rdo::runtime::LPRDOCalc initCalc = event->getBeforeStartModelPlaning();
		if (initCalc)
		{
			runtime()->addInitCalc(initCalc);
		}
	}
}

void RDOParser::runRTPPost()
{
	BOOST_FOREACH(const LPRDORTPResType& type, getRTPResTypes())
	{
		type->setupRuntimeFactory();
	}
}

void RDOParser::checkFunctionName(CREF(RDOParserSrcInfo) src_info)
{
	LPRDOFUNConstant pConstant = findFUNConstant(src_info.src_text());
	if (pConstant)
	{
		error().push_only(src_info, rdo::format("Константа '%s' уже существует", src_info.src_text().c_str()));
//		parser->error("Second appearance of the same constant name: " + *(_cons->getName()));
		error().push_only(pConstant->src_info(), "См. первое определение");
		error().push_done();
	}
	LPRDOFUNSequence pSequence = findFUNSequence(src_info.src_text());
	if (pSequence)
	{
		error().push_only(src_info, rdo::format("Последовательность '%s' уже существует", src_info.src_text().c_str()));
		error().push_only(pSequence->src_info(), "См. первое определение");
		error().push_done();
	}
	LPRDOFUNFunction pFunction = findFUNFunction(src_info.src_text());
	if (pFunction)
	{
		error().push_only(src_info, rdo::format("Функция '%s' уже существует", src_info.src_text().c_str()));
		error().push_only(pFunction->src_info(), "См. первое определение");
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
			error().push_only(src_info, rdo::format("Активность '%s' уже существует", src_info.src_text().c_str()));
			error().push_only((*it_search_act)->src_info(), "См. первое определение");
			error().push_done();
//			error("Activity name: " + *_name + " already defined");
		}
	}
	STL_FOR_ALL_CONST(getDPTSomes(), it_some)
	{
		RDODPTSome::ActivityList::const_iterator it_some_act = std::find_if((*it_some)->getActivities().begin(), (*it_some)->getActivities().end(), compareName<RDODPTSomeActivity>(src_info.src_text()));
		if (it_some_act != (*it_some)->getActivities().end())
		{
			error().push_only(src_info, rdo::format("Активность '%s' уже существует", src_info.src_text().c_str()));
			error().push_only((*it_some_act)->src_info(), "См. первое определение");
			error().push_done();
		}
	}
	STL_FOR_ALL_CONST(getDPTPriors(), it_prior)
	{
		RDODPTPrior::ActivityList::const_iterator it_prior_act = std::find_if((*it_prior)->getActivities().begin(), (*it_prior)->getActivities().end(), compareName<RDODPTPriorActivity>(src_info.src_text()));
		if (it_prior_act != (*it_prior)->getActivities().end())
		{
			error().push_only(src_info, rdo::format("Активность '%s' уже существует", src_info.src_text().c_str()));
			error().push_only((*it_prior_act)->src_info(), "См. первое определение");
			error().push_done();
		}
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
		error().push_only(src_info, rdo::format("Точка '%s' уже существует", src_info.src_text().c_str()));
		error().push_only((*search_it)->src_info(), "См. первое определение");
		error().push_done();
//		error(src_info, "DPT name: " + src_info.src_text() + " already defined");
	}
	DPTSomeList::const_iterator some_it = std::find_if(getDPTSomes().begin(), getDPTSomes().end(), compareName<RDODPTSome>(src_info.src_text()));
	if (some_it != getDPTSomes().end())
	{
		error().push_only(src_info, rdo::format("Точка '%s' уже существует", src_info.src_text().c_str()));
		error().push_only((*some_it)->src_info(), "См. первое определение");
		error().push_done();
	}
}

CLOSE_RDO_PARSER_NAMESPACE
