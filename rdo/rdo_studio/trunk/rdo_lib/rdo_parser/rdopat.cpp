/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdopat.cpp
 * author   : Александ Барс, Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_parser/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdopat.h"
#include "rdo_lib/rdo_parser/rdoparser.h"
#include "rdo_lib/rdo_parser/rdortp.h"
#include "rdo_lib/rdo_parser/rdoparser_lexer.h"
#include "rdo_lib/rdo_parser/rdo_type_range.h"
#include "rdo_lib/rdo_runtime/rdo_pattern.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

int patlex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void paterror(PTR(char) mes)
{}

int pat_preparse_lex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}
void pat_preparse_error(PTR(char) mes)
{}

// ----------------------------------------------------------------------------
// ---------- RDOPATPattern
// ----------------------------------------------------------------------------
RDOPATPattern::RDOPATPattern(PTR(RDOParser) pParser, CREF(RDOParserSrcInfo) name_src_info)
	: RDOParserObject     (pParser      )
	, RDOParserSrcInfo    (name_src_info)
	, m_pCommonChoice     (NULL         )
	, m_pPatRuntime       (NULL         )
	, m_pCurrRelRes       (NULL         )
	, m_currentRelResIndex(0            )
	, m_useCommonChoice   (false        )
{
	CPTR(RDOPATPattern) pattern = parser()->findPATPattern(src_info().src_text());
	if (pattern)
	{
		parser()->error().push_only(src_info(), rdo::format(_T("Паттерн '%s' уже существует"), name().c_str()));
		parser()->error().push_only(pattern->src_info(), _T("См. первое определение"));
		parser()->error().push_done();
	}
	parser()->insertPATPattern(this);
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
	parser()->error().error(convertor_pos, rdo::format(_T("Неверный статус конвертора: %s"), value.c_str()));
	return rdoRuntime::RDOResource::CS_Keep;
}

void RDOPATPattern::beforeRelRensert(CREF(RDOParserSrcInfo) rel_info)
{
	if (findRelevantResource(rel_info.src_text()))
	{
		parser()->error().error(rel_info, rdo::format(_T("Релевантный ресурс '%s' уже определен"), rel_info.src_text().c_str()));
	}
}

void RDOPATPattern::rel_res_insert(PTR(RDORelevantResource) pRelevantResource)
{
	switch (getType())
	{
	case PT_IE       : static_cast<PTR(rdoRuntime::RDOPatternIrregEvent)>(getPatRuntime())->addConvertorStatus     (pRelevantResource->m_statusBegin); break;
	case PT_Event    : static_cast<PTR(rdoRuntime::RDOPatternEvent)>     (getPatRuntime())->addConvertorStatus     (pRelevantResource->m_statusBegin); break;
	case PT_Rule     : static_cast<PTR(rdoRuntime::RDOPatternRule)>      (getPatRuntime())->addConvertorStatus     (pRelevantResource->m_statusBegin); break;
	case PT_Operation: static_cast<PTR(rdoRuntime::RDOPatternOperation)> (getPatRuntime())->addConvertorBeginStatus(pRelevantResource->m_statusBegin); break;
	case PT_Keyboard : static_cast<PTR(rdoRuntime::RDOPatternKeyboard)>  (getPatRuntime())->addConvertorBeginStatus(pRelevantResource->m_statusBegin); break;
	default          : parser()->error().error(src_info(), _T("Неизвестный тип образца"));
	}
	m_relResList.push_back(pRelevantResource);
}

void RDOPATPattern::addRelResConvert(rbool trace, CREF(LPConvertCmdList) commands, CREF(YYLTYPE) convertor_pos, CREF(YYLTYPE) trace_pos, rdoRuntime::RDOResource::ConvertStatus status)
{
	if (status == rdoRuntime::RDOResource::CS_NoChange || status == rdoRuntime::RDOResource::CS_NonExist)
	{
		parser()->error().error(convertor_pos, getErrorMessage_NotNeedConvertor(m_pCurrRelRes->name(), status));
	}

	if (status == rdoRuntime::RDOResource::CS_Create)
	{
		addParamSetCalc(createRelRes(trace));
	}
	else
	{
		if (trace)
		{
			parser()->error().error(trace_pos, _T("Признак трассировки в данном месте возможен только для создаваемого ресурса"));
		}
	}

	if (commands->commands().empty() && status == rdoRuntime::RDOResource::CS_Keep)
	{
		parser()->error().warning(convertor_pos, getWarningMessage_EmptyConvertor(m_pCurrRelRes->name(), status));
	}

	STL_FOR_ALL_CONST(ConvertCmdList::CalcList, commands->commands(), cmdIt)
		addParamSetCalc(*cmdIt);

	ASSERT(m_pCurrRelRes);
	m_pCurrRelRes->getParamSetList().reset();
}

void RDOPATPattern::addParamSetCalc(CREF(rdoRuntime::LPRDOCalc) pCalc)
{
	switch (getType())
	{
	case PT_IE       : static_cast<PTR(rdoRuntime::RDOPatternIrregEvent)>(getPatRuntime())->addConvertorCalc(pCalc); break;
	case PT_Event    : static_cast<PTR(rdoRuntime::RDOPatternEvent)>     (getPatRuntime())->addConvertorCalc(pCalc); break;
	case PT_Rule     : static_cast<PTR(rdoRuntime::RDOPatternRule)>      (getPatRuntime())->addConvertorCalc(pCalc); break;
	case PT_Operation: NEVER_REACH_HERE;
	case PT_Keyboard : NEVER_REACH_HERE;
	default          : parser()->error().error(src_info(), _T("Неизвестный тип образца"));
	}
}

