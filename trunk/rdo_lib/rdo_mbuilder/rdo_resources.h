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

		const std::string&          getName() const    { return m_name;                   }
		rdoRuntime::RDOValue::Type  getType() const    { return m_type.getType();         }
		std::string                 getTypeStr() const { return m_type.getTypeAsString(); }

		bool                        hasDiap() const    { return m_min.getType() != rdoRuntime::RDOValue::rvt_unknow && m_max.getType() != rdoRuntime::RDOValue::rvt_unknow; }
		const rdoRuntime::RDOValue& getMin() const     { return m_min; }
		const rdoRuntime::RDOValue& getMax() const     { return m_max; }

		bool                        hasDefault() const { return m_default.getType() != rdoRuntime::RDOValue::rvt_unknow; }
		const rdoRuntime::RDOValue& getDefault() const { return m_default; }

		const rdoRuntime::RDOEnum&  getEnum() const    { return m_type.getEnum(); }

	private:
		std::string            m_name;
		rdoRuntime::RDOValue   m_type;
		rdoRuntime::RDOValue   m_min;
		rdoRuntime::RDOValue   m_max;
		rdoRuntime::RDOValue   m_default;
	};
	typedef std::list< Param > ParamList;

	ParamList::const_iterator begin() const { return m_params.begin(); }
	ParamList::const_iterator end  () const { return m_params.end();   }

	const std::string& getName() const { return m_name;      }
	bool isPermanent() const           { return m_permanent; }

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
