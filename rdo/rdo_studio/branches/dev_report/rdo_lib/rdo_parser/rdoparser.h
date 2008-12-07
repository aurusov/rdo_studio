#ifndef RDOPARSER_PARSER
#define RDOPARSER_PARSER

#include "rdoparser_object.h"
#include "rdoparser_base.h"
#include "rdoparser_value.h"
#include <rdoruntime_object.h>
#include <rdocommon.h>
#include <rdo_runtime.h>

#include <algorithm>

namespace rdoRuntime
{
class RDORuntime;
}

namespace rdoParse
{

class RDORTPParam;
class RDORTPResType;
class RDORSSResource;
class RDOPATPattern;
class RDOOPROperation;
class RDOFRMFrame;
class RDOFUNFunction;
class RDOFUNSequence;
class RDOFUNConstant;
class RDOFUNGroup;
class RDOSMR;
class RDOOperations;
class RDODPTSearch;
class RDODPTSome;
class RDODPTFree;
class RDODPTFreeActivity;
class RDOPMDPokaz;
class RDOPROCProcess;
class RDORTPEnumParamType;

class RDORTPFuzzyParam;
// ----------------------------------------------------------------------------
// ---------- RDOSyntaxException
// ----------------------------------------------------------------------------
class RDOSyntaxException: public rdoRuntime::RDOException
{
public:
   RDOSyntaxException( const std::string& message ): RDOException( message ) {}

   virtual std::string getType() const { return "RDO Syntax Error"; }
};

// ----------------------------------------------------------------------------
// ---------- RDOParser
// ----------------------------------------------------------------------------
#define DEFINE_OBJECT_CONTAINER_NONAME( Name ) \
public: \
	void                             insert##Name ( RDO##Name* value ); \
	RDO##Name*                       getLast##Name()       { return !m_all##Name.empty() ? m_all##Name.back() : NULL; } \
	const std::vector< RDO##Name* >& get##Name##s () const { return m_all##Name; } \
private: \
	std::vector< RDO##Name* > m_all##Name;

#define DEFINE_OBJECT_CONTAINER( Name ) \
DEFINE_OBJECT_CONTAINER_NONAME( Name ) \
public: \
	const RDO##Name*                 find##Name   ( const std::string& name ) const;

class RDOParser
{
public:
DEFINE_OBJECT_CONTAINER( PATPattern      );
DEFINE_OBJECT_CONTAINER( RTPResType      );
DEFINE_OBJECT_CONTAINER( RSSResource     );
DEFINE_OBJECT_CONTAINER( OPROperation    );
DEFINE_OBJECT_CONTAINER( FRMFrame        );
DEFINE_OBJECT_CONTAINER( FUNConstant     );
DEFINE_OBJECT_CONTAINER( FUNFunction     );
DEFINE_OBJECT_CONTAINER( FUNSequence     );
DEFINE_OBJECT_CONTAINER( DPTSearch       );
DEFINE_OBJECT_CONTAINER( DPTSome         );
DEFINE_OBJECT_CONTAINER( DPTFreeActivity );
DEFINE_OBJECT_CONTAINER( PMDPokaz        );

DEFINE_OBJECT_CONTAINER_NONAME( FUNGroup    );
DEFINE_OBJECT_CONTAINER_NONAME( DPTFree     );
DEFINE_OBJECT_CONTAINER_NONAME( PROCProcess );
DEFINE_OBJECT_CONTAINER_NONAME( Operations  );

DEFINE_OBJECT_CONTAINER_NONAME( RTPEnumParamType );

public:
	RDOParser();
	virtual ~RDOParser();

	rdoRuntime::RDORuntime* runtime() { return &m_runtime; }

	void insertDeletables( RDODeletable* value ) { if ( value ) m_allDeletables.push_back( value );                                             }
	void removeDeletables( RDODeletable* value ) { m_allDeletables.erase( std::find( m_allDeletables.begin(), m_allDeletables.end(), value ) ); }

	RDOParserObject* getLastParsingObject() { return m_parsing_object; }

	void checkFunctionName( const RDOParserSrcInfo& _src_info );
	void checkActivityName( const RDOParserSrcInfo& _src_info );
	void checkDPTName( const RDOParserSrcInfo& _src_info );

	std::vector< RDOFUNGroup* >& getFUNGroupStack() { return m_allFUNGroup; }

	void insertChanges( const std::string& name, const std::string& value );

	bool isHaveKWResources() const           { return m_have_kw_Resources;     }
	void setHaveKWResources( bool value )    { m_have_kw_Resources = value;    }
	bool isHaveKWResourcesEnd() const        { return m_have_kw_ResourcesEnd;  }
	void setHaveKWResourcesEnd( bool value ) { m_have_kw_ResourcesEnd = value; }

	bool isCurrentDPTSearch();

	int getRTP_id() const      { return m_allRTPResType.size()  + 1; }
	int getRSS_id() const      { return m_allRSSResource.size() + 0; }
	int getPAT_id() const      { return m_allPATPattern.size()  + 0; }
	int getPMD_id() const      { return m_allPMDPokaz.size()    + 1; }
	int getFUNCONST_id() const { return m_allFUNConstant.size() + 0; }

