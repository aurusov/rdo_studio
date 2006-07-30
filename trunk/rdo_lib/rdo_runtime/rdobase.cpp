#include "pch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "rdobase.h"
#include <limits>

RDOSimulatorBase::RDOSimulatorBase():
	currentTime( 0 ),
	speed( 1 ),
	speed_range_max( 500000 ),
	next_delay_count( 0 ),
	next_delay_current( 0 ),
	showRate( 60 ),
	msec_wait( 0 ),
	msec_prev( 0 )
{
}

void RDOSimulatorBase::rdoInit()
{
	currentTime = 0;
	onInit();

	timePointList.clear();
	timePointList.push_back(0);
	preProcess();

	speed              = 1;
	next_delay_count   = 0;
	next_delay_current = 0;
	showRate           = 60;
	msec_wait          = 0;
}

void RDOSimulatorBase::rdoDestroy()
{
	onDestroy();
}

bool RDOSimulatorBase::rdoNext()
{
	// «адержка общей скорости моделировани€
	if ( next_delay_count ) {
		next_delay_current++;
		if ( next_delay_current < next_delay_count ) return true;
		next_delay_current = 0;
	}
	// «адержка синхронной скорости моделировани€ (длительность операций)
	if ( msec_wait > 1 ) {
		SYSTEMTIME systime_current;
		::GetSystemTime( &systime_current );
		unsigned int msec_curr = getMSec( systime_current );
		unsigned int msec_delta;
		// ћилисекунды считаютс€ с учетом часов, но при смене суток часы сбрасываютс€ на ноль,
		// и текущее врем€ в милисекундах становитс€ меньше предыдущего. ”читываем этот момент
		// через ветку ELSE. “еперь система сможет учесть переход на один день вперед между
		// двум€ соседники моментами времени, но не сможет учесть на два и более дн€. Ёто
		// €вл€етс€ маразматической ситуаций (ждать слудующего событи€ два дн€), но запросто
		// может потребоватьс€ в системе мониторинга реального времени, котора€ работает в
		// автоматическом режиме, обрабатыва€ информацию с контроллеров. –ƒќ это делать не умеет.
		//  ак решение - отказ от синхронной работы в таких системах, и учет только скорости, или
		// переход на работу с календарем дней, мес€цев и лет. SYSTEMTIME содержит такую информацию.
		if ( msec_curr >= msec_prev ) {
			msec_delta = msec_curr - msec_prev;
		} else {
			msec_delta = UINT_MAX - msec_prev + msec_curr;
		}
		if ( msec_delta <= msec_wait ) return true;
		msec_wait -= msec_delta;
	}
	// ќкончание моделировани€ - сработало событие конца
	if ( endCondition() ) return false;
	// ¬ыполнение операции
	if ( doOperation() ) {
		return true;
	} else {
		// ѕереход к следующей операции
		if ( !timePointList.empty() ) {
			double newTime = timePointList.front();
			timePointList.pop_front();
			if ( currentTime > newTime ) {
				newTime = currentTime;
			}
			msec_wait += (newTime - currentTime) * 3600.0 * 1000.0 / showRate;
			if ( msec_wait > 0 ) {
				if ( currentTime != 0 ) {
					if ( speed > DBL_MIN ) {
						msec_wait = msec_wait / speed;
					} else {
						msec_wait = msec_wait / DBL_MIN;
					}
					SYSTEMTIME systime_current;
					::GetSystemTime( &systime_current );
					msec_prev = getMSec( systime_current );
				} else {
					msec_wait = 0;
				}
			}
			currentTime = newTime;
			return true;
		} else {
			// ќкончание моделировани€ - нет больше событий
			onNothingMoreToDo();
			return false;
		}
	}
	return true;
}

void RDOSimulatorBase::setSpeed( double value )
{
	if ( value < 0 ) value = 0;
	if ( value > 1 ) value = 1;
	speed = value;
	next_delay_count = (1 - speed) * speed_range_max;
	// „тобы сразу перейти к следующей операции
	next_delay_current = next_delay_count;
	msec_wait          = 0;
}

void RDOSimulatorBase::setShowRate( double value )
{
	if ( value < DBL_MIN ) value = DBL_MIN;
	if ( value > DBL_MAX ) value = DBL_MAX;
	showRate = value;
	// „тобы сразу перейти к следующей операции
	next_delay_current = next_delay_count;
	msec_wait          = 0;
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
