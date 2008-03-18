#include "pch.h"
#include "rdo_resources.h"
#include <rdoparser.h>
#include <rdoparser_object.h>
#include <rdortp.h>
#include <rdorss.h>

namespace rdoMBuilder
{

// --------------------------------------------------------------------
// ---------- RDOResType
// --------------------------------------------------------------------
// ---- »нициализаци€ типа ресурса по существующему в пам€ти
// ---- —обирает все параметры существующего в пам€ти типа ресурса
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

RDOResType::Param::Param( const rdoParse::RDORTPParam& param ):
	m_name( param.getName() )
{
	switch ( param.getType()->getType() ) {
		case rdoRuntime::RDOValue::Type::rvt_int: {
			m_type = rdoRuntime::RDOValue( rdoRuntime::RDOValue::Type::rvt_int );
			const rdoParse::RDORTPIntParamType* param_type = static_cast<const rdoParse::RDORTPIntParamType*>(param.getType());
			if ( param_type->getDiap().isExist() ) {
				m_min = param_type->getDiap().getMin();
				m_max = param_type->getDiap().getMax();
			}
			break;
		}
		case rdoRuntime::RDOValue::Type::rvt_real: {
			m_type = rdoRuntime::RDOValue( rdoRuntime::RDOValue::Type::rvt_real );
			const rdoParse::RDORTPRealParamType* param_type = static_cast<const rdoParse::RDORTPRealParamType*>(param.getType());
			if ( param_type->getDiap().isExist() ) {
				m_min = param_type->getDiap().getMin();
				m_max = param_type->getDiap().getMax();
			}
			break;
		}
		case rdoRuntime::RDOValue::Type::rvt_enum: {
			const rdoParse::RDORTPEnumParamType* enumParamType = static_cast<const rdoParse::RDORTPEnumParamType*>(param.getType());
			m_type = rdoRuntime::RDOValue( enumParamType->enu->getEnums() );
			break;
		}
	}
	if ( param.getType()->getDV().isExist() )
	{
		m_default = param.getType()->getDefaultValue( param.getType()->getDV().src_info() );
	}
}

// --------------------------------------------------------------------
// ---- »нициализаци€ *нового* типа ресурса
// --------------------------------------------------------------------
RDOResType::RDOResType( const std::string& name, bool permanent ):
	m_name( name ),
	m_permanent( permanent )
{
}

bool RDOResType::append( const Param& param )
{
	if ( std::find_if(begin(), end(), rdoParse::compareNameRef<Param>(param.getName())) == end() )
	{
		m_params.push_back( param );
		return true;
	}
	else
	{
		return false;
	}
}

RDOResType::Param::Param( const std::string& name, rdoRuntime::RDOValue::Type type ):
	m_name( name ),
	m_type( type )
{
}

RDOResType::Param::Param( const std::string& name, rdoRuntime::RDOValue::Type type, const rdoRuntime::RDOValue& def ):
	m_name( name ),
	m_type( def ),
	m_default( def )
{
}

RDOResType::Param::Param( const std::string& name, const rdoRuntime::RDOValue& type ):
	m_name( name ),
	m_type( type )
{
}

RDOResType::Param::Param( const std::string& name, const rdoRuntime::RDOValue& min, const rdoRuntime::RDOValue& max, const rdoRuntime::RDOValue& def ):
	m_name( name ),
	m_type( min.getType() ),
	m_min( min ),
	m_max( max ),
	m_default( def )
{
}

void RDOResType::Param::setDiap( const rdoRuntime::RDOValue& min, const rdoRuntime::RDOValue& max )
{
	m_min = min;
	m_max = max;
}

void RDOResType::Param::setDefault( const rdoRuntime::RDOValue& def )
{
	m_default = def;
}

bool RDOResType::Param::operator== (const Param& param ) const
{
	return true;
}

// --------------------------------------------------------------------
// ---------- RDOResource
// --------------------------------------------------------------------
// ---- »нициализаци€ ресурса по существующему в пам€ти
// ---- —обирает все параметры существующего в пам€ти ресурса
// --------------------------------------------------------------------
RDOResource::RDOResource( const rdoParse::RDORSSResource& rss ):
	m_name( rss.getName() ),
	m_rtp( *rss.getType() )
{
	if ( m_rtp.size() == rss.getValues().size() )
	{
		unsigned int index = 0;
		RDOResType::ParamList::const_iterator param_it = m_rtp.begin();
		while ( param_it != m_rtp.end() )
		{
			m_params[param_it->getName()] = rss.getValues()[index];
			index++;
			param_it++;
		}
	}
}

// --------------------------------------------------------------------
// ---------- RDOResTypeList
// --------------------------------------------------------------------
// ---- —обирает все типы ресурсов, которые есть у парсера
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

// --------------------------------------------------------------------
// ---- ƒобавление *нового* типа ресурса
// --------------------------------------------------------------------
bool RDOResTypeList::append( const RDOResType& rtp )
{
	if ( std::find_if(begin(), end(), rdoParse::compareNameRef<RDOResType>(rtp.getName())) == end() )
	{
		m_list.push_back( rtp );

		rdoParse::RDORTPResType* pRTP = new rdoParse::RDORTPResType( m_parser, rtp.getName(), rtp.isPermanent() );
		RDOResType::ParamList::const_iterator param = rtp.begin();
		while ( param != rtp.end() )
		{
			rdoParse::RDORTPParamType* pParamType = NULL;
			switch ( param->getType() )
			{
				case rdoRuntime::RDOValue::rvt_int: {
					rdoParse::RDORTPIntDiap* pDiap;
					if ( param->hasDiap() )
					{
						pDiap = new rdoParse::RDORTPIntDiap( m_parser, param->getMin().getInt(), param->getMax().getInt() );
					}
					else
					{
						pDiap = new rdoParse::RDORTPIntDiap( m_parser );
					}
					rdoParse::RDORTPIntDefVal* pDef;
					if ( param->hasDefault() )
					{
						pDef = new rdoParse::RDORTPIntDefVal( m_parser, param->getDefault().getInt() );
					}
					else
					{
						pDef = new rdoParse::RDORTPIntDefVal( m_parser );
					}
					pParamType = new rdoParse::RDORTPIntParamType( m_parser, pDiap, pDef );
					break;
				}
				case rdoRuntime::RDOValue::rvt_real: {
					break;
				}
				case rdoRuntime::RDOValue::rvt_enum: {
					break;
				}
				default: {
					delete pRTP;
					return false;
				}
			}
			pRTP->addParam( new rdoParse::RDORTPParam( pRTP, param->getName(), pParamType ) );
			param++;
		}
		return true;
	}
	else
	{
		return false;
	}
}

// --------------------------------------------------------------------
// ---------- RDOResourceList
// --------------------------------------------------------------------
// ---- —обирает все ресурсы, которые есть у парсера
// --------------------------------------------------------------------
RDOResourceList::RDOResourceList( rdoParse::RDOParser* parser ):
	m_parser( parser )
{
	std::vector< rdoParse::RDORSSResource* >::const_iterator rss_it = m_parser->getRSSResources().begin();
	while ( rss_it != m_parser->getRSSResources().end() ) {
		RDOResource rss( **rss_it );
		m_list.push_back( rss );
		rss_it++;
	}
}

} // rdoMBuilder
