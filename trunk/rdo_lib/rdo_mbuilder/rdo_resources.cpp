#include "pch.h"
#include "rdo_resources.h"
#include <rdoparser.h>
#include <rdortp.h>

namespace rdoMBuilder
{

// --------------------------------------------------------------------
// ---------- RDOResType
// --------------------------------------------------------------------
RDOResType::RDOResType( const rdoParse::RDORTPResType& rtp ):
	m_name( rtp.getName() ),
	m_permanent( rtp.isPermanent() )
{
	std::vector< const rdoParse::RDORTPParam* >::const_iterator param_it = rtp.getParams().begin();
	while ( param_it != rtp.getParams().end() )
	{
		Param param( **param_it );
		m_params.push_back( param );
		param_it++;
	}
}

RDOResType::RDOResType( const std::string& name ):
	m_name( name ),
	m_permanent( true )
{
}

RDOResType::Param::Param( const rdoParse::RDORTPParam& param ):
	m_name( param.getName() )
{
}

// --------------------------------------------------------------------
// ---------- RDOResTypeList
// --------------------------------------------------------------------
RDOResTypeList::RDOResTypeList( rdoParse::RDOParser* parser ):
	m_parser( parser )
{
	std::vector< rdoParse::RDORTPResType* >::const_iterator rtp_it = m_parser->getRTPResType().begin();
	while ( rtp_it != m_parser->getRTPResType().end() ) {
		RDOResType rtp( **rtp_it );
		m_list.push_back( rtp );
		rtp_it++;
	}
}

} // rdoMBuilder
