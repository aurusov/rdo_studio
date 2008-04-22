#ifndef RDO_H
#define RDO_H

#include "rdoruntime_object.h"
#include "rdocalc.h"

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

protected:
	RDOBaseOperation( RDORuntimeParent* parent ): RDORuntimeParent( parent ) {}
	virtual ~RDOBaseOperation() {}
};

// ----------------------------------------------------------------------------
// ---------- RDODPTSearch
// ----------------------------------------------------------------------------
class RDORule;

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

		Activity( RDORule* rule, ValueTime valueTime );
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

} // namespace rdoRuntime

#endif // RDO_H
