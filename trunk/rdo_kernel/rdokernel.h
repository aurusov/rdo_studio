#ifndef RDOKERNEL_H
#define RDOKERNEL_H

#if _MSC_VER > 1000
#pragma once
#endif

#pragma warning( disable : 4786 )

#include <map>
#include <string>

namespace rdoRepository {
class RDORepository;
}

namespace RDOSimulatorNS {
class RdoSimulator;
}

// ----------------------------------------------------------------------------
// ---------- RDOKernel
// ----------------------------------------------------------------------------
typedef void (*OnNotify)();
typedef bool (*OnBoolNotify)();
typedef void (*OnNotifyString)( std::string );
typedef void (*OnCallback)( int parament );

class RDOKernel
{
public:
	enum NotifyType       {
		// these notifies sent by "Repository"
		newModel,			// when repository created a new model
		openModel,			// when repository opened exist model
		saveModel,			// when repository saved current model
		closeModel,			// when repository closed current model
		canNotCloseModel,   // when repository can not close current model ('canCloseModel' notify returns 'false' value)

		// AB 21.02.03 // these notifies sent by "RdoSimulator"
		showFrame,				// on have new frame to show
		parseSMRError,			// on parse SMR file error (when opening model)
		parseError,				// on parse error
		parseSuccess,			// on parse success
		beforeModelStart, 		// when model successfully parsed and ready to start
		afterModelStart,		// when model thread started
		endExecuteModel,		// successfully end modelling
		executeError,			// runtime error during modelling
		modelStopped			// model externally stopped
	};

	enum BoolNotifyType   {
		// these notifies sent by "Repository"
		canCloseModel			// repository send this 'bool' notify before closing current model (before 'closeModel' notify). You can return 'false' value for stop closing.
	};

	enum NotifyStringType {
		buildString,
		debugString,
		traceString
	};

	enum CallbackType     {
		// these callback calling by "RdoSimulator"
		modelExit			// calls after parseError, endExecuteModel, executeError, modelStopped
							// exit code (parament):
							//	0 - normal (endExecuteModel)
							//	1 - parse error (parseError)
							//	2 - run-time error (executeError)
							//	3 - stoped by user (modelStopped)
	};

private:
	rdoRepository::RDORepository* repository;
	RDOSimulatorNS::RdoSimulator* simulator;

	typedef std::multimap< NotifyType, OnNotify >             onNotifyListType;
	typedef std::multimap< BoolNotifyType, OnBoolNotify >     onBoolNotifyListType;
	typedef std::multimap< NotifyStringType, OnNotifyString > onNotifyStringListType;
	typedef std::multimap< CallbackType, OnCallback >         onCallbackListType;

	onNotifyListType       onNotify_list;
	onBoolNotifyListType   onBoolNotify_list;
	onNotifyStringListType onNotifyString_list;
	onCallbackListType     onCallback_list;

public:
	RDOKernel();
	virtual ~RDOKernel();

	rdoRepository::RDORepository* getRepository();
	RDOSimulatorNS::RdoSimulator* getSimulator();

	void setNotifyReflect( NotifyType notifyType, OnNotify fun );
	void setNotifyReflect( BoolNotifyType notifyType, OnBoolNotify fun );
	void setNotifyReflect( NotifyStringType notifyType, OnNotifyString fun );
	void setCallbackReflect( CallbackType callbackType, OnCallback fun );

	void notify( NotifyType notifyType ) const;
	bool boolNotifyAnd( BoolNotifyType notifyType ) const;
	bool boolNotifyOr( BoolNotifyType notifyType ) const;
	void notifyString( NotifyStringType notifyType, std::string str ) const;

	void callback( CallbackType callbackType, int parament ) const;
	void callbackNext( CallbackType callbackType, OnCallback fun, int parament ) const;

	void debug( const char* str, ... );
};

// ----------------------------------------------------------------------------
extern RDOKernel kernel;

#endif // RDOKERNEL_H
