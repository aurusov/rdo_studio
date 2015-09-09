// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
#ifdef CORBA_ENABLE
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#pragma warning(disable: 4996)
#include "simulator/compiler/parser/rdoparser_corba.h"
#include "simulator/compiler/parser/rdortp.h"
#include "simulator/compiler/parser/rdosmr.h"
#include "simulator/compiler/mbuilder/rdo_resources.h"
#include "simulator/runtime/rdo_object.h"
#include "simulator/corba/RDOCorba.hh"
#pragma warning(default: 4996)
// --------------------------------------------------------------------------------

#define file1 "C:\\RTP.txt"
#define file2 "C:\\RSS.txt"

OPEN_RDO_PARSER_NAMESPACE

void print_RTP(RDOCorba::GetRTP_var& my_rtpList)
{
    FILE* f1;
#pragma warning(disable: 4996)
    f1 = fopen(file1, "w");
#pragma warning(default: 4996)

    for (std::size_t i = 0; i < my_rtpList->length(); i++)
    {
        fprintf(f1, "\nИнформация о типе ресурса №%d:\n", i+1);
        fprintf(f1, "   Имя типа ресурса №%d: %s \n", i+1, my_rtpList[i].m_name);

        if (my_rtpList[i].m_type==RDOCorba::rt_permanent)
            fprintf(f1, "   Вид типа ресурса: tr_permanent\n");
        else
            fprintf(f1, "   Вид типа ресурса: tr_temporary\n");
            
        for (std::size_t j = 0; j != my_rtpList[i].m_param_count; j++)
        {
            fprintf(f1, "\n   Информация о параметре №%d:\n", j+1);
            fprintf(f1, "      Имя параметра:  %s \n", my_rtpList[i].m_param[j].m_name);
                
            switch (my_rtpList[i].m_param[j].m_type)
            {
            case RDOCorba::int_type:
                fprintf(f1, "      Тип параметра: integer\n");
                if (my_rtpList[i].m_param[j].m_range_int == 1)
                {
                    fprintf(f1, "      Минимальное значение: %d \n",  my_rtpList[i].m_param[j].m_min_int);
                    fprintf(f1, "      Максимальное значение: %d \n", my_rtpList[i].m_param[j].m_max_int);
                }
                if (my_rtpList[i].m_param[j].m_default_int_ch == 1)
                {
                    fprintf(f1, "      Значение по умолчанию: %d \n", my_rtpList[i].m_param[j].m_default_int);
                }
                break;
            case RDOCorba::double_type:
                    fprintf(f1, "      Тип параметра: real\n");
                    if (my_rtpList[i].m_param[j].m_range_double == 1)
                    {
                        fprintf(f1, "      Минимальное значение: %f \n", my_rtpList[i].m_param[j].m_min_double);
                        fprintf(f1, "      Максимальное значение: %f \n", my_rtpList[i].m_param[j].m_max_double);
                    }
                    if (my_rtpList[i].m_param[j].m_default_double_ch == 1)
                    {
                        fprintf(f1, "      Значение по умолчанию: %f \n", my_rtpList[i].m_param[j].m_default_double);
                    }
                    break;
                case RDOCorba::enum_type:
                {
                    fprintf(f1, "      Тип параметра: enum\n");
                    fprintf(f1, "      Значения параметра:");
                                        
                    CORBA::Long k = 0;
            
                    while (k != my_rtpList[i].m_param[j].m_var_enum_count)
                    {
                        fprintf(f1, "\t%s", my_rtpList[i].m_param[j].m_var_enum[k].pd_data);
                        k++;
                    }

                    fprintf(f1, "\n");

                    if (my_rtpList[i].m_param[j].m_default_enum_ch == 1)
                    {
                        fprintf(f1, "\n      Значение по умолчанию: %s \n", my_rtpList[i].m_param[j].m_default_enum);
                    }
                    break;
                }
                default: break;
            }
        }
    }

    fclose(f1);
}

