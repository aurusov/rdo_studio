#include "pch.h"
#include "rdoparser_corba.h"
#include "rdortp.h"

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
