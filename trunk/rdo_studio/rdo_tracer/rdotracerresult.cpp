#include "stdafx.h"
#include "rdotracerresult.h"
#include "rdotracervalues.h"

using namespace std;

// ----------------------------------------------------------------------------
// ---------- RDOTracerResult
// ----------------------------------------------------------------------------
RDOTracerResult::RDOTracerResult( const RDOTracerResultKind kind )
	: RDOTracerSerie( RDOST_RESULT ),
	resultKind( kind ),
	id( 0 )
{
}

RDOTracerResult::~RDOTracerResult()
{
}

void RDOTracerResult::getCaptions( vector<string> &captions, const int val_count ) const
{
	switch( resultKind ) {
		case RDORK_WATCHQUANT: {
			RDOTracerSerie::getCaptionsInt( captions, val_count );
			break;
		}
		case RDORK_WATCHSTATE: {
			RDOTracerSerie::getCaptionsBool( captions, val_count );
			break;
		}
		case RDORK_WATCHPAR:
		case RDORK_WATCHVALUE: {
			RDOTracerSerie::getCaptionsDouble( captions, val_count );
			break;
		}
	}
}

void RDOTracerResult::setValue( string& line, RDOTracerTimeNow* const time, const int eventIndex )
{
	RDOTracerValue* newvalue = new RDOTracerValue( time, eventIndex );
	double newval;
	trim( line );
	if ( resultKind != RDORK_WATCHSTATE )
		newval = atof( line.c_str() );
	else
		newval = ( line == "TRUE" ) ? 1 : 0;
	newvalue->value = newval;
	addValue( newvalue );
}