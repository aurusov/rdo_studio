/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdo_activity.cpp
 * author   : Урусов Андрей, Лущан Дмитрий
 * date     : 13.04.2008
 * bref     : Описание базового класса для событий и активностей -- RDOActivity
 * indent   : 4T
 */

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/pch.h"
#include "rdo_lib/rdo_runtime/rdo_activity.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
#include "rdo_lib/rdo_runtime/rdocalc.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOActivity
// ----------------------------------------------------------------------------
void RDOActivity::addParamCalc(CREF(LPRDOCalc) pCalc)
{
	m_paramsCalcs.push_back(pCalc);
}

int RDOActivity::getResByRelRes(ruint rel_res_id) const
{
	if (m_relResID.size() <= rel_res_id)
	{
		return 0;
	}
	return m_relResID.at(rel_res_id);
}

void RDOActivity::setRelRes(ruint rel_res_id, ruint res_id)
{
	if ( m_relResID.size() <= rel_res_id )
	{
		m_relResID.resize( rel_res_id + 1 );
	}
	m_relResID[rel_res_id] = res_id; 
}

void RDOActivity::setPatternParameters(RDOSimulator *sim) 
{
	RDORuntime* runtime = static_cast<RDORuntime*>(sim);
	int size = m_paramsCalcs.size();
	for ( int i = 0; i < size; i++ )
	{
		m_paramsCalcs.at(i)->calcValue( runtime );
	}
}

void RDOActivity::getRelevantResources( RDOSimulator* sim, std::list< RDOResource* >& rel_res_list )
{
	rel_res_list.clear();
	int size = m_relResID.size();
	for ( int i = 0; i < size; i++ )
	{
		rel_res_list.push_back( static_cast<RDORuntime*>(sim)->getResourceByID( m_relResID[i] ) );
	}
}

void RDOActivity::updateConvertStatus( RDOSimulator* sim, const std::vector< RDOResource::ConvertStatus >& status_list )
{
	updateRelRes( sim );
	int i = 0;
	std::list< RDOResource* >::iterator it = m_relevantResources.begin();
	while (it != m_relevantResources.end())
	{
		RDOResource* res = *it;
		if ( res )
		{
			res->setState( status_list[i] );
			switch ( status_list[i] )
			{
				case RDOResource::CS_Create:
				{
					res->makeTemporary( true );
					break;
				}
			}
		}
		i++;
		it++;
	}
}

tstring RDOActivity::traceResourcesList(char prefix, PTR(RDOSimulatorTrace) sim)
{
	tstring res;
	for ( std::list< RDOResource* >::const_iterator i = m_relevantResources.begin(); i != m_relevantResources.end(); i++ )
	{
		if ( *i )
		{
			res += (*i)->traceResourceState( prefix, sim );
		}
	}
	return res;
}

tstring RDOActivity::traceResourcesListNumbers(PTR(RDOSimulatorTrace) sim, rbool show_create_index)
{
	std::ostringstream res;
	res << m_relevantResources.size() << " ";
	for ( std::list< RDOResource* >::const_iterator i = m_relevantResources.begin(); i != m_relevantResources.end(); i++ )
	{
#ifdef RDOSIM_COMPATIBLE
		if ( *i && (show_create_index || (!show_create_index && (*i)->getState() != RDOResource::CS_Create)) )
		{
#else
		if ( *i )
		{
#endif
			res << " " << (*i)->traceId();
		}
		else
		{
			res << " 0";
		}
	}
	return res.str();
}

void RDOActivity::incrementRelevantResourceReference( RDOSimulator* sim )
{
	for ( unsigned int i = 0; i < m_relResID.size(); i++ )
	{
		RDOResource* res = static_cast<RDORuntime*>(sim)->getResourceByID( m_relResID.at(i) );
		if ( res && (res->getState() == RDOResource::CS_Keep || res->getState() == RDOResource::CS_Create || res->getState() == RDOResource::CS_Erase ) ) res->incRef();
	}
}

void RDOActivity::decrementRelevantResourceReference( RDOSimulator* sim )
{
	for ( unsigned int i = 0; i < m_relResID.size(); i++ )
	{
		RDOResource* res = static_cast<RDORuntime*>(sim)->getResourceByID( m_relResID.at(i) );
		if ( res && (res->getState() == RDOResource::CS_Keep || res->getState() == RDOResource::CS_Create || res->getState() == RDOResource::CS_Erase ) ) res->decRef();
	}
}

CLOSE_RDO_RUNTIME_NAMESPACE
