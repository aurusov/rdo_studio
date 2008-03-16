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
	rdoMBuilder::RDOResTypeList rtpList( m_parser );
	rdoMBuilder::RDOResTypeList::RTPList::const_iterator rtp_it = rtpList.begin();
	while ( rtp_it != rtpList.end() )
	{
		TRACE("rtp.name = %s\n", rtp_it->getName().c_str());
		rdoMBuilder::RDOResType::ParamList::const_iterator param_it = rtp_it->begin();
		while ( param_it != rtp_it->end() )
		{
			std::string info = rdo::format("  param: %s: %s", param_it->getName().c_str(), param_it->getTypeStr().c_str());
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
	// Тут надо запросить все ресурсы у парного РДО
}

} // namespace rdoParse
