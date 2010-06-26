#ifndef RDO_SIMULATOR_H
#define RDO_SIMULATOR_H

#include "rdo_lib/rdo_runtime/rdo.h"
#include "rdo_lib/rdo_runtime/rdobase.h"
#include "rdo_lib/rdo_runtime/rdo_logic_interface.h"

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDOSimulator - один из базовых классов дл€ RDORuntime
// ----------------------------------------------------------------------------
class RDOSimulator: public RDOSimulatorBase
{
//friend class RDOTrace;

public:
	RDOSimulator();
	virtual ~RDOSimulator();

	void appendLogic(CREF(LPIBaseOperation) logic, LPIBaseOperationContainer parent);

	LPIBaseOperation getMustContinueOpr() const           { return opr_must_continue;  }
	void setMustContinueOpr(CREF(LPIBaseOperation) value) { opr_must_continue = value; }

	virtual void onPutToTreeNode() = 0;

	tstring writeActivitiesStructure(REF(ruint) counter);

	RDOSimulator* createCopy();
	// ƒл€ DPT необходимо перекрыть две нижеследующие функции:
	// 1. —оздает клон RDOSimulator с копи€ми всех ресурсов, но не более
	virtual RDOSimulator* clone()                    = 0;
	// 2. —равнение двух симул€торов по ресурсам
	virtual bool operator == ( RDOSimulator& other ) = 0;

	ruint getSizeofSim() const
	{
		return m_sizeof_sim;
	}

	LPIBaseOperationContainer m_metaLogic;

protected:
	void appendBaseOperation(LPIBaseOperationContainer logic, CREF(LPIBaseOperation) op)
	{
		ASSERT(op);
//		ASSERT(!m_metaLogic->empty());
//		LPIBaseOperationContainer logic = m_metaLogic->back();
		ASSERT(logic);
		logic->append(op);
	}

	// »нициализирует нерегул€рные событи€ и блоки GENERATE: задает врем€ первого срабатывани€
	virtual void preProcess();

	virtual void onResetPokaz()      = 0;
	virtual void onCheckPokaz()      = 0;
	virtual void onAfterCheckPokaz() = 0;

	ruint m_sizeof_sim;

private:
	LPIBaseOperation opr_must_continue;
	virtual bool doOperation();
};

} // namespace rdoRuntime

#endif // RDO_SIMULATOR_H
