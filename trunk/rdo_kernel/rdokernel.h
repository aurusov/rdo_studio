#ifndef RDOKERNEL_H
#define RDOKERNEL_H

#if _MSC_VER > 1000
#pragma once
#endif

#pragma warning( disable : 4786 )

#include <map>
#include <string>

using namespace std;

class RDORepository;
class RdoSimulator;

// ----------------------------------------------------------------------------
// ---------- RDOKernel
// ----------------------------------------------------------------------------
typedef void (*OnNotify)();
typedef bool (*OnBoolNotify)();
typedef void (*OnNotifyString)( string );

class RDOKernel
{
public:
	enum NotifyType       {
		// these notifies sent by "Repository"
		newModel,			// when repository created a new model
		openModel,          // when repository opened exist model
		saveModel,			// when repository saved current model
		closeModel,			// when repository closed current model
		canNotCloseModel,   // when repository can not close current model ('canCloseModel' notify returns 'false' value)

		// AB 21.02.03 // these notifies sent by "RdoSimulator"
		parseSMRError,			// on parse SMR file error (when opening model)
		parseError,				// on parse error
		modelStarted, 			// when model successfully parsed and started
		endExecuteModel,		// successfully end modelling
		executeError,		// successfully end modelling
		modelStopped };		// model externally stopped 

	enum BoolNotifyType   {
		// these notifies sent by "Repository"
		canCloseModel		// repository send this 'bool' notify before closing current model (before 'closeModel' notify). You can return 'false' value for stop closing.
	};

	enum NotifyStringType {
		buildString,
		debugString,
		traceString
	};

private:
	RDORepository* repository;
	RdoSimulator*  simulator;

	typedef multimap< NotifyType, OnNotify >             onNotifyListType;
	typedef multimap< BoolNotifyType, OnBoolNotify >     onBoolNotifyListType;
	typedef multimap< NotifyStringType, OnNotifyString > onNotifyStringListType;

	onNotifyListType       onNotify_list;
	onBoolNotifyListType   onBoolNotify_list;
	onNotifyStringListType onNotifyString_list;

public:
	RDOKernel();
	virtual ~RDOKernel();

	RDORepository* getRepository();
	RdoSimulator*  getSimulator();

	void setNotifyReflect( NotifyType notifyType, OnNotify fun );
	void setNotifyReflect( BoolNotifyType notifyType, OnBoolNotify fun );
	void setNotifyReflect( NotifyStringType notifyType, OnNotifyString fun );

	void notify( NotifyType notifyType ) const;
	bool boolNotifyAnd( BoolNotifyType notifyType ) const;
	bool boolNotifyOr( BoolNotifyType notifyType ) const;
	void notifyString( NotifyStringType notifyType, string str ) const;

	void debug( const char* str, ... );
};

// ----------------------------------------------------------------------------
extern RDOKernel kernel;

#endif // RDOKERNEL_H
