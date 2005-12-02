#include "rdokernel.h"

#include <rdorepository.h>
#include <rdosimwin.h>
#include <stdio.h>
#include <stdarg.h>
#include <algorithm>

using namespace std;

// ----------------------------------------------------------------------------
// ---------- RDOStudioKernel
// ----------------------------------------------------------------------------
RDOKernel kernel;

RDOKernel::RDOKernel():
	repository( NULL ),
	simulator( NULL ),
	notify_count( 0 )
{
}

RDOKernel::~RDOKernel()
{
	if ( repository ) { delete repository; repository = NULL; }
	if ( simulator )  { delete simulator; simulator = NULL; }
}

rdoRepository::RDORepository* RDOKernel::getRepository()
{
	if ( !repository ) {
		repository = new rdoRepository::RDORepository;
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

// -------------------------------------
// notify
// -------------------------------------
void RDOKernel::setNotifyReflect( NotifyType notifyType, OnNotify fun )
{
	unsigned long int th_id_current = ::GetCurrentThreadId();
	bool found = false;
	std::list< RDOKernelSync* >::iterator sync_it = sync.begin();
	while ( sync_it != sync.end() ) {
		if ( (*sync_it)->th_id == th_id_current ) {
			found = true;
			bool flag = true;
			RDOKernelSync::onNotifyListType::iterator it = (*sync_it)->onNotify_list.find( notifyType );
			while ( it != (*sync_it)->onNotify_list.end() ) {
				if ( (*it).second.fun == fun ) {
					flag = false;
					break;
				}
				it++;
			}
			if ( flag ) {
				RDOKernelSync::RDONotifyFun nf;
				nf.fun   = fun;
				nf.th_id = th_id_current;
				(*sync_it)->onNotify_list.insert( RDOKernelSync::onNotifyListType::value_type( notifyType, nf ) );
			}
			break;
		}
		sync_it++;
	}
	if ( !found ) {
 		TRACE( "\nnotify: sync not found\n" );
	}
}

void RDOKernel::notify( NotifyType notifyType )
{
	unsigned long int th_id_current = ::GetCurrentThreadId();
	std::list< RDOKernelSync* >::iterator sync_it = sync.begin();
	while ( sync_it != sync.end() ) {
		if ( (*sync_it)->th_id == th_id_current ) {
			RDOKernelSync::onNotifyListType::const_iterator fun_it = (*sync_it)->onNotify_list.lower_bound( notifyType );
			while ( fun_it != (*sync_it)->onNotify_list.upper_bound( notifyType ) ) {
				(*fun_it).second.fun();
				fun_it++;
			}
		} else {
//			TRACE( "\nSync = %d\n", *sync_it );
			int lock_level = notify_lock();
			(*sync_it)->notify_fromkernel( notifyType );
			notify_wait( lock_level );
		}
		sync_it++;
	}
}

void RDOKernel::notify_fromUI( RDOKernelSync* sync, NotifyType notifyType )
{
	RDOKernelSync::onNotifyListType::const_iterator it = sync->onNotify_list.lower_bound( notifyType );
	while ( it != sync->onNotify_list.upper_bound( notifyType ) ) {
		(*it).second.fun();
		it++;
	}
	notify_unlock();
}

// -------------------------------------
// notify string
// -------------------------------------
void RDOKernel::setNotifyReflect( NotifyStringType notifyType, OnNotifyString fun )
{
	unsigned long int th_id_current = ::GetCurrentThreadId();
	bool found = false;
	std::list< RDOKernelSync* >::iterator sync_it = sync.begin();
	while ( sync_it != sync.end() ) {
		if ( (*sync_it)->th_id == th_id_current ) {
			found = true;
			bool flag = true;
			RDOKernelSync::onNotifyStringListType::iterator it = (*sync_it)->onNotifyString_list.find( notifyType );
			while ( it != (*sync_it)->onNotifyString_list.end() ) {
				if ( (*it).second.fun == fun ) {
					flag = false;
					break;
				}
				it++;
			}
			if ( flag ) {
				RDOKernelSync::RDONotifyStringFun nf;
				nf.fun   = fun;
				nf.th_id = th_id_current;
				(*sync_it)->onNotifyString_list.insert( RDOKernelSync::onNotifyStringListType::value_type( notifyType, nf ) );
			}
			break;
		}
		sync_it++;
	}
	if ( !found ) {
		TRACE( "\nnotify string: sync not found\n" );
	}
}

void RDOKernel::notifyString( NotifyStringType notifyType, const string& str )
{
	unsigned long int th_id_current = ::GetCurrentThreadId();
	std::list< RDOKernelSync* >::iterator sync_it = sync.begin();
	while ( sync_it != sync.end() ) {
		if ( (*sync_it)->th_id == th_id_current ) {
			RDOKernelSync::onNotifyStringListType::const_iterator fun_it = (*sync_it)->onNotifyString_list.lower_bound( notifyType );
			while ( fun_it != (*sync_it)->onNotifyString_list.upper_bound( notifyType ) ) {
				(*fun_it).second.fun( str );
				fun_it++;
			}
		} else {
//			TRACE( "\nSync = %d\n", *sync_it );
			int lock_level = notify_lock();
			(*sync_it)->notifyString_fromkernel( notifyType, str );
			notify_wait( lock_level );
		}
		sync_it++;
	}
}

void RDOKernel::notifyString_fromUI( RDOKernelSync* sync, NotifyStringType notifyType, const string& str )
{
	RDOKernelSync::onNotifyStringListType::const_iterator it = sync->onNotifyString_list.lower_bound( notifyType );
	while ( it != sync->onNotifyString_list.upper_bound( notifyType ) ) {
		(*it).second.fun( str );
		it++;
	}
	notify_unlock();
}


// -------------------------------------
// notify bool
// -------------------------------------
void RDOKernel::setNotifyReflect( NotifyBoolType notifyType, OnNotifyBool fun )
{
	unsigned long int th_id_current = ::GetCurrentThreadId();
	bool found = false;
	std::list< RDOKernelSync* >::iterator sync_it = sync.begin();
	while ( sync_it != sync.end() ) {
		if ( (*sync_it)->th_id == th_id_current ) {
			found = true;
			bool flag = true;
			RDOKernelSync::onNotifyBoolListType::iterator it = (*sync_it)->onNotifyBool_list.find( notifyType );
			while ( it != (*sync_it)->onNotifyBool_list.end() ) {
				if ( (*it).second.fun == fun ) {
					flag = false;
					break;
				}
				it++;
			}
			if ( flag ) {
				RDOKernelSync::RDONotifyBoolFun nf;
				nf.fun   = fun;
				nf.th_id = th_id_current;
				(*sync_it)->onNotifyBool_list.insert( RDOKernelSync::onNotifyBoolListType::value_type( notifyType, nf ) );
			}
			break;
		}
		sync_it++;
	}
	if ( !found ) {
		TRACE( "\nnotify bool: sync not found\n" );
	}
}

bool RDOKernel::notifyBoolAnd( NotifyBoolType notifyType )
{
	unsigned long int th_id_current = ::GetCurrentThreadId();
	std::list< RDOKernelSync* >::iterator sync_it = sync.begin();
	while ( sync_it != sync.end() ) {
		if ( (*sync_it)->th_id == th_id_current ) {
			RDOKernelSync::onNotifyBoolListType::const_iterator fun_it = (*sync_it)->onNotifyBool_list.lower_bound( notifyType );
			while ( fun_it != (*sync_it)->onNotifyBool_list.upper_bound( notifyType ) ) {
				if ( !(*fun_it).second.fun() ) {
					return false;
				}
				fun_it++;
			}
		} else {
			int lock_level = notify_lock();
			(*sync_it)->notifyBoolAnd_fromkernel( notifyType );
			notify_wait( lock_level );
			if ( !(*sync_it)->bool_and_value ) {
				return false;
			}
		}
		sync_it++;
	}
	return true;
}

bool RDOKernel::notifyBoolOr( NotifyBoolType notifyType )
{
	bool flag = true;
	unsigned long int th_id_current = ::GetCurrentThreadId();
	std::list< RDOKernelSync* >::iterator sync_it = sync.begin();
	while ( sync_it != sync.end() ) {
		if ( (*sync_it)->th_id == th_id_current ) {
			RDOKernelSync::onNotifyBoolListType::const_iterator fun_it = (*sync_it)->onNotifyBool_list.lower_bound( notifyType );
			while ( fun_it != (*sync_it)->onNotifyBool_list.upper_bound( notifyType ) ) {
				if ( (*fun_it).second.fun() ) {
					return true;
				} else {
					flag = false;
				}
				fun_it++;
			}
		} else {
			int lock_level = notify_lock();
			(*sync_it)->notifyBoolOr_fromkernel( notifyType );
			notify_wait( lock_level );
			if ( (*sync_it)->bool_or_value ) {
				return true;
			} else {
				flag = false;
			}
		}
		sync_it++;
	}
	return flag;
}

void RDOKernel::notifyBoolAnd_fromUI( RDOKernelSync* sync, NotifyBoolType notifyType )
{
	RDOKernelSync::onNotifyBoolListType::const_iterator it = sync->onNotifyBool_list.lower_bound( notifyType );
	while ( it != sync->onNotifyBool_list.upper_bound( notifyType ) ) {
		if ( !(*it).second.fun() ) {
			sync->bool_and_value = false;
			notify_unlock();
			return;
		}
		it++;
	}
	notify_unlock();
	sync->bool_and_value = true;
	return;
}

void RDOKernel::notifyBoolOr_fromUI( RDOKernelSync* sync, NotifyBoolType notifyType )
{
	bool flag = true;
	RDOKernelSync::onNotifyBoolListType::const_iterator it = sync->onNotifyBool_list.lower_bound( notifyType );
	while ( it != sync->onNotifyBool_list.upper_bound( notifyType ) ) {
		if ( (*it).second.fun() ) {
			sync->bool_and_value = true;
			notify_unlock();
			return;
		} else {
			flag = false;
		}
		it++;
	}
	sync->bool_and_value = flag;
	notify_unlock();
	return;
}

// -------------------------------------
// callback
// -------------------------------------
void RDOKernel::setCallbackReflect( CallbackType callbackType, OnCallback fun )
{
	bool flag = true;
	onCallbackListType::iterator it = onCallback_list.find( callbackType );
	while ( it != onCallback_list.end() ) {
		if ( (*it).second == fun ) {
			flag = false;
			break;
		}
		it++;
	}
	if ( flag ) {
		onCallback_list.insert( onCallbackListType::value_type( callbackType, fun ) );
	}
}

void RDOKernel::callback( CallbackType callbackType, int parament ) const
{
	return;
	onCallbackListType::const_iterator it = onCallback_list.lower_bound( callbackType );
	if ( it != onCallback_list.upper_bound( callbackType ) ) {
		(*it).second( parament );
	}
}

void RDOKernel::callbackNext( CallbackType callbackType, OnCallback fun, int parament ) const
{
	return;
	onCallbackListType::const_iterator it = onCallback_list.lower_bound( callbackType );
	while ( it != onCallback_list.upper_bound( callbackType ) ) {
		if ( (*it).second == fun ) {
			it++;
			if ( it != onCallback_list.upper_bound( callbackType ) ) {
				(*it).second( parament );
			}
			break;
		}
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

int RDOKernel::notify_lock()
{
	mutex_notify.Lock();
	int lock_level = notify_count;
	notify_count++;
	mutex_notify.Unlock();
	return lock_level;
}

void RDOKernel::notify_unlock()
{
	mutex_notify.Lock();
	notify_count--;
	mutex_notify.Unlock();
}

void RDOKernel::notify_wait( int lock_level )
{
	mutex_notify.Lock();
	while ( notify_count > lock_level ) {
		mutex_notify.Unlock();
		::Sleep( 1 );
		mutex_notify.Lock();
	}
	mutex_notify.Unlock();
}

void RDOKernel::insertSyncUI( RDOKernelSync* syncUI )
{
	syncUI->kernel = this;
	sync.push_back( syncUI );
}

void RDOKernel::removeSyncUI( RDOKernelSync* syncUI )
{
	sync.remove( syncUI );
}
