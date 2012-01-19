/*!
  \copyright (c) RDO-Team, 2011
  \file      rdoparser_corba.h
  \authors   Раевнев Никита
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

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

void print_RTP(REF(rdoParse::RDOCorba::GetRTP_var) my_rtpList)
{
	PTR(FILE) f1;
#pragma warning(disable: 4996)
	f1 = fopen(file1, _T("w"));
#pragma warning(default: 4996)

	for (ruint i = 0; i < my_rtpList->length(); i++)
	{
		fprintf(f1, _T("\nИнформация о типе ресурса №%d:\n"), i+1);
		fprintf(f1, _T("   Имя типа ресурса №%d: %s \n"), i+1, my_rtpList[i].m_name);

		if (my_rtpList[i].m_type==rdoParse::RDOCorba::rt_permanent)
			fprintf(f1, _T("   Вид типа ресурса: tr_permanent\n"));
		else
			fprintf(f1, _T("   Вид типа ресурса: tr_temporary\n"));
			
		for (ruint j = 0; j != my_rtpList[i].m_param_count; j++)
		{
			fprintf(f1, _T("\n   Информация о параметре №%d:\n"), j+1);
			fprintf(f1, _T("      Имя параметра:  %s \n"), my_rtpList[i].m_param[j].m_name);
				
			switch (my_rtpList[i].m_param[j].m_type)
			{
			case rdoParse::RDOCorba::int_type:
				fprintf(f1, _T("      Тип параметра: integer\n"));
				if (my_rtpList[i].m_param[j].m_range_int == 1)
				{
					fprintf(f1, _T("      Минимальное значение: %d \n"),  my_rtpList[i].m_param[j].m_min_int);
					fprintf(f1, _T("      Максимальное значение: %d \n"), my_rtpList[i].m_param[j].m_max_int);
				}
				if (my_rtpList[i].m_param[j].m_default_int_ch == 1)
				{
					fprintf(f1, _T("      Значение по умолчанию: %d \n"), my_rtpList[i].m_param[j].m_default_int);
				}
				break;
			case rdoParse::RDOCorba::double_type:
					fprintf(f1, _T("      Тип параметра: real\n"));
					if (my_rtpList[i].m_param[j].m_range_double == 1)
					{
						fprintf(f1, _T("      Минимальное значение: %f \n"), my_rtpList[i].m_param[j].m_min_double);
						fprintf(f1, _T("      Максимальное значение: %f \n"), my_rtpList[i].m_param[j].m_max_double);
					}
					if (my_rtpList[i].m_param[j].m_default_double_ch == 1)
					{
						fprintf(f1, _T("      Значение по умолчанию: %f \n"), my_rtpList[i].m_param[j].m_default_double);
					}
					break;
				case rdoParse::RDOCorba::enum_type:
				{
					fprintf(f1, _T("      Тип параметра: enum\n"));
					fprintf(f1, _T("      Значения параметра:"));
										
					CORBA::Long k = 0;
			
					while (k != my_rtpList[i].m_param[j].m_var_enum_count)
					{
						fprintf(f1, _T("\t%s"), my_rtpList[i].m_param[j].m_var_enum[k].pd_data);
						k++;
					}

					fprintf(f1, _T("\n"));

					if (my_rtpList[i].m_param[j].m_default_enum_ch == 1)
					{
						fprintf(f1, _T("\n      Значение по умолчанию: %s \n"), my_rtpList[i].m_param[j].m_default_enum);
					}
					break;
				}
				default: break;
			}
		}
	}

	fclose(f1);
}

void print_RSS(REF(rdoParse::RDOCorba::GetRSS_var) my_rssList)
{
	PTR(FILE) f2;
#pragma warning(disable: 4996)
	f2 = fopen(file2, _T("w"));
#pragma warning(default: 4996)
	
	for (ruint i = 0; i < my_rssList->length(); i++)
	{
		fprintf(f2, _T("\nИнформация о ресурсе №%d:\n\n"), i+1);
		fprintf(f2, _T("Имя ресурса: %s/ Тип ресурса: %s\n"), my_rssList[i].m_name, my_rssList[i].m_type);

		for (ruint j = 0; j != my_rssList[i].m_param_count; j++)
		{
			switch (my_rssList[i].m_param[j].m_type)
			{
			case rdoParse::RDOCorba::int_type:
				fprintf(f2, _T("  Значение параметра: %s = %d\n"), my_rssList[i].m_param[j].m_name, my_rssList[i].m_param[j].m_int);
				break;
			case rdoParse::RDOCorba::double_type:
				fprintf(f2, _T("  Значение параметра: %s = %f\n"), my_rssList[i].m_param[j].m_name, my_rssList[i].m_param[j].m_double);
				break;
			case rdoParse::RDOCorba::enum_type:
				fprintf(f2, _T("  Значение параметра: %s = %s\n"), my_rssList[i].m_param[j].m_name, my_rssList[i].m_param[j].m_enum);
				break;
			default: break;
			}
		}
	}

	fclose(f2);
}

static CORBA::Object_ptr getObjectReference(CORBA::ORB_ptr orb, CPTR(char) ObjectName)
{
	CosNaming::NamingContext_var rootContext;

	try
	{
		//! Obtain a reference to the root context of the Name service:
		CORBA::Object_var obj;

		obj = orb->resolve_initial_references(_T("NameService"));

		//! Narrow the reference returned.
		rootContext = CosNaming::NamingContext::_narrow(obj);

		if(CORBA::is_nil(rootContext))
		{
			TRACE(_T("Failed to narrow the root naming context.\n"));
			return CORBA::Object::_nil();
		}
	}
	catch (REF(CORBA::NO_RESOURCES))
	{
		TRACE(_T("Caught NO_RESOURCES exception. You must configure omniORB with the location of the naming service.\n"));
		return 0;
	}
	catch (REF(CORBA::ORB::InvalidName))
	{
		//! This should not happen!
		TRACE(_T("Service required is invalid [does not exist].\n"));
		return CORBA::Object::_nil();
	}

	//! Create a name object, containing the name test/context:
	CosNaming::Name name;
	name.length(2);
	name[0].id   = (CPTR(char)) _T("RDO");         //! string copied
	name[0].kind = (CPTR(char)) _T("RDO_context"); //! string copied
	name[1].id   = (CPTR(char)) ObjectName;
	name[1].kind = (CPTR(char)) _T("Object");

	//! Note on kind: The kind field is used to indicate the type
	//! of the object. This is to avoid conventions such as that used
	//! by files (name.type -- e.g. test.ps = postscript etc.)
	try
	{
		//! Resolve the name to an object reference.
		return rootContext->resolve(name);
	}
	catch(REF(CosNaming::NamingContext::NotFound))
	{
		//! This exception is thrown if any of the components of the
		//! path [contexts or the object] aren’t found:
		TRACE(_T("Context not found."));
	}
	catch(REF(CORBA::TRANSIENT))
	{
		TRACE(_T("Caught system exception TRANSIENT -- unable to contact the naming service. Make sure the naming server is running and that omniORB is configured correctly. \n"));
	}
	catch(REF(CORBA::SystemException) ex)
	{
		TRACE1(_T("Caught a CORBA:: %s while using the naming service.\n"), ex._name());
		return 0;
	}

	return CORBA::Object::_nil();
}

//! ----------------------------------------------------------------------------
//! ---------- RDOParserCorbaRTP
//! ----------------------------------------------------------------------------
void RDOParserCorbaRTP::parse(CREF(LPRDOParser) pParser)
{
	//! Тут надо запросить все типы ресурсов у парного РДО,
	//! вызвав с помощью корбы некий метод, который вернёт кучу структур
	//! с описанием RTP и насоздавать этих типов

	rdoParse::RDOParserSMRInfo parser;
	parser.parse();

	CPTR(char) left;
	CPTR(char) right;

	rdoParse::RDOSMR::StringTable tmp = parser.getSMR()->getExternalModelList();
	STL_FOR_ALL_CONST(tmp, it)
	{
		left  = it->first.c_str();
		right = it->second.c_str();

		try
		{
			int argc = 0;

			CORBA::ORB_var         orb         = CORBA::ORB_init   (argc, NULL);
			CORBA::Object_var      obj         = getObjectReference(orb,  left); //! может лучше right
			rdoParse::RDOCorba_var rdocorbaref = rdoParse::RDOCorba::_narrow(obj);

			//-------------------------------------------------------------
			CORBA::Long rtp_count = 0;

			rdoParse::RDOCorba::GetRTP_var tmp_rtp = rdocorbaref->getRDORTPlist(rtp_count);
			rdoParse::RDOCorba::GetRTP_var my_rtpList(tmp_rtp);

			//! Печатаем в файл на С для теста
			print_RTP(my_rtpList);
			//!-------------------------------------------------------------
			//! Добавляем к существующим типам ресурсов и выводим в трассировке
	
			//! Получили список всех описанных типов ресурсов
			rdoMBuilder::RDOResTypeList rtpList(pParser);
	
			for (ruint i = 0; i < my_rtpList->length(); i++)
			{
				rdoMBuilder::RDOResType rtp(my_rtpList[i].m_name.in());

				//! Наполняем его параметрами
				for (ruint j = 0; j != my_rtpList[i].m_param_count; j++)
				{
					switch (my_rtpList[i].m_param[j].m_type)
					{
					case rdoParse::RDOCorba::int_type:
					{
						rdoMBuilder::RDOResType::Param par_int(my_rtpList[i].m_param[j].m_name.in(), rdo::Factory<RDOType__int>::create());

						if (my_rtpList[i].m_param[j].m_range_int == 1)
							par_int.setRange(RDOValue(my_rtpList[i].m_param[j].m_min_int), RDOValue(my_rtpList[i].m_param[j].m_max_int));

						if (my_rtpList[i].m_param[j].m_default_int_ch == 1)
							par_int.setDefault(RDOValue(my_rtpList[i].m_param[j].m_default_int));

						rtp.m_params.append(par_int);
						break;
					}
					case rdoParse::RDOCorba::double_type:
					{
						rdoMBuilder::RDOResType::Param par_double(my_rtpList[i].m_param[j].m_name.in(), rdo::Factory<RDOType__real>::create());

						if (my_rtpList[i].m_param[j].m_range_double == 1)
							par_double.setRange(RDOValue(my_rtpList[i].m_param[j].m_min_double), RDOValue(my_rtpList[i].m_param[j].m_max_double));

						if (my_rtpList[i].m_param[j].m_default_double_ch == 1)
							par_double.setDefault(RDOValue(my_rtpList[i].m_param[j].m_default_double));

						rtp.m_params.append(par_double);
						break;
					}
					case rdoParse::RDOCorba::enum_type:
					{
						//! Создадим список значений параметра перечислимого типа
						rdoRuntime::RDOEnumType::Enums enumList;

						for (CORBA::Long k = 0; k < my_rtpList[i].m_param[j].m_var_enum_count ; k++)
						{
							enumList.push_back(my_rtpList[i].m_param[j].m_var_enum[k].pd_data);
						}

						//! Создадим параметр перечислимого типа
						rdoMBuilder::RDOResType::Param par_enum(my_rtpList[i].m_param[j].m_name.in(), enumList);

						//Добавляем, если есть значение по умолчанию
						if (my_rtpList[i].m_param[j].m_default_enum_ch == 1)
						{
							tstring temp_string;
							temp_string = my_rtpList[i].m_param[j].m_default_enum.in();
							par_enum.setDefault(RDOValue::getIdentificator(temp_string));
						}
						rtp.m_params.append(par_enum);
						break;
					}
					default: break;
					}
				}

				//! Добавляем к существующим типам ресурсов
				if (rtpList.append(rtp))
				{
					//! Добавили успешно
					TRACE(_T("Еще один тип ресурсов добавился!!!\n"));
				}
				else
				{
					//! Неудача, возможно, тип с таким именем уже есть
					TRACE(_T("Где-то есть ошибка или тип ресурса уже существует!!!\n"));
				}
			}

			//! Вывели все типы ресурсов (исключительно для теста)
			STL_FOR_ALL_CONST(rtpList, rtp_it)
			{
				TRACE1(_T("rtp.name = %s\n"), rtp_it->name().c_str());
				STL_FOR_ALL_CONST(rtp_it->m_params, param_it)
				{
					tstring info = rdo::format("  param: %s: %s", param_it->name().c_str() , param_it->typeStr().c_str());
					if (param_it->hasRange())
					{
						info = rdo::format("%s [%s..%s]", info.c_str(), param_it->getMin()->getAsString().c_str(), param_it->getMax()->getAsString().c_str());
					}
					if (param_it->hasDefault())
					{
						info = rdo::format("%s = %s", info.c_str(), param_it->getDefault()->getAsString().c_str());
					}
					TRACE1(_T("%s\n"), info.c_str());

					if (param_it->typeID() ==  rdoRuntime::RDOType::t_enum)
					{
						STL_FOR_ALL_CONST(param_it->getEnum()->getEnums(), enum_it)
						{
							TRACE1(_T("  - enum - %s\n"), enum_it->c_str());
						}
					}
				}
			}

			//! -------------------------------------------------------------
			CORBA::Long rss_count = 0;

			rdoParse::RDOCorba::GetRSS_var tmp_rss = rdocorbaref->getRDORSSPlist(rss_count);
			rdoParse::RDOCorba::GetRSS_var my_rssList(tmp_rss);
		
			//! Печатаем в файл на С для теста:
			print_RSS(my_rssList);

			//! -------------------------------------------------------------
			//! Добавляем к существующим ресурсам и выводим в трассировке

			//! Переписали имеющиеся ресурсы в rssList
			rdoMBuilder::RDOResourceList rssList(pParser);

			for (ruint i = 0; i < my_rssList->length(); i++)
			{
				//! Нашли тип ресурса по известному имени и создали ресурс указанного типа
				rdoMBuilder::RDOResType _rtp = rtpList[my_rssList[i].m_type.in()];
				rdoMBuilder::RDOResource rss(_rtp, my_rssList[i].m_name.in());

				//! rdoMBuilder::RDOResource::Params::const_iterator it_param = rss.begin();

				for (ruint j = 0; j != my_rssList[i].m_param_count; j++)
				{
					//! Записываем начальные значения параметров ресурса
					switch (my_rssList[i].m_param[j].m_type)
					{
					case rdoParse::RDOCorba::int_type:
						rss[my_rssList[i].m_param[j].m_name.in()] = RDOValue(my_rssList[i].m_param[j].m_int);
						break;
					case rdoParse::RDOCorba::double_type:
						rss[my_rssList[i].m_param[j].m_name.in()] = RDOValue(my_rssList[i].m_param[j].m_double);
						break;
					case rdoParse::RDOCorba::enum_type:
					{
						tstring temp_string;
						temp_string = my_rssList[i].m_param[j].m_enum.in();
						rss[my_rssList[i].m_param[j].m_name.in()] = RDOValue::getIdentificator(temp_string);
						break;
					}
					default: break;
					}												 
				 }

				//! Добавляем к существующим ресурсам
				if (rssList.append<rdoParse::RDORSSResource>(rss))
				{
					//! Добавили успешно
					TRACE(_T("Еще один ресурс добавился!!!\n"));
				}
				else
				{
					//! Неудача
					TRACE(_T("Где-то есть ошибка или ресурс уже существует!!!\n"));
				} 
			}

			//! Вывели все ресурсы для теста
			//! rdoMBuilder::RDOResourceList rssList(&parser);
			STL_FOR_ALL_CONST(rssList, rss_it)
			{
				TRACE2(_T("rss.name = %s: %s\n"), rss_it->name().c_str(), rss_it->getType().name().c_str());
				STL_FOR_ALL_CONST((*rss_it), param_it)
				{
					TRACE2(_T("  %s = %s\n"), param_it->first.c_str(), param_it->second->getAsString().c_str());
				}
			}

			//-------------------------------------------------------------
			orb->destroy();
		}	
		catch(REF(CORBA::TRANSIENT))
		{
			TRACE(_T("Caught system exception TRANSIENT -- unable to contact the server.\n"));
		}
		catch(REF(CORBA::SystemException) ex)
		{
			TRACE1(_T("Caught a CORBA:: %s\n"), ex._name());
		}
		catch(REF(CORBA::Exception) ex)
		{
			TRACE1(_T("Caught CORBA::Exception: %s\n"), ex._name());
		}
		catch(REF(omniORB::fatalException) fe)
		{
			TRACE3(_T("Caught omniORB::fatalException: file: %s line: %d mesg: %s\n"), fe.file() , fe.line() , fe.errmsg());
		}
	}
}

//! ----------------------------------------------------------------------------
//! ---------- RDOParserCorbaRSS
//! ----------------------------------------------------------------------------
void RDOParserCorbaRSS::parse(CREF(LPRDOParser) pParser)
{
/*
	//! Тут надо запросить все ресурсы у парного РДО

	//! Количество полученных ресурсов
	ruint rss_count = 1;

	//! Получили список всех типов ресурсов
	rdoMBuilder::RDOResTypeList rtpList(pParser);
	//! Получили список всех ресурсов
	rdoMBuilder::RDOResourceList rssList(pParser);

	for (ruint i = 0; i < rss_count; i++)
	{
		//! Создали новый ресурс
		rdoMBuilder::RDOResource rss(rtpList["Парикмахеры"], "MyRSS1");
		//! Заполнили его параметры
		rss["длительность_max"] = 174;
		//! Добавляем его к списку существующих
		if (rssList.append<rdoParse::RDORSSResource>(rss))
		{
			//! Добавили успешно
		}
		else
		{
			//! Неудача
		}
		//! Пример проверки ресурса на существование
		if (rssList.exist("qqq"))
		{
		}
	}
*/
}

CLOSE_RDO_PARSER_NAMESPACE

#endif // CORBA_ENABLE
