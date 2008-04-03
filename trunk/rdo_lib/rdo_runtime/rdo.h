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
		BOR_planned_and_run,
		BOR_must_continue,
		BOR_done
	};
	RDOBaseOperation( RDORuntimeParent* parent ): RDORuntimeParent( parent ) {}
	virtual ~RDOBaseOperation() {}
	// Вызывается перед стартом прогона для инициализации операции
	// Используется для IE и GENERATE, чтобы задать время прихода первого клиента
	virtual void     init( RDOSimulator* sim ) {}
	// Вызывается для проверки выполнимости операции
	virtual bool     checkOperation( RDOSimulator* sim ) = 0;
	// Вызывается для выполнения
	virtual BOResult doOperation   ( RDOSimulator* sim ) = 0;
	// Вызывается для запланированных в будующем событий: IE, operation_end, keyboard_end
	// Может не использоваться, например, для rule
	virtual void     makePlaned( RDOSimulator* sim, void* param = NULL ) {}
	// Вызывается для продолжения долгой операции, например, DPT search
	virtual BOResult continueOperation( RDOSimulator* sim ) { return BOR_cant_run; }
};

// ----------------------------------------------------------------------------
// ---------- RDOOprContainer
// ----------------------------------------------------------------------------
template<class T>
class RDOOprContainer: public RDOBaseOperation
{
public:
	RDOOprContainer( RDORuntimeParent* parent ):
		RDOBaseOperation( parent ),
		m_first( NULL )
	{
	}
	virtual ~RDOOprContainer() {}

	typedef std::vector< T* >    List;
	typedef List::iterator       Iterator;
	typedef List::const_iterator CIterator;

	Iterator  begin()       { return m_items.begin(); }
	Iterator  end()         { return m_items.end();   }
	CIterator begin() const { return m_items.begin(); }
	CIterator end()   const { return m_items.end();   }

	void append( T& item )
	{
		item.reparent( this );
		m_items.push_back( &item );
	}
	virtual void init( RDOSimulator* sim )
	{
		Iterator it = begin();
		while ( it != end() )
		{
			(*it)->init(sim);
			it++;
		}
	}
	virtual bool checkOperation( RDOSimulator* sim )
	{
		Iterator it = begin();
		while ( it != end() )
		{
			if ( (*it)->checkOperation(sim) )
			{
				m_first = *it;
				return true;
			}
			it++;
		}
		m_first = NULL;
		return false;
	}
	virtual BOResult doOperation( RDOSimulator* sim )
	{
		if ( !m_first )
		{
			if ( !checkOperation(sim) || !m_first )
			{
				return BOR_cant_run;
			}
		}
		BOResult result = m_first->doOperation(sim);
		if ( result == BOR_must_continue ) {
			sim->setMustContinueOpr( m_first );
		}
		return result;
	}
	virtual void makePlaned( RDOSimulator* sim, void* param = NULL )
	{
		Iterator it = begin();
		while ( it != end() )
		{
			(*it)->makePlaned( sim, param );
			it++;
		}
	}
	virtual BOResult continueOperation( RDOSimulator* sim )
	{
		Iterator it = begin();
		while ( it != end() )
		{
			if ( (*it)->continueOperation( sim ) == BOR_must_continue )
			{
				return BOR_must_continue;
			}
			it++;
		}
		return BOR_cant_run;
	}

private:
	std::vector< T* > m_items;
	T*                m_first;
};

// ----------------------------------------------------------------------------
// ---------- RDOBaseLogic
// ----------------------------------------------------------------------------
template< class T >
class RDOBaseLogic: public RDOOprContainer< T >
{
public:
	RDOBaseLogic( RDORuntimeParent* parent ):
		RDOOprContainer< T >( parent ),
		m_childLogic( NULL )
	{
		m_childLogic.reparent( this );
	}
	virtual ~RDOBaseLogic() {}

	RDOOprContainer< RDOBaseOperation > m_childLogic;

	virtual void init( RDOSimulator* sim )
	{
		m_childLogic.init( sim );
		RDOOprContainer<T>::init( sim );
	}
	virtual bool checkOperation( RDOSimulator* sim )
	{
		if ( !m_childLogic.checkOperation( sim ) )
		{
			return RDOOprContainer<T>::checkOperation( sim );
		}
		else
		{
			return true;
		}
	}
	virtual BOResult doOperation( RDOSimulator* sim )
	{
		BOResult result = m_childLogic.doOperation( sim );
		if ( result != BOR_cant_run )
		{
			return RDOOprContainer<T>::doOperation( sim );
		}
		else
		{
			return result;
		}
	}
	virtual void makePlaned( RDOSimulator* sim, void* param = NULL )
	{
		m_childLogic.makePlaned( sim, param );
		RDOOprContainer<T>::makePlaned( sim, param );
	}
	virtual BOResult continueOperation( RDOSimulator* sim )
	{
		BOResult result = m_childLogic.continueOperation( sim );
		if ( result != BOR_must_continue )
		{
			return RDOOprContainer<T>::continueOperation( sim );
		}
		else
		{
			return result;
		}
	}
};

