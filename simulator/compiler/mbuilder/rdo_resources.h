#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include <list>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/debug/rdodebug.h"

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

    RDOList(const parser::LPRDOParser& pParser)
        : m_pParser(pParser)
    {}

    void operator= (const RDOList<T>& obj)
    {
        m_pParser = obj.m_pParser;
        m_list.clear();
        for (CIterator it = obj.begin(); it != obj.end(); ++it)
            m_list.push_back(*it);
    }

     Iterator begin() { return m_list.begin(); }
     Iterator end() { return m_list.end(); }
    CIterator begin() const { return m_list.begin(); }
    CIterator end() const { return m_list.end(); }
    std::size_t size () const { return m_list.size(); }
    CIterator found(const std::string& name) const
    {
        return std::find_if(begin(), end(), parser::compareNameRef<T>(name));
    }
    Iterator found(const std::string& name)
    {
        return std::find_if(begin(), end(), parser::compareNameRef<T>(name));
    }
    bool exist(const std::string& name) const
    {
        return found( name ) != end();
    }
    const T& operator[] (const std::string& name) const
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
    Class(): m_name(""), m_exist( false ) {}               \
                                                           \
    const std::string& name () const { return m_name;  }   \
    bool exist() const { return m_exist; }                 \
                                                           \
private:                                                   \
    std::string m_name;                                    \
    bool m_exist;

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
    RDOResType(const parser::LPRDORTPResType& rtp);
    // Создать новый тип
    RDOResType(const std::string& name, Type type = rt_permanent);

    MBUILDER_OBJECT(Param)
    friend class RDOResType;
    public:
        explicit Param(const parser::LPRDORTPParam& param);
        explicit Param(const std::string& name, const parser::LPTypeInfo& pType,  const parser::LPRDOValue& pDefault);
        explicit Param(const std::string& name, const rdo::intrusive_ptr<parser::RDOType__int>&  pType, const parser::LPRDOValue& pDefault = parser::LPRDOValue(NULL));
        explicit Param(const std::string& name, const rdo::intrusive_ptr<parser::RDOType__real>& pType, const parser::LPRDOValue& pDefault = parser::LPRDOValue(NULL));
        explicit Param(const std::string& name, const rdo::runtime::RDOEnumType::Enums& enums, const parser::LPRDOValue& pDefault = parser::LPRDOValue(NULL));

        const parser::LPTypeInfo& type() const { return m_pType; }
        const rdo::runtime::RDOType::TypeID typeID () const { return m_pType->typeID(); }
        std::string typeStr() const;

        int id() const { return m_id; }

        bool hasRange() const { return (m_pMin && m_pMax) ? m_pMin->typeID() != rdo::runtime::RDOType::t_unknow && m_pMax->typeID() != rdo::runtime::RDOType::t_unknow : false; }
        const parser::LPRDOValue& getMin() const { return m_pMin; }
        const parser::LPRDOValue& getMax() const { return m_pMax; }
        void setRange(const parser::LPRDOValue& pMin, const parser::LPRDOValue& pMax);

        bool hasDefault() const { return m_pDefault ? m_pDefault->defined() : false; }
        const parser::LPRDOValue& getDefault() const  { return m_pDefault; }
        void setDefault(const parser::LPRDOValue& pDefault);

        parser::LPRDOEnumType getEnum() const
        {
            ASSERT(typeID() == rdo::runtime::RDOType::t_enum);
            return type()->itype().object_static_cast<parser::RDOEnumType>();
        }

        bool operator== (const Param& param) const;

    private:
        parser::LPTypeInfo m_pType;
        parser::LPRDOValue m_pMin;
        parser::LPRDOValue m_pMax;
        parser::LPRDOValue m_pDefault;
        int m_id;

        template <class T>
        void initType(const T& pType)
        {
            m_pType = rdo::Factory<parser::TypeInfo>::create(pType, parser::RDOParserSrcInfo());
            ASSERT(m_pType);
        }
#ifdef COMPILER_VISUAL_STUDIO
        template <>
