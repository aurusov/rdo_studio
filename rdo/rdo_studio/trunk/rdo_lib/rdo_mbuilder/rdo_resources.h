#ifndef RDO_MB_RESOURCES_H
#define RDO_MB_RESOURCES_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <rdoruntime_object.h>
#include <rdoparser_object.h>
#include <rdoparser.h>
#include <rdorss.h>
#include <list>

namespace rdoParse
{
class RDOParser;
class RDORTPResType;
class RDORTPParam;
class RDORSSResource;
}

namespace rdoMBuilder {

// --------------------------------------------------------------------
// ---------- RDOList
// --------------------------------------------------------------------
template <class T>
class RDOList
{
public:
	typedef std::list< T >                List;
	typedef typename List::iterator       Iterator;
	typedef typename List::const_iterator CIterator;

	RDOList():
		m_parser( NULL )
	{
	}
	RDOList( rdoParse::RDOParser* parser ):
		m_parser( parser )
	{
	}
	void operator= ( const RDOList<T>& obj )
	{
		m_parser = obj.m_parser;
		m_list.clear();
		CIterator it = obj.begin();
		while ( it != obj.end() )
		{
			m_list.push_back( *it );
			it++;
		}
	}

	 Iterator     begin()       { return m_list.begin(); }
	 Iterator     end  ()       { return m_list.end();   }
	CIterator     begin() const { return m_list.begin(); }
	CIterator     end  () const { return m_list.end();   }
	unsigned int  size () const { return m_list.size();  }
	CIterator     found( const std::string& name ) const
	{
		return std::find_if( begin(), end(), rdoParse::compareNameRef<T>(name) );
	}
	bool exist( const std::string& name ) const
	{
		return found( name ) != end();
	}
	const T& operator[] ( const std::string& name ) const
	{
		List::const_iterator it = found( name );
		if ( it != end() )
		{
			return *it;
		}
		else
		{
			static T __t;
			return __t;
		}
	}

protected:
	List                 m_list;
	rdoParse::RDOParser* m_parser;
};

// --------------------------------------------------------------------
// ---------- Базовая часть mbuilder-объекта
// --------------------------------------------------------------------
#define MBUILDER_OBJECT( Class )                           \
class Class                                                \
{                                                          \
friend class Class##List;                                  \
public:                                                    \
	Class(): m_exist( false ), m_name("") {}               \
	Class( const Class& obj );                             \
                                                           \
	const std::string& name() const    { return m_name;  } \
	bool               exist() const   { return m_exist; } \
	void operator= (const Class& copy );                   \
                                                           \
private:                                                   \
	std::string m_name;                                    \
	bool        m_exist;

// --------------------------------------------------------------------
// ---------- RDOResType
// --------------------------------------------------------------------
MBUILDER_OBJECT(RDOResType)
public:
	enum Type
	{
		rt_permanent,
		rt_temporary
	};
	// Проинициализировать по существующему типу
	RDOResType( const rdoParse::RDORTPResType& rtp );
	// Создать новый тип
	RDOResType( const std::string& name, Type type = rt_permanent );

	MBUILDER_OBJECT(Param)
	friend class RDOResType;
	public:
		Param( const rdoParse::RDORTPParam& param );
		Param( const std::string& name, const rdoRuntime::RDOType* type, const rdoRuntime::RDOValue& def = rdoRuntime::g_unknow );
		Param( const std::string& name, const rdoRuntime::RDOValue& def );
		Param( const std::string& name, const rdoRuntime::RDOValue& min, const rdoRuntime::RDOValue& max, const rdoRuntime::RDOValue& def = rdoRuntime::g_unknow );

		const rdoRuntime::RDOType*     type() const       { return m_type;             }
		const rdoRuntime::RDOType::ID  typeID() const     { return m_type->id();       }
		std::string                    typeStr() const    { return m_type->asString(); }

		int                         id() const         { return m_id;  }

		bool                        hasDiap() const    { return m_min.typeID() != rdoRuntime::RDOType::t_unknow && m_max.typeID() != rdoRuntime::RDOType::t_unknow; }
		const rdoRuntime::RDOValue& getMin() const     { return m_min; }
		const rdoRuntime::RDOValue& getMax() const     { return m_max; }
		void                        setDiap( const rdoRuntime::RDOValue& min, const rdoRuntime::RDOValue& max );

		bool                        hasDefault() const { return m_default.typeID() != rdoRuntime::RDOType::t_unknow; }
		const rdoRuntime::RDOValue& getDefault() const { return m_default; }
		void                        setDefault( const rdoRuntime::RDOValue& def );

