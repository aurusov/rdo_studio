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
class RdoModel;

// ----------------------------------------------------------------------------
// ---------- RDOKernel
// ----------------------------------------------------------------------------
typedef void (*OnNotify)();
typedef bool (*OnBoolNotify)();
typedef void (*OnNotifyString)( string );

class RDOKernel
{
private:
	RDORepository* repository;
	RdoModel*      simulator;

	typedef multimap< int, OnNotify >       onNotifyListType;
	typedef multimap< int, OnBoolNotify >   onBoolNotifyListType;
	typedef multimap< int, OnNotifyString > onNotifyStringListType;

	onNotifyListType       onNotify_list;
	onBoolNotifyListType   onBoolNotify_list;
	onNotifyStringListType onNotifyString_list;

public:
	RDOKernel();
	virtual ~RDOKernel();

	RDORepository* getRepository();
	RdoModel*      getSimulator();

	enum NotifyType       { newModel, openModel, saveModel, closeModel, canNotCloseModel, parseModel, executeModel, stopModel };
	enum BoolNotifyType   { canCloseModel };
	enum NotifyStringType { buildString, debugString, traceString };

	void setNotifyReflect( NotifyType notifyType, OnNotify fun );
	void setNotifyReflect( BoolNotifyType notifyType, OnBoolNotify fun );
	void setNotifyReflect( NotifyStringType notifyType, OnNotifyString fun );

	void onNotify( NotifyType notifyType ) const;
	bool onBoolNotifyAnd( BoolNotifyType notifyType ) const;
	bool onBoolNotifyOr( BoolNotifyType notifyType ) const;
	void onNotifyString( NotifyStringType notifyType, string str ) const;
};

// ----------------------------------------------------------------------------
extern RDOKernel kernel;

#endif // RDOKERNEL_H
