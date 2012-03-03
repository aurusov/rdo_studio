#ifndef RDOPARSER_VALUE_H
#define RDOPARSER_VALUE_H

#include "rdoparser_type.h"
#include <rdo_value.h>

namespace rdoParse 
{

// ----------------------------------------------------------------------------
// ---------- RDOValue
// ----------------------------------------------------------------------------
class RDOValue: public RDOParserSrcInfo
{
public:
	// Для атомарных типов, иначе throw в RDOType::getTypeByID
	RDOValue( const rdoRuntime::RDOValue& value, const RDOParserSrcInfo& src_info = RDOParserSrcInfo() ):
		RDOParserSrcInfo( src_info ),
		m_value( value ),
		m_type( &RDOType::getTypeByID(value.type().id()) )
	{
	}
	RDOValue( const rdoRuntime::RDOValue& value, const RDOType& type, const RDOParserSrcInfo& src_info ):
		RDOParserSrcInfo( src_info ),
		m_value( value ),
		m_type( &type )
	{
	}
	RDOValue( const RDOValue& value ):
		RDOParserSrcInfo( value.src_info() ),
		m_value( value.value() ),
		m_type( &value.type() )
	{
	}
	RDOValue( const RDOType& type, const RDOParserSrcInfo& src_info = RDOParserSrcInfo() ):
		RDOParserSrcInfo( src_info ),
		m_value( type.type() ),
		m_type( &type )
	{
	}
	// Для t_identificator известно только имя, но не тип
	RDOValue( const RDOParserSrcInfo& src_info ):
		RDOParserSrcInfo( src_info ),
		m_value( rdoRuntime::RDOValue(src_info.src_text(), rdoRuntime::g_identificator) ),
		m_type( &rdoParse::g_identificator )
	{
	}
	// Неопределенный тип
	RDOValue():
		RDOParserSrcInfo(),
		m_value( rdoRuntime::RDOValue(rdoRuntime::g_unknow) ),
		m_type( &rdoParse::g_unknow )
	{
	}

	const RDOType&                     type() const { return *m_type;      }
	rdoRuntime::RDOType::ID          typeID() const { return type()->id(); }
	const rdoRuntime::RDOValue&       value() const { return  m_value;     }
	const rdoRuntime::RDOValue* operator-> () const { return &m_value;     }

	bool defined()  const { return m_value.typeID() != rdoRuntime::RDOType::t_unknow; }
	bool constant() const
	{
		return
			m_value.typeID() == rdoRuntime::RDOType::t_int ||
			m_value.typeID() == rdoRuntime::RDOType::t_real ||
			m_value.typeID() == rdoRuntime::RDOType::t_bool ||
			m_value.typeID() == rdoRuntime::RDOType::t_string;
	}

	static RDOValue getIdentificator( const std::string& identificator )
	{
		return RDOValue( RDOParserSrcInfo(identificator) );
	}

private:
	rdoRuntime::RDOValue m_value;
	const RDOType*       m_type;
};

} // namespace rdoParse

#endif // RDOPARSER_VALUE_H
