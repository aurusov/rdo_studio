#include "pch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "rdobase.h"

RDOSimulatorBase::RDOSimulatorBase():
	onlyEndOfOperations( false )
{
}

void RDOSimulatorBase::rdoInit()
{
   currentTime = 0;
   onInit();
//!
   timePointList.clear();
   timePointList.push_back(0);
   preProcess();
   onlyEndOfOperations = false;
//!
}

void RDOSimulatorBase::rdoDestroy()
{
   onDestroy();
}

bool RDOSimulatorBase::rdoNext()
{
	double newTime = timePointList.front();
	timePointList.pop_front();

	rdoDelay( currentTime, newTime );
	currentTime = newTime;

	bool res = false;
	for(;;) {
		if ( !onlyEndOfOperations && endCondition() ) {
			// onEndCondition();
			onlyEndOfOperations = true;
		}

		if( !(res = doOperation(onlyEndOfOperations)) ) break;

//		rdoDelay( currentTime, currentTime );
	}

	if ( timePointList.empty() ) {
		onNothingMoreToDo();
		return false;
	}

	if ( onlyEndOfOperations && !res ) {
		onEndCondition();
		return false;
	}
	return true;
}

void RDOSimulatorBase::rdoPostProcess()
{
	postProcess();
}

/*
void RDOSimulatorBase::rdoRun() 
{
   timePointList.clear();
   timePointList.push_back(0);
   preProcess();
   bool onlyEndOfOperations = false;
   for(;;)
   {
      double newTime = timePointList.front();
      timePointList.pop_front();
      rdoDelay(currentTime, newTime);
      currentTime = newTime;


      bool res = false;
      for(;;)
      {
			if(!onlyEndOfOperations && endCondition())
			{
	//         onEndCondition();
				onlyEndOfOperations = true;
			}

         if(!(res = doOperation(onlyEndOfOperations)))
            break;

	      rdoDelay(currentTime, currentTime);
      }

      if(timePointList.empty())
      {
         onNothingMoreToDo();
         break;
      }

      if(onlyEndOfOperations && !res)
      {
         onEndCondition();
         break;
      }
   }
   postProcess();
}
*/

void RDOSimulatorBase::addTimePoint( double timePoint )
{
	if ( std::find( timePointList.begin(), timePointList.end(), timePoint ) == timePointList.end() ) {
		timePointList.push_back( timePoint );
		timePointList.sort();
	}
}
