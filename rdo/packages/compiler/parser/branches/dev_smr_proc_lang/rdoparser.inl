/*!
  \copyright (c) RDO-Team, 2013
  \file      rdoparser.inl
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      26.05.2013
  \brief
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOParser
// --------------------------------------------------------------------------------
inline CREF(rdo::runtime::LPRDORuntime) RDOParser::runtime() const
{
    return m_pRuntime;
}

inline rbool RDOParser::isPattern() const
{
    return m_pattern;
}

inline REF(RDOParser::FUNGroupList) RDOParser::getFUNGroupStack()
{
    return m_allFUNGroup;
}

inline rbool RDOParser::isHaveKWResources() const
{
    return m_have_kw_Resources;
}

inline void  RDOParser::setHaveKWResources(rbool value)
{
    m_have_kw_Resources = value;
}

inline rbool RDOParser::isHaveKWResourcesEnd() const
{
    return m_have_kw_ResourcesEnd;
}

inline void  RDOParser::setHaveKWResourcesEnd(rbool value)
{
    m_have_kw_ResourcesEnd = value;
}

inline ruint RDOParser::getRTP_id() const
{
    return m_allRTPResType.size()  + 1;
}

inline ruint RDOParser::getRSS_id() const
{
    return m_allRSSResource.size() + 0;
}

inline ruint RDOParser::getPAT_id() const
{
    return m_allPATPattern.size()  + 0;
}

inline ruint RDOParser::getPMD_id()
{
    return m_resultGeneratorID.get();
}

inline ruint RDOParser::getFUNCONST_id() const
{
    return m_allFUNConstant.size() + 0;
}

inline ruint RDOParser::getSeriesCapacity() const
{
    return m_foundRunNumber;
}

inline LPRDOSMR RDOParser::getSMR(ruint value)
{
    return SMRList_1[value];
}

inline void RDOParser::setSMR(CREF(LPRDOSMR) pSMR)
{
    m_pSMR = pSMR;
}

inline rbool RDOParser::hasSMR() const
{
    return m_pSMR ? true : false;
}

inline CREF(Error) RDOParser::error() const
{
    return m_error;
}

inline REF(Error) RDOParser::error()
{
    return m_error;
}

inline REF(RDOParser::Stack) RDOParser::stack()
{
    return m_movementObjectList;
}

inline CREF(RDOParser::PreCastTypeList) RDOParser::getPreCastTypeList() const
{
    return m_preCastTypeList;
}

inline void RDOParser::insertPreCastType(CREF(LPTypeInfo) pType)
{
    m_preCastTypeList.push_back(pType);
}

inline RDOParserContainer::Iterator RDOParser::begin()
{
    return getContainer()->begin();
}

inline RDOParserContainer::Iterator RDOParser::end()
{
    return getContainer()->end();
}

inline RDOParserContainer::Iterator RDOParser::find(ruint index)
{
    return getContainer()->find(index);
}

template <class T>
inline void RDOParser::howIsIt()
{
    howIsIt(identity<T>());
}

template <class T>
inline void RDOParser::howIsIt(identity<T>)
{
    m_pattern = false;
}

inline void RDOParser::howIsIt(identity<LPRDOFUNGroup>)
{}

inline void RDOParser::howIsIt(identity<LPRDOPATPattern>)
{
    m_pattern = true;
}

// --------------------------------------------------------------------------------
// -------------------- RDOParser::Stack
// --------------------------------------------------------------------------------
template <class T>
inline RDOParser::Stack::IndexedStack::ID RDOParser::Stack::push(CREF(rdo::intrusive_ptr<T>) pObject)
{
    rdo::LPISmartPtrWrapper pWrapper = new rdo::smart_ptr_wrapper<T>(pObject);
    return IndexedStack::push(pWrapper);
}

template <class T>
inline rdo::intrusive_ptr<T> RDOParser::Stack::pop(IndexedStack::ID id)
{
    rdo::LPISmartPtrWrapper pWrapper = IndexedStack::pop(id);
    ASSERT(pWrapper);

    // Падение в в этом месте означает, что из стека вытаскивается указатель неправильного типа
    // Что бы узнать тип, необходимо найти команду push для этого указателя
    ASSERT((pWrapper->getRefCounter() && dynamic_cast<CPTR(T)>(pWrapper->getRefCounter())) || !pWrapper->getRefCounter());

    rdo::intrusive_ptr<T> pObject = *reinterpret_cast<PTR(rdo::intrusive_ptr<T>)>(pWrapper->getSmartPtr());
    pWrapper->destroy();
    return pObject;
}

inline rdo::LPISmartPtrWrapper RDOParser::Stack::raw_pop(IndexedStack::ID id)
{
    rdo::LPISmartPtrWrapper pWrapper = IndexedStack::pop(id);
    ASSERT(pWrapper);
    return pWrapper;
}

inline void RDOParser::Stack::clear()
{
    STL_FOR_ALL(m_stack, it)
    {
        it->second->destroy();
    }
}

// --------------------------------------------------------------------------------
// -------------------- RDOParser::Changes
// --------------------------------------------------------------------------------
inline RDOParser::Changes::Changes(CREF(tstring) name, CREF(tstring) value)
    : m_name (name )
    , m_value(value)
{}

// --------------------------------------------------------------------------------
// -------------------- RDOParserTemplate
// --------------------------------------------------------------------------------
template <class Container>
inline RDOParserTemplate<Container>::RDOParserTemplate()
    : RDOParser()
{}

template <class Container>
inline RDOParserTemplate<Container>::~RDOParserTemplate()
{}

template <class Container>
inline void RDOParserTemplate<Container>::init()
{
    m_pContainer = rdo::Factory<Container>::create();
    ASSERT(m_pContainer);
    parent_type::init();
}

template <class Container>
inline void RDOParserTemplate<Container>::deinit()
{
    ASSERT(m_pContainer);
    m_pContainer->clear();
    parent_type::deinit();
}

template <class Container>
inline CREF(LPRDOParserContainer) RDOParserTemplate<Container>::getContainer() const
{
    return m_pContainer;
}

CLOSE_RDO_PARSER_NAMESPACE