		const rdoRuntime::RDOEnumType&  getEnum() const
		{
			_ASSERT(typeID() == rdoRuntime::RDOType::t_enum);
			return *static_cast<const rdoRuntime::RDOEnumType*>(m_type);
		}

		bool operator== ( const Param& param ) const;

	private:
		const rdoRuntime::RDOType* m_type;
		rdoRuntime::RDOValue       m_min;
		rdoRuntime::RDOValue       m_max;
		rdoRuntime::RDOValue       m_default;
		int                        m_id;
	};
	class ParamList: public RDOList<Param>
	{
	public:
		bool append( Param& param );
	};
	ParamList m_params;

	Type   getType()     const { return m_type;                 }
	bool   isPermanent() const { return m_type == rt_permanent; }
	int    id()          const { return m_id;                   }

private:
	Type   m_type;
	int    m_id;
};

// --------------------------------------------------------------------
// ---------- RDOResource
// --------------------------------------------------------------------
MBUILDER_OBJECT(RDOResource)
public:
	// Проинициализировать по существующему ресурсу
	RDOResource( const rdoParse::RDORSSResource& rss );
	// Создать новый ресурс
	RDOResource( const RDOResType& rtp, const std::string& name );

	const RDOResType&  getType() const { return m_rtp; }
	int                getID  () const { return m_id;  }
	
	typedef std::map< std::string, rdoRuntime::RDOValue > Params;

	Params::const_iterator begin() const { return m_params.begin(); }
	Params::const_iterator end  () const { return m_params.end();   }
	unsigned int           size () const { return m_params.size();  }
	Params::mapped_type&   operator[] ( const std::string& param );
	Params::const_iterator operator[] ( const std::string& param ) const;

	template <class T>
	bool checkParserResourceType(const rdoParse::RDOParser& parser) const
	{
		if (!exist())
			return false;

		return dynamic_cast<const T*>(parser.findRSSResource(name())) != NULL;
	}

	template <class T>
	rdoParse::RDORSSResource* createParserResource(rdoParse::RDOParser& parser, int id = rdoParse::RDORSSResource::UNDEFINED_ID) const
	{
		const rdoParse::RDORTPResType* pRTP = parser.findRTPResType(getType().name());
		if ( !pRTP )
			return NULL;

		return new T(&parser, name(), pRTP, id == rdoParse::RDORSSResource::UNDEFINED_ID ? getID() : id);
	}
	bool fillParserResourceParams(rdoParse::RDORSSResource* pRSS) const;

private:
	RDOResType  m_rtp;
	Params      m_params;
	int         m_id;
};

// --------------------------------------------------------------------
// ---------- RDOResTypeList
// --------------------------------------------------------------------
class RDOResTypeList: public RDOList<RDOResType>
{
public:
	RDOResTypeList( rdoParse::RDOParser* parser );
	bool append( RDOResType& rtp );
};

// --------------------------------------------------------------------
// ---------- RDOResourceList
// --------------------------------------------------------------------
class RDOResourceList: public RDOList<RDOResource>
{
public:
	RDOResourceList( rdoParse::RDOParser* parser );

	// --------------------------------------------------------------------
	// ---- Добавление *нового* ресурса
	// --------------------------------------------------------------------
	template<class T> bool append( RDOResource& rss )
	{
		if ( exist(rss.name()) )
			return false;

		rdoParse::RDORSSResource* pRSS = rss.createParserResource<T>(*m_parser);
		if (!rss.fillParserResourceParams(pRSS))
		{
			delete pRSS;
			return false;
		}

		rss.m_exist = true;
		m_list.push_back(rss);
		return true;
	}
	// --------------------------------------------------------------------
	// ---- Замена существующего ресурса новым
	// --------------------------------------------------------------------
	template<class T> bool replace( RDOResource& rssNew )
	{
		Iterator itRSSOld = begin();
		while (itRSSOld != end())
		{
			if (itRSSOld->name() == rssNew.name())
			{
				break;
			}
			++itRSSOld;
		}
		if (itRSSOld == end())
			return false;

		rdoParse::RDORSSResource* pRSS = rssNew.createParserResource<T>(*m_parser, itRSSOld->getID());
		if (!itRSSOld->fillParserResourceParams(pRSS))
		{
			delete pRSS;
			return false;
		}
		rssNew.m_exist = true;
		m_list.push_back(rssNew);
		m_list.erase(itRSSOld);
		return true;
	}
};

} // rdoMBuilder

#endif // RDO_MB_RESOURCES_H
