#ifndef _CONVERTOR_RDOCONVERTER_H_
#define _CONVERTOR_RDOCONVERTER_H_

// ----------------------------------------------------------------------- INCLUDES
#include <algorithm>
#include <stack>
#include <boost/filesystem/path.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdocommon.h"
#include "utils/src/stuff/rdoindexedstack.h"
#include "utils/src/smart_ptr/factory/factory.h"
#include "utils/src/smart_ptr/wrapper/rdosmart_ptr_wrapper.h"

#include "converter/smr2rdox/rdo_object.h"
#include "converter/smr2rdox/rdoparser_base.h"
#include "converter/smr2rdox/rdo_value.h"
#include "converter/smr2rdox/rdoparser_error.h"
#include "converter/smr2rdox/rdo_type_param.h"
#include "converter/smr2rdox/rdortp.h"
#include "converter/smr2rdox/rdopat.h"
#include "converter/smr2rdox/rdoopr.h"
#include "converter/smr2rdox/rdodpt.h"
#include "converter/smr2rdox/rdopmd.h"
#include "converter/smr2rdox/rdofrm.h"
#include "converter/smr2rdox/rdosmr.h"
#include "converter/smr2rdox/update/update_i.h"
#include "converter/smr2rdox/update/document.h"

#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/rdo_object.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

class RDOTypeParam;

class RDORTPFuzzyParam;

// --------------------------------------------------------------------------------
// -------------------- Converter
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

class Converter
{
public:
DEFINE_OBJECT_CONTAINER(PATPattern     );
DEFINE_OBJECT_CONTAINER(RTPResType     );
DEFINE_OBJECT_CONTAINER(RSSResource    );
DEFINE_OBJECT_CONTAINER(OPROperation   );
DEFINE_OBJECT_CONTAINER(FRMFrame       );
DEFINE_OBJECT_CONTAINER(FUNConstant    );
DEFINE_OBJECT_CONTAINER(FUNFunction    );
DEFINE_OBJECT_CONTAINER(FUNSequence    );
DEFINE_OBJECT_CONTAINER(DPTSearch      );
DEFINE_OBJECT_CONTAINER(DPTSome        );
DEFINE_OBJECT_CONTAINER(DPTPrior       );
DEFINE_OBJECT_CONTAINER(DPTFreeActivity);
DEFINE_OBJECT_CONTAINER(PMDResult      );

DEFINE_OBJECT_CONTAINER_NONAME(FUNGroup   );
DEFINE_OBJECT_CONTAINER_NONAME(DPTFree    );
DEFINE_OBJECT_CONTAINER_NONAME(PROCProcess);
DEFINE_OBJECT_CONTAINER_NONAME(Operations );

public:
	Converter();
	virtual ~Converter();

	rdo::runtime::LPRDORuntime runtime() { return m_pRuntime; }

	bool isPattern() const { return m_pattern; }
	FUNGroupList& getFUNGroupStack() { return m_allFUNGroup; }

	void  checkFunctionName    (const RDOParserSrcInfo& src_info);
	void  checkActivityName    (const RDOParserSrcInfo& src_info);
	void  checkDPTName         (const RDOParserSrcInfo& src_info);

	void insertChanges(const std::string& name, const std::string& value);

	bool isHaveKWResources() const { return m_have_kw_Resources; }
	void setHaveKWResources(bool value) { m_have_kw_Resources = value; }
	bool isHaveKWResourcesEnd() const { return m_have_kw_ResourcesEnd; }
	void setHaveKWResourcesEnd(bool value) { m_have_kw_ResourcesEnd = value; }

	bool isCurrentDPTSearch();
	bool isCurrentDPTPrior();

	std::size_t getRTP_id     () const { return m_allRTPResType.size()  + 1; }
	std::size_t getRSS_id     () const { return m_allRSSResource.size() + 0; }
	std::size_t getPAT_id     () const { return m_allPATPattern.size()  + 0; }
	std::size_t getPMD_id     () const { return m_allPMDResult.size()   + 1; }
	std::size_t getFUNCONST_id() const { return m_allFUNConstant.size() + 0; }
	std::size_t getNumberFrame() const { return m_allFRMFrame.size()    + 0; }

	std::string getModelStructure();
	std::string getChanges() const;

	LPRDOSMR getSMR() const { return m_pSMR; }
	void setSMR(const LPRDOSMR& pSMR) { m_pSMR = pSMR; }
	bool hasSMR() const { return m_pSMR ? true : false; }

