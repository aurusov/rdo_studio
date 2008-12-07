#ifndef RDO_LOGIC_DPTSEARCH_H
#define RDO_LOGIC_DPTSEARCH_H

#include "rdo_logic.h"
#include "rdocalc.h"

namespace rdoRuntime {

class RDOSimulator;
class TreeNode;
class TreeRoot;

// ----------------------------------------------------------------------------
// ---------- RDODPTSearch
// ----------------------------------------------------------------------------
class RDORule;

class RDODPTSearch: public RDOLogic
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

	virtual void addActivity( Activity* act );

protected:
	RDODPTSearch( RDOSimulator* sim );
	virtual ~RDODPTSearch();

	std::list< Activity* > activities;
	virtual bool   TermCondition( RDOSimulator* sim )                              = 0;
	virtual double EvaluateBy( RDOSimulator* sim )                                 = 0;
	virtual void   onSearchBegin( RDOSimulator* sim )                              = 0;
	virtual void   onSearchDecisionHeader( RDOSimulator* sim )                     = 0;
	virtual void   onSearchDecision( RDOSimulator* sim, TreeNode* node )           = 0;
	virtual void   onSearchResultSuccess( RDOSimulator* sim, TreeRoot* treeRoot )  = 0;
	virtual void   onSearchResultNotFound( RDOSimulator* sim, TreeRoot* treeRoot ) = 0;
	virtual bool   NeedCompareTops()                                               = 0;
	virtual TreeRoot* createTreeRoot( RDOSimulator* sim )                          = 0;
	virtual BOResult  onContinue( RDOSimulator* sim );

private:
	TreeRoot* treeRoot;
	virtual BOResult onDoOperation( RDOSimulator* sim );
};

} // namespace rdoRuntime

#endif // RDO_LOGIC_DPTSEARCH_H
