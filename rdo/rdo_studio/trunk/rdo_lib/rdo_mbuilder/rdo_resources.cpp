#include "pch.h"
#include "rdo_resources.h"
#include <rdortp.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rdoMBuilder
{

// --------------------------------------------------------------------
// ----------  онструкторы копий и операторы присваивани€
// --------------------------------------------------------------------
RDOResType::RDOResType( const RDOResType& obj )
{
	operator=( obj );
}

RDOResType::Param::Param( const RDOResType::Param& obj )
{
	operator=( obj );
}

RDOResource::RDOResource( const RDOResource& obj )
{
	operator=( obj );
}

void RDOResType::operator= (const RDOResType& obj )
{
	m_name   = obj.m_name;
	m_exist  = obj.m_exist;
	m_type   = obj.m_type;
	m_id     = obj.m_id;
	m_params = obj.m_params;
}

void RDOResType::Param::operator= (const RDOResType::Param& obj )
{
	m_name    = obj.m_name;
	m_exist   = obj.m_exist;
	m_type    = obj.m_type;
	m_id      = obj.m_id;
	m_min     = obj.m_min;
	m_max     = obj.m_max;
	m_default = obj.m_default;
}

void RDOResource::operator= (const RDOResource& obj )
{
	m_name   = obj.m_name;
	m_exist  = obj.m_exist;
	m_rtp    = obj.m_rtp;
	m_id     = obj.m_id;
	m_params.clear();
	Params::const_iterator it = obj.m_params.begin();
	while ( it != obj.m_params.end() )
	{
		m_params[ it->first ] = it->second;
		it++;
	}
}

// --------------------------------------------------------------------
// ---------- RDOResType
// --------------------------------------------------------------------
// ---- »нициализаци€ типа ресурса по существующему в пам€ти
// ---- —обирает все параметры существующего в пам€ти типа ресурса
// --------------------------------------------------------------------
RDOResType::RDOResType( const rdoParse::RDORTPResType& rtp ):
	m_name( rtp.name() ),
	m_type( rtp.isPermanent() ? rt_permanent : rt_temporary ),
	m_exist( true ),
	m_id( rtp.getNumber() )
{
	std::vector< const rdoParse::RDORTPParam* >::const_iterator param_it = rtp.getParams().begin();
	while ( param_it != rtp.getParams().end() )
	{
		Param param( **param_it );
		param.m_id = m_params.size();
		m_params.append( param );
		param_it++;
	}
}

RDOResType::Param::Param( const rdoParse::RDORTPParam& param ):
	m_name( param.name() ),
	m_type( &param.getType()->type().type() ),
	m_exist( true ),
	m_id( -1 )
{
	switch ( typeID() )
	{
		case rdoRuntime::RDOType::t_int:
		{
			const rdoParse::RDORTPIntParamType* param_type = static_cast<const rdoParse::RDORTPIntParamType*>(param.getType());
			if ( param_type->getDiap().isExist() ) {
				m_min = param_type->getDiap().getMin();
				m_max = param_type->getDiap().getMax();
			}
			break;
		}
		case rdoRuntime::RDOType::t_real:
		{
			const rdoParse::RDORTPRealParamType* param_type = static_cast<const rdoParse::RDORTPRealParamType*>(param.getType());
			if ( param_type->getDiap().isExist() ) {
				m_min = param_type->getDiap().getMin();
				m_max = param_type->getDiap().getMax();
			}
			break;
		}
	}
	if ( param.getType()->getDV().isExist() )
	{
		m_default = param.getType()->getDefaultValue( param.getType()->getDV().value() );
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

bool RDOResType::ParamList::append( Param& param )
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

RDOResType::Param::Param( const std::string& name, const rdoRuntime::RDOType* type, const rdoRuntime::RDOValue& def ):
	m_name( name ),
	m_type( type ),
	m_default( def ),
	m_exist( true ),
	m_id( -1 )
{
	if (type->id() == rdoRuntime::RDOType::t_enum && def.typeID() == rdoRuntime::RDOType::t_string)
	{
		m_default = rdoRuntime::RDOValue(getEnum(), def.getAsString());
	}
}

RDOResType::Param::Param( const std::string& name, const rdoRuntime::RDOValue& def ):
	m_name( name ),
	m_type( &def.type() ),
	m_default( def ),
	m_exist( true ),
	m_id( -1 )
{
}

RDOResType::Param::Param( const std::string& name, const rdoRuntime::RDOValue& min, const rdoRuntime::RDOValue& max, const rdoRuntime::RDOValue& def ):
	m_name( name ),
	m_type( &rdoRuntime::RDOType::getTypeByID(min.typeID()) ),
	m_min( min ),
	m_max( max ),
	m_default( def ),
	m_exist( true ),
	m_id( -1 )
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
// ---------- RDOResTypeList
// --------------------------------------------------------------------
// ---- —обирает все типы ресурсов, которые есть у парсера
// --------------------------------------------------------------------
RDOResTypeList::RDOResTypeList( rdoParse::RDOParser* parser ):
	RDOList<RDOResType>( parser )
{
	std::vector< rdoParse::RDORTPResType* >::const_iterator rtp_it = m_parser->getRTPResTypes().begin();
	while ( rtp_it != m_parser->getRTPResTypes().end() ) {
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
			switch ( param->typeID() )
			{
				case rdoRuntime::RDOType::t_int:
				{
					rdoParse::RDORTPIntDiap* pDiap;
					if ( param->hasDiap() )
					{
						pDiap = new rdoParse::RDORTPIntDiap( m_parser, param->getMin().getInt(), param->getMax().getInt() );
					}
					else
					{
						pDiap = new rdoParse::RDORTPIntDiap( m_parser );
					}
					rdoParse::RDORTPDefVal* pDef;
					if ( param->hasDefault() )
					{
						pDef = new rdoParse::RDORTPDefVal( m_parser, param->getDefault() );
					}
					else
					{
						pDef = new rdoParse::RDORTPDefVal( m_parser );
					}
					pParamType = new rdoParse::RDORTPIntParamType( m_parser, pDiap, pDef );
					break;
				}
				case rdoRuntime::RDOType::t_real:
				{
					rdoParse::RDORTPRealDiap* pDiap;
					if ( param->hasDiap() )
					{
						pDiap = new rdoParse::RDORTPRealDiap( m_parser, param->getMin().getDouble(), param->getMax().getDouble() );
					}
					else
					{
						pDiap = new rdoParse::RDORTPRealDiap( m_parser );
					}
					rdoParse::RDORTPDefVal* pDef;
					if ( param->hasDefault() )
					{
						pDef = new rdoParse::RDORTPDefVal( m_parser, param->getDefault() );
					}
					else
					{
						pDef = new rdoParse::RDORTPDefVal( m_parser );
					}
					pParamType = new rdoParse::RDORTPRealParamType( m_parser, pDiap, pDef );
					break;
				}
				case rdoRuntime::RDOType::t_enum:
				{
					rdoParse::RDORTPEnum* pEnum = NULL;
					rdoRuntime::RDOEnumType::CIterator enum_it = param->getEnum().begin();
					while ( enum_it != param->getEnum().end() )
					{
						if ( !pEnum )
						{
							pEnum = new rdoParse::RDORTPEnum( pRTP, rdoParse::RDOValue::getIdentificator(*enum_it) );
						}
						else
						{
							pEnum->add( rdoParse::RDOValue::getIdentificator(*enum_it) );
						}
						enum_it++;
					}
					rdoParse::RDORTPDefVal* pEnumDefValue;
					if ( param->hasDefault() )
					{
						pEnumDefValue = new rdoParse::RDORTPDefVal( m_parser, param->getDefault() );
					}
					else
					{
						pEnumDefValue = new rdoParse::RDORTPDefVal( m_parser );
					}
					pParamType = new rdoParse::RDORTPEnumParamType( pRTP, pEnum, pEnumDefValue, rtp.name() );
					break;
				}
				default:
				{
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
RDOResource::RDOResource( const rdoParse::RDORSSResource& rss )
	: m_name (rss.name()    )
	, m_rtp  (*rss.getType())
	, m_exist(true          )
	, m_id   (rss.getID()   )
{
	if ( m_rtp.m_params.size() == rss.params().size() )
	{
		unsigned int index = 0;
		RDOResType::ParamList::List::const_iterator param_it = m_rtp.m_params.begin();
		while ( param_it != m_rtp.m_params.end() )
		{
//			const rdoRuntime::RDOValue& value = rss.params()[index];
//			m_params[param_it->name()] = rdoParse::RDOValue(value, rdoParse::RDOType(value.type()));
			m_params[param_it->name()] = rss.params()[index];
			index++;
			param_it++;
		}
	}
}

RDOResource::Params::const_iterator RDOResource::operator[] ( const std::string& param ) const
{
	return m_params.find(param);
}

RDOResource::Params::mapped_type& RDOResource::operator[] ( const std::string& param )
{
	RDOResource::Params::iterator param_it = m_params.find(param);
	if ( param_it != m_params.end() )
	{
		return param_it->second;
	}
	else
	{
		static Params::mapped_type tmpValue;
		return tmpValue;
	}
}

bool RDOResource::fillParserResourceParams(rdoParse::RDORSSResource* pRSS) const
{
	RDOResType::ParamList::List::const_iterator param_it = getType().m_params.begin();
	while ( param_it != getType().m_params.end() )
	{
		RDOResource::Params::const_iterator value_it = operator[](param_it->name());
		if ( value_it == end() )
		{
			return false;
		}
		pRSS->addParam( value_it->second );
		param_it++;
	}
	return true;
}

// --------------------------------------------------------------------
// ---- »нициализаци€ *нового* ресурса
// --------------------------------------------------------------------
RDOResource::RDOResource( const RDOResType& rtp, const std::string& name )
	: m_name (name                                  )
	, m_rtp  (rtp                                   )
	, m_exist(false                                 )
	, m_id   (rdoParse::RDORSSResource::UNDEFINED_ID)
{
	RDOResType::ParamList::List::const_iterator param_it = m_rtp.m_params.begin();
	while ( param_it != m_rtp.m_params.end() )
	{
		rdoRuntime::RDOValue value( *param_it->type() );
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

} // rdoMBuilder