tstring RDOPATPattern::getPatternId() const
{ 
	return m_pPatRuntime->traceId(); 
}

void RDOPATPattern::writeModelStructure(REF(std::ostream) stream) const
{
	stream << getPatternId() << " " << name() << " " << getModelStructureLetter() << " " << m_relResList.size();
	STL_FOR_ALL_CONST(RelResList, m_relResList, it)
		stream << " " << (*it)->getType()->getNumber();

	stream << std::endl;
}

CPTR(RDOFUNFunctionParam) RDOPATPattern::findPATPatternParam(CREF(tstring) paramName) const
{
	ParamList::const_iterator it = std::find_if(m_paramList.begin(), m_paramList.end(), compareName<RDOFUNFunctionParam>(paramName));
	return it != m_paramList.end() ? (*it) : NULL;
}

CPTR(RDORelevantResource) RDOPATPattern::findRelevantResource(CREF(tstring) resName) const
{
	RelResList::const_iterator it = std::find_if(m_relResList.begin(), m_relResList.end(), compareName<RDORelevantResource>(resName));
	return it != m_relResList.end() ? (*it) : NULL;
}

int RDOPATPattern::findPATPatternParamNum(CREF(tstring) paramName) const
{
	ParamList::const_iterator it = std::find_if(m_paramList.begin(), m_paramList.end(), compareName<RDOFUNFunctionParam>(paramName));
	return it != m_paramList.end() ? it - m_paramList.begin() : -1;
}

int RDOPATPattern::findRelevantResourceNum(CREF(tstring) resName) const
{
	RelResList::const_iterator it = std::find_if(m_relResList.begin(), m_relResList.end(), compareName<RDORelevantResource>(resName));
	return it != m_relResList.end() ? it - m_relResList.begin() : -1;
}

void RDOPATPattern::add(PTR(RDOFUNFunctionParam) const _param)
{
	CPTR(RDOFUNFunctionParam) param = findPATPatternParam(_param->name());
	if (param)
	{
		parser()->error().push_only(_param->src_info(), rdo::format(_T("Параметр '%s' уже определен"), _param->src_text().c_str()));
		parser()->error().push_only(param->src_info(), _T("См. первое определение"));
		parser()->error().push_done();
	}
	m_paramList.push_back(_param);
}

void RDOPATPattern::setCommonChoiceFirst()
{
	m_useCommonChoice = true;
	m_pCommonChoice   = NULL;
//	parser()->error().error(_T("Вызывать нельзя, т.к. в сообщениях об ошибках используется m_pCommonChoice"));
}

void RDOPATPattern::setCommonChoiceWithMin(PTR(RDOFUNArithm) arithm)
{
	m_useCommonChoice  = true;
	m_useCommonWithMax = false;
	m_pCommonChoice    = arithm;
}

void RDOPATPattern::setCommonChoiceWithMax(PTR(RDOFUNArithm) arithm)
{
	m_useCommonChoice  = true;
	m_useCommonWithMax = true;
	m_pCommonChoice    = arithm;
}

void RDOPATPattern::setTime(PTR(RDOFUNArithm) arithm)
{ 
	switch (getType())
	{
	case PT_IE       : static_cast<PTR(rdoRuntime::RDOPatternIrregEvent)>(getPatRuntime())->setTime(arithm->createCalc(NULL)); break;
	case PT_Operation: static_cast<PTR(rdoRuntime::RDOPatternOperation)> (getPatRuntime())->setTime(arithm->createCalc(NULL)); break;
	case PT_Keyboard : static_cast<PTR(rdoRuntime::RDOPatternKeyboard)>  (getPatRuntime())->setTime(arithm->createCalc(NULL)); break;
	default          : parser()->error().error(src_info(), rdo::format(_T("Для образца типа %s недопустимо использование выражения времени"), typeToString(getType()).c_str()));
	}
}

void RDOPATPattern::addChoiceFromCalc(CREF(rdoRuntime::LPRDOCalc) pCalc)
{
	switch (getType())
	{
	case PT_IE       : static_cast<PTR(rdoRuntime::RDOPatternIrregEvent)>(getPatRuntime())->addPreSelectRelRes(pCalc); break;
	case PT_Event    : static_cast<PTR(rdoRuntime::RDOPatternEvent)>     (getPatRuntime())->addPreSelectRelRes(pCalc); break;
	case PT_Rule     : static_cast<PTR(rdoRuntime::RDOPatternRule)>      (getPatRuntime())->addChoiceFromCalc (pCalc); break;
	case PT_Operation: static_cast<PTR(rdoRuntime::RDOPatternOperation)> (getPatRuntime())->addChoiceFromCalc (pCalc); break;
	case PT_Keyboard : static_cast<PTR(rdoRuntime::RDOPatternKeyboard)>  (getPatRuntime())->addChoiceFromCalc (pCalc); break;
	default          : parser()->error().error(src_info(), rdo::format(_T("Для образца типа %s недопустимо использование условния выбора"), typeToString(getType()).c_str()));
	}
}