	std::vector< rdoSimulator::RDOSyntaxError > m_errors;

	std::string getModelStructure();
	std::string getChanges() const;

	RDOSMR* getSMR() const      { return m_smr; }
	void setSMR( RDOSMR* smr )  { m_smr = smr;  }
	bool hasSMR() const         { return m_smr ? true : false; }

	void parse();
	void parse( std::istream& stream );
	void error( rdoSimulator::RDOSyntaxError::ErrorCode _error_code, ... );
	void error( const std::string& _message, rdoSimulator::RDOSyntaxError::ErrorCode _error_code = rdoSimulator::RDOSyntaxError::UNKNOWN );
	void error( const RDOParserSrcInfo& _src_info, rdoSimulator::RDOSyntaxError::ErrorCode _error_code, ... );
	void error( const RDOParserSrcInfo& _src_info, const std::string& _message, rdoSimulator::RDOSyntaxError::ErrorCode _error_code = rdoSimulator::RDOSyntaxError::UNKNOWN );
	void error( const RDOParserSrcInfo& _src_info1, const RDOParserSrcInfo& _src_info2, const std::string& _message, rdoSimulator::RDOSyntaxError::ErrorCode _error_code = rdoSimulator::RDOSyntaxError::UNKNOWN );
	void error_push_only( rdoSimulator::RDOSyntaxError::ErrorCode _error_code, ... );
	void error_push_only( const RDOParserSrcInfo& _src_info, rdoSimulator::RDOSyntaxError::ErrorCode _error_code, ... );
	void error_push_only( const std::string& _message, rdoSimulator::RDOSyntaxError::ErrorCode _error_code = rdoSimulator::RDOSyntaxError::UNKNOWN );
	void error_push_only( const RDOParserSrcInfo& _src_info, const std::string& _message, rdoSimulator::RDOSyntaxError::ErrorCode _error_code = rdoSimulator::RDOSyntaxError::UNKNOWN );
	void error_push_done();
	void error_modify( const std::string& _message );
	void warning( rdoSimulator::RDOSyntaxError::ErrorCode _error_code, ... );
	void warning( const std::string& _message, rdoSimulator::RDOSyntaxError::ErrorCode _error_code = rdoSimulator::RDOSyntaxError::UNKNOWN );
	void warning( const RDOParserSrcInfo& _src_info, const std::string& _message, rdoSimulator::RDOSyntaxError::ErrorCode _error_code = rdoSimulator::RDOSyntaxError::UNKNOWN );

	RDOValue* addValue( RDOValue* value ) {
		runtime()->memory_insert( sizeof(rdoParse::RDOValue) );
		m_allValues.push_back( value );
		return value;
	}

	static rdoModelObjects::RDOFileType getFileToParse();
	static int lexer_loc_line();

protected:
	RDOParserItem* m_parser_item;
	
	virtual RDOParserContainer* getContainer() = 0;
	RDOParserContainer::CIterator begin()
	{
		return getContainer()->begin();
	}
	RDOParserContainer::CIterator end()
	{
		return getContainer()->end();
	}
	RDOParserContainer::CIterator find( int index )
	{
		return getContainer()->find( index );
	}

	std::vector< RDODeletable* >  m_allDeletables;
	std::vector< RDOValue* >      m_allValues;

	rdoRuntime::RDORuntime m_runtime;

	void parse( rdoModelObjects::RDOParseType file );

private:
	RDOParserObject*   m_parsing_object;
	RDOSMR*            m_smr;
	bool               m_have_kw_Resources;
	bool               m_have_kw_ResourcesEnd;

	struct Changes {
		std::string m_name;
		std::string m_value;
		Changes( const std::string& name, const std::string& value ):
			m_name( name ),
			m_value( value )
		{
		}
	};
	std::vector< Changes > m_changes;

	int lexer_loc_pos();

	static std::list< RDOParser* > s_parserStack;
};

// ----------------------------------------------------------------------------
// ---------- RDOParserTemplate
// ----------------------------------------------------------------------------
template <class Container>
class RDOParserTemplate: public RDOParser
{
public:
	RDOParserTemplate():
		RDOParser(),
		m_container( NULL )
	{
	}

private:
	Container* m_container;
	virtual RDOParserContainer* getContainer()
	{
		if ( !m_container )
		{
			m_container = new Container( this ); 
		}
		return m_container;
	}
};

// ----------------------------------------------------------------------------
// ---------- RDOParserModel
// ----------------------------------------------------------------------------
typedef RDOParserTemplate<RDOParserContainerModel> RDOParserModel;

// ----------------------------------------------------------------------------
// ---------- RDOParserSMRInfo
// ----------------------------------------------------------------------------
typedef RDOParserTemplate<RDOParserContainerSMRInfo> RDOParserSMRInfo;

// ----------------------------------------------------------------------------
// ---------- RDOParserCorbar
// ----------------------------------------------------------------------------
typedef RDOParserTemplate<RDOParserContainerCorba> RDOParserCorba;

} // namespace rdoParse

#endif // RDOPARSER_PARSER
