#include "rdokernel.h"

#include <rdorepository.h>
#include <rdosimwin.h>
#include <stdio.h>
#include <stdarg.h>

using namespace std;

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

RDOSimulatorNS::RdoSimulator* RDOKernel::getSimulator()
{
	if ( !simulator ) {
		simulator = new RDOSimulatorNS::RdoSimulator;
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

void RDOKernel::notify( NotifyType notifyType ) const
{
	onNotifyListType::const_iterator it = onNotify_list.lower_bound( notifyType );
	while ( it != onNotify_list.upper_bound( notifyType ) ) {
		(*it).second();
		it++;
	}
}

bool RDOKernel::boolNotifyAnd( BoolNotifyType notifyType ) const
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

bool RDOKernel::boolNotifyOr( BoolNotifyType notifyType ) const
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

void RDOKernel::notifyString( NotifyStringType notifyType, string str ) const
{
	onNotifyStringListType::const_iterator it = onNotifyString_list.lower_bound( notifyType );
	while ( it != onNotifyString_list.upper_bound( notifyType ) ) {
		(*it).second( str );
		it++;
	}
}

void RDOKernel::debug( const char* str, ... )
{
	vector< char > s;
	s.resize( 256 );
	va_list paramList;
	int size = -1;
	while ( size == -1 ) {
		va_start( paramList, str );
		size = _vsnprintf( s.begin(), s.size(), str, paramList );
		va_end( paramList );
		if ( size == -1 ) {
			s.resize( s.size() + 256 );
		}
	}
	s.resize( size );
	notifyString( debugString, string( s.begin(), s.end() ) );
}
