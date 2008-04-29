#ifndef RDO_SIMULATOR_H
#define RDO_SIMULATOR_H

#include "rdo.h"
#include "rdobase.h"
#include "rdo_logic_opr.h"

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDOSimulator - один из базовых классов дл€ RDORuntime
// ----------------------------------------------------------------------------
class RDOSimulator: public RDOSimulatorBase
{
friend class RDOTrace;

public:
	RDOSimulator():
		RDOSimulatorBase(),
		m_logics( NULL ),
		m_lastLogic( NULL ),
		opr_must_continue( NULL ),
		m_sizeof_sim( 0 )
	{
		m_logics.reparent( this );
	}
	virtual ~RDOSimulator()
	{
	}

	void appendLogic( RDOLogic* logic )
	{
		m_lastLogic = logic;
		m_logics.append( logic );
	}

	RDOBaseOperation* getMustContinueOpr() const       { return opr_must_continue;  }
	void setMustContinueOpr( RDOBaseOperation* value ) { opr_must_continue = value; }

	virtual void onPutToTreeNode() = 0;

	std::string writeActivitiesStructure( int& counter );

	RDOSimulator* createCopy();
	// ƒл€ DPT необходимо перекрыть две нижеследующие функции:
	// 1. —оздает клон RDOSimulator с копи€ми всех ресурсов, но не более
	virtual RDOSimulator* clone()                    = 0;
	// 2. —равнение двух симул€торов по ресурсам
	virtual bool operator == ( RDOSimulator& other ) = 0;

	unsigned int getSizeofSim() const
	{
		return m_sizeof_sim;
	}

protected:
	void appendBaseOperation( RDOBaseOperation* op )
	{
		getLastLogic()->append( op );
	}

	// »нициализирует нерегул€рные событи€ и блоки GENERATE: задает врем€ первого срабатывани€
	virtual void preProcess();

	virtual void onResetPokaz()      = 0;
	virtual void onCheckPokaz()      = 0;
	virtual void onAfterCheckPokaz() = 0;

	unsigned int m_sizeof_sim;

private:
	RDOLogic  m_logics;
	RDOLogic* m_lastLogic;

	RDOLogic* getLastLogic()
	{
		return m_lastLogic;
	}

	RDOBaseOperation* opr_must_continue;
	virtual bool doOperation();
};

} // namespace rdoRuntime

#endif // RDO_SIMULATOR_H
