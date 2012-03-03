#include "stdafx.h"
#include "rdotracerresult.h"
#include "rdotracervalues.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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

void RDOTracerResult::getCaptions( std::vector<std::string> &captions, const int val_count ) const
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

void RDOTracerResult::setValue( std::string& line, RDOTracerTimeNow* const time, const int eventIndex )
{
	RDOTracerValue* newvalue = new RDOTracerValue( time, eventIndex );
	double newval;
	rdo::trim( line );
	if ( resultKind != RDORK_WATCHSTATE )
		newval = atof( line.c_str() );
	else
		newval = ( line == "TRUE" ) ? 1 : 0;
	newvalue->value = newval;
	addValue( newvalue );
}