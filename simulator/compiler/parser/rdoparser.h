#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include <algorithm>
#include <stack>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdocommon.h"
#include "utils/src/stuff/rdoindexedstack.h"
#include "utils/src/stuff/rdoidgenerator.h"
#include "utils/src/smart_ptr/intrusive_ptr/intrusive_ptr.h"
#include "utils/src/smart_ptr/wrapper/rdosmart_ptr_wrapper.h"

#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/rdo_object.h"

#include "simulator/compiler/parser/rdo_object.h"
#include "simulator/compiler/parser/rdoparser_base.h"
#include "simulator/compiler/parser/rdo_value.h"
#include "simulator/compiler/parser/rdoparser_error.h"
#include "simulator/compiler/parser/rdortp.h"
#include "simulator/compiler/parser/rdopat.h"
#include "simulator/compiler/parser/rdodpt.h"
#include "simulator/compiler/parser/rdoprocess.h"
#include "simulator/compiler/parser/rdopmd.h"
#include "simulator/compiler/parser/src/animation/animation_frame.h"
#include "simulator/compiler/parser/src/animation/animation_sprite.h"
#include "simulator/compiler/parser/rdosmr.h"
#include "simulator/compiler/parser/context/context.h"
#include "simulator/compiler/parser/context/stack.h"
#include "simulator/compiler/parser/rdo_array.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

class RDOTypeParam;
class RDOArrayType;

class RDORTPFuzzyParam;

// --------------------------------------------------------------------------------
// -------------------- RDOParser
// --------------------------------------------------------------------------------
#define DEFINE_OBJECT_CONTAINER_MINIMUM(TYPE, NAME) \
public: \
    typedef std::vector<TYPE> NAME##List; \
    void                insert##NAME (TYPE value); \
    TYPE                getLast##NAME()       { return !m_all##NAME.empty() ? m_all##NAME.back() : TYPE(NULL); } \
    const NAME##List&   get##NAME##s () const { return m_all##NAME; } \
private: \
    NAME##List m_all##NAME;

#define DEFINE_OBJECT_CONTAINER_WITHNAME(TYPE, NAME) \
public: \
    const TYPE find##NAME  (const std::string& name) const; \
    bool remove##NAME(const TYPE item);

