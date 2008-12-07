
#ifndef RDO_SIMULATOR
#define RDO_SIMULATOR

#include "RdoBase.h"
#include "SearchTree.h"

class RDOSimulator;
class TreeNode;
class TreeRoot;
class RDODecisionPoint;
class RDOOperation;
class RDOIE;
class CheckOperations;

class RDORule
{
friend RDOSimulator;
friend CheckOperations;
friend RDODecisionPoint;
friend TreeNode;
protected:
   virtual void onBeforeChoiceFrom(RDOSimulator *sim) {}
   virtual void onBeforeRule(RDOSimulator *sim) {}
   virtual void onAfterRule(RDOSimulator *sim, bool inSearch = false) {}
public:
   virtual bool choiceFrom(RDOSimulator *sim) = 0;
   virtual void convertRule(RDOSimulator *sim) = 0;
   virtual ~RDORule() {}
};

class RDOActivity
{
friend TreeNode;
friend RDODecisionPoint;

   bool valueAfter;  // false = valueBefore

protected:
   virtual double costOfRule(RDOSimulator *sim) = 0;
   RDORule *rule;

public:
   RDOActivity(RDORule *r, bool vA): rule(r), valueAfter(vA) {}
   virtual ~RDOActivity();
};

class CheckOperations
{
   RDOSimulator *sim;
public:
   CheckOperations(RDOSimulator *i_sim): sim(i_sim) {}
   bool operator()(RDODecisionPoint *dp);
   bool operator()(RDOOperation *op);
   bool operator()(RDOIE *ie);
   bool operator()(RDORule *ie);
};

class RDODecisionPoint
{
friend RDOSimulator;
friend TreeNode;
friend CheckOperations;
   bool RunSearchInTree(RDOSimulator *sim);

protected:
   std::list<RDOActivity *> activities;
   virtual void onSearchBegin(RDOSimulator *sim) {}
   virtual void onSearchDecisionHeader(RDOSimulator *sim) {}
   virtual void onSearchDecision(RDOSimulator *sim, TreeNode *node) {}
   virtual void onSearchResultSuccess(RDOSimulator *sim, TreeRoot *treeRoot) {}
   virtual void onSearchResultNotFound(RDOSimulator *sim, TreeRoot *treeRoot) {}
   virtual TreeRoot *createTreeRoot(RDOSimulator *sim);

public:
   virtual bool Condition(RDOSimulator *sim) = 0;
   virtual bool TermCondition(RDOSimulator *sim) = 0;
   virtual double EvaluateBy(RDOSimulator *sim) = 0;
   virtual bool NeedCompareTops() = 0;
   virtual ~RDODecisionPoint();
   virtual void addActivity(RDOActivity *act);
};

class RDOIE    // Irregular Event
{
friend RDOSimulator;
friend CheckOperations;
   double time;

protected:
   virtual void onBeforeIrregularEvent(RDOSimulator *sim) {}
   virtual void onAfterIrregularEvent(RDOSimulator *sim) {}
   virtual double getNextTimeInterval(RDOSimulator *sim) = 0;
   virtual void convertEvent(RDOSimulator *sim) = 0;

public:
   virtual ~RDOIE() {}
};

class RDOOperation
{
friend RDOSimulator;
friend CheckOperations;
   double time;
public:
   virtual void convertEnd(RDOSimulator *sim) = 0;
   virtual void convertBegin(RDOSimulator *sim) = 0;
   virtual double getNextTimeInterval(RDOSimulator *sim) = 0;
   virtual bool choiceFrom(RDOSimulator *sim) = 0;

   // The clone function should create and return copy of operation object.
   // Do not forget to copy relevant resources.
   virtual RDOOperation *clone(RDOSimulator *sim) = 0; 

   virtual void onBeforeChoiceFrom(RDOSimulator *sim) {}
   virtual void onBeforeOperationBegin(RDOSimulator *sim) {}
   virtual void onAfterOperationBegin(RDOSimulator *sim) {}
   virtual void onBeforeOperationEnd(RDOSimulator *sim) {}
   virtual void onAfterOperationEnd(RDOSimulator *sim) {}
   virtual ~RDOOperation() {}
};

class RDOPokaz
{
public:
	virtual bool resetPokaz(RDOSimulator *sim) = 0;
	virtual bool checkPokaz(RDOSimulator *sim) = 0;
	virtual bool calcStat(RDOSimulator *sim) = 0;
};

class RDOSimulator: public RDOSimulatorBase
{
friend RDODecisionPoint;
friend RDOOperation;
friend TreeNode;
friend CheckOperations;

   std::list<RDOOperation *> operations;                 // currently processing

   bool doOperation(bool onlyEndOfOperations);
   void addOperation(RDOOperation *op);
   bool checkEndOfOperation();

protected:
   std::list<RDODecisionPoint *> haveDecisionPoints;     // all decision points
   std::list<RDOIE *> haveIrregularEvents;               // all IE in simulator
   std::list<RDORule *> haveRules;                       // all Rules in simulator
   std::list<RDOOperation *> haveOperations;             // all operations in simulator
   std::list<RDOPokaz *> havePokaz;								// all pokaz in simulator

   // Following four members are used in users class derived from RDOSimulator
   // to add Operations, IEs, Rules and DecisionPoints which can be 
   // "happened" in process.
   virtual void addTemplateOperation(RDOOperation *op) { haveOperations.push_back(op); }
   virtual void addTemplateIrregularEvent(RDOIE *ev) { haveIrregularEvents.push_back(ev); }
   virtual void addTemplateRule(RDORule *rule) { haveRules.push_back(rule); }
   virtual void addTemplateDecisionPoint(RDODecisionPoint *dp) { haveDecisionPoints.push_back(dp); }
   virtual void addPokaz(RDOPokaz *pok) { havePokaz.push_back(pok); }

   // These functions called by engine before and after process (see RDOSimulatorBase class)
   // They are used to init all Irregular Events
   virtual void preProcess();
   virtual void postProcess();

	virtual void onAfterCheckPokaz() {}


// If you have decision points you should override following two functions:
   // The clone() function must create new instance of object derived from RDOSimulator 
   // with copy of all resources.
   // Nothing else needed such as operations, rules, IEs and DecisionPoints
   virtual RDOSimulator *clone() { return NULL; } 
   // The operator==() operator must compare only resources
   virtual bool operator == (RDOSimulator &other) { return false; }


   virtual RDOSimulator *createCopy();

public:
   virtual ~RDOSimulator() {}
   void rdoInit();
   void rdoDestroy();
};

#endif //RDO_SIMULATOR
