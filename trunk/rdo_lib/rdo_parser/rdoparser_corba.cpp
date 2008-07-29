#include "pch.h"
#include "rdoparser_corba.h"
#include "rdortp.h"
#include <rdo_resources.h>
#include <rdoruntime_object.h>

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
	rdoRuntime::RDOFuzzySetDefinitionRangeDiscret* fuzzy_setDefinition1 = new rdoRuntime::RDOFuzzySetDefinitionRangeDiscret( m_parser->runtime() );
	fuzzy_setDefinition1->append(0, 50);
	rdoRuntime::RDOFuzzyType* fuzzy_type1  = new rdoRuntime::RDOFuzzyType( fuzzy_setDefinition1 );

	rdoRuntime::RDOFuzzyValue fuzzy_value1(*fuzzy_type1);
	fuzzy_value1.append(1, 1.0)(2, 1.0)(3, 1.0)(4, 1.0)(5, 1.0)(6, 1.0)(7, 1.0)(8, 1.0)(9, 1.0);

	rdoRuntime::RDOFuzzyValue fuzzy_value2(*fuzzy_type1);
	fuzzy_value2.append(10, 1.0);

	fuzzy_value1 = fuzzy_value1 + fuzzy_value2;

	std::string str1 = fuzzy_value1.defuzzyfication().getAsString();

	int i = 1;

/*
	rdoRuntime::RDOFuzzySetDefinitionRangeDiscret* fuzzy_setDefinition1 = new rdoRuntime::RDOFuzzySetDefinitionRangeDiscret( m_parser->runtime() );
	fuzzy_setDefinition1->append(1, 7);
	rdoRuntime::RDOFuzzyType* fuzzy_type1  = new rdoRuntime::RDOFuzzyType( fuzzy_setDefinition1 );

	rdoRuntime::RDOFuzzyValue fuzzy_value1(*fuzzy_type1);
	fuzzy_value1.append(3, 0.9)(4, 1.0)(6, 0.6);

	rdoRuntime::RDOFuzzyValue fuzzy_value2(*fuzzy_type1);
	fuzzy_value2.append(3, 0.7)(5, 1.0)(6, 0.4);

	rdoRuntime::RDOValue rdo_value1( fuzzy_value1 );
	rdoRuntime::RDOValue rdo_value2( fuzzy_value2 );
	rdoRuntime::RDOValue rdo_value3 = rdo_value1 && rdo_value2;
*/
/*
	// Тут надо запросить все типы ресурсов у парного РДО,
	// вызвав с помощью корбы некий метод, который вернёт кучу структур
	// с описанием RTP и насоздавать этих типов


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
