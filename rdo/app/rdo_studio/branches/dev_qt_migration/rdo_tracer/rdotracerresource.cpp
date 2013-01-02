/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdotracerresource.cpp
  \author    Захаров Павел
  \date      11.03.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/rdo_tracer/rdotracerresource.h"
#include "app/rdo_studio_mfc/rdo_tracer/rdotracerrestype.h"
#include "app/rdo_studio_mfc/rdo_tracer/rdotracervalues.h"
#include "app/rdo_studio_mfc/rdo_tracer/rdotracer.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// -------------------- TracerResParam
// --------------------------------------------------------------------------------
TracerResParam::TracerResParam( TracerResource* const res ) :
	TracerSerie( RDOST_RESPARAM ),
	resource( res )
{
}

TracerResParam::~TracerResParam()
{
}

TracerResParamInfo* TracerResParam::getParamInfo() const
{
	int index = resource->getParamIndex( this );
	if ( index != -1 )
		return resource->getType()->getParamInfo( index );
	else
		return NULL;
}

void TracerResParam::getCaptions( std::vector<tstring> &captions, const int val_count ) const
{
	const_cast<CMutex&>(mutex).Lock();

	switch( getParamInfo()->getParamType() ) {
		case RDOPT_INTEGER: {
			TracerSerie::getCaptionsInt( captions, val_count );
			break;
		}
		case RDOPT_REAL: {
			TracerSerie::getCaptionsDouble( captions, val_count );
			break;
		}
		case RDOPT_ENUMERATIVE:
		case RDOPT_BOOL       :
		case RDOPT_STRING     : {
			TracerSerie::getCaptions( captions, val_count );
			int delta = getParamInfo()->getEnumCount();
			minValue = 0;
			maxValue = delta - 1;
			int real_val_count = val_count;
			if ( delta > real_val_count ) {
				while ( (int)(( delta - 1 ) / ( real_val_count - 1 )) != ( (double)(delta - 1) / ( real_val_count - 1 ) ) )
					real_val_count--;
			} else {
				real_val_count = delta;
			}
			int valo = (int)minValue;
			int valoffset = real_val_count != 1
				? ( delta - 1 ) / ( real_val_count - 1 )
				: 0;
			for ( int i = 0; i < real_val_count; i++ ) {
				captions.push_back( getParamInfo()->getEnumValue( valo ) );
				valo += valoffset;
			}
			break;
		}
	}

	const_cast<CMutex&>(mutex).Unlock();
}

// --------------------------------------------------------------------------------
// -------------------- TracerResource
// --------------------------------------------------------------------------------
TracerResource::TracerResource( TracerResType* const type, CREF(tstring) name ) :
	ChartTreeItem(),
	erased( false ),
	resType( type ),
	Name( name ),
	id( 0 )
{
	mutex.Lock();

	int count = resType->getParamsCount();
	for ( int i = 0; i < count; i++ ) {
		addParam( new TracerResParam( this ) );
	}

	mutex.Unlock();
}

TracerResource::~TracerResource()
{
	mutex.Lock();

	int count = params.size();
	for ( int i = 0; i < count; i++ ) {
		delete params.at( i );
	}

	mutex.Unlock();
}

void TracerResource::addParam( TracerResParam* const value )
{
	mutex.Lock();

	value->setTitle( Name + "." + resType->getParamInfo( params.size() )->Name );
	params.push_back( value );

	mutex.Unlock();
}

TracerResParam* TracerResource::getParam( unsigned int index ) const
{
	if ( index >= params.size() || index < 0 )
		return NULL;
	return params.at( index );
}

int TracerResource::getParamIndex( const TracerResParam* const param ) const
{
	int count = params.size();
	for ( int i = 0; i < count; i++ ) {
		if ( params.at( i ) == param )
			return i;
	}
	return -1;
}

void TracerResource::setParams( tstring& line, TracerTimeNow* const time, const int eventIndex, const rbool erasing )
{
	int count = params.size();
	for ( int i = 0; i < count; i++ ) {
		TracerValue* prevval;
		params.at( i )->getLastValue( prevval );
		tstring nextValue = g_pTracer->getNextValue( line );
		double newval;
		if (erasing)
		{
			newval = prevval->value;
		}
		else
		{
			switch (resType->getParamInfo(i)->getParamType())
			{
			case RDOPT_BOOL:
				newval = nextValue == _T("true")
					? 1.0
					: 0.0;
				break;

			case RDOPT_STRING:
				newval = resType->getParamInfo(i)->addStringValue(nextValue);
				break;

			default:
				newval = atof( nextValue.c_str() );
				break;
			}
		}

		if ( true /*!prevval || erasing || prevval->value != newval*/ ) {
			TracerValue* newvalue = new TracerValue( time, eventIndex );
			newvalue->value = newval;
			params.at( i )->addValue( newvalue );
		}
	}
}

void TracerResource::setErased( const rbool value )
{
	if ( erased != value )
		erased = value;
}
