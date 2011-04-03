/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdo_resources.h
 * author   : Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _MBUILDER_RESOURCES_H_
#define _MBUILDER_RESOURCES_H_

#if _MSC_VER > 1000
#pragma once
#endif

// ====================================================================== INCLUDES
#include <list>
// ====================================================================== SYNOPSIS
#include "rdo_common/rdodebug.h"

#include "rdo_lib/rdo_mbuilder/namespace.h"

#include "rdo_lib/rdo_runtime/rdo_object.h"
#include "rdo_lib/rdo_runtime/rdo_enum.h"

#include "rdo_lib/rdo_parser/rdo_object.h"
#include "rdo_lib/rdo_parser/rdoparser.h"
#include "rdo_lib/rdo_parser/rdorss.h"
#include "rdo_lib/rdo_parser/rdo_type.h"
#include "rdo_lib/rdo_parser/rdo_type_param.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE
class RDORTPResType;
class RDORSSResource;
CLOSE_RDO_PARSER_NAMESPACE

OPEN_MBUILDER_NAMESPACE

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
	typedef typename List::const_iterator const_iterator;

	RDOList()
		: m_pParser(NULL)
	{}

	RDOList(CREF(rdoParse::LPRDOParser) pParser)
		: m_pParser(pParser)
	{}

	void operator= (CREF(RDOList<T>) obj)
	{
		m_pParser = obj.m_pParser;
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
	List                   m_list;
	rdoParse::LPRDOParser  m_pParser;
};

// --------------------------------------------------------------------
// ---------- Базовая часть mbuilder-объекта
// --------------------------------------------------------------------
#define MBUILDER_OBJECT(Class)                             \
class Class: public rdoParse::RDOParserSrcInfo             \
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
	RDOResType(CREF(rdoParse::LPRDORTPResType) rtp);
	// Создать новый тип
	RDOResType(CREF(tstring) name, Type type = rt_permanent);

	MBUILDER_OBJECT(Param)
	friend class RDOResType;
	public:
		explicit Param(CREF(rdoParse::LPRDORTPParam) param);
		explicit Param(CREF(tstring) name, CREF(rdoParse::LPRDOTypeParam) type,  CREF(rdoParse::RDOValue) default);
		explicit Param(CREF(tstring) name, CREF(rdo::intrusive_ptr<rdoParse::RDOType__int>)  type,  CREF(rdoParse::RDOValue) default = rdoParse::RDOValue());
		explicit Param(CREF(tstring) name, CREF(rdo::intrusive_ptr<rdoParse::RDOType__real>) type,  CREF(rdoParse::RDOValue) default = rdoParse::RDOValue());
		explicit Param(CREF(tstring) name, CREF(rdoRuntime::RDOEnumType::Enums)              enums, CREF(rdoParse::RDOValue) default = rdoParse::RDOValue());

		CREF(rdoParse::LPRDOTypeParam)     type() const       { return m_type;                   }
		const rdoRuntime::RDOType::TypeID  typeID() const     { return m_type->type()->typeID(); }
		tstring                            typeStr() const    { return m_type->type()->name();   }

		rsint                     id() const          { return m_id;  }

		rbool                     hasDiap() const     { return m_min.typeID() != rdoRuntime::RDOType::t_unknow && m_max.typeID() != rdoRuntime::RDOType::t_unknow; }
		CREF(rdoParse::RDOValue)  getMin () const     { return m_min; }
		CREF(rdoParse::RDOValue)  getMax () const     { return m_max; }
		void                      setDiap(CREF(rdoParse::RDOValue) min, CREF(rdoParse::RDOValue) max);

		rbool                     hasDefault() const  { return m_default.defined(); }
		CREF(rdoParse::RDOValue)  getDefault() const  { return m_default;           }
		void                      setDefault(CREF(rdoParse::RDOValue) default);

		rdoParse::LPRDOEnumType   getEnum() const
		{
			ASSERT(typeID() == rdoRuntime::RDOType::t_enum);
			return type()->type().object_static_cast<rdoParse::RDOEnumType>();
		}

		rbool operator== (CREF(Param) param) const;

	private:
		rdoParse::LPRDOTypeParam   m_type;
		rdoParse::RDOValue         m_min;
		rdoParse::RDOValue         m_max;
		rdoParse::RDOValue         m_default;
		rsint                      m_id;

		template <class T>
		void initType(CREF(T) type)
		{
			m_type = rdo::Factory<rdoParse::RDOTypeParam>::create(type, rdoParse::RDOParserSrcInfo());
			ASSERT(m_type);
		}
		template <>
		void initType(CREF(rdoRuntime::RDOEnumType::Enums) enums)
		{
			rdoParse::LPRDOEnumType pEnum = rdo::Factory<rdoParse::RDOEnumType>::create();
			ASSERT(pEnum)
			STL_FOR_ALL_CONST(enums, it)
			{
				pEnum->add(rdoParse::RDOValue::getIdentificator(*it));
			}
//			m_default = rdoParse::RDOValue(rdoParse::RDOValue::getIdentificator(m_default.value().getAsString()).value(), pEnum, rdoParse::RDOParserSrcInfo(m_default));
			m_type    = rdo::Factory<rdoParse::RDOTypeParam>::create(pEnum, rdoParse::RDOParserSrcInfo());
			ASSERT(m_type);
		}
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
	RDOResource(CREF(rdoParse::LPRDORSSResource) rss);
	// Создать новый ресурс
	RDOResource(CREF(RDOResType) rtp, CREF(tstring) name);

	CREF(RDOResType)  getType() const { return m_rtp; }
	rsint             getID  () const { return m_id;  }

	typedef std::map< tstring, rdoParse::RDOValue > Params;

	Params::const_iterator begin() const { return m_params.begin(); }
	Params::const_iterator end  () const { return m_params.end();   }
	ruint                  size () const { return m_params.size();  }

	REF(Params::mapped_type)   operator[] (CREF(tstring) param);
	Params::const_iterator     operator[] (CREF(tstring) param) const;

	rdoParse::LPRDORSSResource getParserResource(CREF(rdoParse::LPRDOParser) pParser) const;

	template <class T>
	rbool checkParserResourceType(CREF(rdoParse::LPRDOParser) pParser) const
	{
		rdoParse::LPRDORSSResource pResource = getParserResource(pParser);
		return pResource.object_dynamic_cast<T>();
	}

	template <class T>
	rdoParse::LPRDORSSResource createParserResource(CREF(rdoParse::LPRDOParser) pParser, rsint id = rdoParse::RDORSSResource::UNDEFINED_ID) const
	{
		rdoParse::LPRDORTPResType pRTP = pParser->findRTPResType(getType().name());
		if (!pRTP)
			return NULL;

		return rdo::Factory<T>::create(pParser, RDOParserSrcInfo(name()), pRTP, id == rdoParse::RDORSSResource::UNDEFINED_ID ? getID() : id);
	}

	rbool fillParserResourceParams(REF(rdoParse::LPRDORSSResource) pToParserRSS) const;

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
	RDOResTypeList(CREF(rdoParse::LPRDOParser) pParser);
	rbool append(REF(RDOResType) rtp);
};