#endif
        void initType(const rdo::runtime::RDOEnumType::Enums& pType)
        {
            parser::LPRDOEnumType pEnum = rdo::Factory<parser::RDOEnumType>::create();
            ASSERT(pEnum)
            for (const auto& type: pType)
            {
                pEnum->add(parser::RDOValue::getIdentificator(type));
            }
            // m_default = parser::RDOValue(parser::RDOValue::getIdentificator(m_pDefault->value().getAsString()).value(), pEnum, parser::RDOParserSrcInfo(m_default));
            m_pType = rdo::Factory<parser::TypeInfo>::create(pEnum, parser::RDOParserSrcInfo());
            ASSERT(m_pType);
        }
    };
    class ParamList: public RDOList<Param>
    {
    public:
        bool append(Param& param);
    };
    ParamList m_params;

    Type getType() const { return m_type; }
    bool isPermanent() const { return m_type == rt_permanent; }
    std::size_t id() const { return m_id; }

private:
    Type m_type;
    std::size_t m_id;
};

// --------------------------------------------------------------------------------
// -------------------- RDOResource
// --------------------------------------------------------------------------------
MBUILDER_OBJECT(RDOResource)
public:
    // Проинициализировать по существующему ресурсу
    RDOResource(const parser::LPRDORSSResource& rss);
    // Создать новый ресурс
    RDOResource(const RDOResType& rtp, const std::string& name);

    const RDOResType& getType() const { return m_rtp; }
    int getID() const { return m_id; }

    typedef std::map<std::string, parser::LPExpression> Params;

    Params::const_iterator begin() const { return m_params.begin(); }
    Params::const_iterator end() const { return m_params.end(); }
    std::size_t size() const { return m_params.size(); }

    Params::mapped_type& operator[](const std::string& param);
    Params::const_iterator operator[](const std::string& param) const;

    parser::LPRDORSSResource getParserResource(const parser::LPRDOParser& pParser) const;

    template <class T>
    bool checkParserResourceType(const parser::LPRDOParser& pParser) const
    {
        parser::LPRDORSSResource pResource = getParserResource(pParser);
        return pResource.object_dynamic_cast<T>();
    }

    template <class T>
    parser::LPRDORSSResource createParserResource(const parser::LPRDOParser& pParser, std::size_t id = parser::RDORSSResource::UNDEFINED_ID) const
    {
        parser::LPRDORTPResType pRTP = pParser->findRTPResType(getType().name());
        if (!pRTP)
            return NULL;

        return rdo::Factory<T>::create(pParser, RDOParserSrcInfo(name()), pRTP, id == parser::RDORSSResource::UNDEFINED_ID ? getID() : id);
    }

    bool fillParserResourceParams(parser::LPRDORSSResource& pToParserRSS) const;

private:
    RDOResType m_rtp;
    Params m_params;
    std::size_t m_id;
};

// --------------------------------------------------------------------------------
// -------------------- RDOResTypeList
// --------------------------------------------------------------------------------
class RDOResTypeList: public RDOList<RDOResType>
{
public:
    RDOResTypeList(const parser::LPRDOParser& pParser);

    bool append(RDOResType& rtp)
    {
        parser::LPRDORTPResType pResourceType = appendBefore(rtp);
        if (!pResourceType)
        {
            return false;
        }

        return appendAfter(rtp, pResourceType);
    }

private:
    parser::LPRDORTPResType appendBefore(RDOResType& rtp);
    bool appendAfter(RDOResType& rtp, const parser::LPRDORTPResType& pResourceType);
};

// --------------------------------------------------------------------------------
// -------------------- RDOResourceList
// --------------------------------------------------------------------------------
class RDOResourceList: public RDOList<RDOResource>
{
public:
    RDOResourceList(const parser::LPRDOParser& pParser);

    // --------------------------------------------------------------------------------
    // ---- Добавление *нового* ресурса
    // --------------------------------------------------------------------------------
    template<class T> bool append(RDOResource& mbuilderRSS)
    {
        if (exist(mbuilderRSS.name()))
            return false;

        parser::LPRDORSSResource parserRSS(mbuilderRSS.createParserResource<T>(m_pParser));
        if (!parserRSS)
            return false;
        if (!mbuilderRSS.fillParserResourceParams(parserRSS))
            return false;
        parser::RDOParser::s_parser()->insertRSSResource(parser::LPRDORSSResource(parserRSS));
        parserRSS->setTrace(true);
        parserRSS->end();

        mbuilderRSS.m_exist = true;
        m_list.push_back(mbuilderRSS);

        return true;
    }
    // --------------------------------------------------------------------------------
    // ---- Замена существующего ресурса новым
    // --------------------------------------------------------------------------------
    template<class T> bool replace(RDOResource& mbuilderRSSNew)
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
        parser::RDOParser::s_parser()->insertRSSResource(parser::LPRDORSSResource(parserRSSNew));
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
