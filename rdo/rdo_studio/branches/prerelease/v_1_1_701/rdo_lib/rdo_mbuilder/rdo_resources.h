#ifndef RDO_MB_RESOURCES_H
#define RDO_MB_RESOURCES_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <rdoruntime_object.h>
#include <rdoparser_object.h>
#include <rdoparser.h>
#include <rdorss.h>
#include <rdodebug.h>
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

	RDOList()
		: m_parser(NULL)
	{}

	RDOList(PTR(rdoParse::RDOParser) parser)
		: m_parser(parser)
	{}

	void operator= (CREF(RDOList<T>) obj)
	{
		m_parser = obj.m_parser;
		m_list.clear();
		for (CIterator it = obj.begin(); it != obj.end(); ++it)
			m_list.push_back(*it);
	}

	 Iterator     begin()       { return m_list.begin(); }
	 Iterator     end  ()       { return m_list.end();   }
	CIterator     begin() const { return m_list.begin(); }
	CIterator     end  () const { return m_list.end();   }
	ruint         size () const { return m_list.size();  }
	CIterator     found(CREF(tstring) name) const
	{
		return std::find_if(begin(), end(), rdoParse::compareNameRef<T>(name));
	}
	Iterator      found(CREF(tstring) name)
	{
		return std::find_if(begin(), end(), rdoParse::compareNameRef<T>(name));
	}
	rbool exist(CREF(tstring) name) const
	{
		return found( name ) != end();
	}
	CREF(T) operator[] (CREF(tstring) name) const
	{
		List::const_iterator it = found(name);
		if (it != end())
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
	List                      m_list;
	PTR(rdoParse::RDOParser)  m_parser;
};

// --------------------------------------------------------------------
// ---------- Базовая часть mbuilder-объекта
// --------------------------------------------------------------------
#define MBUILDER_OBJECT(Class)                             \
class Class                                                \
{                                                          \
friend class Class##List;                                  \
public:                                                    \
	Class(): m_exist( false ), m_name(_T("")) {}           \
	Class(CREF(Class) obj);                                \
                                                           \
	CREF(tstring)  name () const       { return m_name;  } \
	rbool          exist() const       { return m_exist; } \
	void operator= (CREF(Class) copy);                     \
                                                           \
private:                                                   \
	tstring  m_name;                                       \
	rbool    m_exist;

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
	RDOResType(CREF(rdoParse::RDORTPResType) rtp);
	// Создать новый тип
	RDOResType(CREF(tstring) name, Type type = rt_permanent);

	MBUILDER_OBJECT(Param)
	friend class RDOResType;
	public:
		Param(CREF(rdoParse::RDORTPParam) param);
		Param(CREF(tstring) name, CPTR(rdoRuntime::RDOType) type, CREF(rdoRuntime::RDOValue) def = rdoRuntime::g_unknow);
		Param(CREF(tstring) name, CREF(rdoRuntime::RDOValue) def);
		Param(CREF(tstring) name, CREF(rdoRuntime::RDOValue) min, CREF(rdoRuntime::RDOValue) max, CREF(rdoRuntime::RDOValue) def = rdoRuntime::g_unknow);

		CPTR(rdoRuntime::RDOType)          type() const       { return m_type;             }
		const rdoRuntime::RDOType::TypeID  typeID() const     { return m_type->typeID();   }
		tstring                            typeStr() const    { return m_type->asString(); }

		rsint                         id() const         { return m_id;  }

		rbool                       hasDiap() const     { return m_min.typeID() != rdoRuntime::RDOType::t_unknow && m_max.typeID() != rdoRuntime::RDOType::t_unknow; }
		CREF(rdoRuntime::RDOValue)  getMin () const     { return m_min; }
		CREF(rdoRuntime::RDOValue)  getMax () const     { return m_max; }
		void                        setDiap(CREF(rdoRuntime::RDOValue) min, CREF(rdoRuntime::RDOValue) max);

		rbool                       hasDefault() const { return m_default.typeID() != rdoRuntime::RDOType::t_unknow; }
		CREF(rdoRuntime::RDOValue)  getDefault() const { return m_default; }
		void                        setDefault(CREF(rdoRuntime::RDOValue) def);

		CREF(rdoRuntime::RDOEnumType)  getEnum() const
		{
			ASSERT(typeID() == rdoRuntime::RDOType::t_enum);
			return *static_cast<CPTR(rdoRuntime::RDOEnumType)>(m_type);
		}

		rbool operator== (CREF(Param) param) const;

	private:
		CPTR(rdoRuntime::RDOType)  m_type;
		rdoRuntime::RDOValue       m_min;
		rdoRuntime::RDOValue       m_max;
		rdoRuntime::RDOValue       m_default;
		rsint                      m_id;
	};
	class ParamList: public RDOList<Param>
	{
	public:
		rbool append(REF(Param) param);
	};
	ParamList m_params;

	Type   getType    () const { return m_type;                 }
	rbool  isPermanent() const { return m_type == rt_permanent; }
	rsint  id         () const { return m_id;                   }