// --------------------------------------------------------------------
// ---------- RDOResourceList
// --------------------------------------------------------------------
class RDOResourceList: public RDOList<RDOResource>
{
public:
	RDOResourceList(CREF(rdoParse::LPRDOParser) pParser);

	// --------------------------------------------------------------------
	// ---- Добавление *нового* ресурса
	// --------------------------------------------------------------------
	template<class T> rbool append(REF(RDOResource) mbuilderRSS)
	{
		if (exist(mbuilderRSS.name()))
			return false;

		rdoParse::LPRDORSSResource parserRSS(mbuilderRSS.createParserResource<T>(m_pParser));
		if (!parserRSS)
			return false;
		if (!mbuilderRSS.fillParserResourceParams(parserRSS))
			return false;

		parserRSS->setTrace(true);

		mbuilderRSS.m_exist = true;
		m_list.push_back(mbuilderRSS);

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

		rdoParse::LPRDORSSResource parserRSSPrev = mbuilderRSSPrevIt->getParserResource(m_pParser);
		ASSERT(parserRSSPrev);

		rdoParse::LPRDORSSResource parserRSSNew(mbuilderRSSNew.createParserResource<T>(m_pParser, mbuilderRSSPrevIt->getID()));
		if (!parserRSSNew)
			return false;
		if (!mbuilderRSSPrevIt->fillParserResourceParams(parserRSSNew))
			return false;
		parserRSSNew->setTrace(parserRSSPrev->getTrace());
		mbuilderRSSNew.m_exist = true;
		m_list.push_back(mbuilderRSSNew);

		//! Удалим старый
		m_pParser->removeRSSResource(parserRSSPrev);
		m_list.erase(mbuilderRSSPrevIt);

		return true;
	}
};

CLOSE_MBUILDER_NAMESPACE

#endif //! _MBUILDER_RESOURCES_H_