	const Error& error() const { return m_error; }
	 Error& error() { return m_error; }

	class Stack: private rdo::IndexedStack<rdo::LPISmartPtrWrapper>
	{
	friend class Converter;
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
			rdo::intrusive_ptr<T> pObject = *reinterpret_cast<rdo::intrusive_ptr<T>*>(pWrapper->getSmartPtr());
			pWrapper->destroy();
			return pObject;
		}
	private:
		void clear()
		{
			for (auto& item: m_stack)
			{
				item.second->destroy();
			}
		}
	};

	Stack& stack()
	{
		return m_movementObjectList;
	}

	typedef std::vector<LPRDOTypeParam> PreCastTypeList;
	const PreCastTypeList& getPreCastTypeList() const
	{
		return m_preCastTypeList;
	}
	void insertPreCastType(const LPRDOTypeParam& type)
	{
		m_preCastTypeList.push_back(type);
	}

	void insertDocUpdate(const LPDocUpdate& pDocUpdate);

	static rdo::converter::smr2rdox::RDOFileTypeIn getFileToParse();
	static std::size_t lexer_loc_line();
	static std::size_t lexer_loc_pos();
	static Converter* s_converter();

protected:
	LPRDOParserItem m_pParserItem;
	LPDocument      m_pDocument;

	virtual LPRDOParserContainer& getContainer() = 0;

	RDOParserContainer::Iterator begin()
	{
		return getContainer()->begin();
	}
	RDOParserContainer::Iterator end()
	{
		return getContainer()->end();
	}
	RDOParserContainer::Iterator find(std::size_t index)
	{
		return getContainer()->find(index);
	}

	rdo::runtime::LPRDORuntime m_pRuntime;

private:
	LPRDOSMR m_pSMR;
	bool m_have_kw_Resources;
	bool m_have_kw_ResourcesEnd;
	Error m_error;
	Stack m_movementObjectList;
	PreCastTypeList m_preCastTypeList;
	bool m_pattern;

	template <class T>
	void howIsIt()
	{
		m_pattern = false;
	}

	struct ChangesData
	{
		std::string m_name;
		std::string m_value;

		ChangesData(const std::string& name, const std::string& value)
			: m_name (name )
			, m_value(value)
		{}
	};

	typedef std::vector<ChangesData> ChangesList;
	ChangesList m_changes;

	typedef std::list<Converter*> ParserList;
	static ParserList s_parserStack;
};

template <>
inline void Converter::howIsIt<LPRDOFUNGroup>()
{}

template <>
inline void Converter::howIsIt<LPRDOPATPattern>()
{
	m_pattern = true;
}

// --------------------------------------------------------------------------------
// -------------------- RDOParserTemplate
// --------------------------------------------------------------------------------
template <class Container>
class RDOParserTemplate: public Converter
{
public:
	RDOParserTemplate()
		: Converter()
	{}

private:
	LPRDOParserContainer m_container;

	virtual LPRDOParserContainer& getContainer()
	{
		if (!m_container)
		{
			m_container = rdo::Factory<Container>::create();
			ASSERT(m_container);
		}
		return m_container;
	}
};

// --------------------------------------------------------------------------------
// -------------------- RDOParserModel
// --------------------------------------------------------------------------------
class RDOParserModel: public RDOParserTemplate<RDOParserContainerModel>
{
public:
	enum Result
	{
		CNV_NONE,
		CNV_OK,
		CNV_ERROR
	};
	Result convert(const boost::filesystem::path& smrFullFileName, rdo::converter::smr2rdox::RDOSMRFileInfo& info);
};

// --------------------------------------------------------------------------------
// -------------------- RDOParserSMRInfo
// --------------------------------------------------------------------------------
class RDOParserSMRInfo: public RDOParserTemplate<RDOParserContainerSMRInfo>
{
public:
	typedef std::map<rdo::converter::smr2rdox::RDOFileTypeIn, boost::filesystem::path> FileList;

	bool parseSMR(const boost::filesystem::path& smrFullFileName, boost::filesystem::path& modelName);
	const FileList& getFileList() const;

private:
	FileList m_fileList;

	void insertFileName(rdo::converter::smr2rdox::RDOFileTypeIn type,
	                    const boost::filesystem::path&          modelPath,
	                    const boost::filesystem::path&          modelName,
	                    const boost::filesystem::path&          smrFileName,
	                    const boost::filesystem::path&          nameFromSMR,
	                    const boost::filesystem::path&          fileExt);
};

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE

#endif // _CONVERTOR_RDOCONVERTER_H_
