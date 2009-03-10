#ifndef RDO_LOGIC_H
#define RDO_LOGIC_H

#pragma warning(disable : 4786)  

#include "rdo.h"
#include "rdocalc.h"

namespace rdoRuntime {

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

	typedef          std::vector< T* >     List;
	typedef typename List::iterator        Iterator;
	typedef typename List::const_iterator  CIterator;

	Iterator  begin()       { return m_items.begin(); }
	Iterator  end()         { return m_items.end();   }
	CIterator begin() const { return m_items.begin(); }
	CIterator end()   const { return m_items.end();   }

	void append( T* item )
	{
		if ( item )
		{
			item->reparent( this );
			m_items.push_back( item );
		}
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
// ---------- RDOLogic
// ----------------------------------------------------------------------------
class RDOLogic: public RDOOprContainer< RDOBaseOperation >
{
public:
	RDOLogic( RDOSimulator* sim );
	virtual ~RDOLogic() {}

	RDOOprContainer< RDOBaseOperation > m_childLogic;

	void setCondition( RDOCalc* calc )
	{
		m_condition = calc;
	}

	virtual void onStart( RDOSimulator* sim )
	{
		m_lastCondition = checkSelfCondition( sim );
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
		bool condition = checkSelfCondition( sim );
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
				return RDOOprContainer<RDOBaseOperation>::onCheckCondition( sim );
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
				return RDOOprContainer<RDOBaseOperation>::onDoOperation( sim );
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
		RDOOprContainer<RDOBaseOperation>::onMakePlaned( sim, param );
	}
	virtual BOResult onContinue( RDOSimulator* sim )
	{
		BOResult result = m_childLogic.onContinue( sim );
		if ( result != BOR_must_continue )
		{
			return RDOOprContainer<RDOBaseOperation>::onContinue( sim );
		}
		else
		{
			return result;
		}
	}

private:
	RDOCalc* m_condition;
	bool     m_lastCondition;

	bool checkSelfCondition( RDOSimulator* sim )
	{
		if ( m_condition )
		{
			return m_condition->calcValue( (RDORuntime*)sim ).getAsBool();
		}
		else
		{
			return true;
		}
	}
	void start( RDOSimulator* sim )
	{
		m_childLogic.onStart( sim );
		RDOOprContainer<RDOBaseOperation>::onStart( sim );
	}
	void stop( RDOSimulator* sim )
	{
		m_childLogic.onStop( sim );
		RDOOprContainer<RDOBaseOperation>::onStop( sim );
	}
};

} // namespace rdoRuntime

#endif // RDO_LOGIC_H