void RDOPATPattern::addRelResBody(CREF(RDOParserSrcInfo) body_name)
{ 
	RelResList::const_iterator it = std::find_if(m_relResList.begin(), m_relResList.end(), compareName<RDORelevantResource>(body_name.src_text()));
	if (it == m_relResList.end())
	{
		parser()->error().error(body_name.src_info(), rdo::format(_T("Неизвестный релевантный ресурс: %s"), body_name.src_text().c_str()));
	}
	if (findRelevantResourceNum(body_name.src_text()) != m_currentRelResIndex)
	{
		tstring rel_res_waiting = m_currentRelResIndex < m_relResList.size() ? m_relResList[m_currentRelResIndex]->name().c_str() : _T("");
		parser()->error().error(body_name.src_info(), rdo::format(_T("Ожидается описание релевантного ресурса '%s', вместо него найдено: %s"), rel_res_waiting.c_str(), body_name.src_text().c_str()));
	}
	if ((*it)->m_alreadyHaveConverter)
	{
		parser()->error().error(body_name.src_info(), rdo::format(_T("Релевантный ресурс уже используется: %s"), body_name.src_text().c_str()));
	}
	m_pCurrRelRes = (*it);
	m_pCurrRelRes->m_bodySrcInfo = body_name;
	m_pCurrRelRes->m_alreadyHaveConverter = true;
	m_currentRelResIndex++;
}

void RDOPATPattern::addRelResUsage(PTR(RDOPATChoiceFrom) choice_from, PTR(RDOPATChoiceOrder) choice_order)
{
	if (!m_useCommonChoice)
	{
		if (choice_order->m_type == rdoRuntime::RDOSelectResourceCalc::order_empty)
		{
			if ((m_pCurrRelRes->m_statusBegin != rdoRuntime::RDOResource::CS_Create) && (m_pCurrRelRes->m_statusEnd != rdoRuntime::RDOResource::CS_Create))
			{
//				choice_order->m_type = rdoRuntime::RDOSelectResourceCalc::order_first;
			}
		}
		else if (m_pCurrRelRes->isDirect())
		{
			parser()->error().warning(choice_order->src_info(), rdo::format(_T("Правило выбора '%s' релевантного ресурса '%s' не имеет смысла, т.к. релевантный ресурс определен через имя, а не тип, и не может быть связан с каким-либо другим ресурсом"), choice_order->src_text().c_str(), m_pCurrRelRes->name().c_str()));
		}
	}
	else
	{
		if (choice_order->m_type != rdoRuntime::RDOSelectResourceCalc::order_empty)
		{
			parser()->error().push_only(choice_order->src_info(), _T("Нельзя указать способ выбора релевантного ресурса, т.к. используется единый для всех релевантных ресурсов способ, указанный до ключевого слова $Body"));
			if (m_pCommonChoice)
			{
				parser()->error().push_only(m_pCommonChoice->src_info(), rdo::format(_T("См. '%s'"), m_pCommonChoice->src_text().c_str()));
			}
			parser()->error().push_done();
		}
	}

	if ((m_pCurrRelRes->m_statusBegin == rdoRuntime::RDOResource::CS_Create) || (m_pCurrRelRes->m_statusEnd == rdoRuntime::RDOResource::CS_Create))
	{
		if (choice_from->m_type != RDOPATChoiceFrom::ch_empty)
		{
			parser()->error().error(choice_from->src_info(), _T("Релевантный ресурс создается, для него нельзя использовать Choice from или Choice NoCheck"));
		}
		if (choice_order->m_type != rdoRuntime::RDOSelectResourceCalc::order_empty)
		{
			parser()->error().error(choice_order->src_info(), rdo::format(_T("Релевантный ресурс создается, для него нельзя использовать правило выбора '%s'"), choice_order->asString().c_str()));
		}
	}

	m_pCurrRelRes->m_pChoiceFrom  = choice_from;
	m_pCurrRelRes->m_pChoiceOrder = choice_order;
}

