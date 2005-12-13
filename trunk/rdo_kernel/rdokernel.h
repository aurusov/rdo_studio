#ifndef RDOKERNEL_H
#define RDOKERNEL_H

#if _MSC_VER > 1000
#pragma once
#endif

#pragma warning( disable : 4786 )

#include <map>
#include <list>
#include <vector>
#include <string>
#include <afxmt.h>

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
typedef bool (*OnNotifyBool)();
typedef void (*OnNotifyString)( const std::string& );
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

	enum NotifyBoolType   {
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
							// exit code (parament) look at rdoModel::RDOExitCode
							//	EC_OK            = 0 - normal (endExecuteModel)
							//	EC_ParserError   = 1 - parse error (parseError)
							//	EC_RunTimeError  = 2 - run-time error (executeError)
							//	EC_UserBreak     = 3 - stoped by user (modelStopped)
							//  EC_ModelNotFound = 4 - smr file not found (for autorun + autoexit mode)
	};

	class RDOKernelSync
	{
	friend class RDOKernel;
	private:
		template < class T > class RDONotifyFunT {
		public:
			T fun;
			unsigned long int th_id;
		};
		RDOKernel* kernel;
		unsigned long int th_id;

		typedef RDONotifyFunT< OnNotify > RDONotifyFun;
		typedef std::multimap< NotifyType, RDONotifyFun > onNotifyListType;
		onNotifyListType       onNotify_list;

		typedef RDONotifyFunT< OnNotifyString > RDONotifyStringFun;
		typedef std::multimap< NotifyStringType, RDONotifyStringFun > onNotifyStringListType;
		onNotifyStringListType onNotifyString_list;

		typedef RDONotifyFunT< OnNotifyBool > RDONotifyBoolFun;
		typedef std::multimap< NotifyBoolType, RDONotifyBoolFun > onNotifyBoolListType;
		onNotifyBoolListType   onNotifyBool_list;

	protected:
		bool bool_and_value;
		bool bool_or_value;

		virtual void notify_fromkernel( NotifyType notifyType ) { kernel->notify_unlock(); }
		virtual void notifyString_fromkernel( NotifyStringType notifyType, const std::string& str ) { kernel->notify_unlock(); }
		virtual void notifyBoolAnd_fromkernel( NotifyBoolType notifyType ) { kernel->notify_unlock(); }
		virtual void notifyBoolOr_fromkernel( NotifyBoolType notifyType ) { kernel->notify_unlock(); }

	public:
		RDOKernelSync( unsigned long int _th_id ): th_id( _th_id ), bool_and_value( false ), bool_or_value( false ) {}
		virtual ~RDOKernelSync() {}

		void notify( NotifyType notifyType ) {
			kernel->notify_fromclient( this, notifyType );
		}
		void notifyString( NotifyStringType notifyType, const std::string& str ) {
			kernel->notifyString_fromclient( this, notifyType, str );
		}
		void notifyBoolAnd( NotifyBoolType notifyType ) {
			bool_and_value = false;
			kernel->notifyBoolAnd_fromclient( this, notifyType );
		}
		void notifyBoolOr( NotifyBoolType notifyType ) {
			bool_or_value = false;
			kernel->notifyBoolOr_fromclient( this, notifyType );
		}
	};
	friend class RDOKernelSync;

private:
	rdoRepository::RDORepository* repository;
	RDOSimulatorNS::RdoSimulator* simulator;

//	typedef std::multimap< NotifyType, RDOKernelSync::RDONotifyFun >         onNotifyListType;
//	typedef std::multimap< NotifyType, OnNotify >             onNotifyListType;
//	typedef std::multimap< BoolNotifyType, OnBoolNotify >     onBoolNotifyListType;
//	typedef std::multimap< NotifyStringType, OnNotifyString > onNotifyStringListType;
	typedef std::multimap< CallbackType, OnCallback >         onCallbackListType;

//	onNotifyListType       onNotify_list;
//	onBoolNotifyListType   onBoolNotify_list;
//	onNotifyStringListType onNotifyString_list;
	onCallbackListType     onCallback_list;

	std::list< RDOKernelSync* > sync;
	CMutex mutex_notify;
	int notify_count;

	int notify_lock() {
		mutex_notify.Lock();
		int lock_level = notify_count;
		notify_count++;
		mutex_notify.Unlock();
		return lock_level;
	}
	void notify_unlock() {
		mutex_notify.Lock();
		notify_count--;
		mutex_notify.Unlock();
	}
	void notify_wait( int lock_level ) {
		mutex_notify.Lock();
		while ( notify_count > lock_level ) {
			mutex_notify.Unlock();
			::Sleep( 1 );
			mutex_notify.Lock();
		}
		mutex_notify.Unlock();
	}

	void notify_fromclient( RDOKernelSync* sync, NotifyType notifyType );
	void notifyString_fromclient( RDOKernelSync* sync, NotifyStringType notifyType, const std::string& str );
	void notifyBoolAnd_fromclient( RDOKernelSync* sync, NotifyBoolType notifyType );
	void notifyBoolOr_fromclient( RDOKernelSync* sync, NotifyBoolType notifyType );
/*
template < typename T1, class T2, class T3 >
void _setNotifyReflect( T2 notifyType, T3 fun )
{
 	TRACE( "111 - qqqqqqqqqqqqqqqq\n" );
	unsigned long int th_id_current = ::GetCurrentThreadId();
	bool found = false;
	std::list< RDOKernelSync* >::iterator sync_it = sync.begin();
	while ( sync_it != sync.end() ) {
		if ( (*sync_it)->th_id == th_id_current ) {
			found = true;
			bool flag = true;
			T1::iterator it = (*sync_it)->onNotify_list.find( notifyType );
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
*/

public:
	RDOKernel();
	virtual ~RDOKernel();

	rdoRepository::RDORepository* getRepository();
	RDOSimulatorNS::RdoSimulator* getSimulator();

	void insertSyncClient( RDOKernelSync* syncUI );
	void removeSyncClient( RDOKernelSync* syncUI );

	void setNotifyReflect( NotifyType notifyType, OnNotify fun );
	void setNotifyReflect( NotifyStringType notifyType, OnNotifyString fun );
	void setNotifyReflect( NotifyBoolType notifyType, OnNotifyBool fun );
	void setCallbackReflect( CallbackType callbackType, OnCallback fun );

	void notify( NotifyType notifyType );
	void notifyString( NotifyStringType notifyType, const std::string& str );
	bool notifyBoolAnd( NotifyBoolType notifyType );
	bool notifyBoolOr( NotifyBoolType notifyType );

	void callback( CallbackType callbackType, int parament ) const;
	void callbackNext( CallbackType callbackType, OnCallback fun, int parament ) const;

	void debug( const char* str, ... );
};

// ----------------------------------------------------------------------------
extern RDOKernel kernel;

#endif // RDOKERNEL_H
