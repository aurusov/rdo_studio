#ifndef RDO_SIMULATOR
#define RDO_SIMULATOR

#include "rdobase.h"
#include "rdocalc.h"
#include "rdoruntime_object.h"

namespace rdoRuntime {

class RDOSimulator;
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

	// Вызывается перед стартом прогона и перед возобновление операции
	// Например, условие DPT-some поменялось с false на true
	// Используется для IE и GENERATE, чтобы задать время прихода первого клиента
	virtual void onStart( RDOSimulator* sim )
	{
	}

	// Вызывается при остановке операции, например, условие DPT-some поменялось с true на false
	virtual void onStop( RDOSimulator* sim )
	{
	}

	// Вызывается для проверки выполнимости операции
	virtual bool onCheckCondition( RDOSimulator* sim ) = 0;

	// Вызывается для выполнения правил/операций
	virtual BOResult onDoOperation( RDOSimulator* sim ) = 0;

	// Вызывается для запланированных в будующем событий: IE, operation_end, keyboard_end
	// Может не использоваться, например, для rule
	virtual void onMakePlaned( RDOSimulator* sim, void* param = NULL )
	{
	}

	// Вызывается для продолжения долгой операции, например, DPT search
	virtual BOResult onContinue( RDOSimulator* sim )
	{
		return BOR_cant_run;
	}
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
	virtual void onStart( RDOSimulator* sim )
	{
		Iterator it = begin();
		while ( it != end() )
		{
			(*it)->onStart(sim);
			it++;
		}
	}
	virtual void onStop( RDOSimulator* sim )
	{
		Iterator it = begin();
		while ( it != end() )
		{
			(*it)->onStop(sim);
			it++;
		}
	}
	virtual bool onCheckCondition( RDOSimulator* sim )
	{
		Iterator it = begin();
		while ( it != end() )
		{
			if ( (*it)->onCheckCondition(sim) )
			{
				m_first = *it;
				return true;
			}
			it++;
		}
		m_first = NULL;
		return false;
	}
	virtual BOResult onDoOperation( RDOSimulator* sim )
	{
		if ( !m_first )
		{
			if ( !onCheckCondition(sim) || !m_first )
			{
				return BOR_cant_run;
			}
		}
		BOResult result = m_first->onDoOperation(sim);
		if ( result == BOR_must_continue ) {
			sim->setMustContinueOpr( m_first );
		}
		return result;
	}
	virtual void onMakePlaned( RDOSimulator* sim, void* param = NULL )
	{
		Iterator it = begin();
		while ( it != end() )
		{
			(*it)->onMakePlaned( sim, param );
			it++;
		}
	}
	virtual BOResult onContinue( RDOSimulator* sim )
	{
		Iterator it = begin();
		while ( it != end() )
		{
			if ( (*it)->onContinue( sim ) == BOR_must_continue )
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
		m_childLogic( NULL ),
		m_condition( NULL ),
		m_lastCondition( false )
	{
		m_childLogic.reparent( this );
	}
	virtual ~RDOBaseLogic() {}

	RDOOprContainer< RDOBaseOperation > m_childLogic;

	void setCondition( RDOCalc* calc )
	{
		m_condition = calc;
	}

	virtual void onStart( RDOSimulator* sim )
	{
		m_lastCondition = checkCondition( sim );
		if ( m_lastCondition )
		{
			start( sim );
		}
	}
	virtual void onStop( RDOSimulator* sim )
	{
		m_lastCondition = false;
		stop( sim );
	}
	virtual bool onCheckCondition( RDOSimulator* sim )
	{
		bool condition = checkCondition( sim );
		if ( condition != m_lastCondition )
		{
			m_lastCondition = condition;
			if ( condition )
			{
				start( sim );
			}
			else
			{
				stop( sim );
			}
		}
		if ( condition )
		{
			if ( !m_childLogic.onCheckCondition( sim ) )
			{
				return RDOOprContainer<T>::onCheckCondition( sim );
			}
			else
			{
				return true;
			}
		}
		else
		{
			return false;
		}
	}
	virtual BOResult onDoOperation( RDOSimulator* sim )
	{
		if ( m_lastCondition )
		{
			BOResult result = m_childLogic.onDoOperation( sim );
			if ( result == BOR_cant_run )
			{
				return RDOOprContainer<T>::onDoOperation( sim );
			}
			else
			{
				return result;
			}
		}
		else
		{
			return BOR_cant_run;
		}
	}
	virtual void onMakePlaned( RDOSimulator* sim, void* param = NULL )
	{
		m_childLogic.onMakePlaned( sim, param );
		RDOOprContainer<T>::onMakePlaned( sim, param );
	}
	virtual BOResult onContinue( RDOSimulator* sim )
	{
		BOResult result = m_childLogic.onContinue( sim );
		if ( result != BOR_must_continue )
		{
			return RDOOprContainer<T>::onContinue( sim );
		}
		else
		{
			return result;
		}
	}

private:
	RDOCalc* m_condition;
	bool     m_lastCondition;

	bool checkCondition( RDOSimulator* sim )
	{
		if ( m_condition )
		{
			return m_condition->calcValueBase( (RDORuntime*)sim ).getBool();
		}
		else
		{
			return true;
		}
	}
	void start( RDOSimulator* sim )
	{
		m_childLogic.onStart( sim );
		RDOOprContainer<T>::onStart( sim );
	}
	void stop( RDOSimulator* sim )
	{
		m_childLogic.onStop( sim );
		RDOOprContainer<T>::onStop( sim );
	}
};

// ----------------------------------------------------------------------------
// ---------- RDOBaseLogic
// ----------------------------------------------------------------------------
typedef RDOBaseLogic< RDOBaseOperation > RDOLogicContainer;

// ----------------------------------------------------------------------------
// ---------- RDOIE
// ----------------------------------------------------------------------------
class RDOIE: public RDOBaseOperation
{
friend class RDOSimulator;

public:
	RDOIE( RDORuntimeParent* runtime ): RDOBaseOperation( runtime ) {}
	virtual ~RDOIE() {}

protected:
	virtual void   convertEvent( RDOSimulator* sim )        = 0;
	virtual double getNextTimeInterval( RDOSimulator* sim ) = 0;

	virtual void onBeforeIrregularEvent( RDOSimulator* sim )  {}
	virtual void onAfterIrregularEvent ( RDOSimulator* sim )  {}

private:
	double  m_time;
	virtual void     onStart         ( RDOSimulator* sim );
	virtual void     onStop          ( RDOSimulator* sim );
	virtual bool     onCheckCondition( RDOSimulator* sim );
	virtual BOResult onDoOperation   ( RDOSimulator* sim );
	virtual void     onMakePlaned    ( RDOSimulator* sim, void* param = NULL );
};

// ----------------------------------------------------------------------------
// ---------- RDORule
// ----------------------------------------------------------------------------
class RDORule: public RDOBaseOperation
{
friend class RDOSimulator;
friend class RDODPTSearch;
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
	virtual bool     onCheckCondition( RDOSimulator* sim );
	virtual BOResult onDoOperation   ( RDOSimulator* sim );
};

// ----------------------------------------------------------------------------
// ---------- RDOOperation
// ----------------------------------------------------------------------------
class RDOOperation: public RDOBaseOperation
{
friend class RDOSimulator;

public:
	RDOOperation( RDORuntimeParent* runtime ):
		RDOBaseOperation( runtime ),
		m_clones( NULL )
	{
		m_clones.reparent( this );
	}
	virtual ~RDOOperation()
	{
		m_clones.deleteObjects();
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

private:
	RDORuntimeParent m_clones;

	virtual bool     onCheckCondition( RDOSimulator* sim );
	virtual BOResult onDoOperation   ( RDOSimulator* sim );
	virtual void     onMakePlaned    ( RDOSimulator* sim, void* param = NULL );
};

// ----------------------------------------------------------------------------
// ---------- RDODPTSearch
// ----------------------------------------------------------------------------
class RDODPTSearch: public RDOBaseOperation
{
friend class RDOSimulator;
friend class TreeNode;

public:
	// ----------------------------------------------------------------------------
	// ---------- Activity
	// ----------------------------------------------------------------------------
	class Activity: public RDORuntimeObject
	{
	public:
		enum ValueTime
		{
			vt_before,
			vt_after
		};

		Activity( RDORule* rule, ValueTime valueTime ):
			RDORuntimeObject( rule ),
			m_rule( rule ),
			m_valueTime( valueTime )
		{
		}
		virtual ~Activity()
		{
		}

		RDORule*  rule()            { return m_rule;      }
		ValueTime valueTime() const { return m_valueTime; }

		virtual double cost( RDOSimulator* sim ) = 0;

	private:
		ValueTime m_valueTime;

	protected:
		RDORule* m_rule;
	};

private:
	TreeRoot* treeRoot;
	RDOBaseOperation::BOResult RunSearchInTree( RDOSimulator* sim );
	virtual bool     onCheckCondition( RDOSimulator* sim );
	virtual BOResult onDoOperation   ( RDOSimulator* sim );

protected:
	std::list< Activity* > activities;
	virtual void onSearchBegin( RDOSimulator* sim )                              = 0;
	virtual void onSearchDecisionHeader( RDOSimulator* sim )                     = 0;
	virtual void onSearchDecision( RDOSimulator* sim, TreeNode* node )           = 0;
	virtual void onSearchResultSuccess( RDOSimulator* sim, TreeRoot* treeRoot )  = 0;
	virtual void onSearchResultNotFound( RDOSimulator* sim, TreeRoot* treeRoot ) = 0;
	virtual TreeRoot* createTreeRoot( RDOSimulator* sim )                        = 0;
	virtual BOResult  onContinue( RDOSimulator* sim );

public:
	RDODPTSearch( RDORuntimeParent* runtime ):
		RDOBaseOperation( runtime ),
		treeRoot( NULL )
	{
	}
	virtual ~RDODPTSearch();
	virtual bool Condition( RDOSimulator* sim )     = 0;
	virtual bool TermCondition( RDOSimulator* sim ) = 0;
	virtual double EvaluateBy( RDOSimulator* sim )  = 0;
	virtual bool NeedCompareTops()                  = 0;
	virtual void addActivity( Activity* act );
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
	virtual bool calcStat  ( RDOSimulator* sim ) = 0;
};

// ----------------------------------------------------------------------------
// ---------- RDOSimulator - один из базовых классов для RDORuntime
// ----------------------------------------------------------------------------
class RDOSimulator: public RDOSimulatorBase
{
friend class RDODPTSearch;
friend class RDOOperation;
friend class TreeNode;
friend class RDORule;
friend class RDOIE;
friend class RDOOperation;
friend class RDODPTSearchTrace;

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
