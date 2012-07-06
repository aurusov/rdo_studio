#ifndef RDO_PROCESS_METHOD_ALGORITHM_H
#define RDO_PROCESS_METHOD_ALGORITHM_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "rdo_studio/rdo_process/rp_method/rdoprocess_method.h"
#include "rdo_studio/rdo_process/rp_method/rdoprocess_object_flowchart.h"

// ----------------------------------------------------------------------------
// ---------- RPMethodAlgorithm
// ----------------------------------------------------------------------------
class RPMethodAlgorithm: public rpMethod::RPMethod
{
public:
	RPMethodAlgorithm( RPObject* parent );
	virtual ~RPMethodAlgorithm();
	static rpMethod::RPMethod* registerMethod();

	virtual rp::string getVersionDesc() const { return "альфа"; }
	virtual rp::string getDescription() const { return "Тестовый метод, используется исключительно для отладки проги"; }
	virtual rp::string getClassName() const { return "RPMethodAlgorithm"; }
	void registerObject();

	virtual RPObjectFlowChart* makeFlowChart( RPObject* parent );
};

// ----------------------------------------------------------------------------
// ---------- RPAlgorithmFlowChart
// ----------------------------------------------------------------------------
class RPAlgorithmFlowChart: public RPObjectFlowChart
{
friend class RPMethodAlgorithm;
private:
	static RPObject* newObject( RPObject* parent );
public:
	RPAlgorithmFlowChart( RPObject* parent );
	virtual ~RPAlgorithmFlowChart();
	virtual rp::string getClassName() const { return "RPAlgorithmFlowChart"; }
	virtual rpMethod::RPMethod* getMethod();
};

extern RPMethodAlgorithm* algorithm;

#endif // RDO_PROCESS_METHOD_ALGORITHM_H
