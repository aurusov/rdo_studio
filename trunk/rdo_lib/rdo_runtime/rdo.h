#ifndef RDO_SIMULATOR
#define RDO_SIMULATOR

#include "rdobase.h"
#include "searchtree.h"
#include <rdoparsebase.h>

class RDOSimulator;
class TreeNode;
class TreeRoot;
class RDODecisionPoint;
class RDOOperation;
class RDOIE;
class CheckOperations;

namespace rdoRuntime {
class RDOCalc;
}

class RDOBaseOperation: public rdoParse::RDODeletable
{
public:
	enum BOResult {
		BOR_cant_run = 0,
		BOR_can_run,
		BOR_planned_done,
		BOR_planned_only,
		BOR_planned_and_run
	};
	RDOBaseOperation()          {}
	virtual ~RDOBaseOperation() {}
	// ¬ызываетс€ перед стартом прогона дл€ инициализации операции
	// »спользуетс€ дл€ IE и GENERATE, чтобы задать врем€ прихода первого клиента
	virtual void init( RDOSimulator* sim ) {}
	// ¬ызываетс€ дл€ проверки выполнимости операции и сразу выполн€ет
	// такие образцы, как DPTSearch, rule, operation_begin, keyboard_begin
	// ƒолжна вернуть true, если операци€ выполнилась или запланировалась
	virtual BOResult checkOperation( RDOSimulator* sim ) = 0;
	// ¬ызываетс€ дл€ запланированных в будующем событий: IE, operation_end, keyboard_end
	// ћожет не использоватьс€, например, дл€ rule
	virtual void makePlanned( RDOSimulator* sim, void* param = NULL )  {}
};

class RDORule: public RDOBaseOperation
{
friend RDOSimulator;
friend CheckOperations;
friend RDODecisionPoint;
friend TreeNode;

private:
	virtual RDOBaseOperation::BOResult checkOperation( RDOSimulator* sim );

protected:
	virtual void onBeforeChoiceFrom( RDOSimulator* sim )                 {}
	virtual void onBeforeRule( RDOSimulator* sim )                       {}
	virtual void onAfterRule( RDOSimulator* sim, bool inSearch = false ) {}

public:
	virtual ~RDORule() {}
	virtual bool choiceFrom( RDOSimulator* sim )  = 0;
	virtual void convertRule( RDOSimulator* sim ) = 0;
};

class RDOActivity: public rdoParse::RDODeletable
{
friend TreeNode;
friend RDODecisionPoint;

private:
	bool valueAfter;  // false = valueBefore

protected:
	virtual double costOfRule( RDOSimulator* sim ) = 0;
	RDORule* rule;

public:
	RDOActivity( RDORule* r, bool vA ):
		rule( r ),
		valueAfter( vA )
	{
	}
	virtual ~RDOActivity();
};

class CheckOperations: public rdoParse::RDODeletable
{
private:
	RDOSimulator* sim;

public:
	CheckOperations( RDOSimulator* _sim ): sim( _sim ) {}
	bool operator()( RDOBaseOperation* op );
};

class RDODecisionPoint: public RDOBaseOperation
{
friend RDOSimulator;
friend TreeNode;
friend CheckOperations;

private:
	bool RunSearchInTree( RDOSimulator* sim );
	virtual RDOBaseOperation::BOResult checkOperation( RDOSimulator* sim );

protected:
	std::list< RDOActivity* > activities;
	virtual void onSearchBegin( RDOSimulator* sim )                              = 0;
	virtual void onSearchDecisionHeader( RDOSimulator* sim )                     = 0;
	virtual void onSearchDecision( RDOSimulator* sim, TreeNode* node )           = 0;
	virtual void onSearchResultSuccess( RDOSimulator* sim, TreeRoot* treeRoot )  = 0;
	virtual void onSearchResultNotFound( RDOSimulator* sim, TreeRoot* treeRoot ) = 0;
	virtual TreeRoot* createTreeRoot( RDOSimulator* sim );

public:
	virtual ~RDODecisionPoint();
	virtual bool Condition( RDOSimulator* sim )     = 0;
	virtual bool TermCondition( RDOSimulator* sim ) = 0;
	virtual double EvaluateBy( RDOSimulator* sim )  = 0;
	virtual bool NeedCompareTops()                  = 0;
	virtual void addActivity( RDOActivity* act );
};

