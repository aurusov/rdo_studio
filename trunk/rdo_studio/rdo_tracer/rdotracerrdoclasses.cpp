#include "stdafx.h"
#include "rdotracerrdoclasses.h"
#include "rdotracertrace.h"

// ----------------------------------------------------------------------------
// ---------- RDOTracerResParamInfo
// ----------------------------------------------------------------------------
RDOTracerResParamInfo::RDOTracerResParamInfo( const RDOTracerResParamType type ) :
	paramType( type ),
	enumValues( NULL )
{
	if ( paramType == RDOPT_ENUMERATIVE )
		enumValues = new RDOStringVector();
}

RDOTracerResParamInfo::~RDOTracerResParamInfo()
{
	if ( enumValues )
		delete enumValues;
};

int RDOTracerResParamInfo::addEnumValue( const string& value )
{
	if ( !enumValues )
		return -1;
	enumValues->push_back( value );
	return enumValues->size() - 1;
}

static string nullStr = "";

string RDOTracerResParamInfo::getEnumValue( const int index ) const
{
	if ( !enumValues )
		return nullStr;
	if ( index >= enumValues->size() || index < 0 )
		return nullStr;
	return enumValues->at( index );
}

// ----------------------------------------------------------------------------
// ---------- RDOTracerResType
// ----------------------------------------------------------------------------
RDOTracerResType::RDOTracerResType( const RDOTracerResTypeKind kind ) :
	RDOTracerTreeItem(),
	restypeKind( kind )
{
}

RDOTracerResType::~RDOTracerResType()
{
	int count = paramsInfo.size();
	for (int i = 0; i < count; i++ ) {
		delete paramsInfo.at( i );
	}
};

int RDOTracerResType::addParamInfo( RDOTracerResParamInfo* const value )
{
	paramsInfo.push_back( value );
	return paramsInfo.size() - 1;
}

RDOTracerResParamInfo* RDOTracerResType::getParamInfo( const int index ) const
{
	if ( index >= paramsInfo.size() || index < 0 )
		return NULL;
	return paramsInfo.at( index );
}

// ----------------------------------------------------------------------------
// ---------- RDOTracerValue
// ----------------------------------------------------------------------------
RDOTracerValue::RDOTracerValue( RDOTracerTimeNow* const timenow )
	: modeltime( timenow ),
	eventIndex( timenow->eventCount )
{
}

RDOTracerValue::~RDOTracerValue()
{
}

// ----------------------------------------------------------------------------
// ---------- RDOTracerSerie
// ----------------------------------------------------------------------------
RDOTracerSerie::RDOTracerSerie( RDOTracerSerieKind _serieKind ) :
	RDOTracerTreeItem( true ),
	serieKind( _serieKind ),
	minValue( 0 ),
	maxValue( 0 )
{
}

RDOTracerSerie::~RDOTracerSerie()
{
	int count = values.size();
	for (int i = 0; i < count; i++ ) {
		delete values.at( i );
	}
};

string RDOTracerSerie::getTitle() const
{
	return title;
}

void RDOTracerSerie::setTitle( const string& value )
{
	if ( title != value )
		title = value;
}

int RDOTracerSerie::addValue( RDOTracerValue* const value )
{
	values.push_back( value );
	if ( value->value < minValue ) minValue = value->value;
	if ( value->value > maxValue ) maxValue = value->value;
	return values.size() - 1;
}

RDOTracerValue* RDOTracerSerie::getValue( const int index ) const
{
	if ( index >= values.size() || index < 0 )
		return NULL;
	return values.at( index );
}

int RDOTracerSerie::getValueCount() const
{
	return values.size();
}

RDOTracerValue* RDOTracerSerie::getLastValue() const
{
	if ( !values.size() )
		return NULL;
	return values.at( values.size() - 1 );
}

void RDOTracerSerie::drawSerie( CDC &dc, CRect &rect )
{
	int oldBkMode = dc.SetBkMode( TRANSPARENT );
	CPen penBlack;
	penBlack.CreatePen( PS_SOLID, 0, RGB( 255, 0, 0 ) );
	CPen* pOldPen = dc.SelectObject( &penBlack );
	double timerange = trace.getMaxTime()->time;
	long double kx, ky;
	if ( timerange )
		kx = rect.Width() / timerange;
	else
		kx = 0;
	if ( maxValue != minValue )
		ky = rect.Height() / ( maxValue - minValue );
	else
		ky = 0;
	int count = values.size();
	int prevx = rect.left + kx * values.at( 0 )->modeltime->time;
	int prevy = rect.bottom - ky * values.at( 0 )->value;
	dc.MoveTo( prevx, prevy );
	int x, y;
	RDOTracerValue* val = NULL;
	for ( int i = 1; i < count; i++ ) {
		val = values.at( i );
		y = rect.bottom;
		y -= min( ky * val->value, rect.Height() );
		x = rect.left;
		x += min ( kx * val->modeltime->time, rect.Width() );
		dc.LineTo( x, prevy );
		dc.LineTo( x, y );
		prevx = x;
		prevy = y;
	}
	if ( !( serieKind == RDOST_RESPARAM && ((RDOTracerResParam*)this)->getResource()->isErased() ) && x < rect.right ) {
		dc.LineTo( rect.right, y );
	}
	dc.SelectObject( pOldPen );
	dc.SetBkMode( oldBkMode );
}

