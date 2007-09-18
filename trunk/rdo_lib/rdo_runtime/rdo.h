#ifndef RDO_SIMULATOR
#define RDO_SIMULATOR

#include "rdobase.h"
#include "searchtree.h"
#include "rdoruntime_object.h"

namespace rdoRuntime {

class RDOCalc;
class RDOSimulator;
class RDODecisionPoint;
class TreeNode;
class TreeRoot;

// ----------------------------------------------------------------------------
// ---------- RDOBaseOperation - базовый класс для паттернов, процессов и блоков процесса
// ----------------------------------------------------------------------------
// Надо бы сделать его базовым для всех возможных логик
// ----------------------------------------------------------------------------
class RDOBaseOperation: public RDORuntimeParent
{
public:
	enum BOResult {
		BOR_cant_run = 0,
		BOR_can_run,
		BOR_planned_done,
		BOR_planned_only,
		BOR_planned_and_run
	};
	RDOBaseOperation( RDORuntimeParent* _parent  ): RDORuntimeParent( _parent  ) {}
	virtual ~RDOBaseOperation() {}
	// Вызывается перед стартом прогона для инициализации операции
	// Используется для IE и GENERATE, чтобы задать время прихода первого клиента
	virtual void init( RDOSimulator* sim ) {}
	// Вызывается для проверки выполнимости операции и сразу выполняет
	// такие образцы, как DPTSearch, rule, operation_begin, keyboard_begin
	// Должна вернуть true, если операция выполнилась или запланировалась
	virtual BOResult checkOperation( RDOSimulator* sim ) = 0;
	// Вызывается для запланированных в будующем событий: IE, operation_end, keyboard_end
	// Может не использоваться, например, для rule
	virtual void makePlaned( RDOSimulator* sim, void* param = NULL ) {}
};

// ----------------------------------------------------------------------------
// ---------- RDOIE - irregular_event
// ----------------------------------------------------------------------------
class RDOIE: public RDOBaseOperation
{
friend class RDOSimulator;
friend class CheckOperations;

private:
	double time;
	virtual void init( RDOSimulator* sim );
	virtual RDOBaseOperation::BOResult checkOperation( RDOSimulator* sim );
	virtual void makePlaned( RDOSimulator* sim, void* param = NULL );

protected:
	virtual void convertEvent( RDOSimulator* sim )            = 0;
	virtual double getNextTimeInterval( RDOSimulator* sim )   = 0;

	virtual void onBeforeIrregularEvent( RDOSimulator* sim )  {}
	virtual void onAfterIrregularEvent( RDOSimulator* sim )   {}

public:
	RDOIE( RDORuntimeParent* _runtime ): RDOBaseOperation( _runtime ) {}
	virtual ~RDOIE() {}
};

// ----------------------------------------------------------------------------
// ---------- RDORule - rule
// ----------------------------------------------------------------------------
class RDORule: public RDOBaseOperation
{
friend class RDOSimulator;
friend class CheckOperations;
friend class RDODecisionPoint;
friend class TreeNode;

private:
	virtual RDOBaseOperation::BOResult checkOperation( RDOSimulator* sim );

protected:
	virtual void onBeforeChoiceFrom( RDOSimulator* sim )                 {}
	virtual void onBeforeRule( RDOSimulator* sim )                       {}
	virtual void onAfterRule( RDOSimulator* sim, bool inSearch = false ) {}

public:
	RDORule( RDORuntimeParent* _runtime ): RDOBaseOperation( _runtime ) {}
	virtual ~RDORule() {}
	virtual bool choiceFrom( RDOSimulator* sim )  = 0;
	virtual void convertRule( RDOSimulator* sim ) = 0;
};

// ----------------------------------------------------------------------------
// ---------- RDOOperation - operation
// ----------------------------------------------------------------------------
class RDOOperation: public RDOBaseOperation
{
friend class RDOSimulator;
friend class CheckOperations;

private:
	double time;
	bool convert_end;
	RDORuntimeParent clones;

