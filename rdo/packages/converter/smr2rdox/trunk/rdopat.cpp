/*!
  \copyright (c) RDO-Team, 2011
  \file      rdopat.cpp
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "converter/smr2rdox/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/rdopat.h"
#include "converter/smr2rdox/rdoparser.h"
#include "converter/smr2rdox/rdortp.h"
#include "converter/smr2rdox/rdoparser_lexer.h"
#include "converter/smr2rdox/rdo_type_range.h"
#include "converter/smr2rdox/runtime/rdo_ie.h"
#include "simulator/runtime/calc/procedural/calc_nop.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_NAMESPACE

int cnv_patlex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void cnv_paterror(PTR(char) message)
{
	UNUSED(message);
}

int pat_preparse_lex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void pat_preparse_error(PTR(char) message)
{
	UNUSED(message);
}

// --------------------------------------------------------------------------------
// -------------------- RDOPATPattern
// --------------------------------------------------------------------------------
RDOPATPattern::RDOPATPattern(CREF(RDOParserSrcInfo) name_src_info)
	: RDOParserSrcInfo    (name_src_info)
	, m_pCommonChoice     (NULL         )
	, m_pPatRuntime       (NULL         )
	, m_pCurrRelRes       (NULL         )
	, m_currentRelResIndex(0            )
	, m_useCommonChoice   (false        )
{
	LPRDOPATPattern pPatternExist = Converter::s_converter()->findPATPattern(src_info().src_text());
	if (pPatternExist)
	{
		rdoConverter::g_error().push_only(src_info(), rdo::format(_T("Паттерн '%s' уже существует"), name().c_str()));
		rdoConverter::g_error().push_only(pPatternExist->src_info(), _T("См. первое определение"));
		rdoConverter::g_error().push_done();
	}
	Converter::s_converter()->insertPATPattern(this);
}

tstring RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::ConvertStatus value)
{
	switch (value)
	{
	case rdoRuntime::RDOResource::CS_Keep    : return _T("Keep");
	case rdoRuntime::RDOResource::CS_Create  : return _T("Create");
	case rdoRuntime::RDOResource::CS_Erase   : return _T("Erase");
	case rdoRuntime::RDOResource::CS_NonExist: return _T("NonExist");
	case rdoRuntime::RDOResource::CS_NoChange: return _T("NoChange");
	default                                  : NEVER_REACH_HERE;
	}
	return tstring();
}

rdoRuntime::RDOResource::ConvertStatus RDOPATPattern::StrToStatus(CREF(tstring) value, CREF(YYLTYPE) convertor_pos)
{
	if (value == _T("Keep") || value == _T("keep"))
	{
		return rdoRuntime::RDOResource::CS_Keep;
	}
	else if (value == _T("Create") || value == _T("create"))
	{
		return rdoRuntime::RDOResource::CS_Create;
	}
	else if (value == _T("Erase") || value == _T("erase"))
	{
		return rdoRuntime::RDOResource::CS_Erase;
	}
	else if (value == _T("NonExist") || value == _T("nonexist"))
	{
		return rdoRuntime::RDOResource::CS_NonExist;
	}
	else if (value == _T("NoChange") || value == _T("nochange"))
	{
		return rdoRuntime::RDOResource::CS_NoChange;
	}
	rdoConverter::g_error().error(convertor_pos, rdo::format(_T("Неверный статус конвертора: %s"), value.c_str()));
	return rdoRuntime::RDOResource::CS_Keep;
}

void RDOPATPattern::beforeRelRensert(CREF(RDOParserSrcInfo) rel_info)
{
	if (findRelevantResource(rel_info.src_text()))
	{
		rdoConverter::g_error().error(rel_info, rdo::format(_T("Релевантный ресурс '%s' уже определен"), rel_info.src_text().c_str()));
	}
}

void RDOPATPattern::rel_res_insert(CREF(LPRDORelevantResource) pRelevantResource)
{
	ASSERT(pRelevantResource);
	switch (getType())
	{
	case PT_IE       : getPatRuntime<rdoRuntime::RDOPatternIrregEvent>()->addConvertorStatus     (pRelevantResource->m_statusBegin); break;
	case PT_Rule     : getPatRuntime<rdoRuntime::RDOPatternRule      >()->addConvertorStatus     (pRelevantResource->m_statusBegin); break;
	case PT_Operation: getPatRuntime<rdoRuntime::RDOPatternOperation >()->addConvertorBeginStatus(pRelevantResource->m_statusBegin); break;
	case PT_Keyboard : getPatRuntime<rdoRuntime::RDOPatternKeyboard  >()->addConvertorBeginStatus(pRelevantResource->m_statusBegin); break;
	default          : rdoConverter::g_error().error(src_info(), _T("Неизвестный тип образца"));
	}
	m_relResList.push_back(pRelevantResource);
}

void RDOPATPattern::addRelResConvert(rbool trace, CREF(LPConvertCmdList) commands, CREF(YYLTYPE) convertor_pos, CREF(YYLTYPE) trace_pos, rdoRuntime::RDOResource::ConvertStatus status)
{
	if (status == rdoRuntime::RDOResource::CS_NoChange || status == rdoRuntime::RDOResource::CS_NonExist)
	{
		rdoConverter::g_error().error(convertor_pos, getErrorMessage_NotNeedConvertor(m_pCurrRelRes->name(), status));
	}

	if (status == rdoRuntime::RDOResource::CS_Create)
	{
		addParamSetCalc(createRelRes(trace));
	}
	else
	{
		if (trace)
		{
			rdoConverter::g_error().error(trace_pos, _T("Признак трассировки в данном месте возможен только для создаваемого ресурса"));
		}
	}

	if (commands->commands().empty() && status == rdoRuntime::RDOResource::CS_Keep)
	{
		rdoConverter::g_error().warning(convertor_pos, getWarningMessage_EmptyConvertor(m_pCurrRelRes->name(), status));
	}

	STL_FOR_ALL_CONST(commands->commands(), cmdIt)
		addParamSetCalc(*cmdIt);

	ASSERT(m_pCurrRelRes);
	m_pCurrRelRes->getParamSetList().reset();
}

void RDOPATPattern::addParamSetCalc(CREF(rdoRuntime::LPRDOCalc) pCalc)
{
	switch (getType())
	{
	case PT_IE       : getPatRuntime<rdoRuntime::RDOPatternIrregEvent>()->addConvertorCalc(pCalc); break;
	case PT_Rule     : getPatRuntime<rdoRuntime::RDOPatternRule      >()->addConvertorCalc(pCalc); break;
	case PT_Operation: NEVER_REACH_HERE;
	case PT_Keyboard : NEVER_REACH_HERE;
	default          : rdoConverter::g_error().error(src_info(), _T("Неизвестный тип образца"));
	}
}

tstring RDOPATPattern::getPatternId() const
{ 
	return m_pPatRuntime->traceId(); 
}

void RDOPATPattern::writeModelStructure(REF(std::ostream) stream) const
{
	stream << getPatternId() << " " << name() << " " << getModelStructureLetter() << " " << m_relResList.size();
	STL_FOR_ALL_CONST(m_relResList, it)
		stream << " " << (*it)->getType()->getNumber();

	stream << std::endl;
}

LPRDOParam RDOPATPattern::findPATPatternParam(CREF(tstring) paramName) const
{
	ParamList::const_iterator it = std::find_if(m_paramList.begin(), m_paramList.end(), compareName<RDOParam>(paramName));
	return it != m_paramList.end() ? (*it) : LPRDOParam(NULL);
}

LPRDORelevantResource RDOPATPattern::findRelevantResource(CREF(tstring) resName) const
{
	RelResList::const_iterator it = std::find_if(m_relResList.begin(), m_relResList.end(), compareName<RDORelevantResource>(resName));
	return it != m_relResList.end() ? (*it) : LPRDORelevantResource(NULL);
}

int RDOPATPattern::findPATPatternParamNum(CREF(tstring) paramName) const
{
	ParamList::const_iterator it = std::find_if(m_paramList.begin(), m_paramList.end(), compareName<RDOParam>(paramName));
	return it != m_paramList.end() ? it - m_paramList.begin() : -1;
}

ruint RDOPATPattern::findRelevantResourceNum(CREF(tstring) resName) const
{
	RelResList::const_iterator it = std::find_if(m_relResList.begin(), m_relResList.end(), compareName<RDORelevantResource>(resName));
	return it != m_relResList.end() ? it - m_relResList.begin() : ruint(~0);
}

void RDOPATPattern::add(CREF(LPRDOParam) pParam)
{
	ASSERT(pParam);

	LPRDOParam pParamExist = findPATPatternParam(pParam->name());
	if (pParamExist)
	{
		rdoConverter::g_error().push_only(pParam->src_info(), rdo::format(_T("Параметр '%s' уже определен"), pParam->src_text().c_str()));
		rdoConverter::g_error().push_only(pParamExist->src_info(), _T("См. первое определение"));
		rdoConverter::g_error().push_done();
	}
	m_paramList.push_back(pParam);
}

void RDOPATPattern::setCommonChoiceFirst()
{
	m_useCommonChoice = true;
	m_pCommonChoice   = NULL;
//	rdoConverter::g_error().error(_T("Вызывать нельзя, т.к. в сообщениях об ошибках используется m_pCommonChoice"));
}

void RDOPATPattern::setCommonChoiceWithMin(CREF(LPRDOFUNArithm) arithm)
{
	m_useCommonChoice  = true;
	m_useCommonWithMax = false;
	m_pCommonChoice    = arithm;
}

void RDOPATPattern::setCommonChoiceWithMax(CREF(LPRDOFUNArithm) arithm)
{
	m_useCommonChoice  = true;
	m_useCommonWithMax = true;
	m_pCommonChoice    = arithm;
}

void RDOPATPattern::setTime(REF(LPRDOFUNArithm) arithm)
{ 
	switch (getType())
	{
	case PT_IE       : getPatRuntime<rdoRuntime::RDOPatternIrregEvent>()->setTime(arithm->createCalc(NULL)); time = arithm; break;
	case PT_Operation: getPatRuntime<rdoRuntime::RDOPatternOperation >()->setTime(arithm->createCalc(NULL)); break;
	case PT_Keyboard : getPatRuntime<rdoRuntime::RDOPatternKeyboard  >()->setTime(arithm->createCalc(NULL)); break;
	default          : rdoConverter::g_error().error(src_info(), rdo::format(_T("Для образца типа %s недопустимо использование выражения времени"), typeToString(getType()).c_str()));
	}
}

void RDOPATPattern::addChoiceFromCalc(CREF(rdoRuntime::LPRDOCalc) pCalc)
{
	switch (getType())
	{
	case PT_IE       : getPatRuntime<rdoRuntime::RDOPatternIrregEvent>()->addPreSelectRelRes(pCalc); break;
	case PT_Rule     : getPatRuntime<rdoRuntime::RDOPatternRule      >()->addChoiceFromCalc (pCalc); break;
	case PT_Operation: getPatRuntime<rdoRuntime::RDOPatternOperation >()->addChoiceFromCalc (pCalc); break;
	case PT_Keyboard : getPatRuntime<rdoRuntime::RDOPatternKeyboard  >()->addChoiceFromCalc (pCalc); break;
	default          : rdoConverter::g_error().error(src_info(), rdo::format(_T("Для образца типа %s недопустимо использование условния выбора"), typeToString(getType()).c_str()));
	}
}

void RDOPATPattern::addRelResBody(CREF(RDOParserSrcInfo) body_name)
{ 
	RelResList::const_iterator it = std::find_if(m_relResList.begin(), m_relResList.end(), compareName<RDORelevantResource>(body_name.src_text()));
	if (it == m_relResList.end())
	{
		rdoConverter::g_error().error(body_name.src_info(), rdo::format(_T("Неизвестный релевантный ресурс: %s"), body_name.src_text().c_str()));
	}
	if (findRelevantResourceNum(body_name.src_text()) != m_currentRelResIndex)
	{
		tstring rel_res_waiting = m_currentRelResIndex < m_relResList.size() ? m_relResList[m_currentRelResIndex]->name().c_str() : _T("");
		rdoConverter::g_error().error(body_name.src_info(), rdo::format(_T("Ожидается описание релевантного ресурса '%s', вместо него найдено: %s"), rel_res_waiting.c_str(), body_name.src_text().c_str()));
	}
	if ((*it)->m_alreadyHaveConverter)
	{
		rdoConverter::g_error().error(body_name.src_info(), rdo::format(_T("Релевантный ресурс уже используется: %s"), body_name.src_text().c_str()));
	}
	m_pCurrRelRes = (*it);
	m_pCurrRelRes->m_bodySrcInfo = body_name;
	m_pCurrRelRes->m_alreadyHaveConverter = true;
	m_currentRelResIndex++;
}

void RDOPATPattern::addRelResUsage(CREF(LPRDOPATChoiceFrom) pChoiceFrom, CREF(LPRDOPATChoiceOrder) pChoiceOrder)
{
	if (!m_useCommonChoice)
	{
		if (pChoiceOrder->m_type == rdoRuntime::RDOSelectResourceCalc::order_empty)
		{
			if ((m_pCurrRelRes->m_statusBegin != rdoRuntime::RDOResource::CS_Create) && (m_pCurrRelRes->m_statusEnd != rdoRuntime::RDOResource::CS_Create))
			{
//				pChoiceOrder->m_type = rdoRuntime::RDOSelectResourceCalc::order_first;
			}
		}
		else if (m_pCurrRelRes->isDirect())
		{
			rdoConverter::g_error().warning(pChoiceOrder->src_info(), rdo::format(_T("Правило выбора '%s' релевантного ресурса '%s' не имеет смысла, т.к. релевантный ресурс определен через имя, а не тип, и не может быть связан с каким-либо другим ресурсом"), pChoiceOrder->src_text().c_str(), m_pCurrRelRes->name().c_str()));
		}
	}
	else
	{
		if (pChoiceOrder->m_type != rdoRuntime::RDOSelectResourceCalc::order_empty)
		{
			rdoConverter::g_error().push_only(pChoiceOrder->src_info(), _T("Нельзя указать способ выбора релевантного ресурса, т.к. используется единый для всех релевантных ресурсов способ, указанный до ключевого слова $Body"));
			if (m_pCommonChoice)
			{
				rdoConverter::g_error().push_only(m_pCommonChoice->src_info(), rdo::format(_T("См. '%s'"), m_pCommonChoice->src_text().c_str()));
			}
			rdoConverter::g_error().push_done();
		}
	}

	if ((m_pCurrRelRes->m_statusBegin == rdoRuntime::RDOResource::CS_Create) || (m_pCurrRelRes->m_statusEnd == rdoRuntime::RDOResource::CS_Create))
	{
		if (pChoiceFrom->m_type != RDOPATChoiceFrom::ch_empty)
		{
			rdoConverter::g_error().error(pChoiceFrom->src_info(), _T("Релевантный ресурс создается, для него нельзя использовать Choice from или Choice NoCheck"));
		}
		if (pChoiceOrder->m_type != rdoRuntime::RDOSelectResourceCalc::order_empty)
		{
			rdoConverter::g_error().error(pChoiceOrder->src_info(), rdo::format(_T("Релевантный ресурс создается, для него нельзя использовать правило выбора '%s'"), pChoiceOrder->asString().c_str()));
		}
	}

	m_pCurrRelRes->m_pChoiceFrom  = pChoiceFrom;
	m_pCurrRelRes->m_pChoiceOrder = pChoiceOrder;
}

void RDOPATPattern::end()
{
	int size = m_relResList.size();
	for (int i = 0; i < size; i++)
	{
		LPRDORelevantResource pCurrRelRes = m_relResList.at(i);
		if (!pCurrRelRes->m_alreadyHaveConverter)
		{
			/// @todo А почему нельзя сделать warning ? Возможно, есть жесткое требование недопустить пустого рел. ресурса.
			rdoConverter::g_error().error(pCurrRelRes->src_info(), rdo::format(_T("Релевантный ресурс '%s' не используется в образце '%s'"), pCurrRelRes->name().c_str(), name().c_str()));
		}
		m_pPatRuntime->addPreSelectRelRes(pCurrRelRes->createPreSelectRelResCalc());
	}

	if (m_useCommonChoice)
	{
		//! first
		//! Работает неправильно, а как обыкновенный first
		if (!m_pCommonChoice)
		{
			//! first
			std::vector<rdoRuntime::LPIRDOSelectResourceCommon> resSelectors;
			for (int i = 0; i < size; i++)
			{
				if (m_relResList.at(i)->m_statusBegin == rdoRuntime::RDOResource::CS_Keep || m_relResList.at(i)->m_statusBegin == rdoRuntime::RDOResource::CS_Erase || m_relResList.at(i)->m_statusBegin == rdoRuntime::RDOResource::CS_NoChange)
				{
					resSelectors.push_back(m_relResList.at(i)->createSelectResourceCommonChoiceCalc());
				}
			}
			rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOSelectResourceCommonCalc>::create(resSelectors, m_useCommonWithMax, rdoRuntime::LPRDOCalc(NULL));
			ASSERT(pCalc);
			rdoRuntime::RDOSrcInfo srcInfo(src_info());
			srcInfo.setSrcText(_T("first"));
			pCalc->setSrcInfo(srcInfo);
			addChoiceFromCalc(pCalc);
		}
		else
		{
			//! with_min/with_max
			std::vector<rdoRuntime::LPIRDOSelectResourceCommon> resSelectors;
			for (int i = 0; i < size; i++)
			{
				if (m_relResList.at(i)->m_statusBegin == rdoRuntime::RDOResource::CS_Keep || m_relResList.at(i)->m_statusBegin == rdoRuntime::RDOResource::CS_Erase || m_relResList.at(i)->m_statusBegin == rdoRuntime::RDOResource::CS_NoChange)
				{
					resSelectors.push_back(m_relResList.at(i)->createSelectResourceCommonChoiceCalc());
				}
			}
			rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOSelectResourceCommonCalc>::create(resSelectors, m_useCommonWithMax, m_pCommonChoice->createCalc());
			pCalc->setSrcInfo(m_pCommonChoice->src_info());
			addChoiceFromCalc(pCalc);
		}
	}
	else
	{
		for (int i = 0; i < size; i++)
		{
			rdoRuntime::LPRDOCalc pCalc = m_relResList.at(i)->createSelectResourceChoiceCalc();
			addChoiceFromCalc(pCalc);
		}
	}
}

// --------------------------------------------------------------------------------
// -------------------- RDOPatternIrregEvent
// --------------------------------------------------------------------------------
RDOPatternIrregEvent::RDOPatternIrregEvent(CREF(RDOParserSrcInfo) name_src_info, rbool trace)
	: RDOPATPattern(name_src_info)
{
	m_pPatRuntime = rdo::Factory<rdoRuntime::RDOPatternIrregEvent>::create(trace);
	ASSERT(m_pPatRuntime);
	m_pPatRuntime->setTraceID(Converter::s_converter()->getPAT_id());
}

void RDOPatternIrregEvent::addRelRes(CREF(RDOParserSrcInfo) rel_info, CREF(RDOParserSrcInfo) type_info, rdoRuntime::RDOResource::ConvertStatus beg, CREF(YYLTYPE) convertor_pos)
{
	beforeRelRensert(rel_info);
	if (beg == rdoRuntime::RDOResource::CS_NonExist || beg == rdoRuntime::RDOResource::CS_NoChange)
	{
		rdoConverter::g_error().error(convertor_pos, _T("Статусы конверторов NonExist и NoChange не могут быть использованы в нерегулярном событии"));
	}

	LPRDORelevantResource pRelevantResource;
	LPRDORSSResource res = Converter::s_converter()->findRSSResource(type_info.src_text());
	if (res)
	{
		switch (beg)
		{
		case rdoRuntime::RDOResource::CS_Create: rdoConverter::g_error().error(type_info, rdo::format(_T("При создания ресурса '%s' требуется указать его тип, но указан просто ресурс (%s)"), rel_info.src_text().c_str(), type_info.src_text().c_str())); break;
		case rdoRuntime::RDOResource::CS_Erase : rdoConverter::g_error().error(convertor_pos, _T("Удалять ресурсы в нерегулярном событии нельзя")); break;
		}
		pRelevantResource = rdo::Factory<RDORelevantResourceDirect>::create(rel_info, rel_res_count(), res, beg);
		ASSERT(pRelevantResource);
		rel_res_insert(pRelevantResource);
	}
	else
	{
		LPRDORTPResType pResType = Converter::s_converter()->findRTPResType(type_info.src_text());
		if (!pResType)
		{
			rdoConverter::g_error().error(type_info, rdo::format(_T("Неизвестный тип ресурса: %s"), type_info.src_text().c_str()));
		}
		switch (beg)
		{
		case rdoRuntime::RDOResource::CS_Create: if (!pResType->isTemporary()) rdoConverter::g_error().error(type_info, rdo::format(_T("Тип ресурса '%s' постоянный. Динамически создавать от него ресурсы нельзя"), type_info.src_text().c_str())); break;
		case rdoRuntime::RDOResource::CS_Keep  :
		case rdoRuntime::RDOResource::CS_Erase : rdoConverter::g_error().error(convertor_pos, _T("Статусы конверторов Keep и Erase могут быть использованы в нерегулярном событии с описателем в виде ресурса, но не типа ресурса")); break;
		}
		pRelevantResource = rdo::Factory<RDORelevantResourceByType>::create(rel_info, rel_res_count(), pResType, beg);
		ASSERT(pRelevantResource);
		rel_res_insert(pRelevantResource);
	}

	if (pRelevantResource->m_statusBegin == rdoRuntime::RDOResource::CS_Erase)
	{
		rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOEraseResRelCalc>::create(pRelevantResource->m_relResID, pRelevantResource->name());
		ASSERT(pCalc);
		rdoRuntime::RDOSrcInfo srcInfo(rel_info);
		srcInfo.setSrcText(rdo::format(_T("Удаление временного ресурса %s"), rel_info.src_text().c_str()));
		pCalc->setSrcInfo(srcInfo);
		getPatRuntime<rdoRuntime::RDOPatternIrregEvent>()->addEraseCalc(pCalc);
	}
}

void RDOPatternIrregEvent::addRelResUsage(CREF(LPRDOPATChoiceFrom) pChoiceFrom, CREF(LPRDOPATChoiceOrder) pChoiceOrder)
{
	if (pChoiceFrom->m_type != RDOPATChoiceFrom::ch_empty)
	{
		rdoConverter::g_error().error(pChoiceFrom->src_info(), _T("Релевантные ресурсы нерегулярного события нельзя выбирать с помощью Choice from или Choice NoCheck"));
	}
	if (pChoiceOrder->m_type != rdoRuntime::RDOSelectResourceCalc::order_empty)
	{
		rdoConverter::g_error().error(pChoiceFrom->src_info(), rdo::format(_T("Для релевантных ресурсов нерегулярного события нельзя использовать правило выбора '%s'"), pChoiceOrder->asString().c_str()));
	}
	m_pCurrRelRes->m_pChoiceFrom  = pChoiceFrom;
	m_pCurrRelRes->m_pChoiceOrder = pChoiceOrder;
}

rdoRuntime::LPRDOCalc RDOPATPattern::createRelRes(rbool trace) const
{
	UNUSED(trace);

	std::vector<rdoRuntime::RDOValue> params_default;
	STL_FOR_ALL_CONST(m_pCurrRelRes->getType()->getParams(), it)
	{
		if (!(*it)->getDefault()->defined())
		{
			params_default.push_back(rdoRuntime::RDOValue(0));
			if (!m_pCurrRelRes->getParamSetList().find((*it)->name()))
			{
				rdoConverter::g_error().error(m_pCurrRelRes->src_info(), rdo::format(_T("При создании ресурса необходимо определить все его параметры. Не найдено определение параметра: %s"), (*it)->name().c_str()));
			}
		}
		else
		{
			params_default.push_back((*it)->getDefault()->value());
		}
	}
	rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOCalcNop>::create();
	ASSERT(pCalc);
	rdoRuntime::RDOSrcInfo srcInfo(m_pCurrRelRes->src_info());
	srcInfo.setSrcText(rdo::format(_T("Создание временного ресурса %s"), m_pCurrRelRes->name().c_str()));
	pCalc->setSrcInfo(srcInfo);
	return pCalc;
}

tstring RDOPatternIrregEvent::getErrorMessage_NotNeedConvertor(CREF(tstring) name, rdoRuntime::RDOResource::ConvertStatus status)
{
	return rdo::format(_T("Для релевантного ресурса '%s' не требуется конвертор (Convert_event), т.к. его статус: %s"), name.c_str(), RDOPATPattern::StatusToStr(status).c_str());
}

tstring RDOPatternIrregEvent::getWarningMessage_EmptyConvertor(CREF(tstring) name, rdoRuntime::RDOResource::ConvertStatus status)
{
	return rdo::format(_T("Для релевантного ресурса '%s' указан пустой конвертор (Convert_event), хотя его статус: %s"), name.c_str(), RDOPATPattern::StatusToStr(status).c_str());
}

// --------------------------------------------------------------------------------
// -------------------- RDOPatternRule
// --------------------------------------------------------------------------------
RDOPatternRule::RDOPatternRule(CREF(RDOParserSrcInfo) name_src_info, rbool trace)
	: RDOPATPattern(name_src_info)
{
//	Converter::s_converter()->runtime()->addRuntimeRule((RDOPatternRule *)(m_pPatRuntime = new RDOPatternRule(Converter::s_converter()->runtime(), _trace))); 
	m_pPatRuntime = rdo::Factory<rdoRuntime::RDOPatternRule>::create(trace);
	ASSERT(m_pPatRuntime);
	m_pPatRuntime->setTraceID(Converter::s_converter()->getPAT_id());
}

void RDOPatternRule::addRelRes(CREF(RDOParserSrcInfo) rel_info, CREF(RDOParserSrcInfo) type_info, rdoRuntime::RDOResource::ConvertStatus beg, CREF(YYLTYPE) convertor_pos)
{
	beforeRelRensert(rel_info);
	if (beg == rdoRuntime::RDOResource::CS_NonExist)
	{
		rdoConverter::g_error().error(convertor_pos, rdo::format(_T("Нельзя использовать статус конвертора '%s' в продукционном правиле"), RDOPATPattern::StatusToStr(beg).c_str()));
	}

	LPRDORelevantResource pRelevantResource;
	LPRDORSSResource res = Converter::s_converter()->findRSSResource(type_info.src_text());
	if (res)
	{
		if (beg == rdoRuntime::RDOResource::CS_Create)
		{
			rdoConverter::g_error().error(type_info, rdo::format(_T("При создания ресурса '%s' требуется указать его тип, но указан просто ресурс (%s)"), rel_info.src_text().c_str(), type_info.src_text().c_str()));
		}
		if (beg == rdoRuntime::RDOResource::CS_Erase)
		{
			rdoConverter::g_error().error(convertor_pos, rdo::format(_T("Недопустимый статус конвертора для ресурса: %s"), RDOPATPattern::StatusToStr(beg).c_str()));
		}
		pRelevantResource = rdo::Factory<RDORelevantResourceDirect>::create(rel_info, rel_res_count(), res, beg);
		ASSERT(pRelevantResource);
		rel_res_insert(pRelevantResource);
	}
	else
	{
		LPRDORTPResType pResType = Converter::s_converter()->findRTPResType(type_info.src_text());
		if (!pResType)
		{
			rdoConverter::g_error().error(type_info, rdo::format(_T("Неизвестный тип ресурса: %s"), type_info.src_text().c_str()));
		}
		if (!pResType->isTemporary() && (beg == rdoRuntime::RDOResource::CS_Create || beg == rdoRuntime::RDOResource::CS_Erase))
		{
			rdoConverter::g_error().error(type_info, rdo::format(_T("Для создании или удалении ресурса его тип должен быть временным (temporary), а не постоянным (permanent), как у '%s'"), type_info.src_text().c_str()));
		}
		pRelevantResource = rdo::Factory<RDORelevantResourceByType>::create(rel_info, rel_res_count(), pResType, beg);
		ASSERT(pRelevantResource);
		rel_res_insert(pRelevantResource);
	}
	if (pRelevantResource->m_statusBegin == rdoRuntime::RDOResource::CS_Erase)
	{
		rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOEraseResRelCalc>::create(pRelevantResource->m_relResID, pRelevantResource->name());
		ASSERT(pCalc);
		rdoRuntime::RDOSrcInfo srcInfo(rel_info);
		srcInfo.setSrcText(rdo::format(_T("Удаление временного ресурса %s"), rel_info.src_text().c_str()));
		pCalc->setSrcInfo(srcInfo);
		getPatRuntime<rdoRuntime::RDOPatternRule>()->addEraseCalc(pCalc);
	}
}

tstring RDOPatternRule::getErrorMessage_NotNeedConvertor(CREF(tstring) name, rdoRuntime::RDOResource::ConvertStatus status)
{
	return rdo::format(_T("Для релевантного ресурса '%s' не требуется конвертор (Convert_rule), т.к. его статус: %s"), name.c_str(), RDOPATPattern::StatusToStr(status).c_str());
}

tstring RDOPatternRule::getWarningMessage_EmptyConvertor(CREF(tstring) name, rdoRuntime::RDOResource::ConvertStatus status)
{
	return rdo::format(_T("Для релевантного ресурса '%s' указан пустой конвертор (Convert_rule), хотя его статус: %s"), name.c_str(), RDOPATPattern::StatusToStr(status).c_str());
}

// --------------------------------------------------------------------------------
// -------------------- RDOPatternOperation
// --------------------------------------------------------------------------------
RDOPatternOperation::RDOPatternOperation(CREF(RDOParserSrcInfo) name_src_info, rbool trace)
	: RDOPATPattern  (name_src_info )
	, m_convertorType(convert_unknow)
{
	m_pPatRuntime = rdo::Factory<rdoRuntime::RDOPatternOperation>::create(trace);
	ASSERT(m_pPatRuntime);
	m_pPatRuntime->setTraceID(Converter::s_converter()->getPAT_id());
}

RDOPatternOperation::RDOPatternOperation(rbool trace, CREF(RDOParserSrcInfo) name_src_info)
	: RDOPATPattern  (name_src_info )
	, m_convertorType(convert_unknow)
{
	UNUSED(trace);
}

void RDOPatternOperation::rel_res_insert(CREF(LPRDORelevantResource) pRelevantResource)
{
	ASSERT(pRelevantResource);
	RDOPATPattern::rel_res_insert(pRelevantResource);
	getPatRuntime<rdoRuntime::RDOPatternOperation>()->addConvertorEndStatus(pRelevantResource->m_statusEnd);
}

void RDOPatternOperation::addRelRes(CREF(RDOParserSrcInfo) rel_info, CREF(RDOParserSrcInfo) type_info, rdoRuntime::RDOResource::ConvertStatus beg, CREF(YYLTYPE) convertor_pos)
{
	UNUSED(rel_info );
	UNUSED(type_info);
	UNUSED(beg      );

	rdoConverter::g_error().error(convertor_pos, _T("Внутренняя ошибка парсера"));
}

void RDOPatternOperation::addRelRes(CREF(RDOParserSrcInfo) rel_info, CREF(RDOParserSrcInfo) type_info, rdoRuntime::RDOResource::ConvertStatus beg, rdoRuntime::RDOResource::ConvertStatus end, CREF(YYLTYPE) convertor_begin_pos, CREF(YYLTYPE) convertor_end_pos)
{
	beforeRelRensert(rel_info);
	switch (beg)
	{
	case rdoRuntime::RDOResource::CS_Keep:
		if (end != rdoRuntime::RDOResource::CS_Keep && end != rdoRuntime::RDOResource::CS_Erase && end != rdoRuntime::RDOResource::CS_NoChange)
		{
			rdoConverter::g_error().error(convertor_end_pos, rdo::format(_T("Статус конвертора конца несовместим со статусом конвертора начала: %s и %s"), RDOPATPattern::StatusToStr(beg).c_str(), RDOPATPattern::StatusToStr(end).c_str()));
		}
		break;
	case rdoRuntime::RDOResource::CS_Create:
		if (end != rdoRuntime::RDOResource::CS_Keep && end != rdoRuntime::RDOResource::CS_Erase && end != rdoRuntime::RDOResource::CS_NoChange)
		{
			rdoConverter::g_error().error(convertor_end_pos, rdo::format(_T("Статус конвертора конца несовместим со статусом конвертора начала: %s и %s"), RDOPATPattern::StatusToStr(beg).c_str(), RDOPATPattern::StatusToStr(end).c_str()));
		}
		break;
	case rdoRuntime::RDOResource::CS_Erase:
		if (end != rdoRuntime::RDOResource::CS_NonExist)
		{
			rdoConverter::g_error().error(convertor_end_pos, rdo::format(_T("Статус конвертора конца несовместим со статусом конвертора начала: %s и %s"), RDOPATPattern::StatusToStr(beg).c_str(), RDOPATPattern::StatusToStr(end).c_str()));
		}
		break;
	case rdoRuntime::RDOResource::CS_NonExist:
		if (end != rdoRuntime::RDOResource::CS_Create)
		{
			rdoConverter::g_error().error(convertor_end_pos, rdo::format(_T("Статус конвертора конца несовместим со статусом конвертора начала: %s и %s"), RDOPATPattern::StatusToStr(beg).c_str(), RDOPATPattern::StatusToStr(end).c_str()));
		}
		break;
	case rdoRuntime::RDOResource::CS_NoChange:
		if (end != rdoRuntime::RDOResource::CS_Keep && end != rdoRuntime::RDOResource::CS_Erase && end != rdoRuntime::RDOResource::CS_NoChange)
		{
			rdoConverter::g_error().error(convertor_end_pos, rdo::format(_T("Статус конвертора конца несовместим со статусом конвертора начала: %s и %s"), RDOPATPattern::StatusToStr(beg).c_str(), RDOPATPattern::StatusToStr(end).c_str()));
		}
		break;
	};

	LPRDORelevantResource pRelevantResource;
	LPRDORSSResource res = Converter::s_converter()->findRSSResource(type_info.src_text());
	if (res)
	{
		if (beg == rdoRuntime::RDOResource::CS_Create)
		{
			rdoConverter::g_error().error(convertor_begin_pos, rdo::format(_T("При создания ресурса '%s' требуется указать его тип, но указан просто ресурс (%s)"), rel_info.src_text().c_str(), type_info.src_text().c_str()));
		}
		if (end == rdoRuntime::RDOResource::CS_Create)
		{
			rdoConverter::g_error().error(convertor_end_pos, rdo::format(_T("При создания ресурса '%s' требуется указать его тип, но указан просто ресурс (%s)"), rel_info.src_text().c_str(), type_info.src_text().c_str()));
		}
		if (beg == rdoRuntime::RDOResource::CS_Erase || beg == rdoRuntime::RDOResource::CS_NonExist)
		{
			rdoConverter::g_error().error(convertor_begin_pos, rdo::format(_T("Недопустимый статус конвертора начала для ресурса: %s"), RDOPATPattern::StatusToStr(beg).c_str()));
		}
		if (end == rdoRuntime::RDOResource::CS_Erase || end == rdoRuntime::RDOResource::CS_NonExist)
		{
			rdoConverter::g_error().error(convertor_end_pos, rdo::format(_T("Недопустимый статус конвертора конца для ресурса: %s"), RDOPATPattern::StatusToStr(end).c_str()));
		}
		pRelevantResource = rdo::Factory<RDORelevantResourceDirect>::create(rel_info, rel_res_count(), res, beg, end);
		ASSERT(pRelevantResource);
		rel_res_insert(pRelevantResource);
	}
	else
	{
		LPRDORTPResType pResType = Converter::s_converter()->findRTPResType(type_info.src_text());
		if (!pResType)
		{
			rdoConverter::g_error().error(type_info, rdo::format(_T("Неизвестный тип ресурса: %s"), type_info.src_text().c_str()));
		}
		if (pResType->isPermanent())
		{
			if (beg == rdoRuntime::RDOResource::CS_Create || beg == rdoRuntime::RDOResource::CS_Erase || beg == rdoRuntime::RDOResource::CS_NonExist)
			{
				rdoConverter::g_error().error(convertor_begin_pos, rdo::format(_T("Недопустимый статус конвертора начала для постоянного типа: %s"), RDOPATPattern::StatusToStr(beg).c_str()));
			}
			if (end == rdoRuntime::RDOResource::CS_Create || end == rdoRuntime::RDOResource::CS_Erase || end == rdoRuntime::RDOResource::CS_NonExist)
			{
				rdoConverter::g_error().error(convertor_end_pos, rdo::format(_T("Недопустимый статус конвертора конца для постоянного типа: %s"), RDOPATPattern::StatusToStr(end).c_str()));
			}
		}
		pRelevantResource = rdo::Factory<RDORelevantResourceByType>::create(rel_info, rel_res_count(), pResType, beg, end);
		ASSERT(pRelevantResource);
		rel_res_insert(pRelevantResource);
	}
	if (pRelevantResource->m_statusBegin == rdoRuntime::RDOResource::CS_Erase)
	{
		rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOEraseResRelCalc>::create(pRelevantResource->m_relResID, pRelevantResource->name());
		ASSERT(pCalc);
		rdoRuntime::RDOSrcInfo srcInfo(rel_info);
		srcInfo.setSrcText(rdo::format(_T("Удаление временного ресурса %s"), rel_info.src_text().c_str()));
		pCalc->setSrcInfo(srcInfo);
		getPatRuntime<rdoRuntime::RDOPatternOperation>()->addEraseBeginCalc(pCalc);
	}
	if (pRelevantResource->m_statusEnd == rdoRuntime::RDOResource::CS_Erase)
	{
		rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOEraseResRelCalc>::create(pRelevantResource->m_relResID, pRelevantResource->name());
		ASSERT(pCalc);
		rdoRuntime::RDOSrcInfo srcInfo(rel_info);
		srcInfo.setSrcText(rdo::format(_T("Удаление временного ресурса %s"), rel_info.src_text().c_str()));
		pCalc->setSrcInfo(srcInfo);
		getPatRuntime<rdoRuntime::RDOPatternOperation>()->addEraseEndCalc(pCalc);
	}
}

void RDOPatternOperation::addRelResConvertBeginEnd(rbool trace_begin, CREF(LPConvertCmdList) cmd_begin, rbool trace_end, CREF(LPConvertCmdList) cmd_end, CREF(YYLTYPE) convertor_begin_pos, CREF(YYLTYPE) convertor_end_pos, CREF(YYLTYPE) trace_begin_pos, CREF(YYLTYPE) trace_end_pos)
{
	if (cmd_begin)
	{
		m_convertorType = convert_begin;
		ASSERT(m_pCurrRelRes);
		addRelResConvert(trace_begin, cmd_begin, convertor_begin_pos, trace_begin_pos, m_pCurrRelRes->m_statusBegin);
		m_convertorType = convert_unknow;
	}
	if (cmd_end)
	{
		m_convertorType = convert_end;
		ASSERT(m_pCurrRelRes);
		addRelResConvert(trace_end, cmd_end, convertor_end_pos, trace_end_pos, m_pCurrRelRes->m_statusEnd);
		m_convertorType = convert_unknow;
	}
}

void RDOPatternOperation::addParamSetCalc(CREF(rdoRuntime::LPRDOCalc) pCalc)
{
	switch (m_convertorType)
	{
	case convert_begin: getPatRuntime<rdoRuntime::RDOPatternOperation>()->addConvertorBeginCalc(pCalc); break;
	case convert_end  : getPatRuntime<rdoRuntime::RDOPatternOperation>()->addConvertorEndCalc  (pCalc); break;
	default           : NEVER_REACH_HERE;
	}
}

tstring RDOPatternOperation::getErrorMessage_NotNeedConvertor(CREF(tstring) name, rdoRuntime::RDOResource::ConvertStatus status)
{
	switch (m_convertorType)
	{
	case convert_begin: return rdo::format(_T("Для релевантного ресурса '%s' не требуется конвертор начала (Convert_begin), т.к. его статус: %s"), name.c_str(), RDOPATPattern::StatusToStr(status).c_str()); break;
	case convert_end  : return rdo::format(_T("Для релевантного ресурса '%s' не требуется конвертор конца (Convert_end), т.к. его статус: %s"), name.c_str(), RDOPATPattern::StatusToStr(status).c_str()); break;
	default           : NEVER_REACH_HERE;
	}
	return tstring();
}

tstring RDOPatternOperation::getWarningMessage_EmptyConvertor(CREF(tstring) name, rdoRuntime::RDOResource::ConvertStatus status)
{
	switch (m_convertorType)
	{
	case convert_begin: return rdo::format(_T("Для релевантного ресурса '%s' указан пустой конвертор начала (Convert_begin), хотя его статус: %s"), name.c_str(), RDOPATPattern::StatusToStr(status).c_str()); break;
	case convert_end  : return rdo::format(_T("Для релевантного ресурса '%s' указан пустой конвертор конца (Convert_end), хотя его статус: %s"), name.c_str(), RDOPATPattern::StatusToStr(status).c_str()); break;
	default           : NEVER_REACH_HERE;
	}
	return tstring();
}

// --------------------------------------------------------------------------------
// -------------------- RDOPatternKeyboard
// --------------------------------------------------------------------------------
RDOPatternKeyboard::RDOPatternKeyboard(CREF(RDOParserSrcInfo) name_src_info, rbool trace)
	: RDOPatternOperation(trace, name_src_info)
{
	m_pPatRuntime = rdo::Factory<rdoRuntime::RDOPatternKeyboard>::create(trace);
	ASSERT(m_pPatRuntime);
	m_pPatRuntime->setTraceID(Converter::s_converter()->getPAT_id());
}

// --------------------------------------------------------------------------------
// -------------------- RDORelevantResource
// --------------------------------------------------------------------------------
rdoRuntime::LPRDOCalc RDORelevantResource::getChoiceCalc()
{
	if (m_pChoiceFrom && m_pChoiceFrom->m_type == rdoConverter::RDOPATChoiceFrom::ch_from)
	{
		return m_pChoiceFrom->m_pLogic->getCalc(rdoRuntime::RDOType::t_int);
	}
	return NULL;
}

rdoRuntime::LPRDOCalc RDORelevantResource::getSelectCalc()
{
	if (m_pChoiceOrder && m_pChoiceOrder->m_pArithm)
	{
		return m_pChoiceOrder->m_pArithm->createCalc(NULL);
	}
	return NULL;
}

rdoRuntime::RDOSelectResourceCalc::Type RDORelevantResource::getSelectType() const
{
	return m_pChoiceOrder ? m_pChoiceOrder->m_type : rdoRuntime::RDOSelectResourceCalc::order_empty;
}

// --------------------------------------------------------------------------------
// -------------------- RDORelevantResourceDirect - по имени ресурса
// --------------------------------------------------------------------------------
rdoRuntime::LPRDOCalc RDORelevantResourceDirect::createPreSelectRelResCalc()
{
	rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOSelectResourceDirectCalc>::create(m_relResID, m_pResource->getID());
	ASSERT(pCalc);
	rdoRuntime::RDOSrcInfo srcInfo(src_info());
	srcInfo.setSrcText(rdo::format(_T("Предварительный выбор рел. ресурса %s"), src_text().c_str()));
	pCalc->setSrcInfo(srcInfo);
	return pCalc;
}

rdoRuntime::LPRDOCalc RDORelevantResourceDirect::createSelectResourceChoiceCalc()
{
	rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOSelectResourceDirectCalc>::create(m_relResID, m_pResource->getID(), getChoiceCalc(), getSelectCalc(), getSelectType());
	pCalc->setSrcInfo(m_pChoiceFrom->src_info());
	return pCalc;
}

rdoRuntime::LPRDOCalc RDORelevantResourceDirect::createSelectFirstResourceChoiceCalc()
{
	return rdo::Factory<rdoRuntime::RDOSelectResourceDirectCalc>::create(m_relResID, m_pResource->getID(), getChoiceCalc());
}

rdoRuntime::LPIRDOSelectResourceCommon RDORelevantResourceDirect::createSelectResourceCommonChoiceCalc()
{
	rdoRuntime::LPRDOSelectResourceDirectCommonCalc pDirectCommonCalc = rdo::Factory<rdoRuntime::RDOSelectResourceDirectCommonCalc>::create(m_relResID, m_pResource->getID(), getChoiceCalc());
	rdoRuntime::LPIRDOSelectResourceCommon pSelectResourceCommon = pDirectCommonCalc.interface_cast<rdoRuntime::IRDOSelectResourceCommon>();
	ASSERT(pSelectResourceCommon);
	return pSelectResourceCommon;
}

LPRDORTPResType RDORelevantResourceDirect::getType() const 
{ 
	return m_pResource->getType(); 
}

// --------------------------------------------------------------------------------
// -------------------- RDORelevantResourceByType - по имени типа
// --------------------------------------------------------------------------------
rdoRuntime::LPRDOCalc RDORelevantResourceByType::createPreSelectRelResCalc()
{
	if ((m_statusBegin != rdoRuntime::RDOResource::CS_Create) && (m_statusEnd != rdoRuntime::RDOResource::CS_Create))
	{
		rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOSelectResourceByTypeCalc>::create(m_relResID, m_pResType->getNumber());
		ASSERT(pCalc);
		rdoRuntime::RDOSrcInfo srcInfo(src_info());
		srcInfo.setSrcText(rdo::format(_T("Предварительный выбор рел. ресурса %s"), src_text().c_str()));
		pCalc->setSrcInfo(srcInfo);
		return pCalc;
	}
	else
	{
		if (m_statusBegin == rdoRuntime::RDOResource::CS_NonExist)
		{
			return rdo::Factory<rdoRuntime::RDOSelectResourceNonExistCalc>::create(m_relResID);
		}
		else
		{
			rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOCalcConst>::create(1);
			ASSERT(pCalc);
			rdoRuntime::RDOSrcInfo srcInfo(src_info());
			srcInfo.setSrcText(rdo::format(_T("Предварительный выбор рел. ресурса перед созданием %s"), src_text().c_str()));
			pCalc->setSrcInfo(srcInfo);
			return pCalc;
		}
	}
}

rdoRuntime::LPRDOCalc RDORelevantResourceByType::createSelectResourceChoiceCalc()
{
	if ((m_statusBegin != rdoRuntime::RDOResource::CS_Create) && (m_statusEnd != rdoRuntime::RDOResource::CS_Create))
	{
		rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOSelectResourceByTypeCalc>::create(m_relResID, m_pResType->getNumber(), getChoiceCalc(), getSelectCalc(), getSelectType());
		pCalc->setSrcInfo(m_pChoiceFrom->src_info());
		return pCalc;
	}
	else
	{
		if (m_statusBegin == rdoRuntime::RDOResource::CS_NonExist)
		{
			return rdo::Factory<rdoRuntime::RDOSelectResourceNonExistCalc>::create(m_relResID);
		}
		else
		{
			rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOCalcConst>::create(1);
			ASSERT(pCalc);
			rdoRuntime::RDOSrcInfo srcInfo(src_info());
			srcInfo.setSrcText(rdo::format(_T("Перед созданием рел. ресурса %s"), src_text().c_str()));
			pCalc->setSrcInfo(srcInfo);
			return pCalc;
		}
	}
}

rdoRuntime::LPRDOCalc RDORelevantResourceByType::createSelectFirstResourceChoiceCalc()
{
	if ((m_statusBegin != rdoRuntime::RDOResource::CS_Create) && (m_statusEnd != rdoRuntime::RDOResource::CS_Create))
	{
		return rdo::Factory<rdoRuntime::RDOSelectResourceByTypeCalc>::create(m_relResID, m_pResType->getNumber(), getChoiceCalc());
	}
	else
	{
		if (m_statusBegin == rdoRuntime::RDOResource::CS_NonExist)
		{
			return rdo::Factory<rdoRuntime::RDOSelectResourceNonExistCalc>::create(m_relResID);
		}
		else
		{
			return rdo::Factory<rdoRuntime::RDOCalcConst>::create(1);
		}
	}
}

rdoRuntime::LPIRDOSelectResourceCommon RDORelevantResourceByType::createSelectResourceCommonChoiceCalc()
{
	rdoRuntime::LPRDOSelectResourceByTypeCommonCalc pByTypeCommonCalc = rdo::Factory<rdoRuntime::RDOSelectResourceByTypeCommonCalc>::create(m_relResID, m_pResType->getNumber(), getChoiceCalc());
	rdoRuntime::LPIRDOSelectResourceCommon pSelectResourceCommon = pByTypeCommonCalc.interface_cast<rdoRuntime::IRDOSelectResourceCommon>();
	ASSERT(pSelectResourceCommon);
	return pSelectResourceCommon;
}

CLOSE_RDO_CONVERTER_NAMESPACE
