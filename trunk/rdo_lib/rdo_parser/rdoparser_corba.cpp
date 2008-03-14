#include "pch.h"
#include "rdoparser_corba.h"
#include "rdortp.h"
#include <rdo_resources.h>

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
			TRACE("  param: %s\n", param_it->getName().c_str());
			param_it++;
		}
		rtp_it++;
	}

	// Тут надо запросить все типы ресурсов у парного РДО,
	// вызвав с помощью корбы некий метод, который вернут кучу структур
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