private:
	Type   m_type;
	rsint  m_id;
};

// --------------------------------------------------------------------
// ---------- RDOResource
// --------------------------------------------------------------------
MBUILDER_OBJECT(RDOResource)
public:
	// Проинициализировать по существующему ресурсу
	RDOResource(CREF(rdoParse::RDORSSResource) rss);
	// Создать новый ресурс
	RDOResource(CREF(RDOResType) rtp, CREF(tstring) name);

	CREF(RDOResType)  getType() const { return m_rtp; }
	rsint             getID  () const { return m_id;  }

	typedef std::map< tstring, rdoRuntime::RDOValue > Params;

	Params::const_iterator begin() const { return m_params.begin(); }
	Params::const_iterator end  () const { return m_params.end();   }
	ruint                  size () const { return m_params.size();  }

	REF(Params::mapped_type)   operator[] (CREF(tstring) param);
	Params::const_iterator     operator[] (CREF(tstring) param) const;

	CPTR(rdoParse::RDORSSResource) getParserResource(CREF(rdoParse::RDOParser) parser) const;

	template <class T>
	rbool checkParserResourceType(CREF(rdoParse::RDOParser) parser) const
	{
		return dynamic_cast<CPTR(T)>(getParserResource(parser)) != NULL;
	}

	template <class T>
	PTR(rdoParse::RDORSSResource) createParserResource(REF(rdoParse::RDOParser) parser, rsint id = rdoParse::RDORSSResource::UNDEFINED_ID) const
	{
		CPTR(rdoParse::RDORTPResType) pRTP = parser.findRTPResType(getType().name());
		if (!pRTP)
			return NULL;

		return new T(&parser, name(), pRTP, id == rdoParse::RDORSSResource::UNDEFINED_ID ? getID() : id);
	}

	rbool fillParserResourceParams(PTR(rdoParse::RDORSSResource) toParserRSS) const;

private:
	RDOResType  m_rtp;
	Params      m_params;
	rsint       m_id;
};

// --------------------------------------------------------------------
// ---------- RDOResTypeList
// --------------------------------------------------------------------
class RDOResTypeList: public RDOList<RDOResType>
{
public:
	RDOResTypeList(PTR(rdoParse::RDOParser) parser);
	rbool append(REF(RDOResType) rtp);
};

// --------------------------------------------------------------------
// ---------- RDOResourceList
// --------------------------------------------------------------------
class RDOResourceList: public RDOList<RDOResource>
{
public:
	RDOResourceList(PTR(rdoParse::RDOParser) parser);

	// --------------------------------------------------------------------
	// ---- Добавление *нового* ресурса
	// --------------------------------------------------------------------
	template<class T> rbool append(REF(RDOResource) mbuilderRSS)
	{
		if (exist(mbuilderRSS.name()))
			return false;

		std::auto_ptr<rdoParse::RDORSSResource> parserRSS(mbuilderRSS.createParserResource<T>(*m_parser));
		if (!parserRSS.get())
			return false;
		if (!mbuilderRSS.fillParserResourceParams(parserRSS.get()))
			return false;

		parserRSS.get()->setTrace(true);

		mbuilderRSS.m_exist = true;
		m_list.push_back(mbuilderRSS);

		//! Снимаем моникер на парсер-ресурс, чтобы он не удалился при выходе
		parserRSS.release();
		return true;
	}
	// --------------------------------------------------------------------
	// ---- Замена существующего ресурса новым
	// --------------------------------------------------------------------
	template<class T> rbool replace(REF(RDOResource) mbuilderRSSNew)
	{
		Iterator mbuilderRSSPrevIt = found(mbuilderRSSNew.name());
		if (mbuilderRSSPrevIt == end())
			return false;

		CPTR(rdoParse::RDORSSResource) parserRSSPrev = mbuilderRSSPrevIt->getParserResource(*m_parser);
		ASSERT(parserRSSPrev);

		std::auto_ptr<rdoParse::RDORSSResource> parserRSSNew(mbuilderRSSNew.createParserResource<T>(*m_parser, mbuilderRSSPrevIt->getID()));
		if (!parserRSSNew.get())
			return false;
		if (!mbuilderRSSPrevIt->fillParserResourceParams(parserRSSNew.get()))
			return false;
		parserRSSNew.get()->setTrace(parserRSSPrev->getTrace());
		mbuilderRSSNew.m_exist = true;
		m_list.push_back(mbuilderRSSNew);

		//! Удалим старый
		m_parser->removeRSSResource(parserRSSPrev);
		m_list.erase(mbuilderRSSPrevIt);

		//! Снимаем моникер на парсер-ресурс, чтобы он не удалился при выходе
		parserRSSNew.release();
		return true;
	}
};

} // rdoMBuilder

#endif // RDO_MB_RESOURCES_H