void RDOPATPattern::end()
{
	int size = m_relResList.size();
	for (int i = 0; i < size; i++)
	{
		PTR(RDORelevantResource) m_pCurrRelRes = m_relResList.at(i);
		if (!m_pCurrRelRes->m_alreadyHaveConverter)
		{
			//! TODO: А почему нельзя сделать warning ? Возможно, есть жесткое требование недопустить пустого рел. ресурса.
			parser()->error().error(m_pCurrRelRes->src_info(), rdo::format(_T("Релевантный ресурс '%s' не используется в образце '%s'"), m_pCurrRelRes->name().c_str(), name().c_str()));
		}
		m_pPatRuntime->addPreSelectRelRes(m_pCurrRelRes->createPreSelectRelResCalc());
	}

	if (m_useCommonChoice)
	{
		//! first
		//! Работает неправильно, а как обыкновенный first
		if (m_pCommonChoice == NULL)
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
			rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOSelectResourceCommonCalc>::create(resSelectors, m_useCommonWithMax, rdo::smart_ptr_null());
			pCalc->setSrcInfo(src_info() );
			pCalc->setSrcText(_T("first"));
			addChoiceFromCalc(pCalc      );
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

// ----------------------------------------------------------------------------
// ---------- RDOPatternIrregEvent
// ----------------------------------------------------------------------------
RDOPatternIrregEvent::RDOPatternIrregEvent(PTR(RDOParser) pParser, CREF(RDOParserSrcInfo) name_src_info, rbool trace)
	: RDOPATPattern(pParser, name_src_info)
{ 
	m_pPatRuntime = new rdoRuntime::RDOPatternIrregEvent(parser()->runtime(), trace); 
	m_pPatRuntime->setTraceID(parser()->getPAT_id());
}

void RDOPatternIrregEvent::addRelRes(CREF(RDOParserSrcInfo) rel_info, CREF(RDOParserSrcInfo) type_info, rdoRuntime::RDOResource::ConvertStatus beg, CREF(YYLTYPE) convertor_pos)
{
	beforeRelRensert(rel_info);
	if (beg == rdoRuntime::RDOResource::CS_NonExist || beg == rdoRuntime::RDOResource::CS_NoChange)
	{
		parser()->error().error(convertor_pos, _T("Статусы конверторов NonExist и NoChange не могут быть использованы в нерегулярном событии"));
	}

	PTR(RDORelevantResource) m_relResList = NULL;
	CPTRC(RDORSSResource)    res    = parser()->findRSSResource(type_info.src_text());
	if (res)
	{
		switch (beg)
		{
		case rdoRuntime::RDOResource::CS_Create: parser()->error().error(type_info, rdo::format(_T("При создания ресурса '%s' требуется указать его тип, но указан просто ресурс (%s)"), rel_info.src_text().c_str(), type_info.src_text().c_str())); break;
		case rdoRuntime::RDOResource::CS_Erase : parser()->error().error(convertor_pos, _T("Удалять ресурсы в нерегулярном событии нельзя")); break;
		}
		m_relResList = new RDORelevantResourceDirect(this, rel_info, rel_res_count(), res, beg);
		rel_res_insert(m_relResList);
	}
	else
	{
		LPRDORTPResType pResType = parser()->findRTPResType(type_info.src_text());
		if (!pResType)
		{
			parser()->error().error(type_info, rdo::format(_T("Неизвестный тип ресурса: %s"), type_info.src_text().c_str()));
		}
		switch (beg)
		{
		case rdoRuntime::RDOResource::CS_Create: if (!pResType->isTemporary()) parser()->error().error(type_info, rdo::format(_T("Тип ресурса '%s' постоянный. Динамически создавать от него ресурсы нельзя"), type_info.src_text().c_str())); break;
		case rdoRuntime::RDOResource::CS_Keep  :
		case rdoRuntime::RDOResource::CS_Erase : parser()->error().error(convertor_pos, _T("Статусы конверторов Keep и Erase могут быть использованы в нерегулярном событии с описателем в виде ресурса, но не типа ресурса")); break;
		}
		m_relResList = new RDORelevantResourceByType(this, rel_info, rel_res_count(), pResType, beg);
		rel_res_insert(m_relResList);
	}
	if (m_relResList->m_statusBegin == rdoRuntime::RDOResource::CS_Erase)
	{
		rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOCalcEraseRes>::create(m_relResList->m_relResID, m_relResList->name());
		pCalc->setSrcInfo(rel_info);
		pCalc->setSrcText(rdo::format(_T("Удаление временного ресурса %s"), rel_info.src_text().c_str()));
		static_cast<PTR(rdoRuntime::RDOPatternIrregEvent)>(getPatRuntime())->addEraseCalc(pCalc);
	}
}

void RDOPatternIrregEvent::addRelResUsage(PTR(RDOPATChoiceFrom) choice_from, PTR(RDOPATChoiceOrder) choice_order)
{
	if (choice_from->m_type != RDOPATChoiceFrom::ch_empty)
	{
		parser()->error().error(choice_from->src_info(), _T("Релевантные ресурсы нерегулярного события нельзя выбирать с помощью Choice from или Choice NoCheck"));
	}
	if (choice_order->m_type != rdoRuntime::RDOSelectResourceCalc::order_empty)
	{
		parser()->error().error(choice_from->src_info(), rdo::format(_T("Для релевантных ресурсов нерегулярного события нельзя использовать правило выбора '%s'"), choice_order->asString().c_str()));
	}
	m_pCurrRelRes->m_pChoiceFrom  = choice_from;
	m_pCurrRelRes->m_pChoiceOrder = choice_order;
}

rdoRuntime::LPRDOCalc RDOPATPattern::createRelRes(rbool trace) const
{
	std::vector<rdoRuntime::RDOValue> params_default;
	STL_FOR_ALL_CONST(RDORTPResType::ParamList, m_pCurrRelRes->getType()->getParams(), it)
	{
		if (!(*it)->getDefault().defined())
		{
			params_default.push_back(rdoRuntime::RDOValue(0));
			if (!m_pCurrRelRes->getParamSetList().find((*it)->name()))
			{
				parser()->error().error(m_pCurrRelRes->src_info(), rdo::format(_T("При создании ресурса необходимо определить все его параметры. Не найдено определение параметра: %s"), (*it)->name().c_str()));
			}
		}
		else
		{
			params_default.push_back((*it)->getDefault().value());
		}
	}
	rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOCalcCreateEmptyResource>::create(m_pCurrRelRes->getType()->getNumber(), trace, params_default, m_pCurrRelRes->m_relResID);
	pCalc->setSrcInfo(m_pCurrRelRes->src_info());
	pCalc->setSrcText(rdo::format(_T("Создание временного ресурса %s"), m_pCurrRelRes->name().c_str()));
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

// ----------------------------------------------------------------------------
// ---------- RDOPatternEvent
// ----------------------------------------------------------------------------
RDOPatternEvent::RDOPatternEvent(PTR(RDOParser) pParser, CREF(RDOParserSrcInfo) name_src_info, rbool trace)
	: RDOPATPattern(pParser, name_src_info)
{ 
	m_pPatRuntime = new rdoRuntime::RDOPatternEvent(parser()->runtime(), trace); 
	m_pPatRuntime->setTraceID(parser()->getPAT_id());
}

void RDOPatternEvent::addRelRes(CREF(RDOParserSrcInfo) rel_info, CREF(RDOParserSrcInfo) type_info, rdoRuntime::RDOResource::ConvertStatus beg, CREF(YYLTYPE) convertor_pos)
{
	beforeRelRensert(rel_info);
	if (beg == rdoRuntime::RDOResource::CS_NonExist || beg == rdoRuntime::RDOResource::CS_NoChange)
	{
		parser()->error().error(convertor_pos, _T("Статусы конверторов NonExist и NoChange не могут быть использованы в событии"));
	}

	PTR(RDORelevantResource) m_relResList = NULL;
	CPTRC(RDORSSResource)    res    = parser()->findRSSResource(type_info.src_text());
	if (res)
	{
		switch (beg)
		{
		case rdoRuntime::RDOResource::CS_Create: parser()->error().error(type_info, rdo::format(_T("При создания ресурса '%s' требуется указать его тип, но указан просто ресурс (%s)"), rel_info.src_text().c_str(), type_info.src_text().c_str())); break;
		case rdoRuntime::RDOResource::CS_Erase : parser()->error().error(convertor_pos, _T("Удалять ресурсы в событии нельзя")); break;
		}
		m_relResList = new RDORelevantResourceDirect(this, rel_info, rel_res_count(), res, beg);
		rel_res_insert(m_relResList);
	}
	else
	{
		LPRDORTPResType pResType = parser()->findRTPResType(type_info.src_text());
		if (!pResType)
		{
			parser()->error().error(type_info, rdo::format(_T("Неизвестный тип ресурса: %s"), type_info.src_text().c_str()));
		}
		switch (beg)
		{
		case rdoRuntime::RDOResource::CS_Create: if (!pResType->isTemporary()) parser()->error().error(type_info, rdo::format(_T("Тип ресурса '%s' постоянный. Динамически создавать от него ресурсы нельзя"), type_info.src_text().c_str())); break;
		case rdoRuntime::RDOResource::CS_Keep  :
		case rdoRuntime::RDOResource::CS_Erase : parser()->error().error(convertor_pos, _T("Статусы конверторов Keep и Erase могут быть использованы в событии с описателем в виде ресурса, но не типа ресурса")); break;
		}
		m_relResList = new RDORelevantResourceByType(this, rel_info, rel_res_count(), pResType, beg);
		rel_res_insert(m_relResList);
	}
	if (m_relResList->m_statusBegin == rdoRuntime::RDOResource::CS_Erase)
	{
		rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOCalcEraseRes>::create(m_relResList->m_relResID, m_relResList->name());
		pCalc->setSrcInfo(rel_info);
		pCalc->setSrcText(rdo::format(_T("Удаление временного ресурса %s"), rel_info.src_text().c_str()));
		static_cast<PTR(rdoRuntime::RDOPatternIrregEvent)>(getPatRuntime())->addEraseCalc(pCalc);
	}
}

void RDOPatternEvent::addRelResUsage(PTR(RDOPATChoiceFrom) choice_from, PTR(RDOPATChoiceOrder) choice_order)
{
	if (choice_from->m_type != RDOPATChoiceFrom::ch_empty)
	{
		parser()->error().error(choice_from->src_info(), _T("Релевантные ресурсы события нельзя выбирать с помощью Choice from или Choice NoCheck"));
	}
	if (choice_order->m_type != rdoRuntime::RDOSelectResourceCalc::order_empty)
	{
		parser()->error().error(choice_from->src_info(), rdo::format(_T("Для релевантных ресурсов события нельзя использовать правило выбора '%s'"), choice_order->asString().c_str()));
	}
	m_pCurrRelRes->m_pChoiceFrom  = choice_from;
	m_pCurrRelRes->m_pChoiceOrder = choice_order;
}

tstring RDOPatternEvent::getErrorMessage_NotNeedConvertor(CREF(tstring) name, rdoRuntime::RDOResource::ConvertStatus status)
{
	return rdo::format(_T("Для релевантного ресурса '%s' не требуется конвертор (Convert_event), т.к. его статус: %s"), name.c_str(), RDOPATPattern::StatusToStr(status).c_str());
}

tstring RDOPatternEvent::getWarningMessage_EmptyConvertor(CREF(tstring) name, rdoRuntime::RDOResource::ConvertStatus status)
{
	return rdo::format(_T("Для релевантного ресурса '%s' указан пустой конвертор (Convert_event), хотя его статус: %s"), name.c_str(), RDOPATPattern::StatusToStr(status).c_str());
}

// ----------------------------------------------------------------------------
// ---------- RDOPatternRule
// ----------------------------------------------------------------------------
RDOPatternRule::RDOPatternRule(PTR(RDOParser) pParser, CREF(RDOParserSrcInfo) name_src_info, rbool trace)
	: RDOPATPattern(pParser, name_src_info)
{ 
//	parser()->runtime()->addRuntimeRule((RDOPatternRule *)(m_pPatRuntime = new RDOPatternRule(parser()->runtime(), _trace))); 
	m_pPatRuntime = new rdoRuntime::RDOPatternRule(parser()->runtime(), trace);
	m_pPatRuntime->setTraceID(parser()->getPAT_id());
}

void RDOPatternRule::addRelRes(CREF(RDOParserSrcInfo) rel_info, CREF(RDOParserSrcInfo) type_info, rdoRuntime::RDOResource::ConvertStatus beg, CREF(YYLTYPE) convertor_pos)
{
	beforeRelRensert(rel_info);
	if (beg == rdoRuntime::RDOResource::CS_NonExist)
	{
		parser()->error().error(convertor_pos, rdo::format(_T("Нельзя использовать статус конвертора '%s' в продукционном правиле"), RDOPATPattern::StatusToStr(beg).c_str()));
	}

	PTR(RDORelevantResource) m_relResList = NULL;
	CPTRC(RDORSSResource)    res    = parser()->findRSSResource(type_info.src_text());
	if (res)
	{
		if (beg == rdoRuntime::RDOResource::CS_Create)
		{
			parser()->error().error(type_info, rdo::format(_T("При создания ресурса '%s' требуется указать его тип, но указан просто ресурс (%s)"), rel_info.src_text().c_str(), type_info.src_text().c_str()));
		}
		if (beg == rdoRuntime::RDOResource::CS_Erase)
		{
			parser()->error().error(convertor_pos, rdo::format(_T("Недопустимый статус конвертора для ресурса: %s"), RDOPATPattern::StatusToStr(beg).c_str()));
		}
		m_relResList = new RDORelevantResourceDirect(this, rel_info, rel_res_count(), res, beg);
		rel_res_insert(m_relResList);
	}
	else
	{
		LPRDORTPResType pResType = parser()->findRTPResType(type_info.src_text());
		if (!pResType)
		{
			parser()->error().error(type_info, rdo::format(_T("Неизвестный тип ресурса: %s"), type_info.src_text().c_str()));
		}
		if (!pResType->isTemporary() && (beg == rdoRuntime::RDOResource::CS_Create || beg == rdoRuntime::RDOResource::CS_Erase))
		{
			parser()->error().error(type_info, rdo::format(_T("Для создании или удалении ресурса его тип должен быть временным (temporary), а не постоянным (permanent), как у '%s'"), type_info.src_text().c_str()));
		}
		m_relResList = new RDORelevantResourceByType(this, rel_info, rel_res_count(), pResType, beg);
		rel_res_insert(m_relResList);
	}
	if (m_relResList->m_statusBegin == rdoRuntime::RDOResource::CS_Erase)
	{
		rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOCalcEraseRes>::create(m_relResList->m_relResID, m_relResList->name());
		pCalc->setSrcInfo(rel_info);
		pCalc->setSrcText(rdo::format(_T("Удаление временного ресурса %s"), rel_info.src_text().c_str()));
		static_cast<PTR(rdoRuntime::RDOPatternRule)>(getPatRuntime())->addEraseCalc(pCalc);
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

// ----------------------------------------------------------------------------
// ---------- RDOPatternOperation
// ----------------------------------------------------------------------------
RDOPatternOperation::RDOPatternOperation(PTR(RDOParser) pParser, CREF(RDOParserSrcInfo) name_src_info, rbool trace)
	: RDOPATPattern  (pParser, name_src_info)
	, m_convertorType(convert_unknow        )
{ 
	m_pPatRuntime = new rdoRuntime::RDOPatternOperation(parser()->runtime(), trace);
	m_pPatRuntime->setTraceID(parser()->getPAT_id());
}

RDOPatternOperation::RDOPatternOperation(PTR(RDOParser) pParser, rbool trace, CREF(RDOParserSrcInfo) name_src_info)
	: RDOPATPattern  (pParser, name_src_info)
	, m_convertorType(convert_unknow        )
{}

void RDOPatternOperation::rel_res_insert(PTR(RDORelevantResource) pRelevantResource)
{
	RDOPATPattern::rel_res_insert(pRelevantResource);
	static_cast<PTR(rdoRuntime::RDOPatternOperation)>(getPatRuntime())->addConvertorEndStatus(pRelevantResource->m_statusEnd);
}

void RDOPatternOperation::addRelRes(CREF(RDOParserSrcInfo) rel_info, CREF(RDOParserSrcInfo) type_info, rdoRuntime::RDOResource::ConvertStatus beg, CREF(YYLTYPE) convertor_pos)
{
	parser()->error().error(convertor_pos, _T("Внутренняя ошибка парсера"));
}

void RDOPatternOperation::addRelRes(CREF(RDOParserSrcInfo) rel_info, CREF(RDOParserSrcInfo) type_info, rdoRuntime::RDOResource::ConvertStatus beg, rdoRuntime::RDOResource::ConvertStatus end, CREF(YYLTYPE) convertor_begin_pos, CREF(YYLTYPE) convertor_end_pos)
{
	beforeRelRensert(rel_info);
	switch (beg)
	{
	case rdoRuntime::RDOResource::CS_Keep:
		if (end != rdoRuntime::RDOResource::CS_Keep && end != rdoRuntime::RDOResource::CS_Erase && end != rdoRuntime::RDOResource::CS_NoChange)
		{
			parser()->error().error(convertor_end_pos, rdo::format(_T("Статус конвертора конца несовместим со статусом конвертора начала: %s и %s"), RDOPATPattern::StatusToStr(beg).c_str(), RDOPATPattern::StatusToStr(end).c_str()));
		}
		break;
	case rdoRuntime::RDOResource::CS_Create:
		if (end != rdoRuntime::RDOResource::CS_Keep && end != rdoRuntime::RDOResource::CS_Erase && end != rdoRuntime::RDOResource::CS_NoChange)
		{
			parser()->error().error(convertor_end_pos, rdo::format(_T("Статус конвертора конца несовместим со статусом конвертора начала: %s и %s"), RDOPATPattern::StatusToStr(beg).c_str(), RDOPATPattern::StatusToStr(end).c_str()));
		}
		break;
	case rdoRuntime::RDOResource::CS_Erase:
		if (end != rdoRuntime::RDOResource::CS_NonExist)
		{
			parser()->error().error(convertor_end_pos, rdo::format(_T("Статус конвертора конца несовместим со статусом конвертора начала: %s и %s"), RDOPATPattern::StatusToStr(beg).c_str(), RDOPATPattern::StatusToStr(end).c_str()));
		}
		break;
	case rdoRuntime::RDOResource::CS_NonExist:
		if (end != rdoRuntime::RDOResource::CS_Create)
		{
			parser()->error().error(convertor_end_pos, rdo::format(_T("Статус конвертора конца несовместим со статусом конвертора начала: %s и %s"), RDOPATPattern::StatusToStr(beg).c_str(), RDOPATPattern::StatusToStr(end).c_str()));
		}
		break;
	case rdoRuntime::RDOResource::CS_NoChange:
		if (end != rdoRuntime::RDOResource::CS_Keep && end != rdoRuntime::RDOResource::CS_Erase && end != rdoRuntime::RDOResource::CS_NoChange)
		{
			parser()->error().error(convertor_end_pos, rdo::format(_T("Статус конвертора конца несовместим со статусом конвертора начала: %s и %s"), RDOPATPattern::StatusToStr(beg).c_str(), RDOPATPattern::StatusToStr(end).c_str()));
		}
		break;
	};

	PTR(RDORelevantResource) m_relResList = NULL;
	CPTRC(RDORSSResource)    res    = parser()->findRSSResource(type_info.src_text());
	if (res)
	{
		if (beg == rdoRuntime::RDOResource::CS_Create)
		{
			parser()->error().error(convertor_begin_pos, rdo::format(_T("При создания ресурса '%s' требуется указать его тип, но указан просто ресурс (%s)"), rel_info.src_text().c_str(), type_info.src_text().c_str()));
		}
		if (end == rdoRuntime::RDOResource::CS_Create)
		{
			parser()->error().error(convertor_end_pos, rdo::format(_T("При создания ресурса '%s' требуется указать его тип, но указан просто ресурс (%s)"), rel_info.src_text().c_str(), type_info.src_text().c_str()));
		}
		if (beg == rdoRuntime::RDOResource::CS_Erase || beg == rdoRuntime::RDOResource::CS_NonExist)
		{
			parser()->error().error(convertor_begin_pos, rdo::format(_T("Недопустимый статус конвертора начала для ресурса: %s"), RDOPATPattern::StatusToStr(beg).c_str()));
		}
		if (end == rdoRuntime::RDOResource::CS_Erase || end == rdoRuntime::RDOResource::CS_NonExist)
		{
			parser()->error().error(convertor_end_pos, rdo::format(_T("Недопустимый статус конвертора конца для ресурса: %s"), RDOPATPattern::StatusToStr(end).c_str()));
		}
		m_relResList = new RDORelevantResourceDirect(this, rel_info, rel_res_count(), res, beg, end);
		rel_res_insert(m_relResList);
	}
	else
	{
		LPRDORTPResType pResType = parser()->findRTPResType(type_info.src_text());
		if (!pResType)
		{
			parser()->error().error(type_info, rdo::format(_T("Неизвестный тип ресурса: %s"), type_info.src_text().c_str()));
		}
		if (pResType->isPermanent())
		{
			if (beg == rdoRuntime::RDOResource::CS_Create || beg == rdoRuntime::RDOResource::CS_Erase || beg == rdoRuntime::RDOResource::CS_NonExist)
			{
				parser()->error().error(convertor_begin_pos, rdo::format(_T("Недопустимый статус конвертора начала для постоянного типа: %s"), RDOPATPattern::StatusToStr(beg).c_str()));
			}
			if (end == rdoRuntime::RDOResource::CS_Create || end == rdoRuntime::RDOResource::CS_Erase || end == rdoRuntime::RDOResource::CS_NonExist)
			{
				parser()->error().error(convertor_end_pos, rdo::format(_T("Недопустимый статус конвертора конца для постоянного типа: %s"), RDOPATPattern::StatusToStr(end).c_str()));
			}
		}
		m_relResList = new RDORelevantResourceByType(this, rel_info, rel_res_count(), pResType, beg, end);
		rel_res_insert(m_relResList);
	}
	if (m_relResList->m_statusBegin == rdoRuntime::RDOResource::CS_Erase)
	{
		rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOCalcEraseRes>::create(m_relResList->m_relResID, m_relResList->name());
		pCalc->setSrcInfo(rel_info);
		pCalc->setSrcText(rdo::format(_T("Удаление временного ресурса %s"), rel_info.src_text().c_str()));
		static_cast<PTR(rdoRuntime::RDOPatternOperation)>(getPatRuntime())->addEraseBeginCalc(pCalc);
	}
	if (m_relResList->m_statusEnd == rdoRuntime::RDOResource::CS_Erase)
	{
		rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOCalcEraseRes>::create(m_relResList->m_relResID, m_relResList->name());
		pCalc->setSrcInfo(rel_info);
		pCalc->setSrcText(rdo::format(_T("Удаление временного ресурса %s"), rel_info.src_text().c_str()));
		static_cast<PTR(rdoRuntime::RDOPatternOperation)>(getPatRuntime())->addEraseEndCalc(pCalc);
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
	case convert_begin: static_cast<PTR(rdoRuntime::RDOPatternOperation)>(getPatRuntime())->addConvertorBeginCalc(pCalc); break;
	case convert_end  : static_cast<PTR(rdoRuntime::RDOPatternOperation)>(getPatRuntime())->addConvertorEndCalc  (pCalc); break;
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

// ----------------------------------------------------------------------------
// ---------- RDOPatternKeyboard
// ----------------------------------------------------------------------------
RDOPatternKeyboard::RDOPatternKeyboard(PTR(RDOParser) pParser, CREF(RDOParserSrcInfo) name_src_info, rbool trace)
	: RDOPatternOperation(pParser, trace, name_src_info)
{
	m_pPatRuntime = new rdoRuntime::RDOPatternKeyboard(parser()->runtime(), trace); 
	m_pPatRuntime->setTraceID(parser()->getPAT_id());
}

// ----------------------------------------------------------------------------
// ---------- RDORelevantResource
// ----------------------------------------------------------------------------
PTR(rdoRuntime::RDOCalc) RDORelevantResource::getChoiceCalc() const
{
	if (m_pChoiceFrom && m_pChoiceFrom->m_type == rdoParse::RDOPATChoiceFrom::ch_from)
	{
		return m_pChoiceFrom->m_pLogic->getCalc(rdoRuntime::RDOType::t_int);
	}
	return NULL;
}

PTR(rdoRuntime::RDOCalc) RDORelevantResource::getSelectCalc() const
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

// ----------------------------------------------------------------------------
// ---------- RDORelevantResourceDirect - по имени ресурса
// ----------------------------------------------------------------------------
rdoRuntime::LPRDOCalc RDORelevantResourceDirect::createPreSelectRelResCalc()
{
	rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOSelectResourceDirectCalc>::create(m_relResID, m_pResource->getID());
	pCalc->setSrcInfo(src_info());
	pCalc->setSrcText(rdo::format(_T("Предварительный выбор рел. ресурса %s"), pCalc->src_text().c_str()));
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
	rdoRuntime::LPIRDOSelectResourceCommon pSelectResourceCommon = pDirectCommonCalc;
	return pSelectResourceCommon;
}

LPRDORTPResType RDORelevantResourceDirect::getType() const 
{ 
	return m_pResource->getType(); 
}

// ----------------------------------------------------------------------------
// ---------- RDORelevantResourceByType - по имени типа
// ----------------------------------------------------------------------------
rdoRuntime::LPRDOCalc RDORelevantResourceByType::createPreSelectRelResCalc()
{
	if ((m_statusBegin != rdoRuntime::RDOResource::CS_Create) && (m_statusEnd != rdoRuntime::RDOResource::CS_Create))
	{
		rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOSelectResourceByTypeCalc>::create(m_relResID, m_pResType->getNumber());
		pCalc->setSrcInfo(src_info());
		pCalc->setSrcText(rdo::format(_T("Предварительный выбор рел. ресурса %s"), pCalc->src_text().c_str()));
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
			pCalc->setSrcInfo(src_info());
			pCalc->setSrcText(rdo::format(_T("Предварительный выбор рел. ресурса перед созданием %s"), pCalc->src_text().c_str()));
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
			pCalc->setSrcInfo(src_info());
			pCalc->setSrcText(rdo::format(_T("Перед созданием рел. ресурса %s"), pCalc->src_text().c_str()));
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
	return rdo::Factory<rdoRuntime::RDOSelectResourceByTypeCommonCalc>::create(m_relResID, m_pResType->getNumber(), getChoiceCalc());
}

CLOSE_RDO_PARSER_NAMESPACE
