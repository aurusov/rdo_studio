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
	{
		// Получили список всех типов ресурсов
		rdoMBuilder::RDOResTypeList rtpList( m_parser );
		// Создали новый тип ресурса
		rdoMBuilder::RDOResType rtp( "MyRTP" );
		rtp.m_params.append( rdoMBuilder::RDOResType::Param("p1", rdoRuntime::RDOValue::rvt_int) );
		rtp.m_params.append( rdoMBuilder::RDOResType::Param("p2", rdoRuntime::RDOValue::rvt_int) );
		// Добавили его к списку существующих
		rtpList.append( rtp );
	}

	// Вывели все типы ресурсов
	rdoMBuilder::RDOResTypeList rtpList( m_parser );
	rdoMBuilder::RDOResTypeList::List::const_iterator rtp_it = rtpList.begin();
	while ( rtp_it != rtpList.end() )
	{
		TRACE("rtp.name = %s\n", rtp_it->name().c_str());
		rdoMBuilder::RDOResType::ParamList::List::const_iterator param_it = rtp_it->m_params.begin();
		while ( param_it != rtp_it->m_params.end() )
		{
			std::string info = rdo::format("  param: %s: %s", param_it->name().c_str(), param_it->getTypeStr().c_str());
			if ( param_it->hasDiap() )
			{
				info = rdo::format("%s [%s..%s]", info.c_str(), param_it->getMin().getAsString().c_str(), param_it->getMax().getAsString().c_str());
			}
			if ( param_it->hasDefault() )
			{
				info = rdo::format("%s = %s", info.c_str(), param_it->getDefault().getAsString().c_str());
			}
			TRACE( "%s\n", info.c_str() );

			if ( param_it->getType() == rdoRuntime::RDOValue::rvt_enum )
			{
				rdoRuntime::RDOEnum::CIterator enum_it = param_it->getEnum().begin();
				while ( enum_it != param_it->getEnum().end() )
				{
					TRACE( "  - enum - %s\n", enum_it->c_str() );
					enum_it++;
				}
			}
			param_it++;
		}
		rtp_it++;
	}

	// Тут надо запросить все типы ресурсов у парного РДО,
	// вызвав с помощью корбы некий метод, который вернёт кучу структур
	// с описанием RTP
//	RDORTPResType* rtp = new RDORTPResType( parser, std::string("RTPCorba"), true );
//	rtp->addParam( "param1", rdoRuntime::RDOValue::pt_int );
}

// ----------------------------------------------------------------------------
// ---------- RDOParserCorbaRSS
// ----------------------------------------------------------------------------
void RDOParserCorbaRSS::parse()
{
	{
		// Получили список всех типов ресурсов
		rdoMBuilder::RDOResTypeList rtpList( m_parser );
		// Получили список всех ресурсов
		rdoMBuilder::RDOResourceList rssList( m_parser );
		// Создали новый ресурс
		rdoMBuilder::RDOResource rss( rtpList["Парикмахеры"], "MyRSS1" );
		rss["длительность_max"] = 174;
		// Добавляем его к списку существующих
		rssList.append( rss );

		rssList.exist("qqq");
	}

	// Вывели все ресурсы
	rdoMBuilder::RDOResourceList rssList( m_parser );
	rdoMBuilder::RDOResourceList::List::const_iterator rss_it = rssList.begin();
	while ( rss_it != rssList.end() )
	{
		TRACE("rss.name = %s: %s\n", rss_it->name().c_str(), rss_it->getType().name().c_str());
		rdoMBuilder::RDOResource::Params::const_iterator param_it = rss_it->begin();
		while ( param_it != rss_it->end() )
		{
			TRACE("  %s = %s\n", param_it->first.c_str(), param_it->second.getAsString().c_str());
			param_it++;
		}
		rss_it++;
	}

	// Тут надо запросить все ресурсы у парного РДО
}

} // namespace rdoParse
