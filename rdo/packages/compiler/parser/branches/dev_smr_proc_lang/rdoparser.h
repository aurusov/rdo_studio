/*!
  \copyright (c) RDO-Team, 2011
  \file      rdoparser.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

#ifndef _RDOPARSER_H_
#define _RDOPARSER_H_

// ----------------------------------------------------------------------- INCLUDES
#include <algorithm>
#include <stack>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdocommon.h"
#include "utils/rdoindexedstack.h"
#include "utils/smart_ptr/intrusive_ptr.h"
#include "utils/rdosmart_ptr_wrapper.h"
#include "utils/rdoidgenerator.h"

#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/rdo_object.h"

#include "simulator/compiler/parser/rdo_object.h"
#include "simulator/compiler/parser/rdoparser_base.h"
#include "simulator/compiler/parser/rdo_value.h"
#include "simulator/compiler/parser/rdoparser_error.h"
#include "simulator/compiler/parser/rdopatpreparse.h"
#include "simulator/compiler/parser/rdortp.h"
#include "simulator/compiler/parser/rdopat.h"
#include "simulator/compiler/parser/rdopatpreparse.h"
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
	CREF(NAME##List)    get##NAME##s () const { return m_all##NAME; } \
private: \
	NAME##List m_all##NAME;

#define DEFINE_OBJECT_CONTAINER_WITHNAME(TYPE, NAME) \
public: \
	const TYPE find##NAME  (CREF(tstring) name) const; \
	rbool      remove##NAME(const TYPE item);

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

CLASS(RDOParser):
	    INSTANCE_OF      (Context     )
	AND IMPLEMENTATION_OF(IContextFind)
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
DEFINE_OBJECT_CONTAINER(Event         );
DEFINE_OBJECT_CONTAINER(ResultGroup   );
DEFINE_OBJECT_CONTAINER(PROCProcess   );

DEFINE_OBJECT_CONTAINER_NONAME(FUNGroup);

public:
	virtual void init  ();
	virtual void deinit();

	ruint  m_currentRunNumber;

	CREF(rdo::runtime::LPRDORuntime) runtime() const;

	rbool             isPattern       () const;
	REF(FUNGroupList) getFUNGroupStack();

	void  checkFunctionName    (CREF(RDOParserSrcInfo) src_info);
	void  checkActivityName    (CREF(RDOParserSrcInfo) src_info);
	void  checkDPTName         (CREF(RDOParserSrcInfo) src_info);

	void  insertChanges        (CREF(tstring) name, CREF(tstring) value);

	rbool isHaveKWResources    () const;
	void  setHaveKWResources   (rbool value);
	rbool isHaveKWResourcesEnd () const;
	void  setHaveKWResourcesEnd(rbool value);

	rbool isCurrentDPTSearch   ();
	rbool isCurrentDPTPrior    ();

	ruint getRTP_id        () const;
	ruint getRSS_id        () const;
	ruint getPAT_id        () const;
	ruint getPMD_id        ();
	ruint getFUNCONST_id   () const;
	ruint getSeriesCapacity() const;

	void  setCurrentRunNumber (ruint  value);
	void  foundEndOfNextRun   ();
	void  pushToContainer     ();

	rbool check               () const;

	tstring getModelStructure();
	tstring getChanges       () const;

	LPRDOSMR getSMR(ruint value);
	void     setSMR(CREF(LPRDOSMR) pSMR);
	rbool    hasSMR() const;

	void parse();
	void parse(ruint count);
	void parse(REF(std::istream) stream);
	void castInitToRuntime();

	CREF(Error) error() const;
	 REF(Error) error();

	class Stack: private rdo::IndexedStack<rdo::LPISmartPtrWrapper>
	{
		friend class RDOParser;
	public:
		typedef rdo::IndexedStack<rdo::LPISmartPtrWrapper> IndexedStack;

		template <class T>
		IndexedStack::ID push(CREF(rdo::intrusive_ptr<T>) pObject);

		template <class T>
		rdo::intrusive_ptr<T> pop(IndexedStack::ID id);

		rdo::LPISmartPtrWrapper raw_pop(IndexedStack::ID id);

	private:
		void clear();
	};

	REF(Stack) stack();

	typedef std::vector<LPTypeInfo> PreCastTypeList;
	CREF(PreCastTypeList) getPreCastTypeList() const;
	void insertPreCastType(CREF(LPTypeInfo) pType);

	LPContextStack contextStack();
	LPContext      context     () const;

	static rdoModelObjects::RDOFileType getFileToParse();
	static ruint                        lexer_loc_line();
	static ruint                        lexer_loc_pos ();
	static tstring                      lexer_text    ();
	static LPRDOParser                  s_parser      ();

protected:
	RDOParser();
	virtual ~RDOParser();

	LPRDOParserItem m_parser_item;

	virtual CREF(LPRDOParserContainer) getContainer() const = 0;

	RDOParserContainer::Iterator begin();
	RDOParserContainer::Iterator end();
	RDOParserContainer::Iterator find(ruint index);

	rdo::runtime::LPRDORuntime m_pRuntimeBackup;

	void parse(rdoModelObjects::RDOParseType file);

private:
	LPRDOSMR              m_pSMR;
	rbool                 m_have_kw_Resources;
	rbool                 m_have_kw_ResourcesEnd;
	Error                 m_error;
	Stack                 m_movementObjectList;
	PreCastTypeList       m_preCastTypeList;
	LPContextStack        m_pContextStack;
	rbool                 m_pattern;
	rdo::IDGenerator      m_resultGeneratorID;
	ruint                 m_foundRunNumber;

	template <class T>
	void howIsIt();

	template <class T>
	void howIsIt(identity<T>);

	void howIsIt(identity<LPRDOFUNGroup>);

	void howIsIt(identity<LPRDOPATPattern>);

	struct Changes;
	typedef std::vector<Changes> ChangesList;
	ChangesList m_changes;

	typedef std::list<LPRDOParser> ParserList;
	static ParserList s_parserStack;

	typedef std::vector<LPRDOSMR> RDOSMRList;
	RDOSMRList SMRList;

	DECLARE_IContextFind;
};
DECLARE_POINTER(RDOParser);

// --------------------------------------------------------------------------------
// -------------------- RDOParser::Changes
// --------------------------------------------------------------------------------
struct RDOParser::Changes
{
    tstring m_name;
    tstring m_value;
    Changes(CREF(tstring) name, CREF(tstring) value);
};

// --------------------------------------------------------------------------------
// -------------------- RDOParserTemplate
// --------------------------------------------------------------------------------
template <class Container>
class RDOParserTemplate: public RDOParser
{
DECLARE_FACTORY(RDOParserTemplate<Container>);

private:
	typedef  RDOParser  parent_type;

    RDOParserTemplate();
    virtual ~RDOParserTemplate();

    virtual void init();

    virtual void deinit();

    virtual CREF(LPRDOParserContainer) getContainer() const;

	LPRDOParserContainer m_pContainer;
};

// --------------------------------------------------------------------------------
// -------------------- RDOParserModel
// --------------------------------------------------------------------------------
typedef RDOParserTemplate<RDOParserContainerModel> RDOParserModel;

// --------------------------------------------------------------------------------
// -------------------- RDOParserSMRInfo
// --------------------------------------------------------------------------------
typedef RDOParserTemplate<RDOParserContainerSMRInfo> RDOParserSMRInfo;

#ifdef CORBA_ENABLE
// --------------------------------------------------------------------------------
// -------------------- RDOParserCorbar
// --------------------------------------------------------------------------------
typedef RDOParserTemplate<RDOParserContainerCorba> RDOParserCorba;
#endif

CLOSE_RDO_PARSER_NAMESPACE

#include "simulator/compiler/parser/rdoparser.inl"

#endif // _RDOPARSER_H_
