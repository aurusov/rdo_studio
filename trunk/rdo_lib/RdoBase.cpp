#include "pch.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "RdoBase.h"

void RDOSimulatorBase::rdoInit()
{
   currentTime = 0;
   onInit();
}

void RDOSimulatorBase::rdoDestroy()
{
   onDestroy();
}

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

      if(!onlyEndOfOperations && endCondition())
      {
//         onEndCondition();
         onlyEndOfOperations = true;
      }

      bool res = false;
      for(;;)
      {
         if(!(res = doOperation(onlyEndOfOperations)))
            break;

//	      rdoDelay(currentTime, currentTime);
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

void RDOSimulatorBase::addTimePoint(double timePoint)
{
   timePointList.push_back(timePoint);
   timePointList.sort();
}
