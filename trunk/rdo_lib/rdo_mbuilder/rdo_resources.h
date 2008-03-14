#ifndef RDO_MB_RESOURCES_H
#define RDO_MB_RESOURCES_H

#if _MSC_VER > 1000
#pragma once
#endif

namespace rdoParse
{
class RDOParser;
class RDORTPResType;
class RDORTPParam;
}

namespace rdoMBuilder {

// --------------------------------------------------------------------
// ---------- RDOResType
// --------------------------------------------------------------------
class RDOResType
{
public:
	RDOResType( const rdoParse::RDORTPResType& rtp );
	RDOResType( const std::string& name );

	class Param
	{
	public:
		Param( const rdoParse::RDORTPParam& param );

		const std::string& getName() const { return m_name; }

	private:
		std::string m_name;
	};
	typedef std::list< Param > ParamList;

	ParamList::const_iterator begin() const { return m_params.begin(); }
	ParamList::const_iterator end  () const { return m_params.end();   }

	const std::string& getName() const { return m_name; }

private:
	std::string m_name;
	bool        m_permanent;
	ParamList   m_params;
};

// --------------------------------------------------------------------
// ---------- RDOResTypeList
// --------------------------------------------------------------------
class RDOResTypeList
{
public:
	RDOResTypeList( rdoParse::RDOParser* parser );

	typedef std::list< RDOResType > RTPList;

	RTPList::const_iterator begin() const { return m_list.begin(); }
	RTPList::const_iterator end  () const { return m_list.end();   }

private:
	RTPList                    m_list;
	const rdoParse::RDOParser* m_parser;
};

} // rdoMBuilder

#endif // RDO_MB_RESOURCES_H
