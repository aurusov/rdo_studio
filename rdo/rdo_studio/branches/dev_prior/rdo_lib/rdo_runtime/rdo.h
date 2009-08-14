#ifndef RDO_H
#define RDO_H

#include "rdoruntime_object.h"

namespace rdoRuntime
{
class RDOSimulator;
};

// ----------------------------------------------------------------------------
// ---------- RDOBaseOperation - базовый класс для паттернов, процессов и блоков процесса
// ----------------------------------------------------------------------------
// Надо бы сделать его базовым для всех возможных логик
// ----------------------------------------------------------------------------
class IBaseOperation
{
public:
	enum BOResult
	{
		BOR_cant_run = 0,
		BOR_planned_and_run,
		BOR_must_continue,
		BOR_done
	};

	// Вызывается перед стартом прогона и перед возобновление операции
	// Например, условие DPT-some поменялось с false на true
	// Используется для IE и GENERATE, чтобы задать время прихода первого клиента
	virtual void onStart(PTR(rdoRuntime::RDOSimulator) sim) = 0;

	// Вызывается при остановке операции, например, условие DPT-some поменялось с true на false
	virtual void onStop(PTR(rdoRuntime::RDOSimulator) sim) = 0;

	// Вызывается для проверки выполнимости операции
	virtual rbool onCheckCondition(PTR(rdoRuntime::RDOSimulator) sim) = 0;

	// Вызывается для выполнения правил/операций
	virtual BOResult onDoOperation(PTR(rdoRuntime::RDOSimulator) sim) = 0;

	// Вызывается для запланированных в будующем событий: IE, operation_end, keyboard_end
	// Может не использоваться, например, для rule
	virtual void onMakePlaned(PTR(rdoRuntime::RDOSimulator) sim, PTR(void) param) = 0;

	// Вызывается для продолжения долгой операции, например, DPT search
	virtual BOResult onContinue(PTR(rdoRuntime::RDOSimulator) sim) = 0;
};

#define DECLARE_IBaseOperation \
	virtual void     onStart         (PTR(rdoRuntime::RDOSimulator) sim); \
	virtual void     onStop          (PTR(rdoRuntime::RDOSimulator) sim); \
	virtual rbool    onCheckCondition(PTR(rdoRuntime::RDOSimulator) sim); \
	virtual BOResult onDoOperation   (PTR(rdoRuntime::RDOSimulator) sim); \
	virtual void     onMakePlaned    (PTR(rdoRuntime::RDOSimulator) sim, PTR(void) param); \
	virtual BOResult onContinue      (PTR(rdoRuntime::RDOSimulator) sim);

#endif // RDO_H
