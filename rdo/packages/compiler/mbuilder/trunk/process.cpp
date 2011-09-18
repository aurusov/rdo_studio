<<<<<<< .working
/*!
  \copyright (c) RDO-Team, 2011
  \file      rdodpt.cpp
  \authors   Р‘Р°СЂСЃ РђР»РµРєСЃР°РЅРґСЂ
  \authors   РЈСЂСѓСЃРѕРІ РђРЅРґСЂРµР№ (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/mbuilder/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/mbuilder/process.h"

#include "simulator/compiler/parser/rdoparser.h"
// --------------------------------------------------------------------------------

OPEN_MBUILDER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- BlockForQueue
// --------------------------------------------------------------------------------
void BlockForQueue::createRes(RDOResType rtp, CREF(tstring) res_name)
{
	// РџРѕР»СѓС‡РёР»Рё СЃРїРёСЃРѕРє РІСЃРµС… СЂРµСЃСѓСЂСЃРѕРІ
	RDOResourceList rssList(rdoParse::RDOParser::s_parser());
	// РЎРѕР·РґР°РґРёРј СЂРµСЃСѓСЂСЃ
	RDOResource rss(rtp, res_name);
	// Р”РѕР±Р°РІРёРј РµРіРѕ РІ СЃРёСЃС‚РµРјСѓ
	rssList.append<rdoParse::RDORSSResource>(rss);
}

rbool BlockForQueue::checkType(RDOResType rtp, CREF(rdoParse::RDOParserSrcInfo) info)
{
	// "РґР»РёРЅР°_РѕС‡РµСЂРµРґРё"
	tstring rtp_param_name = rdoRuntime::RDOPROCQueue::getQueueParamName();
	// РўРёРї РЅР°Р№РґРµРЅ, РїСЂРѕРІРµСЂРёРј РµРіРѕ РЅР° РЅР°Р»РёС‡РёРµ РїР°СЂР°РјРµС‚СЂР° "РґР»РёРЅР°_РѕС‡РµСЂРµРґРё"
	if (!rtp.m_params[rtp_param_name].exist())
		rdoParse::RDOParser::s_parser()->error().error(rtp.src_info(), rdo::format(_T("РЈ С‚РёРїР° СЂРµСЃСѓСЂСЃР° '%s' РЅРµС‚ РїР°СЂР°РјРµС‚СЂР° integer '%s'"), rtp.name().c_str(), rtp_param_name.c_str()));

	CREF(RDOResType::Param) param = rtp.m_params[rtp_param_name];
	// РџР°СЂР°РјРµС‚СЂ РЎРѕСЃС‚РѕСЏРЅРёРµ РµСЃС‚СЊ, РЅР°РґРѕ РїСЂРѕРІРµСЂРёС‚СЊ, С‡С‚РѕР±С‹ РІ РЅРµРј Р±С‹Р»Рё Р·РЅР°С‡РµРЅРёСЏ РЎРІРѕР±РѕРґРµРЅ Рё Р—Р°РЅСЏС‚
	// Р”Р»СЏ РЅР°С‡Р°Р»Р° РїСЂРѕРІРµСЂРёРј С‚РёРї РїР°СЂР°РјРµС‚СЂР°
	if (param.typeID() != rdoRuntime::RDOType::t_int)
		rdoParse::RDOParser::s_parser()->error().error(param.src_info(), rdo::format(_T("РЈ С‚РёРїР° СЂРµСЃСѓСЂСЃР° '%s' РїР°СЂР°РјРµС‚СЂ '%s' РЅРµ СЏРІР»СЏРµС‚СЃСЏ РїР°СЂР°РјРµС‚СЂРѕРј int"), rtp.name().c_str(), rtp_param_name.c_str()));

	return true;
}
RDOResType BlockForQueue::createType(CREF(tstring) rtp_name, CREF(rdoParse::RDOParserSrcInfo) info)
{
	// "РґР»РёРЅР°_РѕС‡РµСЂРµРґРё"
	tstring rtp_param_name = rdoRuntime::RDOPROCQueue::getQueueParamName();
	// Р·РЅР°С‡РµРЅРёРµ РґР»РёРЅС‹ РѕС‡РµСЂРµРґРё РїРѕ СѓРјРѕР»С‡Р°РЅРёСЋ
	rdoParse::RDOValue default(rdoRuntime::RDOPROCQueue::getDefaultValue(), info);
	// РџРѕР»СѓС‡РёР»Рё СЃРїРёСЃРѕРє РІСЃРµС… С‚РёРїРѕРІ СЂРµСЃСѓСЂСЃРѕРІ
	RDOResTypeList rtpList(rdoParse::RDOParser::s_parser());
	// РЎРѕР·РґР°РґРёРј С‚РёРї СЂРµСЃСѓСЂСЃР°
	RDOResType rtp(rtp_name);
	// РЎРѕР·РґР°РґРёРј РїР°СЂР°РјРµС‚СЂ С‚РёРїР° integer
	rtp.m_params.append(RDOResType::Param(rtp_param_name, rdo::Factory<rdoParse::RDOType__int>::create(), default));
	// Р”РѕР±Р°РІРёРј С‚РёРї СЂРµСЃСѓСЂСЃР°
	if (!rtpList.append<rdoRuntime::RDOResourceType>(rtp))
	{
		rdoParse::RDOParser::s_parser()->error().error(info, rdo::format(_T("РћС€РёР±РєР° СЃРѕР·РґР°РЅРёСЏ С‚РёРїР° СЂРµСЃСѓСЂСЃР°: %s"), rtp_name.c_str()));
	}
	return rtp;
}

// --------------------------------------------------------------------------------
// -------------------- BlockForSeize
// --------------------------------------------------------------------------------
rbool BlockForSeize::checkType(RDOResType rtp, CREF(rdoParse::RDOParserSrcInfo) info)
{
	// "РЎРѕСЃС‚РѕСЏРЅРёРµ"
	tstring rtp_param_name = rdoRuntime::RDOPROCBlockForSeize::getStateParamName();
	// "РЎРІРѕР±РѕРґРµРЅ"
	tstring rtp_state_free = rdoRuntime::RDOPROCBlockForSeize::getStateEnumFree();
	// "Р—Р°РЅСЏС‚"
	tstring rtp_state_buzy = rdoRuntime::RDOPROCBlockForSeize::getStateEnumBuzy();
	// РўРёРї РЅР°Р№РґРµРЅ, РїСЂРѕРІРµСЂРёРј РµРіРѕ РЅР° РЅР°Р»РёС‡РёРµ РїРµСЂРµС‡РёСЃР»РёРјРѕРіРѕ РїР°СЂР°РјРµС‚СЂР°
	if (!rtp.m_params[rtp_param_name].exist())
		rdoParse::RDOParser::s_parser()->error().error(info, rdo::format(_T("РЈ С‚РёРїР° СЂРµСЃСѓСЂСЃР° '%s' РЅРµС‚ РїР°СЂР°РјРµС‚СЂР° РїРµСЂРµС‡РёСЃР»РёРјРѕРіРѕ С‚РёРїР° '%s'"), rtp.name().c_str(), rtp_param_name.c_str()));

	CREF(RDOResType::Param) param = rtp.m_params[rtp_param_name];
	// РџР°СЂР°РјРµС‚СЂ РЎРѕСЃС‚РѕСЏРЅРёРµ РµСЃС‚СЊ, РЅР°РґРѕ РїСЂРѕРІРµСЂРёС‚СЊ, С‡С‚РѕР±С‹ РІ РЅРµРј Р±С‹Р»Рё Р·РЅР°С‡РµРЅРёСЏ РЎРІРѕР±РѕРґРµРЅ Рё Р—Р°РЅСЏС‚
	// Р”Р»СЏ РЅР°С‡Р°Р»Р° РїСЂРѕРІРµСЂРёРј С‚РёРї РїР°СЂР°РјРµС‚СЂР°
	if (param.typeID() != rdoRuntime::RDOType::t_enum)
		rdoParse::RDOParser::s_parser()->error().error(param.src_info(), rdo::format(_T("РЈ С‚РёРїР° СЂРµСЃСѓСЂСЃР° '%s' РїР°СЂР°РјРµС‚СЂ '%s' РЅРµ СЏРІР»СЏРµС‚СЃСЏ РїР°СЂР°РјРµС‚СЂРѕРј РїРµСЂРµС‡РёСЃР»РёРјРѕРіРѕ С‚РёРїР°"), rtp.name().c_str(), rtp_param_name.c_str()));

	// РўРµРїРµСЂСЊ РїСЂРѕРІРµСЂРёРј СЃР°РјРё Р·РЅР°С‡РµРЅРёСЏ
	if (!param.getEnum()->getEnums()->exist(rtp_state_free) || !param.getEnum()->getEnums()->exist(rtp_state_buzy))
		rdoParse::RDOParser::s_parser()->error().error(param.src_info(), rdo::format(_T("РЈ С‚РёРїР° СЂРµСЃСѓСЂСЃР° '%s' РїРµСЂРµС‡РёСЃР»РёРјС‹Р№ РїР°СЂР°РјРµС‚СЂ '%s' РґРѕР»Р¶РµРЅ РёРјРµС‚СЊ РєР°Рє РјРёРЅРёРјСѓРј РґРІР° РѕР±СЏР·Р°С‚РµР»СЊРЅС‹С… Р·РЅР°С‡РµРЅРёСЏ: %s Рё %s"), rtp.name().c_str(), param.name().c_str(), rtp_state_free.c_str(), rtp_state_buzy.c_str()));

	return true;
}

void BlockForSeize::createRes(RDOResType rtp, CREF(tstring) res_name)
{
	// РџРѕР»СѓС‡РёР»Рё СЃРїРёСЃРѕРє РІСЃРµС… СЂРµСЃСѓСЂСЃРѕРІ
	RDOResourceList rssList(rdoParse::RDOParser::s_parser());
	// РЎРѕР·РґР°РґРёРј СЂРµСЃСѓСЂСЃ
	RDOResource rss(rtp, res_name);
	// Р”РѕР±Р°РІРёРј РµРіРѕ РІ СЃРёСЃС‚РµРјСѓ
	rssList.append<rdoParse::RDOPROCResource>(rss);
}

void BlockForSeize::reobjectRes(RDOResType rtp, CREF(tstring) res_name)
{
	// РџРѕР»СѓС‡РёР»Рё СЃРїРёСЃРѕРє РІСЃРµС… СЂРµСЃСѓСЂСЃРѕРІ
	RDOResourceList rssList(rdoParse::RDOParser::s_parser());
	// РЎРѕР·РґР°РґРёРј СЂРµСЃСѓСЂСЃ
	RDOResource rssNew(rtp, res_name);
	// Р”РѕР±Р°РІРёРј РµРіРѕ РІ СЃРёСЃС‚РµРјСѓ
	rssList.replace<rdoParse::RDOPROCResource>(rssNew);
}

RDOResType BlockForSeize::createType(CREF(tstring) rtp_name, CREF(rdoParse::RDOParserSrcInfo) info)
{
	// "РЎРѕСЃС‚РѕСЏРЅРёРµ"
	tstring rtp_param_name = rdoRuntime::RDOPROCBlockForSeize::getStateParamName();
	// "РЎРІРѕР±РѕРґРµРЅ"
	tstring rtp_state_free = rdoRuntime::RDOPROCBlockForSeize::getStateEnumFree();
	rdoParse::RDOValue default(rtp_state_free, info);
	default.setSrcText(rtp_state_free);
	// "Р—Р°РЅСЏС‚"
	tstring rtp_state_buzy = rdoRuntime::RDOPROCBlockForSeize::getStateEnumBuzy();

	// РџРѕР»СѓС‡РёР»Рё СЃРїРёСЃРѕРє РІСЃРµС… С‚РёРїРѕРІ СЂРµСЃСѓСЂСЃРѕРІ
	RDOResTypeList rtpList(rdoParse::RDOParser::s_parser());
	// РЎРѕР·РґР°РґРёРј С‚РёРї СЂРµСЃСѓСЂСЃР°
	RDOResType rtp(rtp_name);
	// РЎРѕР·РґР°РґРёРј РїР°СЂР°РјРµС‚СЂ РїРµСЂРµС‡РёСЃР»РёРјРѕРіРѕ С‚РёРїР° - "РЎРѕСЃС‚РѕСЏРЅРёРµ"
	rtp.m_params.append(RDOResType::Param(
		rtp_param_name,
		rdoRuntime::RDOEnumType::Enums(rtp_state_free)(rtp_state_buzy),
		default
	));
	// Р”РѕР±Р°РІРёРј С‚РёРї СЂРµСЃСѓСЂСЃР°
	if (!rtpList.append<rdoRuntime::RDOResourceTypeProccess>(rtp))
	{
		rdoParse::RDOParser::s_parser()->error().error(info, rdo::format(_T("РћС€РёР±РєР° СЃРѕР·РґР°РЅРёСЏ С‚РёРїР° СЂРµСЃСѓСЂСЃР°: %s"), rtp_name.c_str()));
	}
	return rtp;
}

CLOSE_MBUILDER_NAMESPACE
=======
/*!
  \copyright (c) RDO-Team, 2011
  \file      rdodpt.cpp
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/mbuilder/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/mbuilder/process.h"

#include "simulator/compiler/parser/rdoparser.h"
// --------------------------------------------------------------------------------

OPEN_MBUILDER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- BlockForQueue
// --------------------------------------------------------------------------------
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

// --------------------------------------------------------------------------------
// -------------------- BlockForSeize
// --------------------------------------------------------------------------------
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
>>>>>>> .merge-right.r5003
