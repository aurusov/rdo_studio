#include "rdokernel.h"

#include <rdorepository.h>
#include <simulator.h>

// ----------------------------------------------------------------------------
// ---------- RDOStudioKernel
// ----------------------------------------------------------------------------
RDOKernel kernel;

RDOKernel::RDOKernel():
	repository( NULL ),
	simulator( NULL )
{
}

RDOKernel::~RDOKernel()
{
	if ( repository ) { delete repository; repository = NULL; }
	if ( simulator )  { delete simulator; simulator = NULL; }
}

RDORepository* RDOKernel::getRepository()
{
	if ( !repository ) {
		repository = new RDORepository;
	}
	return repository;
}

RdoModel* RDOKernel::getSimulator()
{
	if ( !simulator ) {
		simulator = new RdoModel;
	}
	return simulator;
}

void RDOKernel::setNotifyReflect( NotifyType notifyType, OnNotify fun )
{
	bool flag = true;
	onNotifyListType::iterator it = onNotify_list.find( notifyType );
	while ( it != onNotify_list.end() ) {
		if ( (*it).second == fun ) {
			flag = false;
			break;
		}
		it++;
	}
	if ( flag ) {
		onNotify_list.insert( onNotifyListType::value_type( notifyType, fun  ) );
	}
}

void RDOKernel::setNotifyReflect( BoolNotifyType notifyType, OnBoolNotify fun )
{
	bool flag = true;
	onBoolNotifyListType::iterator it = onBoolNotify_list.find( notifyType );
	while ( it != onBoolNotify_list.end() ) {
		if ( (*it).second == fun ) {
			flag = false;
			break;
		}
		it++;
	}
	if ( flag ) {
		onBoolNotify_list.insert( onBoolNotifyListType::value_type( notifyType, fun  ) );
	}
}

void RDOKernel::setNotifyReflect( NotifyStringType notifyType, OnNotifyString fun )
{
	bool flag = true;
	onNotifyStringListType::iterator it = onNotifyString_list.find( notifyType );
	while ( it != onNotifyString_list.end() ) {
		if ( (*it).second == fun ) {
			flag = false;
			break;
		}
		it++;
	}
	if ( flag ) {
		onNotifyString_list.insert( onNotifyStringListType::value_type( notifyType, fun  ) );
	}
}

void RDOKernel::onNotify( NotifyType notifyType ) const
{
	onNotifyListType::const_iterator it = onNotify_list.lower_bound( notifyType );
	while ( it != onNotify_list.upper_bound( notifyType ) ) {
		(*it).second();
		it++;
	}
}

bool RDOKernel::onBoolNotifyAnd( BoolNotifyType notifyType ) const
{
	onBoolNotifyListType::const_iterator it = onBoolNotify_list.lower_bound( notifyType );
	while ( it != onBoolNotify_list.upper_bound( notifyType ) ) {
		if ( !(*it).second() ) {
			return false;
		}
		it++;
	}
	return true;
}

bool RDOKernel::onBoolNotifyOr( BoolNotifyType notifyType ) const
{
	bool flag = true;
	onBoolNotifyListType::const_iterator it = onBoolNotify_list.lower_bound( notifyType );
	while ( it != onBoolNotify_list.upper_bound( notifyType ) ) {
		if ( (*it).second() ) {
			return true;
		} else {
			flag = false;
		}
		it++;
	}
	return flag;
}

void RDOKernel::onNotifyString( NotifyStringType notifyType, string str ) const
{
	onNotifyStringListType::const_iterator it = onNotifyString_list.lower_bound( notifyType );
	while ( it != onNotifyString_list.upper_bound( notifyType ) ) {
		(*it).second( str );
		it++;
	}
}
