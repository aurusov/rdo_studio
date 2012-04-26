/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_resources.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

#ifndef _MBUILDER_RESOURCES_H_
#define _MBUILDER_RESOURCES_H_

// ----------------------------------------------------------------------- INCLUDES
#include <list>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdodebug.h"

#include "simulator/compiler/mbuilder/namespace.h"

#include "simulator/runtime/rdo_object.h"
#include "simulator/runtime/rdo_enum.h"

#include "simulator/compiler/parser/rdo_object.h"
#include "simulator/compiler/parser/rdoparser.h"
#include "simulator/compiler/parser/rdorss.h"
#include "simulator/compiler/parser/type/info.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE
class RDORTPResType;
class RDORSSResource;
CLOSE_RDO_PARSER_NAMESPACE

OPEN_COMPILER_MBUILDER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOList
// --------------------------------------------------------------------------------
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

	RDOList(CREF(parser::LPRDOParser) pParser)
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
		return std::find_if(begin(), end(), parser::compareNameRef<T>(name));
	}
	Iterator      found(CREF(tstring) name)
	{
		return std::find_if(begin(), end(), parser::compareNameRef<T>(name));
	}
	rbool exist(CREF(tstring) name) const
	{
		return found( name ) != end();
	}
	CREF(T) operator[] (CREF(tstring) name) const
	{
		typename List::const_iterator it = found(name);
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
	List                 m_list;
	parser::LPRDOParser  m_pParser;
};

// --------------------------------------------------------------------------------
// -------------------- Базовая часть mbuilder-объекта
// --------------------------------------------------------------------------------
#define MBUILDER_OBJECT(Class)                             \
class Class: public parser::RDOParserSrcInfo               \
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

// --------------------------------------------------------------------------------
// -------------------- RDOResType
// --------------------------------------------------------------------------------
MBUILDER_OBJECT(RDOResType)
public:
	enum Type
	{
		rt_permanent,
		rt_temporary
	};
	// Проинициализировать по существующему типу
	RDOResType(CREF(parser::LPRDORTPResType) rtp);
	// Создать новый тип
	RDOResType(CREF(tstring) name, Type type = rt_permanent);

	MBUILDER_OBJECT(Param)
	friend class RDOResType;
	public:
		explicit Param(CREF(parser::LPRDORTPParam) param);
		explicit Param(CREF(tstring) name, CREF(parser::LPTypeInfo) pType,  CREF(parser::LPRDOValue) pDefault);
		explicit Param(CREF(tstring) name, CREF(rdo::intrusive_ptr<parser::RDOType__int>)  pType, CREF(parser::LPRDOValue) pDefault = parser::LPRDOValue(NULL));
		explicit Param(CREF(tstring) name, CREF(rdo::intrusive_ptr<parser::RDOType__real>) pType, CREF(parser::LPRDOValue) pDefault = parser::LPRDOValue(NULL));
		explicit Param(CREF(tstring) name, CREF(rdo::runtime::RDOEnumType::Enums)          enums, CREF(parser::LPRDOValue) pDefault = parser::LPRDOValue(NULL));

		CREF(parser::LPTypeInfo)             type   () const { return m_pType;                   }
		const rdo::runtime::RDOType::TypeID  typeID () const { return m_pType->type()->typeID(); }
		tstring                              typeStr() const { return m_pType->type()->name();   }

		rsint                    id() const          { return m_id;  }

		rbool                    hasRange() const    { return (m_pMin && m_pMax) ? m_pMin->typeID() != rdo::runtime::RDOType::t_unknow && m_pMax->typeID() != rdo::runtime::RDOType::t_unknow : false; }
		CREF(parser::LPRDOValue) getMin  () const    { return m_pMin; }
		CREF(parser::LPRDOValue) getMax  () const    { return m_pMax; }
		void                     setRange(CREF(parser::LPRDOValue) pMin, CREF(parser::LPRDOValue) pMax);

		rbool                    hasDefault() const  { return m_pDefault ? m_pDefault->defined() : false; }
		CREF(parser::LPRDOValue) getDefault() const  { return m_pDefault; }
		void                     setDefault(CREF(parser::LPRDOValue) pDefault);

		parser::LPRDOEnumType    getEnum() const
		{
			ASSERT(typeID() == rdo::runtime::RDOType::t_enum);
			return type()->type().object_static_cast<parser::RDOEnumType>();
		}

		rbool operator== (CREF(Param) param) const;

	private:
		parser::LPTypeInfo  m_pType;
		parser::LPRDOValue  m_pMin;
		parser::LPRDOValue  m_pMax;
		parser::LPRDOValue  m_pDefault;
		rsint               m_id;

		template <class T>
		void initType(CREF(T) pType)
		{
			m_pType = rdo::Factory<parser::TypeInfo>::create(pType, parser::RDOParserSrcInfo());
			ASSERT(m_pType);
		}
#ifdef COMPILER_VISUAL_STUDIO
		template <>
