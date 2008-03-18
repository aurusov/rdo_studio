#ifndef RDO_MB_RESOURCES_H
#define RDO_MB_RESOURCES_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <rdoruntime_object.h>

namespace rdoParse
{
class RDOParser;
class RDORTPResType;
class RDORTPParam;
class RDORSSResource;
}

namespace rdoMBuilder {

// --------------------------------------------------------------------
// ---------- RDOResType
// --------------------------------------------------------------------
class RDOResType
{
public:
	RDOResType( const rdoParse::RDORTPResType& rtp );
	RDOResType( const std::string& name, bool permanent = true );

	class Param
	{
	public:
		Param( const rdoParse::RDORTPParam& param );
		Param( const std::string& name, rdoRuntime::RDOValue::Type type );
		Param( const std::string& name, rdoRuntime::RDOValue::Type type, const rdoRuntime::RDOValue& def );
		Param( const std::string& name, const rdoRuntime::RDOValue& type );
		Param( const std::string& name, const rdoRuntime::RDOValue& min, const rdoRuntime::RDOValue& max, const rdoRuntime::RDOValue& def = rdoRuntime::RDOValue::rvt_unknow );

		const std::string&          getName() const    { return m_name;                   }
		rdoRuntime::RDOValue::Type  getType() const    { return m_type.getType();         }
		std::string                 getTypeStr() const { return m_type.getTypeAsString(); }

		bool                        hasDiap() const    { return m_min.getType() != rdoRuntime::RDOValue::rvt_unknow && m_max.getType() != rdoRuntime::RDOValue::rvt_unknow; }
		const rdoRuntime::RDOValue& getMin() const     { return m_min; }
		const rdoRuntime::RDOValue& getMax() const     { return m_max; }
		void                        setDiap( const rdoRuntime::RDOValue& min, const rdoRuntime::RDOValue& max );

		bool                        hasDefault() const { return m_default.getType() != rdoRuntime::RDOValue::rvt_unknow; }
		const rdoRuntime::RDOValue& getDefault() const { return m_default; }
		void                        setDefault( const rdoRuntime::RDOValue& def );

		const rdoRuntime::RDOEnum&  getEnum() const    { return m_type.getEnum(); }

		bool operator== ( const Param& param ) const;

	private:
		std::string            m_name;
		rdoRuntime::RDOValue   m_type;
		rdoRuntime::RDOValue   m_min;
		rdoRuntime::RDOValue   m_max;
		rdoRuntime::RDOValue   m_default;
	};
	typedef std::list< Param > ParamList;

	bool                      append( const Param& param );

	ParamList::const_iterator begin() const { return m_params.begin(); }
	ParamList::const_iterator end  () const { return m_params.end();   }
	unsigned int              size () const { return m_params.size();  }

	const std::string& getName() const { return m_name;      }
	bool isPermanent() const           { return m_permanent; }

private:
	std::string m_name;
	bool        m_permanent;
	ParamList   m_params;
};

// --------------------------------------------------------------------
// ---------- RDOResource
// --------------------------------------------------------------------
class RDOResource
{
public:
	RDOResource( const rdoParse::RDORSSResource& rss );

	const std::string& getName() const { return m_name; }
	const RDOResType&  getType() const { return m_rtp;  }

	typedef std::map< std::string, rdoRuntime::RDOValue > ParamList;

	ParamList::const_iterator begin() const { return m_params.begin(); }
	ParamList::const_iterator end  () const { return m_params.end();   }
	unsigned int              size () const { return m_params.size();  }

private:
	std::string m_name;
	RDOResType  m_rtp;
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

	bool append( const RDOResType& rtp );

	RTPList::const_iterator begin() const { return m_list.begin(); }
	RTPList::const_iterator end  () const { return m_list.end();   }

private:
	RTPList              m_list;
	rdoParse::RDOParser* m_parser;
};

// --------------------------------------------------------------------
// ---------- RDOResourceList
// --------------------------------------------------------------------
class RDOResourceList
{
public:
	RDOResourceList( rdoParse::RDOParser* parser );

	typedef std::list< RDOResource > RSSList;

	RSSList::const_iterator begin() const { return m_list.begin(); }
	RSSList::const_iterator end  () const { return m_list.end();   }

private:
	RSSList              m_list;
	rdoParse::RDOParser* m_parser;
};

} // rdoMBuilder

#endif // RDO_MB_RESOURCES_H
