#ifndef RDO_PROCESS_METHOD_ALGORITHM_H
#define RDO_PROCESS_METHOD_ALGORITHM_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <rdoprocess_method.h>
#include <rdoprocess_object_flowchart.h>

// ----------------------------------------------------------------------------
// ---------- RPMethodAlgorithm
// ----------------------------------------------------------------------------
class RPMethodAlgorithm: public rpMethod::RPMethod
{
public:
	RPMethodAlgorithm( RPObject* parent );
	virtual ~RPMethodAlgorithm();
	virtual rp::string getClassName() const { return "RPMethodAlgorithm"; }
	virtual Info getInfo() const;
	void registerObject();
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
};

extern "C" {
	RP_METHOD_DLL rpMethod::RPMethod* registerMethod( RPObjectFactory* _factory, RPProject* _project );
}

#endif // RDO_PROCESS_METHOD_ALGORITHM_H
