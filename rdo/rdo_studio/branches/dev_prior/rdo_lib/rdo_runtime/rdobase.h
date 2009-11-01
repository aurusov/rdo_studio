#ifndef RDOBASE_H
#define RDOBASE_H

#include "rdoruntime_object.h"
#include "rdo_runtime_interface_registrator.h"
#include <rdocommon.h>

#ifdef RDO_MT
#include <afxwin.h>
#else
#include <windows.h>
#endif

namespace rdoRuntime
{

class RDOBaseOperation;

class RDOSimulatorBase: public RDORuntimeParent
{
public:
	// Публичные методы управления симулятором
	virtual void rdoInit();
	virtual bool rdoNext();
	virtual void rdoPostProcess();

	void setStartTime( double value )          { m_startTime = value;  }
	double getCurrentTime() const              { return m_currentTime; }

	

	RunTimeMode getMode() const                { return m_mode;        }
	void setMode( RunTimeMode _mode );

	double getSpeed() const                    { return m_speed;       }
	void setSpeed( double persent );

	double getShowRate() const                 { return m_showRate;    }
	void setShowRate( double value );

	void addTimePoint   (double timePoint, CREF(LPIBaseOperation) opr, void* param = NULL);
	void removeTimePoint(CREF(LPIBaseOperation) opr);

	void inc_cnt_events()      { m_cnt_events++;      }
	void inc_cnt_choice_from() { m_cnt_choice_from++; }
	void inc_cnt_calc_arithm() { m_cnt_calc_arithm++; }
	void inc_cnt_calc_logic()  { m_cnt_calc_logic++;  }

	unsigned int get_cnt_events()      { return m_cnt_events;      }
	unsigned int get_cnt_choice_from() { return m_cnt_choice_from; }
	unsigned int get_cnt_calc_arithm() { return m_cnt_calc_arithm; }
	unsigned int get_cnt_calc_logic()  { return m_cnt_calc_logic;  }

	static unsigned int getMSec( const SYSTEMTIME& systime )
	{
		return systime.wMilliseconds + systime.wSecond * 1000 + systime.wMinute * 1000 * 60 + systime.wHour * 1000 * 60 * 60;
	}

protected:
	RDOSimulatorBase();
	virtual ~RDOSimulatorBase() {}

	struct BOPlanned
	{
		LPIBaseOperation  m_opr;
		void*             m_param;
		BOPlanned()                                          : m_opr( NULL )      , m_param( NULL )         {}
		BOPlanned( const BOPlanned& copy )                   : m_opr( copy.m_opr ), m_param( copy.m_param ) {}
		BOPlanned( LPIBaseOperation opr, void* param = NULL ): m_opr( opr )       , m_param( param )        {}
	};
	typedef std::list< BOPlanned >              BOPlannedItem;
	typedef std::map< double, BOPlannedItem* >  BOPlannedMap;
	BOPlannedMap                                m_timePoints;

	bool m_check_operation;

	void setCurrentTime( double value ) { m_currentTime = value; }

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

private:
	double m_startTime;
	double m_currentTime;
	double m_nextTime;


	RunTimeMode  m_mode;

	double       m_speed;
	unsigned int m_speed_range_max;
	unsigned int m_next_delay_count;
	unsigned int m_next_delay_current;

	double       m_showRate;
	double       m_msec_wait;
	unsigned int m_msec_prev;

	unsigned int m_cnt_events;
	unsigned int m_cnt_choice_from;
	unsigned int m_cnt_calc_arithm;
	unsigned int m_cnt_calc_logic;
};

} // namespace rdoRuntime;

#endif // RDOBASE_H