class RDOIE: public RDOBaseOperation    // Irregular Event
{
friend RDOSimulator;
friend CheckOperations;

private:
	double time;
	virtual void init( RDOSimulator* sim );
	virtual RDOBaseOperation::BOResult checkOperation( RDOSimulator* sim );
	virtual void makePlanned( RDOSimulator* sim, void* param = NULL );

protected:
	virtual void convertEvent( RDOSimulator* sim )            = 0;
	virtual double getNextTimeInterval( RDOSimulator* sim )   = 0;

	virtual void onBeforeIrregularEvent( RDOSimulator* sim )  {}
	virtual void onAfterIrregularEvent( RDOSimulator* sim )   {}

public:
	virtual ~RDOIE() {}
};

class RDOOperation: public RDOBaseOperation
{
friend RDOSimulator;
friend CheckOperations;

private:
	double time;
	bool convert_end;
	std::list< RDOOperation* > operation_clone;

	virtual RDOBaseOperation::BOResult checkOperation( RDOSimulator* sim );
	virtual void makePlanned( RDOSimulator* sim, void* param = NULL );

public:
	RDOOperation():
		RDOBaseOperation(),
		time( 0 ),
		convert_end( false )
	{
	}
	virtual ~RDOOperation()
	{
		DeleteAllObjects( operation_clone );
	}
	virtual bool   choiceFrom( RDOSimulator* sim )          = 0;
	virtual void   convertEnd( RDOSimulator* sim )          = 0;
	virtual void   convertBegin( RDOSimulator* sim )        = 0;
	virtual double getNextTimeInterval( RDOSimulator* sim ) = 0;

	// The clone function should create and return copy of operation object.
	// Do not forget to copy relevant resources.
	virtual RDOOperation* clone( RDOSimulator* sim ) = 0;

	virtual void onBeforeChoiceFrom( RDOSimulator* sim )     {}
	virtual void onBeforeOperationBegin( RDOSimulator* sim ) {}
	virtual void onBeforeOperationEnd( RDOSimulator* sim )   {}
	virtual void onAfterOperationBegin( RDOSimulator* sim )  {}
	virtual void onAfterOperationEnd( RDOSimulator* sim )    {}
};

class RDOPokaz: public rdoParse::RDODeletable
{
public:
	virtual bool resetPokaz( RDOSimulator* sim ) = 0;
	virtual bool checkPokaz( RDOSimulator* sim ) = 0;
	virtual bool calcStat( RDOSimulator* sim )   = 0;
};

class RDOSimulator: public RDOSimulatorBase
{
friend RDODecisionPoint;
friend RDOOperation;
friend TreeNode;
friend RDORule;
friend RDOIE;
friend RDODecisionPoint;
friend RDOOperation;
friend class RDODecisionPointTrace;

private:
	virtual bool doOperation();

protected:
	std::vector< RDOBaseOperation* > haveBaseOperations; // все паттерны, точки прин€ти€ решений и процессы: DP, IE, Rules, Operations, Process

	// Following member are used in users class derived from RDOSimulator
	// to add Operations, IEs, Rules and DecisionPoints which can be 
	// "happened" in process.
	virtual void addTemplateBaseOperation( RDOBaseOperation* op ) { haveBaseOperations.push_back(op); }

	// »нициализирует нерегул€рные событи€ и блоки GENERATE: задает врем€ первого срабатывани€
	virtual void preProcess();

	virtual void onResetPokaz()      = 0;
	virtual void onCheckPokaz()      = 0;
	virtual void onAfterCheckPokaz() = 0;

	// If you have decision points you should override following two functions:
	// The clone() function must create new instance of object derived from RDOSimulator 
	// with copy of all resources.
	// Nothing else needed such as operations, rules, IEs and DecisionPoints
	virtual RDOSimulator* clone() { return NULL; }
	// The operator==() operator must compare only resources
	virtual bool operator == ( RDOSimulator& other ) { return false; }
	unsigned int sizeof_sim;

	virtual RDOSimulator* createCopy();

public:
	RDOSimulator(): RDOSimulatorBase(), sizeof_sim( 0 ) {}
	virtual ~RDOSimulator() {}
	void rdoInit();
	void rdoDestroy();

	virtual void onPutToTreeNode() = 0;
};

#endif //RDO_SIMULATOR