void print_RSS(RDOCorba::GetRSS_var& my_rssList)
{
    FILE* f2;
#pragma warning(disable: 4996)
    f2 = fopen(file2, "w");
#pragma warning(default: 4996)
    
    for (std::size_t i = 0; i < my_rssList->length(); i++)
    {
        fprintf(f2, "\nИнформация о ресурсе №%d:\n\n", i+1);
        fprintf(f2, "Имя ресурса: %s/ Тип ресурса: %s\n", my_rssList[i].m_name, my_rssList[i].m_type);

        for (std::size_t j = 0; j != my_rssList[i].m_param_count; j++)
        {
            switch (my_rssList[i].m_param[j].m_type)
            {
            case RDOCorba::int_type:
                fprintf(f2, "  Значение параметра: %s = %d\n", my_rssList[i].m_param[j].m_name, my_rssList[i].m_param[j].m_int);
                break;
            case RDOCorba::double_type:
                fprintf(f2, "  Значение параметра: %s = %f\n", my_rssList[i].m_param[j].m_name, my_rssList[i].m_param[j].m_double);
                break;
            case RDOCorba::enum_type:
                fprintf(f2, "  Значение параметра: %s = %s\n", my_rssList[i].m_param[j].m_name, my_rssList[i].m_param[j].m_enum);
                break;
            default: break;
            }
        }
    }

    fclose(f2);
}

static CORBA::Object_ptr getObjectReference(CORBA::ORB_ptr orb, const char* ObjectName)
{
    CosNaming::NamingContext_var rootContext;

    try
    {
        // Obtain a reference to the root context of the Name service:
        CORBA::Object_var obj;

        obj = orb->resolve_initial_references("NameService");

        // Narrow the reference returned.
        rootContext = CosNaming::NamingContext::_narrow(obj);

        if(CORBA::is_nil(rootContext))
        {
            TRACE("Failed to narrow the root naming context.\n");
            return CORBA::Object::_nil();
        }
    }
    catch (const CORBA::NO_RESOURCES&)
    {
        TRACE("Caught NO_RESOURCES exception. You must configure omniORB with the location of the naming service.\n");
        return 0;
    }
    catch (const CORBA::ORB::InvalidName&)
    {
        // This should not happen!
        TRACE("Service required is invalid [does not exist].\n");
        return CORBA::Object::_nil();
    }

    // Create a name object, containing the name test/context:
    CosNaming::Name name;
    name.length(2);
    name[0].id = (const char*)"RDO"; // string copied
    name[0].kind = (const char*)"RDO_context"; // string copied
    name[1].id = (const char*)ObjectName;
    name[1].kind = (const char*)"Object";

    // Note on kind: The kind field is used to indicate the type
    // of the object. This is to avoid conventions such as that used
    // by files (name.type -- e.g. test.ps = postscript etc.)
    try
    {
        // Resolve the name to an object reference.
        return rootContext->resolve(name);
    }
    catch(const CosNaming::NamingContext::NotFound&)
    {
        // This exception is thrown if any of the components of the
        // path [contexts or the object] aren’t found:
        TRACE("Context not found.");
    }
    catch(const CORBA::TRANSIENT&)
    {
        TRACE("Caught system exception TRANSIENT -- unable to contact the naming service. Make sure the naming server is running and that omniORB is configured correctly. \n");
    }
    catch(const CORBA::SystemException& ex)
    {
        TRACE1("Caught a CORBA:: %s while using the naming service.\n", ex._name());
        return 0;
    }

    return CORBA::Object::_nil();
}

