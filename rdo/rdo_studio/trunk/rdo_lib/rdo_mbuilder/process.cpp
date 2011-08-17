/*
  \copyright (c) RDO-Team, 2011
  \file      rdodpt.cpp
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_mbuilder/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_mbuilder/process.h"

#include "rdo_lib/rdo_parser/rdoparser.h"
// ===============================================================================

OPEN_MBUILDER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- BlockForQueue
// ----------------------------------------------------------------------------
void BlockForQueue::createRes(RDOResType rtp, CREF(tstring) res_name)
{
	// Получили список всех ресурсов
	RDOResourceList rssList(rdoParse::RDOParser::s_parser());
	// Создадим ресурс
	RDOResource rss(rtp, res_name);
	// Добавим его в систему
	rssList.append<rdoParse::RDORSSResource>(rss);
}

rbool BlockForQueue::checkType(RDOResType rtp, CREF(rdoParse::RDOParserSrcInfo) info)
{
	// "длина_очереди"
	tstring rtp_param_name = rdoRuntime::RDOPROCQueue::getQueueParamName();
	// Тип найден, проверим его на наличие параметра "длина_очереди"
	if (!rtp.m_params[rtp_param_name].exist())
		rdoParse::RDOParser::s_parser()->error().error(rtp.src_info(), rdo::format(_T("У типа ресурса '%s' нет параметра integer '%s'"), rtp.name().c_str(), rtp_param_name.c_str()));

	CREF(RDOResType::Param) param = rtp.m_params[rtp_param_name];
	// Параметр Состояние есть, надо проверить, чтобы в нем были значения Свободен и Занят
	// Для начала проверим тип параметра
	if (param.typeID() != rdoRuntime::RDOType::t_int)
		rdoParse::RDOParser::s_parser()->error().error(param.src_info(), rdo::format(_T("У типа ресурса '%s' параметр '%s' не является параметром int"), rtp.name().c_str(), rtp_param_name.c_str()));

	return true;
}
RDOResType BlockForQueue::createType(CREF(tstring) rtp_name, CREF(rdoParse::RDOParserSrcInfo) info)
{
	// "длина_очереди"
	tstring rtp_param_name = rdoRuntime::RDOPROCQueue::getQueueParamName();
	// значение длины очереди по умолчанию
	rdoParse::RDOValue default(rdoRuntime::RDOPROCQueue::getDefaultValue(), info);
	// Получили список всех типов ресурсов
	RDOResTypeList rtpList(rdoParse::RDOParser::s_parser());
	// Создадим тип ресурса
	RDOResType rtp(rtp_name);
	// Создадим параметр типа integer
	rtp.m_params.append(RDOResType::Param(rtp_param_name, rdo::Factory<rdoParse::RDOType__int>::create(), default));
	// Добавим тип ресурса
	if (!rtpList.append<rdoRuntime::RDOResourceType>(rtp))
	{
		rdoParse::RDOParser::s_parser()->error().error(info, rdo::format(_T("Ошибка создания типа ресурса: %s"), rtp_name.c_str()));
	}
	return rtp;
}

// ----------------------------------------------------------------------------
// ---------- BlockForSeize
// ----------------------------------------------------------------------------
rbool BlockForSeize::checkType(RDOResType rtp, CREF(rdoParse::RDOParserSrcInfo) info)
{
	// "Состояние"
	tstring rtp_param_name = rdoRuntime::RDOPROCBlockForSeize::getStateParamName();
	// "Свободен"
	tstring rtp_state_free = rdoRuntime::RDOPROCBlockForSeize::getStateEnumFree();
	// "Занят"
	tstring rtp_state_buzy = rdoRuntime::RDOPROCBlockForSeize::getStateEnumBuzy();
	// Тип найден, проверим его на наличие перечислимого параметра
	if (!rtp.m_params[rtp_param_name].exist())
		rdoParse::RDOParser::s_parser()->error().error(info, rdo::format(_T("У типа ресурса '%s' нет параметра перечислимого типа '%s'"), rtp.name().c_str(), rtp_param_name.c_str()));

	CREF(RDOResType::Param) param = rtp.m_params[rtp_param_name];
	// Параметр Состояние есть, надо проверить, чтобы в нем были значения Свободен и Занят
	// Для начала проверим тип параметра
	if (param.typeID() != rdoRuntime::RDOType::t_enum)
		rdoParse::RDOParser::s_parser()->error().error(param.src_info(), rdo::format(_T("У типа ресурса '%s' параметр '%s' не является параметром перечислимого типа"), rtp.name().c_str(), rtp_param_name.c_str()));

	// Теперь проверим сами значения
	if (!param.getEnum()->getEnums()->exist(rtp_state_free) || !param.getEnum()->getEnums()->exist(rtp_state_buzy))
		rdoParse::RDOParser::s_parser()->error().error(param.src_info(), rdo::format(_T("У типа ресурса '%s' перечислимый параметр '%s' должен иметь как минимум два обязательных значения: %s и %s"), rtp.name().c_str(), param.name().c_str(), rtp_state_free.c_str(), rtp_state_buzy.c_str()));

	return true;
}

void BlockForSeize::createRes(RDOResType rtp, CREF(tstring) res_name)
{
	// Получили список всех ресурсов
	RDOResourceList rssList(rdoParse::RDOParser::s_parser());
	// Создадим ресурс
	RDOResource rss(rtp, res_name);
	// Добавим его в систему
	rssList.append<rdoParse::RDOPROCResource>(rss);
}

void BlockForSeize::reobjectRes(RDOResType rtp, CREF(tstring) res_name)
{
	// Получили список всех ресурсов
	RDOResourceList rssList(rdoParse::RDOParser::s_parser());
	// Создадим ресурс
	RDOResource rssNew(rtp, res_name);
	// Добавим его в систему
	rssList.replace<rdoParse::RDOPROCResource>(rssNew);
}

RDOResType BlockForSeize::createType(CREF(tstring) rtp_name, CREF(rdoParse::RDOParserSrcInfo) info)
{
	// "Состояние"
	tstring rtp_param_name = rdoRuntime::RDOPROCBlockForSeize::getStateParamName();
	// "Свободен"
	tstring rtp_state_free = rdoRuntime::RDOPROCBlockForSeize::getStateEnumFree();
	rdoParse::RDOValue default(rtp_state_free, info);
	default.setSrcText(rtp_state_free);
	// "Занят"
	tstring rtp_state_buzy = rdoRuntime::RDOPROCBlockForSeize::getStateEnumBuzy();

	// Получили список всех типов ресурсов
	RDOResTypeList rtpList(rdoParse::RDOParser::s_parser());
	// Создадим тип ресурса
	RDOResType rtp(rtp_name);
	// Создадим параметр перечислимого типа - "Состояние"
	rtp.m_params.append(RDOResType::Param(
		rtp_param_name,
		rdoRuntime::RDOEnumType::Enums(rtp_state_free)(rtp_state_buzy),
		default
	));
	// Добавим тип ресурса
	if (!rtpList.append<rdoRuntime::RDOResourceTypeProccess>(rtp))
	{
		rdoParse::RDOParser::s_parser()->error().error(info, rdo::format(_T("Ошибка создания типа ресурса: %s"), rtp_name.c_str()));
	}
	return rtp;
}

CLOSE_MBUILDER_NAMESPACE
