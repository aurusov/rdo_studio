#ifndef _LIB_RUNTIME_RESOURCE_H_
#define _LIB_RUNTIME_RESOURCE_H_

// ----------------------------------------------------------------------- INCLUDES
#include <boost/operators.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdotrace.h"
#include "simulator/runtime/rdo_object.h"
#include "simulator/runtime/rdo_value.h"
#include "simulator/runtime/calc/resource/calc_relevant.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

class RDORuntime;
PREDECLARE_OBJECT_INTERFACE(IResourceType);

PREDECLARE_POINTER(RDOResource);
class RDOResource
	: public virtual rdo::counter_reference
	, public RDORuntimeObject
	, public RDOTraceableObject
	, public boost::operators<RDOResource>
{
public:
		enum ConvertStatus
	{
		CS_None = 0,
		CS_Keep,
		CS_Create,
		CS_Erase,
		CS_NonExist,
		CS_NoChange
	};

	typedef  std::vector<RDOValue>      ParamList;
	typedef  ParamList::const_iterator  ParamCIt;

	RDOResource(const LPRDORuntime& pRuntime, const ParamList& paramList, LPIResourceType pResType, std::size_t resID, std::size_t typeID, bool trace, bool temporary, bool isNested = 0);
	RDOResource(const LPRDORuntime& pRuntime, const RDOResource& copy);
	virtual ~RDOResource();

	bool operator== (const RDOResource& other) const;

	ConvertStatus getState() const;
	const RDOValue& getParam(std::size_t index) const;
	bool checkType(std::size_t type) const;
	bool canFree() const;
	const LPIResourceType& getResType() const;
	std::size_t getType() const;
	virtual std::size_t paramsCount() const;
	LPRDOResource clone(const LPRDORuntime& pRuntime) const;
	const ParamList& getParamList() const;

	virtual void appendParams(const ParamCIt& from_begin, const ParamCIt& from_end);
	void setRuntime(const LPRDORuntime& pRuntime);
	void makeTemporary(bool value);
	void setState(ConvertStatus value);
	std::string traceResourceState(char prefix, const LPRDORuntime& pRuntime);
	RDOValue& getParamRaw(std::size_t index);
	void setParam(std::size_t index, const RDOValue& value);
	std::string getTypeId();
	std::string traceParametersValue();
	virtual std::string whoAreYou();
	void incRef();
	void decRef();
	void onDestroy(const LPRDORuntime& pRuntime, const LPRDOEraseResRelCalc& pCalc);
	bool isNested();

protected:
	ParamList m_paramList;
	bool m_temporary;
	bool m_isNested;
	ConvertStatus m_state;

private:
	std::size_t m_type;
	std::size_t m_referenceCount;
	LPIResourceType m_resType;
	std::string m_typeId;

	std::string traceTypeId();
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_RESOURCE_H_
