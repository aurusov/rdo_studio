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
	rdoRuntime::RDOFuzzyFixedSet fuzzy_fixedSet1( m_parser->runtime() );
	fuzzy_fixedSet1.append( rdoRuntime::RDOValue(0), 1.0 );
	fuzzy_fixedSet1.append( rdoRuntime::RDOValue(1), 1.0 );
	fuzzy_fixedSet1.append( rdoRuntime::RDOValue(2), 0.0 );
	rdoRuntime::RDOFuzzyType  fuzzy_type1 ( &fuzzy_fixedSet1 );
	rdoRuntime::RDOFuzzyValue fuzzy_value1( fuzzy_type1  );
	rdoRuntime::RDOValue      rdo_value1  ( fuzzy_value1 );
	rdoRuntime::RDOValue      rdo_value2  ( rdo_value1 );
	int i = rdo_value2.getInt();
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
