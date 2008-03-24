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
	m_type( rtp.isPermanent() ? rt_permanent : rt_temporary ),
	m_exist( true ),
	m_id( rtp.getNumber() )
{
	std::vector< const rdoParse::RDORTPParam* >::const_iterator param_it = rtp.getParams().begin();
	while ( param_it != rtp.getParams().end() )
	{
		Param param( **param_it );
		m_params.append( param );
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
RDOResType::RDOResType( const std::string& name, Type type ):
	m_name( name ),
	m_type( type ),
	m_exist( false ),
	m_id( -1 )
{
}

bool RDOResType::ParamList::append( const Param& param )
{
	if ( std::find_if(begin(), end(), rdoParse::compareNameRef<Param>(param.name())) == end() )
	{
		m_list.push_back( param );
		return true;
	}
	else
	{
		return false;
	}
}

RDOResType::Param::Param( const std::string& name, rdoRuntime::RDOValue::Type type ):
	m_name( name ),
	m_type( type ),
	m_exist( true )
{
}

RDOResType::Param::Param( const std::string& name, rdoRuntime::RDOValue::Type type, const rdoRuntime::RDOValue& def ):
	m_name( name ),
	m_type( def ),
	m_default( def ),
	m_exist( true )
{
}

RDOResType::Param::Param( const std::string& name, const rdoRuntime::RDOValue& type ):
	m_name( name ),
	m_type( type ),
	m_exist( true )
{
}

RDOResType::Param::Param( const std::string& name, const rdoRuntime::RDOValue& min, const rdoRuntime::RDOValue& max, const rdoRuntime::RDOValue& def ):
	m_name( name ),
	m_type( min.getType() ),
	m_min( min ),
	m_max( max ),
	m_default( def ),
	m_exist( true )
{
}

RDOResType::Param::Param( const std::string& name, rdoRuntime::RDOEnum* enums ):
	m_name( name ),
	m_exist( true )
{
	if ( enums )
	{
		m_type = *enums;
	}
	else
	{
		m_exist = false;
		m_type  = rdoRuntime::RDOValue::rvt_enum;
	}
}

RDOResType::Param::Param( const std::string& name, rdoRuntime::RDOEnum* enums, const rdoRuntime::RDOEnum::EnumItem& defaultValue ):
	m_name( name ),
	m_exist( true )
{
	if ( enums )
	{
		m_type = *enums;
		setDefault( rdoRuntime::RDOValue(*enums, defaultValue) );
	}
	else
	{
		m_exist = false;
		m_type  = rdoRuntime::RDOValue::rvt_enum;
	}
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
// ---------- RDOResTypeList
// --------------------------------------------------------------------
// ---- —обирает все типы ресурсов, которые есть у парсера
// --------------------------------------------------------------------
RDOResTypeList::RDOResTypeList( rdoParse::RDOParser* parser ):
	RDOList<RDOResType>( parser )
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
bool RDOResTypeList::append( RDOResType& rtp )
{
	if ( std::find_if(begin(), end(), rdoParse::compareNameRef<RDOResType>(rtp.name())) == end() )
	{
		rdoParse::RDORTPResType* pRTP = new rdoParse::RDORTPResType( m_parser, rtp.name(), rtp.isPermanent() );
		RDOResType::ParamList::List::const_iterator param = rtp.m_params.begin();
		while ( param != rtp.m_params.end() )
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
					rdoParse::RDORTPRealDiap* pDiap;
					if ( param->hasDiap() )
					{
						pDiap = new rdoParse::RDORTPRealDiap( m_parser, param->getMin().getDouble(), param->getMax().getDouble() );
					}
					else
					{
						pDiap = new rdoParse::RDORTPRealDiap( m_parser );
					}
					rdoParse::RDORTPRealDefVal* pDef;
					if ( param->hasDefault() )
					{
						pDef = new rdoParse::RDORTPRealDefVal( m_parser, param->getDefault().getDouble() );
					}
					else
					{
						pDef = new rdoParse::RDORTPRealDefVal( m_parser );
					}
					pParamType = new rdoParse::RDORTPRealParamType( m_parser, pDiap, pDef );
					break;
				}
				case rdoRuntime::RDOValue::rvt_enum: {
					rdoParse::RDORTPEnum* pEnum = NULL;
					rdoRuntime::RDOEnum::CIterator enum_it = param->getEnum().begin();
					while ( enum_it != param->getEnum().end() )
					{
						if ( !pEnum )
						{
							pEnum = new rdoParse::RDORTPEnum( pRTP, *enum_it );
						}
						else
						{
							pEnum->add( *enum_it );
						}
						enum_it++;
					}
					rdoParse::RDORTPEnumDefVal* pEnumDefValue;
					if ( param->hasDefault() )
					{
						pEnumDefValue = new rdoParse::RDORTPEnumDefVal( m_parser, param->getDefault().getAsString() );
					}
					else
					{
						pEnumDefValue = new rdoParse::RDORTPEnumDefVal( m_parser );
					}
					pParamType = new rdoParse::RDORTPEnumParamType( pRTP, pEnum, pEnumDefValue, rtp.name() );
					break;
				}
				default: {
					delete pRTP;
					return false;
				}
			}
			pRTP->addParam( new rdoParse::RDORTPParam( pRTP, param->name(), pParamType ) );
			param++;
		}
		rtp.m_exist = true;
		rtp.m_id    = pRTP->getNumber();
		m_list.push_back( rtp );
		return true;
	}
	else
	{
		return false;
	}
}

