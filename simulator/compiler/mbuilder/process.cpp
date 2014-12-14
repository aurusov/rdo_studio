// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/mbuilder/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/mbuilder/process.h"
#include "simulator/compiler/parser/rdoparser.h"
// --------------------------------------------------------------------------------

OPEN_COMPILER_MBUILDER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- BlockForQueue
// --------------------------------------------------------------------------------
void BlockForQueue::createRes(RDOResType rtp, const std::string& res_name)
{
    // Получили список всех ресурсов
    RDOResourceList rssList(parser::RDOParser::s_parser());
    // Создадим ресурс
    RDOResource rss(rtp, res_name);
    // Добавим его в систему
    rssList.append<parser::RDORSSResource>(rss);
}

bool BlockForQueue::checkType(RDOResType rtp, const parser::RDOParserSrcInfo& /*info*/)
{
    // "длина_очереди"
    const std::string rtp_param_name = rdo::runtime::RDOPROCQueue::getQueueParamName();
    // Тип найден, проверим его на наличие параметра "длина_очереди"
    if (!rtp.m_params[rtp_param_name].exist())
        parser::RDOParser::s_parser()->error().error(rtp.src_info(), rdo::format("У типа ресурса '%s' нет параметра integer '%s'", rtp.name().c_str(), rtp_param_name.c_str()));

    const RDOResType::Param& param = rtp.m_params[rtp_param_name];
    if (param.typeID() != rdo::runtime::RDOType::Type::INT)
        parser::RDOParser::s_parser()->error().error(param.src_info(), rdo::format("У типа ресурса '%s' параметр '%s' не является параметром int", rtp.name().c_str(), rtp_param_name.c_str()));

    parser::LPRDORTPResType pResType = parser::RDOParser::s_parser()->findRTPResType(rtp.name());
    ASSERT(pResType);
    pResType->setSubtype(parser::RDORTPResType::RT_PROCESS_RESOURCE);

    return true;
}

RDOResType BlockForQueue::createType(const std::string& rtp_name, const parser::RDOParserSrcInfo& info)
{
    // "длина_очереди"
    const std::string rtp_param_name = rdo::runtime::RDOPROCQueue::getQueueParamName();
    // значение длины очереди по умолчанию
    parser::LPRDOValue pDefaultValue = rdo::Factory<parser::RDOValue>::create(
        rdo::explicit_value<std::size_t>(rdo::runtime::RDOPROCQueue::getDefaultValue()),
        info
    );
    ASSERT(pDefaultValue);
    // Получили список всех типов ресурсов
    RDOResTypeList rtpList(parser::RDOParser::s_parser());
    // Создадим тип ресурса
    RDOResType rtp(rtp_name);
    // Создадим параметр типа integer
    RDOResType::Param param(rtp_param_name, rdo::Factory<parser::RDOType__INT>::create(), pDefaultValue);
    rtp.m_params.append(param);
    // Добавим тип ресурса
    if (!rtpList.append(rtp))
    {
        parser::RDOParser::s_parser()->error().error(info, rdo::format("Ошибка создания типа ресурса: %s", rtp_name.c_str()));
    }
    else
    {
        parser::LPRDORTPResType pResType = parser::RDOParser::s_parser()->findRTPResType(rtp_name);
        ASSERT(pResType);
        pResType->setSubtype(parser::RDORTPResType::RT_PROCESS_RESOURCE);
    }
    return rtp;
}

