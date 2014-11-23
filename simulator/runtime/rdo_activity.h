/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_activity.h
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      13.04.2008
  \brief     Описание базового класса для событий и активностей -- RDOActivity
  \indent    4T
*/

#ifndef _LIB_RUNTIME_ACTIVITY_H_
#define _LIB_RUNTIME_ACTIVITY_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo.h"
#include "simulator/runtime/rdo_resource.h"
#include "simulator/runtime/rdo_model_i.h"
#include "simulator/runtime/rdo_activity_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/*!
  \class   RDOActivity
  \brief   Активность БЗ
*/
class RDOActivity: public RDOTraceableObject, public IActivity, public IActivityTrace
{
protected:
	RDOActivity(bool trace, const std::string& name);
	virtual ~RDOActivity();

	std::string m_oprName;
	std::list<LPRDOResource> m_relevantResources; // Список релевантных ресурсов
	std::vector<std::size_t> m_relResID; // Содержит список id ресурсов, которые стали релевантными образцу
	std::vector<LPRDOCalc> m_paramsCalcs;

	void setPatternParameters(const LPRDORuntime& pRuntime, const std::vector<LPRDOCalc>& params);
	void setPatternParameters(const LPRDORuntime& pRuntime, const std::vector<RDOValue>& params);
	void getRelevantResources(const LPRDORuntime& pRuntime, std::list<LPRDOResource>& rel_res_list);
	void updateRelRes        (const LPRDORuntime& pRuntime);
	void updateConvertStatus (const LPRDORuntime& pRuntime, const std::vector<RDOResource::ConvertStatus>& status_list);

private:
	DECLARE_IActivity;
	DECLARE_IActivityTrace;
};

/*!
  \class   RDOActivityPattern
  \brief   Образец активности БЗ
*/
template<class T>
class RDOActivityPattern: public RDOActivity, public IModelStructure, public IActivityPatternTrace
{
protected:
	RDOActivityPattern(const rdo::intrusive_ptr<T>& pPattern, bool trace, const std::string& name);
	virtual ~RDOActivityPattern();

	rdo::intrusive_ptr<T> m_pPattern;

private:
	void writeModelStructure(std::ostream& stream) const;
	const std::string& tracePatternId() const;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/rdo_activity-inl.h"

#endif // _LIB_RUNTIME_ACTIVITY_H_