	virtual RDOBaseOperation::BOResult checkOperation( RDOSimulator* sim );
	virtual void makePlaned( RDOSimulator* sim, void* param = NULL );

public:
	RDOOperation( RDORuntimeParent* _runtime ):
		RDOBaseOperation( _runtime ),
		time( 0 ),
		convert_end( false ),
		clones( NULL )
	{
		clones.reparent( this );
	}
	virtual ~RDOOperation()
	{
		clones.deleteObjects();
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

// ----------------------------------------------------------------------------
// ---------- RDOActivity - активность внутри DPT
// ----------------------------------------------------------------------------
class RDOActivity: public RDORuntimeObject
{
friend class TreeNode;
friend class RDODecisionPoint;

private:
	bool valueAfter;  // false = valueBefore

protected:
	virtual double costOfRule( RDOSimulator* sim ) = 0;
	RDORule* rule;

public:
	RDOActivity( RDORule* _rule, bool _valueAfter ):
		RDORuntimeObject( _rule ),
		rule( _rule ),
		valueAfter( _valueAfter )
	{
	}
	virtual ~RDOActivity()
	{
	}
};

// ----------------------------------------------------------------------------
// ---------- RDODecisionPoint - DPT
// ----------------------------------------------------------------------------
class RDODecisionPoint: public RDOBaseOperation
{
friend class RDOSimulator;
friend class TreeNode;
friend class CheckOperations;

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
	virtual TreeRoot* createTreeRoot( RDOSimulator* sim )                        = 0;

public:
	RDODecisionPoint( RDORuntimeParent* _runtime ): RDOBaseOperation( _runtime ) {}
	virtual ~RDODecisionPoint();
	virtual bool Condition( RDOSimulator* sim )     = 0;
	virtual bool TermCondition( RDOSimulator* sim ) = 0;
	virtual double EvaluateBy( RDOSimulator* sim )  = 0;
	virtual bool NeedCompareTops()                  = 0;
	virtual void addActivity( RDOActivity* act );
};

// ----------------------------------------------------------------------------
// ---------- RDOPokaz - базовый класс для собираемых показателей
// ----------------------------------------------------------------------------
class RDOPokaz: public RDORuntimeObject
{
public:
	RDOPokaz( RDORuntimeParent* _runtime ): RDORuntimeObject( _runtime ) {}
	virtual bool resetPokaz( RDOSimulator* sim ) = 0;
	virtual bool checkPokaz( RDOSimulator* sim ) = 0;
	virtual bool calcStat( RDOSimulator* sim )   = 0;
};

// ----------------------------------------------------------------------------
// ---------- RDOSimulator - один из базовых классов для RDORuntime
// ----------------------------------------------------------------------------
class RDOSimulator: public RDOSimulatorBase
{
friend class RDODecisionPoint;
friend class RDOOperation;
friend class TreeNode;
friend class RDORule;
friend class RDOIE;
friend class RDOOperation;
friend class RDODecisionPointTrace;

private:
	virtual bool doOperation();

protected:
	std::vector< RDOBaseOperation* > haveBaseOperations; // все паттерны, точки принятия решений и процессы: DP, IE, Rules, Operations, Process

	// Following member are used in users class derived from RDOSimulator
	// to add Operations, IEs, Rules and DecisionPoints which can be 
	// "happened" in process.
	virtual void addTemplateBaseOperation( RDOBaseOperation* op ) { haveBaseOperations.push_back(op); }

	// Инициализирует нерегулярные события и блоки GENERATE: задает время первого срабатывания
	virtual void preProcess();

	virtual void onResetPokaz()      = 0;
	virtual void onCheckPokaz()      = 0;
	virtual void onAfterCheckPokaz() = 0;

	RDOSimulator* createCopy();
	// Для DPT необходимо перекрыть две нижеследующие функции:
	// 1. Создает клон RDOSimulator с копиями всех ресурсов, но не более
	virtual RDOSimulator* clone()                    = 0;
	// 2. Сравнение двух симуляторов по ресурсам
	virtual bool operator == ( RDOSimulator& other ) = 0;

	unsigned int sizeof_sim;

public:
	RDOSimulator( RDORuntimeParent* _runtime ):
		RDOSimulatorBase( _runtime ),
		sizeof_sim( 0 )
	{
	}
	virtual ~RDOSimulator()
	{
	}

	virtual void onPutToTreeNode() = 0;
};

// ----------------------------------------------------------------------------
// ---------- CheckOperations - фуктрорал проверки на запуск RDOBaseOperation
// ----------------------------------------------------------------------------
class CheckOperations: public RDORuntimeObject
{
private:
	RDOSimulator* sim;

public:
	CheckOperations( RDOSimulator* _sim ):
		RDORuntimeObject( _sim ), //qq
		sim( _sim )
	{
	}
	bool operator()( RDOBaseOperation* opr );
};

} // namespace rdoRuntime

#endif //RDO_SIMULATOR