// ----------------------------------------------------------------------------
// ---------- RDOParserCorbaRTP
// ----------------------------------------------------------------------------
void RDOParserCorbaRTP::parse(const LPRDOParser& pParser)
{
    // Тут надо запросить все типы ресурсов у парного РДО,
    // вызвав с помощью корбы некий метод, который вернёт кучу структур
    // с описанием RTP и насоздавать этих типов

    RDOParserSMRInfo parser;
    parser.parse();

    const char* left;
    const char* right;

    RDOSMR::StringTable tmp = parser.getSMR()->getExternalModelList();
    for (const auto& pair: tmp)
    {
        left  = pair.first.c_str();
        right = pair.second.c_str();

        try
        {
            int argc = 0;

            CORBA::ORB_var         orb         = CORBA::ORB_init   (argc, NULL);
            CORBA::Object_var      obj         = getObjectReference(orb,  left); // может лучше right
            RDOCorba_var rdocorbaref = RDOCorba::_narrow(obj);

            //-------------------------------------------------------------
            CORBA::Long rtp_count = 0;

            RDOCorba::GetRTP_var tmp_rtp = rdocorbaref->getRDORTPlist(rtp_count);
            RDOCorba::GetRTP_var my_rtpList(tmp_rtp);

            // Печатаем в файл на С для теста
            print_RTP(my_rtpList);
            //-------------------------------------------------------------
            // Добавляем к существующим типам ресурсов и выводим в трассировке
    
            // Получили список всех описанных типов ресурсов
            rdo::compiler::mbuilder::RDOResTypeList rtpList(pParser);
    
            for (std::size_t i = 0; i < my_rtpList->length(); i++)
            {
                rdo::compiler::mbuilder::RDOResType rtp(my_rtpList[i].m_name.in());

                // Наполняем его параметрами
                for (std::size_t j = 0; j != my_rtpList[i].m_param_count; j++)
                {
                    switch (my_rtpList[i].m_param[j].m_type)
                    {
                    case RDOCorba::int_type:
                    {
                        rdo::compiler::mbuilder::RDOResType::Param par_int(my_rtpList[i].m_param[j].m_name.in(), rdo::Factory<RDOType__INT>::create());

                        if (my_rtpList[i].m_param[j].m_range_int == 1)
                            par_int.setRange(RDOValue(my_rtpList[i].m_param[j].m_min_int), RDOValue(my_rtpList[i].m_param[j].m_max_int));

                        if (my_rtpList[i].m_param[j].m_default_int_ch == 1)
                            par_int.setDefault(RDOValue(my_rtpList[i].m_param[j].m_default_int));

                        rtp.m_params.append(par_int);
                        break;
                    }
                    case RDOCorba::double_type:
                    {
                        rdo::compiler::mbuilder::RDOResType::Param par_double(my_rtpList[i].m_param[j].m_name.in(), rdo::Factory<RDOType__REAL>::create());

                        if (my_rtpList[i].m_param[j].m_range_double == 1)
                            par_double.setRange(RDOValue(my_rtpList[i].m_param[j].m_min_double), RDOValue(my_rtpList[i].m_param[j].m_max_double));

                        if (my_rtpList[i].m_param[j].m_default_double_ch == 1)
                            par_double.setDefault(RDOValue(my_rtpList[i].m_param[j].m_default_double));

                        rtp.m_params.append(par_double);
                        break;
                    }
                    case RDOCorba::enum_type:
                    {
                        // Создадим список значений параметра перечислимого типа
                        rdo::runtime::RDOEnumType::Enums enumList;

                        for (CORBA::Long k = 0; k < my_rtpList[i].m_param[j].m_var_enum_count ; k++)
                        {
                            enumList.push_back(my_rtpList[i].m_param[j].m_var_enum[k].pd_data);
                        }

                        // Создадим параметр перечислимого типа
                        rdo::compiler::mbuilder::RDOResType::Param par_enum(my_rtpList[i].m_param[j].m_name.in(), enumList);

                        //Добавляем, если есть значение по умолчанию
                        if (my_rtpList[i].m_param[j].m_default_enum_ch == 1)
                        {
                            std::string temp_string;
                            temp_string = my_rtpList[i].m_param[j].m_default_enum.in();
                            par_enum.setDefault(RDOValue::getIdentificator(temp_string));
                        }
                        rtp.m_params.append(par_enum);
                        break;
                    }
                    default: break;
                    }
                }

                // Добавляем к существующим типам ресурсов
                if (rtpList.append(rtp))
                {
                    // Добавили успешно
                    TRACE("Еще один тип ресурсов добавился!!!\n");
                }
                else
                {
                    // Неудача, возможно, тип с таким именем уже есть
                    TRACE("Где-то есть ошибка или тип ресурса уже существует!!!\n");
                }
            }

            // Вывели все типы ресурсов (исключительно для теста)
            for (const auto& rtp: rtpList)
            {
                TRACE1("rtp.name = %s\n", rtp.name().c_str());
                for (const auto& param: rtp.m_params)
                {
                    const std::string info = rdo::format("  param: %s: %s", param.name().c_str() , param.typeStr().c_str());
                    if (param.hasRange())
                    {
                        info = rdo::format("%s [%s..%s]", info.c_str(), param.getMin()->getAsString().c_str(), param.getMax()->getAsString().c_str());
                    }
                    if (param.hasDefault())
                    {
                        info = rdo::format("%s = %s", info.c_str(), param.getDefault()->getAsString().c_str());
                    }
                    TRACE1("%s\n", info.c_str());

                    if (param.typeID() ==  rdo::runtime::RDOType::Type::ENUM)
                    {
                        for (const auto& enumValue: param.getEnum()->getEnums())
                        {
                            TRACE1("  - enum - %s\n", enumValue.c_str());
                        }
                    }
                }
            }

            // -------------------------------------------------------------
            CORBA::Long rss_count = 0;

            RDOCorba::GetRSS_var tmp_rss = rdocorbaref->getRDORSSPlist(rss_count);
            RDOCorba::GetRSS_var my_rssList(tmp_rss);
        
            // Печатаем в файл на С для теста:
            print_RSS(my_rssList);

            // -------------------------------------------------------------
            // Добавляем к существующим ресурсам и выводим в трассировке

            // Переписали имеющиеся ресурсы в rssList
            rdo::compiler::mbuilder::RDOResourceList rssList(pParser);

            for (std::size_t i = 0; i < my_rssList->length(); i++)
            {
                // Нашли тип ресурса по известному имени и создали ресурс указанного типа
                rdo::compiler::mbuilder::RDOResType _rtp = rtpList[my_rssList[i].m_type.in()];
                rdo::compiler::mbuilder::RDOResource rss(_rtp, my_rssList[i].m_name.in());

                // rdo::compiler::mbuilder::RDOResource::Params::const_iterator it_param = rss.begin();

                for (std::size_t j = 0; j != my_rssList[i].m_param_count; j++)
                {
                    // Записываем начальные значения параметров ресурса
                    switch (my_rssList[i].m_param[j].m_type)
                    {
                    case RDOCorba::int_type:
                        rss[my_rssList[i].m_param[j].m_name.in()] = RDOValue(my_rssList[i].m_param[j].m_int);
                        break;
                    case RDOCorba::double_type:
                        rss[my_rssList[i].m_param[j].m_name.in()] = RDOValue(my_rssList[i].m_param[j].m_double);
                        break;
                    case RDOCorba::enum_type:
                    {
                        std::string temp_string;
                        temp_string = my_rssList[i].m_param[j].m_enum.in();
                        rss[my_rssList[i].m_param[j].m_name.in()] = RDOValue::getIdentificator(temp_string);
                        break;
                    }
                    default: break;
                    }                                                 
                 }

                // Добавляем к существующим ресурсам
                if (rssList.append<RDORSSResource>(rss))
                {
                    // Добавили успешно
                    TRACE("Еще один ресурс добавился!!!\n");
                }
                else
                {
                    // Неудача
                    TRACE("Где-то есть ошибка или ресурс уже существует!!!\n");
                } 
            }

            // Вывели все ресурсы для теста
            // rdo::compiler::mbuilder::RDOResourceList rssList(&parser);
            for (const auto& rss: rssList)
            {
                TRACE2("rss.name = %s: %s\n", rss.name().c_str(), rss.getType().name().c_str());
                for (const auto& param: rss)
                {
                    TRACE2("  %s = %s\n", param.first.c_str(), param.second->getAsString().c_str());
                }
            }

            //-------------------------------------------------------------
            orb->destroy();
        }    
        catch(const CORBA::TRANSIENT&)
        {
            TRACE("Caught system exception TRANSIENT -- unable to contact the server.\n");
        }
        catch(const CORBA::SystemException& ex)
        {
            TRACE1("Caught a CORBA:: %s\n", ex._name());
        }
        catch(const CORBA::Exception& ex)
        {
            TRACE1("Caught CORBA::Exception: %s\n", ex._name());
        }
        catch(const omniORB::fatalException& fe)
        {
            TRACE3("Caught omniORB::fatalException: file: %s line: %d mesg: %s\n", fe.file() , fe.line() , fe.errmsg());
        }
    }
}

// ----------------------------------------------------------------------------
// ---------- RDOParserCorbaRSS
// ----------------------------------------------------------------------------
void RDOParserCorbaRSS::parse(const LPRDOParser& pParser)
{
/*
    // Тут надо запросить все ресурсы у парного РДО

    // Количество полученных ресурсов
    const std::size_t rss_count = 1;

    // Получили список всех типов ресурсов
    rdo::compiler::mbuilder::RDOResTypeList rtpList(pParser);
    // Получили список всех ресурсов
    rdo::compiler::mbuilder::RDOResourceList rssList(pParser);

    for (std::size_t i = 0; i < rss_count; i++)
    {
        // Создали новый ресурс
        rdo::compiler::mbuilder::RDOResource rss(rtpList["Парикмахеры"], "MyRSS1");
        // Заполнили его параметры
        rss["длительность_max"] = 174;
        // Добавляем его к списку существующих
        if (rssList.append<RDORSSResource>(rss))
        {
            // Добавили успешно
        }
        else
        {
            // Неудача
        }
        // Пример проверки ресурса на существование
        if (rssList.exist("qqq"))
        {
        }
    }
*/
}

CLOSE_RDO_PARSER_NAMESPACE

#endif // CORBA_ENABLE
