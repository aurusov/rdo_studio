#ifndef RDO_H
#define RDO_H

#include "rdoruntime_object.h"

namespace rdoRuntime {

class RDOSimulator;

// ----------------------------------------------------------------------------
// ---------- RDOBaseOperation - базовый класс для паттернов, процессов и блоков процесса
// ----------------------------------------------------------------------------
// Надо бы сделать его базовым для всех возможных логик
// ----------------------------------------------------------------------------
class RDOBaseOperation: public RDORuntimeParent
{
public:
	enum BOResult {
		BOR_cant_run = 0,
		BOR_planned_and_run,
		BOR_must_continue,
		BOR_done
	};

	// Вызывается перед стартом прогона и перед возобновление операции
	// Например, условие DPT-some поменялось с false на true
	// Используется для IE и GENERATE, чтобы задать время прихода первого клиента
	virtual void onStart( RDOSimulator* sim )
	{
	}

	// Вызывается при остановке операции, например, условие DPT-some поменялось с true на false
	virtual void onStop( RDOSimulator* sim )
	{
	}

	// Вызывается для проверки выполнимости операции
	virtual bool onCheckCondition( RDOSimulator* sim ) = 0;

	// Вызывается для выполнения правил/операций
	virtual BOResult onDoOperation( RDOSimulator* sim ) = 0;

	// Вызывается для запланированных в будующем событий: IE, operation_end, keyboard_end
	// Может не использоваться, например, для rule
	virtual void onMakePlaned( RDOSimulator* sim, void* param = NULL )
	{
	}

	// Вызывается для продолжения долгой операции, например, DPT search
	virtual BOResult onContinue( RDOSimulator* sim )
	{
		return BOR_cant_run;
	}

protected:
	RDOBaseOperation( RDORuntimeParent* parent ): RDORuntimeParent( parent ) {}
	virtual ~RDOBaseOperation() {}
};

} // namespace rdoRuntime

#endif // RDO_H