// ----------------------------------------------------------------------------
// ---------- RDOBaseLogic
// ----------------------------------------------------------------------------
typedef RDOBaseLogic< RDOBaseOperation > RDOLogicContainer;

// ----------------------------------------------------------------------------
// ---------- RDOIE - irregular_event
// ----------------------------------------------------------------------------
class RDOIE: public RDOBaseOperation
{
friend class RDOSimulator;

public:
	RDOIE( RDORuntimeParent* runtime ): RDOBaseOperation( runtime ) {}
	virtual ~RDOIE() {}

protected:
	virtual void convertEvent( RDOSimulator* sim )            = 0;
	virtual double getNextTimeInterval( RDOSimulator* sim )   = 0;

	virtual void onBeforeIrregularEvent( RDOSimulator* sim )  {}
	virtual void onAfterIrregularEvent( RDOSimulator* sim )   {}

private:
	double  m_time;
	virtual void init( RDOSimulator* sim );
	virtual bool     checkOperation( RDOSimulator* sim );
	virtual BOResult doOperation   ( RDOSimulator* sim );
	virtual void makePlaned( RDOSimulator* sim, void* param = NULL );
};

// ----------------------------------------------------------------------------
// ---------- RDORule - rule
// ----------------------------------------------------------------------------
class RDORule: public RDOBaseOperation
{
friend class RDOSimulator;
friend class RDODecisionPoint;
friend class TreeNode;

public:
	RDORule( RDORuntimeParent* runtime ): RDOBaseOperation( runtime ) {}
	virtual ~RDORule() {}
	virtual bool choiceFrom( RDOSimulator* sim )  = 0;
	virtual void convertRule( RDOSimulator* sim ) = 0;

protected:
	virtual void onBeforeChoiceFrom( RDOSimulator* sim )                 {}
	virtual void onBeforeRule( RDOSimulator* sim )                       {}
	virtual void onAfterRule( RDOSimulator* sim, bool inSearch = false ) {}

private:
	virtual bool     checkOperation( RDOSimulator* sim );
	virtual BOResult doOperation   ( RDOSimulator* sim );
};

// ----------------------------------------------------------------------------
// ---------- RDOOperation - operation
// ----------------------------------------------------------------------------
class RDOOperation: public RDOBaseOperation
{
friend class RDOSimulator;

private:
	double time;
	bool convert_end;
	RDORuntimeParent clones;

	virtual bool     checkOperation( RDOSimulator* sim );
	virtual BOResult doOperation   ( RDOSimulator* sim );
	virtual void makePlaned( RDOSimulator* sim, void* param = NULL );

public:
	RDOOperation( RDORuntimeParent* runtime ):
		RDOBaseOperation( runtime ),
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

private:
	TreeRoot* treeRoot;
	RDOBaseOperation::BOResult RunSearchInTree( RDOSimulator* sim );
	virtual bool     checkOperation( RDOSimulator* sim );
	virtual BOResult doOperation   ( RDOSimulator* sim );

protected:
	std::list< RDOActivity* > activities;
	virtual void onSearchBegin( RDOSimulator* sim )                              = 0;
	virtual void onSearchDecisionHeader( RDOSimulator* sim )                     = 0;
	virtual void onSearchDecision( RDOSimulator* sim, TreeNode* node )           = 0;
	virtual void onSearchResultSuccess( RDOSimulator* sim, TreeRoot* treeRoot )  = 0;
	virtual void onSearchResultNotFound( RDOSimulator* sim, TreeRoot* treeRoot ) = 0;
	virtual TreeRoot* createTreeRoot( RDOSimulator* sim )                        = 0;
	virtual RDOBaseOperation::BOResult continueOperation( RDOSimulator* sim );

public:
	RDODecisionPoint( RDORuntimeParent* runtime ):
		RDOBaseOperation( runtime ),
		treeRoot( NULL )
	{
	}
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
	RDOPokaz( RDORuntimeParent* runtime ): RDORuntimeObject( runtime ) {}
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
	RDOBaseOperation* opr_must_continue;
	virtual bool doOperation();

protected:
//	std::vector< RDOBaseOperation* > m_logics; // все паттерны, точки принятия решений и процессы: DP, IE, Rules, Operations, Process
	RDOLogicContainer m_logics;

	// Following member are used in users class derived from RDOSimulator
	// to add Operations, IEs, Rules and DecisionPoints which can be 
	// "happened" in process.
	virtual void addTemplateBaseOperation( RDOBaseOperation* op )
	{
		m_logics.append(*op);
	}

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
	RDOSimulator():
		RDOSimulatorBase(),
		m_logics( NULL ),
		opr_must_continue( NULL ),
		sizeof_sim( 0 )
	{
		m_logics.reparent( this );
	}
	virtual ~RDOSimulator()
	{
	}

	RDOBaseOperation* getMustContinueOpr() const       { return opr_must_continue;  }
	void setMustContinueOpr( RDOBaseOperation* value ) { opr_must_continue = value; }

	virtual void onPutToTreeNode() = 0;
};

} // namespace rdoRuntime

#endif //RDO_SIMULATOR
