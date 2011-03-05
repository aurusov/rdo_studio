/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdo_activity.h
 * author   : Урусов Андрей, Лущан Дмитрий
 * date     : 13.04.2008
 * bref     : Описание базового класса для событий и активностей -- RDOActivity
 * indent   : 4T
 */

#ifndef _RDO_ACTIVITY_H_
#define _RDO_ACTIVITY_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/rdostream.h"
#include "rdo_lib/rdo_runtime/rdo.h"
#include "rdo_lib/rdo_runtime/rdo_resource.h"
#include "rdo_lib/rdo_runtime/rdo_model_i.h"
#include "rdo_lib/rdo_runtime/rdo_activity_i.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOActivity
// ----------------------------------------------------------------------------
class RDOActivity: public RDOTraceableObject, public IActivity, public IActivityTrace, CAST_TO_UNKNOWN
{
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDOTraceableObject)
	QUERY_INTERFACE(IActivity)
	QUERY_INTERFACE(IActivityTrace)
QUERY_INTERFACE_END

protected:
	RDOActivity( RDORuntimeParent* parent, bool trace, CREF(tstring) name ):
		RDOTraceableObject( trace ),
		m_oprName( name )
	{}
	virtual ~RDOActivity() {}

	std::string               m_oprName;
	std::list< RDOResource* > m_relevantResources; // Список релевантных ресурсов
	std::vector< int >        m_relResID;          // Содержит список id ресурсов, которые стали релевантными образцу
	std::vector<LPRDOCalc>    m_paramsCalcs;

	void setPatternParameters( RDOSimulator* sim );
	void getRelevantResources( RDOSimulator* sim, std::list< RDOResource* >& rel_res_list );
	void incrementRelevantResourceReference( RDOSimulator* sim );
	void decrementRelevantResourceReference( RDOSimulator* sim );

	void updateRelRes( RDOSimulator* sim )
	{
		getRelevantResources( sim, m_relevantResources );
	}
	void updateConvertStatus( RDOSimulator* sim, const std::vector< RDOResource::ConvertStatus >& status_list );

private:
	DECLARE_IActivity;
	DECLARE_IActivityTrace;
};

// ----------------------------------------------------------------------------
// ---------- RDOActivityPattern
// ----------------------------------------------------------------------------
template< class T >
class RDOActivityPattern: public RDOActivity, public IModelStructure, public IActivityPatternTrace
{
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDOActivity)
	QUERY_INTERFACE(IModelStructure)
	QUERY_INTERFACE(IActivityPatternTrace)
QUERY_INTERFACE_END

protected:
	RDOActivityPattern( RDORuntimeParent* parent, T* pattern, bool trace, CREF(tstring) name ):
		RDOActivity( parent, trace, name ),
		m_pattern( pattern )
	{
	}
	virtual ~RDOActivityPattern() {}

	T* m_pattern;

private:
	void writeModelStructure(REF(std::ostream) stream) const
	{
		stream << m_oprName << " " << tracePatternId() << std::endl;
	}
	CREF(tstring) tracePatternId() const
	{
		return m_pattern->traceId();
	}
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif //! _RDO_ACTIVITY_H_