#define DEFINE_OBJECT_CONTAINER_NONAME(NAME) \
DEFINE_OBJECT_CONTAINER_MINIMUM(LPRDO##NAME, NAME)

#define DEFINE_OBJECT_CONTAINER(NAME) \
DEFINE_OBJECT_CONTAINER_MINIMUM(LPRDO##NAME, NAME) \
DEFINE_OBJECT_CONTAINER_WITHNAME(LPRDO##NAME, NAME)

PREDECLARE_POINTER(RDOParser);

template <typename T>
struct identity
{
    typedef T type;
};

class RDOParser
    : public Context
    , public IContextFind
{
DECLARE_FACTORY(RDOParser);

public:
DEFINE_OBJECT_CONTAINER(PATPattern    );
DEFINE_OBJECT_CONTAINER(RTPResType    );
DEFINE_OBJECT_CONTAINER(RSSResource   );
DEFINE_OBJECT_CONTAINER(FRMFrame      );
DEFINE_OBJECT_CONTAINER(FRMSprite     );
DEFINE_OBJECT_CONTAINER(FRMCommandList);
DEFINE_OBJECT_CONTAINER(FUNConstant   );
DEFINE_OBJECT_CONTAINER(FUNFunction   );
DEFINE_OBJECT_CONTAINER(FUNSequence   );
DEFINE_OBJECT_CONTAINER(DPTSearch     );
DEFINE_OBJECT_CONTAINER(DPTSome       );
DEFINE_OBJECT_CONTAINER(DPTPrior      );
DEFINE_OBJECT_CONTAINER(DPTActivity   );
DEFINE_OBJECT_CONTAINER(ResultGroup   );
DEFINE_OBJECT_CONTAINER(PROCProcess   );

DEFINE_OBJECT_CONTAINER_NONAME(FUNGroup);

public:
    virtual void init  ();
    virtual void deinit();

    const rdo::runtime::LPRDORuntime& runtime() const { return m_pRuntime; }

    bool isPattern() const { return m_pattern; }
    FUNGroupList& getFUNGroupStack() { return m_allFUNGroup; }

    void  checkFunctionName    (const RDOParserSrcInfo& src_info);
    void  checkActivityName    (const RDOParserSrcInfo& src_info);
    void  checkDPTName         (const RDOParserSrcInfo& src_info);

    void insertChanges (const std::string& name, const std::string& value);

    bool isCurrentDPTSearch();
    bool isCurrentDPTPrior();

    std::size_t getRTP_id() const { return m_allRTPResType.size()  + 1; }
    std::size_t getRSS_id() const { return m_allRSSResource.size() + 0; }
    std::size_t getPAT_id() const { return m_allPATPattern.size()  + 0; }
    std::size_t getPMD_id() { return m_resultGeneratorID.get(); }
    std::size_t getFUNCONST_id() const { return m_allFUNConstant.size() + 0; }

    std::string getModelStructure();
    std::string getChanges() const;

    LPRDOSMR getSMR() const { return m_pSMR; }
    void setSMR(const LPRDOSMR& pSMR) { m_pSMR = pSMR; }
    bool hasSMR() const { return m_pSMR ? true : false; }

    void parse();
    void parse(std::istream& stream);

    void beforeRun();

    const Error& error() const { return m_error; }
     Error& error() { return m_error; }

    class Stack: private rdo::IndexedStack<rdo::LPISmartPtrWrapper>
    {
    friend class RDOParser;
    public:
        typedef rdo::IndexedStack<rdo::LPISmartPtrWrapper> IndexedStack;

        template <class T>
        IndexedStack::ID push(const rdo::intrusive_ptr<T>& pObject)
        {
            rdo::LPISmartPtrWrapper pWrapper = new rdo::smart_ptr_wrapper<T>(pObject);
            return IndexedStack::push(pWrapper);
        }
        template <class T>
        rdo::intrusive_ptr<T> pop(IndexedStack::ID id)
        {
            rdo::LPISmartPtrWrapper pWrapper = IndexedStack::pop(id);
            ASSERT(pWrapper);

            // Падение в в этом месте означает, что из стека вытаскивается указатель неправильного типа
            // Что бы узнать тип, необходимо найти команду push для этого указателя
            ASSERT((pWrapper->getRefCounter() && dynamic_cast<const T*>(pWrapper->getRefCounter())) || !pWrapper->getRefCounter());

            rdo::intrusive_ptr<T> pObject = *reinterpret_cast<rdo::intrusive_ptr<T>*>(pWrapper->getSmartPtr());
            pWrapper->destroy();
            return pObject;
        }

        rdo::LPISmartPtrWrapper raw_pop(IndexedStack::ID id)
        {
            rdo::LPISmartPtrWrapper pWrapper = IndexedStack::pop(id);
            ASSERT(pWrapper);
            return pWrapper;
        }

    private:
        void clear()
        {
            for (const auto& stack: m_stack)
                stack.second->destroy();
        }
    };

    Stack& stack()
    {
        return m_movementObjectList;
    }

    typedef std::vector<LPTypeInfo> PreCastTypeList;
    void insertPreCastType(const LPTypeInfo& pType)
    {
        m_preCastTypeList.push_back(pType);
    }

    LPContextStack contextStack();
    LPContext      context     () const;

    static rdo::FileType getFileToParse();
    static std::size_t lexer_loc_line();
    static std::size_t lexer_loc_pos();
    static std::string lexer_text();
    static LPRDOParser s_parser();

    template <class T>
    void howIsIt()
    {
        howIsIt(identity<T>());
    }

    static const std::string METHOD_TIME_NOW;
    static const std::string METHOD_SECONDS;
    static const std::string METHOD_TERMINATE_COUNTER;

private:
    RDOParser();
    virtual ~RDOParser();

    typedef std::vector<LPRDOParserItem> Compilers;

    rdo::runtime::LPRDORuntime m_pRuntime;
    LPRDOSMR m_pSMR;
    Error m_error;
    Stack m_movementObjectList;
    PreCastTypeList m_preCastTypeList;
    LPContextStack m_pContextStack;
    bool m_pattern;
    rdo::IDGenerator m_resultGeneratorID;
    Compilers m_compilers;
    LPRDOParserItem m_parser_item;

    void runRSSPost();
    void runSMRPost();
    void runRTPPost();

    template <class T>
    void howIsIt(identity<T>)
    {
        m_pattern = false;
    }

    void howIsIt(identity<LPRDOFUNGroup>)
    {}

    void howIsIt(identity<LPRDOPATPattern>)
    {
        m_pattern = true;
    }

    struct Changes
    {
        std::string m_name;
        std::string m_value;
        Changes(const std::string& name, const std::string& value)
            : m_name (name )
            , m_value(value)
        {}
    };
    typedef std::vector<Changes> ChangesList;
    ChangesList m_changes;

    typedef std::list<LPRDOParser> ParserList;
    static ParserList s_parserStack;

    DECLARE_IContextFind;
};
DECLARE_POINTER(RDOParser);

CLOSE_RDO_PARSER_NAMESPACE
