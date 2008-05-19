#ifndef RDOPROCESS_H
#define RDOPROCESS_H

#include "rdo.h"
#include "rdo_runtime.h"

namespace rdoRuntime {

class RDOCalc;

// ----------------------------------------------------------------------------
// ---------- RDOPROCBlock
// ----------------------------------------------------------------------------
class RDOPROCBlock: public RDOBaseOperation
{
friend class RDOPROCTransact;
friend class RDOPROCProcess;

protected:
	RDOPROCProcess* process;
	std::list< RDOPROCTransact* > transacts;

	RDOPROCBlock( RDOPROCProcess* _process );
	virtual ~RDOPROCBlock() {}

public:
	virtual void TransactGoIn( RDOPROCTransact* _transact );
	virtual void TransactGoOut( RDOPROCTransact* _transact );
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCProcess
// ----------------------------------------------------------------------------
class RDOPROCTransact;

class RDOPROCProcess: public RDOLogic
{
friend class RDOPROCBlock;

protected:
	std::string                  name;
	RDOPROCProcess*              parent;
	std::list< RDOPROCProcess* > child;

public:
	RDOPROCProcess( const std::string& _name, RDOSimulator* sim );
	void insertChild( RDOPROCProcess* value );

	void next( RDOPROCTransact* transact );
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCTransact
// ----------------------------------------------------------------------------
class RDOPROCResource;
class RDOPROCTransact: public RDOResource
{
friend class RDOPROCProcess;

protected:
	RDOPROCBlock* block;
	std::list< RDOPROCResource* > resources;
//	int id;
//	static int ID;
public:
	RDOPROCBlock* getBlock();
	static int typeID;
	void addRes (RDOPROCResource* res);
	void removeRes (RDOPROCResource* res);
	bool findRes (RDOPROCResource* res);
	RDOPROCTransact( RDOSimulator* sim, RDOPROCBlock* _block );
	void next();
//	int GetNumber(){return id;}
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCResource
// ----------------------------------------------------------------------------
enum  forResource{turn_On = 1, turn_Off = 2, not_Seize = 3, free = 4 };

class RDOPROCResource: public RDOResource
{
protected: 
	bool turnOn;
	RDOPROCBlock* block;
public:
	std::list<RDOPROCTransact*> transacts;
	forResource AreYouReady(RDOPROCTransact* transact);
	bool BuzyInAnotherBlock (RDOPROCTransact* transact);
	bool whoYou();
	void youIs(forResource _this);
	RDOPROCBlock* getBlock();
	void setBlock(RDOPROCBlock* _block);
public:
	RDOPROCResource( RDORuntime* _runtime, int _number, unsigned int type, bool _trace );
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCGenerate
// ----------------------------------------------------------------------------
class RDOPROCGenerate: public RDOPROCBlock
{
protected:
	double   timeNext;
	RDOCalc* timeCalc;

protected:
	virtual void     onStart         ( RDOSimulator* sim );
	virtual bool     onCheckCondition( RDOSimulator* sim );
	virtual BOResult onDoOperation   ( RDOSimulator* sim );

public:
	RDOPROCGenerate( RDOPROCProcess* _process, RDOCalc* time ): RDOPROCBlock( _process ), timeNext( 0 ), timeCalc( time ) {}
	void calcNextTimeInterval( RDOSimulator* sim );
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCBlockForSeize
// ----------------------------------------------------------------------------
struct runtime_for_Seize{
	RDOPROCResource* rss;
	RDOValue     enum_free;
	RDOValue     enum_buzy;
};

struct parser_for_Seize
{
int Id_res;
int Id_param;
};

class RDOPROCBlockForSeize: public RDOPROCBlock
{
protected:
std::vector < parser_for_Seize > from_par;
std::vector < runtime_for_Seize > from_run;
virtual void onStart( RDOSimulator* sim );

public:
	RDOPROCBlockForSeize( RDOPROCProcess* _process, std::vector < parser_for_Seize > From_Par  );
	
	static std::string getStateParamName() { return "Состояние"; }
	static std::string getStateEnumFree()  { return "Свободен";  }
	static std::string getStateEnumBuzy()  { return "Занят";     }
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCSeize
// ----------------------------------------------------------------------------
class RDOPROCSeize: public RDOPROCBlockForSeize
{
private:
	int Busy_Res;
	virtual bool     onCheckCondition( RDOSimulator* sim );
	virtual BOResult onDoOperation   ( RDOSimulator* sim );
	std::map < int, forResource > for_turn;
public:
	bool AllResFree ();
	bool BuzyInAnotherBlockTurnOn ();
	RDOPROCSeize( RDOPROCProcess* _process, std::vector < parser_for_Seize > From_Par ): RDOPROCBlockForSeize( _process, From_Par ) {Busy_Res = 0;}
	virtual void TransactGoIn( RDOPROCTransact* _transact );
	virtual void TransactGoOut( RDOPROCTransact* _transact );

};

// ----------------------------------------------------------------------------
// ---------- RDOPROCRelease
// ----------------------------------------------------------------------------
class RDOPROCRelease: public RDOPROCBlockForSeize
{
private:
	virtual bool     onCheckCondition( RDOSimulator* sim );
	virtual BOResult onDoOperation   ( RDOSimulator* sim );

public:
	RDOPROCRelease( RDOPROCProcess* _process, std::vector < parser_for_Seize > From_Par ): RDOPROCBlockForSeize( _process, From_Par ) {}
};


// ----------------------------------------------------------------------------
// ---------- RDOPROCAdvance
// ----------------------------------------------------------------------------
class RDOPROCAdvance: public RDOPROCBlock
{
protected:
	RDOCalc* delayCalc;

	struct LeaveTr {
		RDOPROCTransact* transact;
		double           timeLeave;
		LeaveTr( RDOPROCTransact* _transact, double _timeLeave ):
			transact( _transact ),
			timeLeave( _timeLeave )
		{
		}
	};
	std::list< LeaveTr > leave_list;

	virtual bool     onCheckCondition( RDOSimulator* sim );
	virtual BOResult onDoOperation   ( RDOSimulator* sim );

public:
	RDOPROCAdvance( RDOPROCProcess* _process, RDOCalc* _delayCalc ): RDOPROCBlock( _process ), delayCalc( _delayCalc ) {}
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCTerminate
// ----------------------------------------------------------------------------
class RDOPROCTerminate: public RDOPROCBlock
{
protected:
	virtual bool     onCheckCondition( RDOSimulator* sim );
	virtual BOResult onDoOperation( RDOSimulator* sim );

public:
	RDOPROCTerminate( RDOPROCProcess* _process ): RDOPROCBlock( _process ) {}
};

} // namespace rdoRuntime

#endif // RDOPROCESS_H