#endif
		void initType(CREF(rdo::runtime::RDOEnumType::Enums) pType)
		{
			parser::LPRDOEnumType pEnum = rdo::Factory<parser::RDOEnumType>::create();
			ASSERT(pEnum)
			STL_FOR_ALL_CONST(pType, it)
			{
				pEnum->add(parser::RDOValue::getIdentificator(*it));
			}
			// m_default = parser::RDOValue(parser::RDOValue::getIdentificator(m_pDefault->value().getAsString()).value(), pEnum, parser::RDOParserSrcInfo(m_default));
			m_pType = rdo::Factory<parser::TypeInfo>::create(pEnum, parser::RDOParserSrcInfo());
			ASSERT(m_pType);
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
	ruint  id         () const { return m_id;                   }

private:
	Type   m_type;
	ruint  m_id;
};
	
// --------------------------------------------------------------------------------
// -------------------- RDOResource
// --------------------------------------------------------------------------------
MBUILDER_OBJECT(RDOResource)
public:
	// Проинициализировать по существующему ресурсу
	RDOResource(CREF(parser::LPRDORSSResource) rss);
	// Создать новый ресурс
	RDOResource(CREF(RDOResType) rtp, CREF(tstring) name);

	CREF(RDOResType)  getType() const { return m_rtp; }
	rsint             getID  () const { return m_id;  }

	typedef std::map<tstring, parser::LPRDOValue> Params;

	Params::const_iterator begin() const { return m_params.begin(); }
	Params::const_iterator end  () const { return m_params.end();   }
	ruint                  size () const { return m_params.size();  }

	REF(Params::mapped_type)   operator[] (CREF(tstring) param);
	Params::const_iterator     operator[] (CREF(tstring) param) const;

	parser::LPRDORSSResource getParserResource(CREF(parser::LPRDOParser) pParser) const;

	template <class T>
	rbool checkParserResourceType(CREF(parser::LPRDOParser) pParser) const
	{
		parser::LPRDORSSResource pResource = getParserResource(pParser);
		return pResource.object_dynamic_cast<T>();
	}

	template <class T>
	parser::LPRDORSSResource createParserResource(CREF(parser::LPRDOParser) pParser, rsint id = parser::RDORSSResource::UNDEFINED_ID) const
	{
		parser::LPRDORTPResType pRTP = pParser->findRTPResType(getType().name());
		if (!pRTP)
			return NULL;

		return rdo::Factory<T>::create(pParser, RDOParserSrcInfo(name()), pRTP, id == parser::RDORSSResource::UNDEFINED_ID ? getID() : id);
	}

	rbool fillParserResourceParams(REF(parser::LPRDORSSResource) pToParserRSS) const;

private:
	RDOResType  m_rtp;
	Params      m_params;
	rsint       m_id;
};

// --------------------------------------------------------------------------------
// -------------------- RDOResTypeList
// --------------------------------------------------------------------------------
class RDOResTypeList: public RDOList<RDOResType>
{
public:
	RDOResTypeList(CREF(parser::LPRDOParser) pParser);

	rbool append(REF(RDOResType) rtp)
	{
		parser::LPRDORTPResType pResourceType = appendBefore(rtp);
		if (!pResourceType)
		{
			return false;
		}

		return appendAfter(rtp, pResourceType);
	}

private:
	parser::LPRDORTPResType appendBefore(REF(RDOResType) rtp);
	rbool                   appendAfter (REF(RDOResType) rtp, CREF(parser::LPRDORTPResType) pResourceType);
};

// --------------------------------------------------------------------------------
// -------------------- RDOResourceList
// --------------------------------------------------------------------------------
class RDOResourceList: public RDOList<RDOResource>
{
public:
	RDOResourceList(CREF(parser::LPRDOParser) pParser);

	// --------------------------------------------------------------------------------
	// ---- Добавление *нового* ресурса
	// --------------------------------------------------------------------------------
	template<class T> rbool append(REF(RDOResource) mbuilderRSS)
	{
		if (exist(mbuilderRSS.name()))
			return false;

		parser::LPRDORSSResource parserRSS(mbuilderRSS.createParserResource<T>(m_pParser));
		if (!parserRSS)
			return false;
		if (!mbuilderRSS.fillParserResourceParams(parserRSS))
			return false;

		parserRSS->setTrace(true);

		mbuilderRSS.m_exist = true;
		m_list.push_back(mbuilderRSS);

		return true;
	}
	// --------------------------------------------------------------------------------
	// ---- Замена существующего ресурса новым
	// --------------------------------------------------------------------------------
	template<class T> rbool replace(REF(RDOResource) mbuilderRSSNew)
	{
		Iterator mbuilderRSSPrevIt = found(mbuilderRSSNew.name());
		if (mbuilderRSSPrevIt == end())
			return false;

		parser::LPRDORSSResource parserRSSPrev = mbuilderRSSPrevIt->getParserResource(m_pParser);
		ASSERT(parserRSSPrev);

		parser::LPRDORSSResource parserRSSNew(mbuilderRSSNew.createParserResource<T>(m_pParser, mbuilderRSSPrevIt->getID()));
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

CLOSE_COMPILER_MBUILDER_NAMESPACE

#endif // _MBUILDER_RESOURCES_H_
