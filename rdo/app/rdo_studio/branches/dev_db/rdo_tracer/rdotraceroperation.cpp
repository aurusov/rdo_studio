/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdotraceroperation.cpp
  \author    Захаров Павел
  \date      11.03.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/rdo_tracer/rdotraceroperation.h"
#include "app/rdo_studio/rdo_tracer/rdotracervalues.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// -------------------- TracerOperationBase
// --------------------------------------------------------------------------------
TracerOperationBase::TracerOperationBase( TracerPattern* const pat )
	: TracerSerie( RDOST_OPERATION ),
	pattern( pat )
{
}

TracerOperationBase::~TracerOperationBase()
{
}

void TracerOperationBase::incOperationsCount( TracerTimeNow* const time, const int eventIndex )
{
	TracerValue* newval = new TracerValue( time, eventIndex );
	TracerValue* prevval;
	getLastValue( prevval );
	newval->value = prevval ? prevval->value + 1 : 1;
	addValue( newval );
}

void TracerOperationBase::getCaptions( std::vector<tstring> &captions, const int val_count ) const
{
	TracerSerie::getCaptionsInt( captions, val_count );
}

void TracerOperationBase::monitorTime( TracerTimeNow* const time, const int eventIndex )
{
	UNUSED(time      );
	UNUSED(eventIndex);
}

// --------------------------------------------------------------------------------
// -------------------- TracerOperation
// --------------------------------------------------------------------------------
TracerOperation::TracerOperation( TracerPattern* const pat ) :
	TracerOperationBase( pat )
{
}

TracerOperation::~TracerOperation()
{
}

void TracerOperation::start( TracerTimeNow* const time, const int eventIndex )
{
	TracerOperationBase::incOperationsCount( time, eventIndex );
}

void TracerOperation::accomplish( TracerTimeNow* const time, const int eventIndex )
{
	TracerValue* lastval;
	getLastValue( lastval );
	if ( lastval ) {
		TracerValue* newval = new TracerValue( time, eventIndex );
		newval->value = lastval->value - 1;
		addValue( newval );
	}
}

// --------------------------------------------------------------------------------
// -------------------- TracerIrregularEvent
// --------------------------------------------------------------------------------
TracerEvent::TracerEvent( TracerPattern* const pat ) :
	TracerOperationBase( pat )
{
}

TracerEvent::~TracerEvent()
{
}

void TracerEvent::occurs( TracerTimeNow* const time, const int eventIndex )
{
	TracerOperationBase::incOperationsCount( time, eventIndex );
}

void TracerEvent::monitorTime( TracerTimeNow* const time, const int eventIndex )
{
	TracerValue* prevval;
	getLastValue( prevval );
	TracerValue* newval = NULL;
	if ( prevval && prevval->value != 0 ) {
		if ( *prevval->getModelTime() != *time ) {
			newval = new TracerValue( prevval->getModelTime(), prevval->getModelTime()->eventCount );
			newval->value = 0;
		}
	}
	if ( !prevval ) {
		newval = new TracerValue( time, eventIndex );
		newval->value = 0;
	}
	if( newval ) {
		addValue( newval );
	}
}