// ----------------------------------------------------------------------------
// ---------- RDOTracerResParam
// ----------------------------------------------------------------------------
RDOTracerResParam::RDOTracerResParam( RDOTracerResource* const res ) :
	RDOTracerSerie( RDOST_RESPARAM ),
	resource( res )
{
}

RDOTracerResParam::~RDOTracerResParam()
{
}

RDOTracerResParamInfo* RDOTracerResParam::getParamInfo() const
{
	int index = resource->getParamIndex( this );
	if ( index != -1 )
		return resource->getType()->getParamInfo( index );
	else
		return NULL;
}

// ----------------------------------------------------------------------------
// ---------- RDOTracerResource
// ----------------------------------------------------------------------------
RDOTracerResource::RDOTracerResource( RDOTracerResType* const type, string& name ) :
	RDOTracerTreeItem(),
	erased( false ),
	resType( type ),
	Name( name ),
	id( 0 )
{
	int count = resType->getParamsCount();
	for ( int i = 0; i < count; i++ ) {
		addParam( new RDOTracerResParam( this ) );
	}
}

RDOTracerResource::~RDOTracerResource()
{
	int count = params.size();
	for ( int i = 0; i < count; i++ ) {
		delete params.at( i );
	}
}

int RDOTracerResource::addParam( RDOTracerResParam* const value )
{
	value->setTitle( Name + "." + resType->getParamInfo( params.size() )->Name );
	params.push_back( value );
	return params.size() - 1;
}

RDOTracerResParam* RDOTracerResource::getParam( const int index ) const
{
	if ( index >= params.size() || index < 0 )
		return NULL;
	return params.at( index );
}

int RDOTracerResource::getParamIndex( const RDOTracerResParam* const param ) const
{
	int count = params.size();
	for ( int i = 0; i < count; i++ ) {
		if ( params.at( i ) == param )
			return i;
	}
	return -1;
}

void RDOTracerResource::setParams( string& line, RDOTracerTimeNow* const time, const bool erasing )
{
	int count = params.size();
	for ( int i = 0; i < count; i++ ) {
		RDOTracerValue* prevval = params.at( i )->getLastValue();
		double newval = erasing ? prevval->value : atof( trace.getNextValue( line ).c_str() );
		if ( !prevval || erasing || prevval->value != newval ) {
			RDOTracerValue* newvalue = new RDOTracerValue( time );
			newvalue->value = newval;
			params.at( i )->addValue( newvalue );
		}
	}
}

void RDOTracerResource::setErased( const bool value )
{
	if ( erased != value )
		erased = value;
}

// ----------------------------------------------------------------------------
// ---------- RDOTracerPattern
// ----------------------------------------------------------------------------
RDOTracerPattern::RDOTracerPattern( const RDOTracerPatternKind kind ) :
	RDOTracerTreeItem(),
	patKind( kind )
{
}

RDOTracerPattern::~RDOTracerPattern()
{
}

// ----------------------------------------------------------------------------
// ---------- RDOTracerOperation
// ----------------------------------------------------------------------------
RDOTracerOperation::RDOTracerOperation( RDOTracerPattern* const pat )
	: RDOTracerSerie( RDOST_OPERATION ),
	pattern( pat )
{
}

RDOTracerOperation::~RDOTracerOperation()
{
}

void RDOTracerOperation::start( RDOTracerTimeNow* const time )
{
	RDOTracerValue* newval = new RDOTracerValue( time );
	RDOTracerValue* prevval = getLastValue();
	newval->value = prevval ? prevval->value + 1 : 1;
	addValue( newval );
}

void RDOTracerOperation::accomplish( RDOTracerTimeNow* const time )
{
	RDOTracerValue* newval = new RDOTracerValue( time );
	newval->value = getLastValue()->value - 1;
	addValue( newval );
}

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