// --------------------------------------------------------------------------------
// -------------------- BlockForSeize
// --------------------------------------------------------------------------------
bool BlockForSeize::checkType(RDOResType rtp, const parser::RDOParserSrcInfo& info)
{
    // "Состояние"
    const std::string rtp_param_name = rdo::runtime::RDOPROCBlockForSeize::getStateParamName();
    // "Свободен"
    const std::string rtp_state_free = rdo::runtime::RDOPROCBlockForSeize::getStateEnumFree();
    // "Занят"
    const std::string rtp_state_buzy = rdo::runtime::RDOPROCBlockForSeize::getStateEnumBuzy();
    // Тип найден, проверим его на наличие перечислимого параметра
    if (!rtp.m_params[rtp_param_name].exist())
        parser::RDOParser::s_parser()->error().error(info, rdo::format("У типа ресурса '%s' нет параметра перечислимого типа '%s'", rtp.name().c_str(), rtp_param_name.c_str()));

    const RDOResType::Param& param = rtp.m_params[rtp_param_name];
    // Параметр Состояние есть, надо проверить, чтобы в нем были значения Свободен и Занят
    // Для начала проверим тип параметра
    if (param.typeID() != rdo::runtime::RDOType::Type::ENUM)
        parser::RDOParser::s_parser()->error().error(param.src_info(), rdo::format("У типа ресурса '%s' параметр '%s' не является параметром перечислимого типа", rtp.name().c_str(), rtp_param_name.c_str()));

    // Теперь проверим сами значения
    if (!param.getEnum()->exist(rtp_state_free) || !param.getEnum()->exist(rtp_state_buzy))
        parser::RDOParser::s_parser()->error().error(param.src_info(), rdo::format("У типа ресурса '%s' перечислимый параметр '%s' должен иметь как минимум два обязательных значения: %s и %s", rtp.name().c_str(), param.name().c_str(), rtp_state_free.c_str(), rtp_state_buzy.c_str()));

    parser::LPRDORTPResType pResType = parser::RDOParser::s_parser()->findRTPResType(rtp.name());
    ASSERT(pResType);
    pResType->setSubtype(parser::RDORTPResType::RT_PROCESS_RESOURCE);

    return true;
}

void BlockForSeize::createRes(RDOResType rtp, const std::string& res_name)
{
    // Получили список всех ресурсов
    RDOResourceList rssList(parser::RDOParser::s_parser());
    // Создадим ресурс
    RDOResource rss(rtp, res_name);
    // Добавим его в систему
    rssList.append<parser::RDORSSResource>(rss);
}

void BlockForSeize::reobjectRes(RDOResType rtp, const std::string& res_name)
{
    // Получили список всех ресурсов
    RDOResourceList rssList(parser::RDOParser::s_parser());
    // Создадим ресурс
    RDOResource rssNew(rtp, res_name);
    // Добавим его в систему
    rssList.replace<parser::RDORSSResource>(rssNew);
}

RDOResType BlockForSeize::createType(const std::string& rtp_name, const parser::RDOParserSrcInfo& info)
{
    // "Состояние"
    const std::string rtp_param_name = rdo::runtime::RDOPROCBlockForSeize::getStateParamName();
    // "Свободен"
    const std::string rtp_state_free = rdo::runtime::RDOPROCBlockForSeize::getStateEnumFree();
    parser::LPRDOValue pDefaultValue = rdo::Factory<parser::RDOValue>::create(
        rdo::explicit_value<std::string>(rtp_state_free),
        info
    );
    ASSERT(pDefaultValue);
    pDefaultValue->setSrcText(rtp_state_free);
    // "Занят"
    const std::string rtp_state_buzy = rdo::runtime::RDOPROCBlockForSeize::getStateEnumBuzy();

    // Получили список всех типов ресурсов
    RDOResTypeList rtpList(parser::RDOParser::s_parser());
    // Создадим тип ресурса
    RDOResType rtp(rtp_name);
    // Создадим параметр перечислимого типа - "Состояние"
    RDOResType::Param param(
        rtp_param_name,
        rdo::runtime::RDOEnumType::Enums(rtp_state_free)(rtp_state_buzy),
        pDefaultValue
    );
    rtp.m_params.append(param);
    // Добавим тип ресурса
    if (!rtpList.append(rtp))
    {
        parser::RDOParser::s_parser()->error().error(info, rdo::format("Ошибка создания типа ресурса: %s", rtp_name.c_str()));
    }
    else
    {
        parser::LPRDORTPResType pResType = parser::RDOParser::s_parser()->findRTPResType(rtp_name);
        ASSERT(pResType);
        pResType->setSubtype(parser::RDORTPResType::RT_PROCESS_RESOURCE);
    }
    return rtp;
}

CLOSE_COMPILER_MBUILDER_NAMESPACE