// --------------------------------------------------------------------
// ---------- RDOResource
// --------------------------------------------------------------------
// ---- »нициализаци€ ресурса по существующему в пам€ти
// ---- —обирает все параметры существующего в пам€ти ресурса
// --------------------------------------------------------------------
RDOResource::RDOResource( const rdoParse::RDORSSResource& rss ):
	m_name( rss.getName() ),
	m_rtp( *rss.getType() ),
	m_exist( true )
{
	if ( m_rtp.m_params.size() == rss.getValues().size() )
	{
		unsigned int index = 0;
		RDOResType::ParamList::List::const_iterator param_it = m_rtp.m_params.begin();
		while ( param_it != m_rtp.m_params.end() )
		{
			m_params[param_it->name()] = rss.getValues()[index];
			index++;
			param_it++;
		}
	}
}

RDOResource::Params::const_iterator RDOResource::operator[] ( const std::string& param ) const
{
	return m_params.find(param);
}

rdoRuntime::RDOValue& RDOResource::operator[] ( const std::string& param )
{
	RDOResource::Params::iterator param_it = m_params.find(param);
	if ( param_it != m_params.end() )
	{
		return param_it->second;
	}
	else
	{
		static rdoRuntime::RDOValue tmpValue;
		return tmpValue;
	}
}

// --------------------------------------------------------------------
// ---- »нициализаци€ *нового* ресурса
// --------------------------------------------------------------------
RDOResource::RDOResource( const RDOResType& rtp, const std::string& name ):
	m_name( name ),
	m_rtp( rtp ),
	m_exist( false )
{
	RDOResType::ParamList::List::const_iterator param_it = m_rtp.m_params.begin();
	while ( param_it != m_rtp.m_params.end() )
	{
		rdoRuntime::RDOValue value( param_it->getTypeObject() );
		if ( param_it->hasDefault() )
		{
			value = param_it->getDefault();
		}
		else if ( param_it->hasDiap() )
		{
			value = param_it->getMin();
		}
		m_params[param_it->name()] = value;
		param_it++;
	}
}

// --------------------------------------------------------------------
// ---------- RDOResourceList
// --------------------------------------------------------------------
// ---- —обирает все ресурсы, которые есть у парсера
// --------------------------------------------------------------------
RDOResourceList::RDOResourceList( rdoParse::RDOParser* parser ):
	RDOList<RDOResource>( parser )
{
	std::vector< rdoParse::RDORSSResource* >::const_iterator rss_it = m_parser->getRSSResources().begin();
	while ( rss_it != m_parser->getRSSResources().end() ) {
		RDOResource rss( **rss_it );
		m_list.push_back( rss );
		rss_it++;
	}
}

// --------------------------------------------------------------------
// ---- ƒобавление *нового* ресурса
// --------------------------------------------------------------------
bool RDOResourceList::append( RDOResource& rss )
{
	if ( std::find_if(begin(), end(), rdoParse::compareNameRef<RDOResource>(rss.name())) == end() )
	{
		const rdoParse::RDORTPResType* pRTP = m_parser->findRTPResType(rss.getType().name());
		if ( !pRTP )
		{
			return false;
		}
		rdoParse::RDORSSResource* pRSS = new rdoParse::RDORSSResource( m_parser, rss.name(), pRTP );
		RDOResType::ParamList::List::const_iterator param_it = rss.getType().m_params.begin();
		while ( param_it != rss.getType().m_params.end() )
		{
			RDOResource::Params::const_iterator value_it = const_cast<const RDOResource&>(rss)[param_it->name()];
			if ( value_it == rss.end() )
			{
				delete pRSS;
				return false;
			}
			pRSS->addValue( value_it->second );
			param_it++;
		}
		rss.m_exist = true;
		m_list.push_back( rss );
		return true;
	}
	else
	{
		return false;
	}
}

} // rdoMBuilder
