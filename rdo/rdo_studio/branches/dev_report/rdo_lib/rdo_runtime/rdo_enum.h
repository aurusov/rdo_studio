#ifndef RDO_ENUM_H
#define RDO_ENUM_H

#include "rdo_type.h"
#include "rdoruntime_object.h"
#include <rdocommon.h>
#include <string>

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDOEnumType
// ----------------------------------------------------------------------------
class RDOEnumType: public RDOType, public RDORuntimeObject
{
public:
	typedef std::string              EnumItem;
	typedef rdo::vector< EnumItem >  Enums;
	typedef Enums::const_iterator    CIterator;

	enum { END = ~0 };

	RDOEnumType( RDORuntimeParent* parent ):
		RDORuntimeObject( parent ),
		RDOType( RDOType::t_enum )
	{
	}
	RDOEnumType( RDORuntimeParent* parent, const Enums& enums ):
		RDORuntimeObject( parent ),
		RDOType( RDOType::t_enum ),
		m_enum( enums )
	{
	}
	void add( const std::string& next )
	{
		m_enum.push_back( next );
	}
	unsigned int findEnum( const std::string& val ) const
	{
		CIterator it = std::find( m_enum.begin(), m_enum.end(), val );
		return it != m_enum.end() ? it - m_enum.begin() : END;
	}
	bool exist( const std::string& val ) const
	{
		return findEnum( val ) != END;
	}
	virtual std::string asString() const
	{
		return str();
	}

	bool            empty() const     { return m_enum.empty(); }
	const CIterator begin() const     { return m_enum.begin(); }
	const CIterator end() const       { return m_enum.end();   }
	const Enums&    getValues() const { return m_enum;	       }

private:
	Enums m_enum;
	
	std::string str() const
	{
		std::string _str = "(";
		std::vector< std::string >::const_iterator it = begin();
		while ( it != end() )
		{
			_str += *it;
			it++;
			if ( it != end() )
			{
				_str += ", ";
			}
		}
		_str += ")";
		return _str;
	}
};

} // namespace rdoRuntime

#endif // RDO_ENUM_H
