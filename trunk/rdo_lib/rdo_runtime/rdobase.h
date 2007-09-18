#ifndef RDOBASE_H
#define RDOBASE_H

#include "rdoruntime_object.h"
#include "rdodefines.h"
#include <rdocommon.h>

namespace rdoRuntime
{

class RDOBaseOperation;

class RDOSimulatorBase: public RDORuntimeParent
{
private:
	double startTime;
	double currentTime;
	double nextTime;

	RunTimeMode  mode;

	double       speed;
	unsigned int speed_range_max;
	unsigned int next_delay_count;
	unsigned int next_delay_current;

	double       showRate;
	double       msec_wait;
	unsigned int msec_prev;

protected:
	struct BOPlanned {
		RDOBaseOperation* opr;
		void*             param;
		BOPlanned()                                             : opr( NULL )    , param( NULL )       {}
		BOPlanned( const BOPlanned& copy )                      : opr( copy.opr ), param( copy.param ) {}
		BOPlanned( RDOBaseOperation* _opr, void* _param = NULL ): opr( _opr )    , param( _param )     {}
	};
	std::map< double, std::list< BOPlanned >* > timePointList;

	bool check_operation;

	void setCurrentTime( double value ) { currentTime = value; }

	// Выполнение любых операций (паттерны, DPT и процессы)
	// Если вернулось значение true, то необходимо вызвать doOperation
	// и в следующий раз/ без перевода модельного времени вперед
	virtual bool doOperation()  = 0;

	// Проверка на условие конца моделирования
	virtual bool endCondition() = 0;

	// Проверка на точки останова
	virtual bool breakPoints() = 0;

	// Инициализация/очистка симулятора
	virtual void onInit()    = 0;
	virtual void onDestroy() = 0;

	// Различные функции конца моделирования
	// Вызывется, если нет больше событий
	virtual void onNothingMoreToDo() = 0;
	// Вызывется, если сработало условие конца моделирования
	virtual void onEndCondition()    = 0;
	// Вызывется, tесли возникла ошибка прогона
	virtual void onRuntimeError()    = 0;
	// Вызывается, если модель остановлена пользователем
	virtual void onUserBreak()       = 0;

	// Вызывается в самом начале прогона
	virtual void preProcess()  = 0;
	// Вызывается непосредвтсенно перед окончанием прогона
	virtual void postProcess() = 0;

	// Проверка на нажатие клавиши или активной области
	virtual bool isKeyDown()   = 0;

	// Вызывается при увеличении модельного времени
	virtual void onNewTimeNow() {};

public:
	// Публичные методы управления симулятором
	virtual void rdoInit();
	virtual void rdoDestroy();
	virtual bool rdoNext();
	virtual void rdoPostProcess();

	void setStartTime( double value )       { startTime = value;  }
	double getCurrentTime() const           { return currentTime; }

	RunTimeMode getMode() const             { return mode;        }
	void setMode( RunTimeMode _mode );

	double getSpeed() const                 { return speed;       }
	void setSpeed( double persent );

	double getShowRate() const              { return showRate;    }
	void setShowRate( double value );

	void addTimePoint( double timePoint, RDOBaseOperation* opr = NULL, void* param = NULL  );

	RDOSimulatorBase( RDORuntimeParent* _runtime );
	virtual ~RDOSimulatorBase() {}

	static unsigned int getMSec( const SYSTEMTIME& systime ) {
		return systime.wMilliseconds + systime.wSecond * 1000 + systime.wMinute * 1000 * 60 + systime.wHour * 1000 * 60 * 60;
	}
};

} // namespace rdoRuntime;

#endif // RDOBASE_H
