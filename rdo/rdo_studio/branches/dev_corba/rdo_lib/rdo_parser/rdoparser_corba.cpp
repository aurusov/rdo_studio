#include "pch.h"
#include "rdoparser_corba.h"
#include "rdortp.h"
#include <rdo_resources.h>
#include <rdoruntime_object.h>
#include "RDOCorba.hh"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rdoParse
{

// ----------------------------------------------------------------------------
// ---------- RDOParserCorbaRTP
// ----------------------------------------------------------------------------
void RDOParserCorbaRTP::parse()
{
	// Тут надо запросить все типы ресурсов у парного РДО,
	// вызвав с помощью корбы некий метод, который вернёт кучу структур
	// с описанием RTP и насоздавать этих типов

/*
	//-------------------------Пока сделаю вручную без коннекта
	//Количество новых типов ресурсов (только один пока для начала)
	CORBA::Long rtp_count = 1;

	//Массив из количества параметров ресурсов каждого типа ресурсов (2 разных пока для начала)
	rdoParse::RDOCorba::PARAM_count params_count;
	params_count.length( rtp_count );
	params_count[0] = 2;

	rdoParse::RDOCorba::GetRTP my_rtpList;
	my_rtpList.length( rtp_count);

	//Инициализация буферных структур, чтобы из них создать далее тип
	//Конечно, они будут уже заполнены
	for (CORBA::Long i = 0; i < rtp_count; i++)
	{
		my_rtpList[i].m_name = "Тип ресурса №1";
		my_rtpList[i].m_type = rdoParse::RDOCorba::TypeRTP::rt_permanent;
		my_rtpList[i].m_param.length(params_count[i]);

		my_rtpList[i].m_param[0].m_name = "Целый";
		my_rtpList[i].m_param[0].m_type = rdoParse::RDOCorba::TypeParam::int_type;
		my_rtpList[i].m_param[0].m_diap_int = 1;
		my_rtpList[i].m_param[0].m_default_int_ch = 1;
		my_rtpList[i].m_param[0].m_min_int = 1;
		my_rtpList[i].m_param[0].m_max_int = 10;
		my_rtpList[i].m_param[0].m_default_int = 5;

		my_rtpList[i].m_param[1].m_name = "Вещественный";
		my_rtpList[i].m_param[1].m_type = rdoParse::RDOCorba::TypeParam::double_type;
		my_rtpList[i].m_param[1].m_diap_double = 1;
		my_rtpList[i].m_param[1].m_default_double_ch = 1;
		my_rtpList[i].m_param[1].m_min_double = 1;
		my_rtpList[i].m_param[1].m_max_double = 10;
		my_rtpList[i].m_param[1].m_default_double = 5;
	}

	//Создаем и добавляем копии собранных типов ресурсов
	
	// Получили список всех описанных типов ресурсов
	rdoMBuilder::RDOResTypeList rtpList( m_parser );
	
	for (CORBA::Long i = 0; i < rtp_count; i++)
	{

		// Создаем новый тип ресурса
		rdoMBuilder::RDOResType rtp( my_rtpList[i].m_name.in() );
		// Наполняем его параметрами

		for (CORBA::Long j = 0; j < params_count[i]; j++)
		{
			switch (my_rtpList[i].m_param[j].m_type ){
				case rdoParse::RDOCorba::TypeParam::int_type:{
			
					unsigned int fl = 0;

					if ( my_rtpList[i].m_param[j].m_diap_int == 1 &&  my_rtpList[i].m_param[j].m_default_int_ch == 1) 
						fl = 1;
					else
					{
						if ( my_rtpList[i].m_param[j].m_diap_int == 1 )		
							fl = 2;
						else	
						{
							if ( my_rtpList[i].m_param[j].m_default_int_ch = 1 )
								fl = 3;
						}
					}

					switch (fl){
						case 0:{//Нет ни диапазона, ни значения по умолчанию
							rtp.m_params.append( rdoMBuilder::RDOResType::Param( my_rtpList[i].m_param[j].m_name.in() , rdoRuntime::g_int) );
							break;
						}
   						case 1:{//Есть и диапазон, и значение по умолчанию
							rtp.m_params.append( rdoMBuilder::RDOResType::Param( my_rtpList[i].m_param[j].m_name.in() , my_rtpList[i].m_param[j].m_min_int , my_rtpList[i].m_param[j].m_max_int , my_rtpList[i].m_param[j].m_default_int ) );
							break;
						}
						case 2:{//Есть только дипапазон значений
							rtp.m_params.append( rdoMBuilder::RDOResType::Param( my_rtpList[i].m_param[j].m_name.in() , my_rtpList[i].m_param[j].m_min_int , my_rtpList[i].m_param[j].m_max_int ) );
							break;
						}
   						case 3:{//Есть толко значение по умолчанию
							rtp.m_params.append( rdoMBuilder::RDOResType::Param( my_rtpList[i].m_param[j].m_name.in() , rdoRuntime::g_int , my_rtpList[i].m_param[j].m_default_int ) );
							break;
						}
						default: break;
					}
					break;
				}
				case rdoParse::RDOCorba::TypeParam::double_type:{
			
					unsigned int fl = 0;

					if ( my_rtpList[i].m_param[j].m_diap_double == 1 &&  my_rtpList[i].m_param[j].m_default_double_ch == 1) 
						fl = 1;
					else
					{
						if ( my_rtpList[i].m_param[j].m_diap_double == 1 )		
							fl = 2;
						else	
						{
							if ( my_rtpList[i].m_param[j].m_default_double_ch = 1 )
								fl = 3;
						}
					}

					switch (fl){
						case 0:{//Нет ни диапазона, ни значения по умолчанию
							rtp.m_params.append( rdoMBuilder::RDOResType::Param( my_rtpList[i].m_param[j].m_name.in() , rdoRuntime::g_real) );
							break;
						}
   						case 1:{//Есть и диапазон, и значение по умолчанию
							rtp.m_params.append( rdoMBuilder::RDOResType::Param( my_rtpList[i].m_param[j].m_name.in() , my_rtpList[i].m_param[j].m_min_double , my_rtpList[i].m_param[j].m_max_double , my_rtpList[i].m_param[j].m_default_double ) );
							break;
						}
						case 2:{//Есть только дипапазон значений
							rtp.m_params.append( rdoMBuilder::RDOResType::Param( my_rtpList[i].m_param[j].m_name.in() , my_rtpList[i].m_param[j].m_min_double , my_rtpList[i].m_param[j].m_max_double ) );
							break;
						}
   						case 3:{//Есть толко значение по умолчанию
							rtp.m_params.append( rdoMBuilder::RDOResType::Param( my_rtpList[i].m_param[j].m_name.in() , rdoRuntime::g_real , my_rtpList[i].m_param[j].m_default_double ) );
							break;
						}
						default: break;
					}
					break;
				}
				case rdoParse::RDOCorba::TypeParam::enum_type:{
		

					//Пока не заморачиваюсь

					break;
				}
				default: break;
			}
		
		}

		//Добавляем к существующим
		if ( rtpList.append( rtp ) )
		{
			// Добавили успешно
		}
		else
		{
			// Неудача, возможно, тип с таким именем уже есть
			//parser->error( info, rdo::format("Ошибка создания типа ресурса: %s", rtp_name.c_str()) );
	
		}
	}

*/

/*



	// Количество полученных типов ресурсов
	unsigned int rtp_count = 1;

	// Получили список всех типов ресурсов
	rdoMBuilder::RDOResTypeList rtpList( m_parser );

	for ( unsigned int i = 0; i < rtp_count; i++ )
	{
		// Создали новый тип ресурса
		rdoMBuilder::RDOResType rtp( "MyRTP" );
		// Наполнили его параметрами
		rtp.m_params.append( rdoMBuilder::RDOResType::Param("p1", rdoRuntime::g_int) );
		rtp.m_params.append( rdoMBuilder::RDOResType::Param("p2", rdoRuntime::g_int) );
		// Добавили его к списку существующих
		if ( rtpList.append( rtp ) )
		{
			// Добавили успешно
			unsigned int fl = 0;

		}
		else
		{
			// Неудача, возможно, тип с таким именем уже есть
		}
	}

*/

}

// ----------------------------------------------------------------------------
// ---------- RDOParserCorbaRSS
// ----------------------------------------------------------------------------
void RDOParserCorbaRSS::parse()
{
	// Тут надо запросить все ресурсы у парного РДО

	// Количество полученных ресурсов
	unsigned int rss_count = 1;

	// Получили список всех типов ресурсов
	rdoMBuilder::RDOResTypeList rtpList( m_parser );
	// Получили список всех ресурсов
	rdoMBuilder::RDOResourceList rssList( m_parser );

	for ( unsigned int i = 0; i < rss_count; i++ )
	{
		// Создали новый ресурс
		rdoMBuilder::RDOResource rss( rtpList["Парикмахеры"], "MyRSS1" );
		// Заполнили его параметры
		rss["длительность_max"] = 174;
		// Добавляем его к списку существующих
		if ( rssList.append<rdoParse::RDORSSResource>( rss ) )
		{
			// Добавили успешно
		}
		else
		{
			// Неудача
		}
		// Пример проверки ресурса на существование
		if ( rssList.exist("qqq") )
		{
		}
	}
}

} // namespace rdoParse
