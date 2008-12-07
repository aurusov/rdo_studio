#ifndef RDOTRACEROPERATION_H
#define RDOTRACEROPERATION_H
#pragma once

#include "rdotracerserie.h"

// ----------------------------------------------------------------------------
// ---------- RDOTracerOperationBase
// ----------------------------------------------------------------------------
class RDOTracerPattern;

class RDOTracerOperationBase: public RDOTracerSerie
{
protected:
	RDOTracerPattern* pattern;
	std::string Name;
	void incOperationsCount( RDOTracerTimeNow* const time, const int eventIndex );
public:
	RDOTracerOperationBase( RDOTracerPattern* const pat );
	virtual ~RDOTracerOperationBase();

	RDOTracerPattern* getPattern() const { return pattern; };
	//void start( RDOTracerTimeNow* const time, const int eventIndex );
	//void accomplish( RDOTracerTimeNow* const time, const int eventIndex );
	void setName( const std::string& name ) { Name = name; if ( title.empty() ) title = Name; };
	const std::string getName() const { return Name; };
	virtual void getCaptions( std::vector<std::string> &captions, const int val_count ) const;
	virtual void monitorTime( RDOTracerTimeNow* const time, const int eventIndex );
};

// ----------------------------------------------------------------------------
// ---------- RDOTracerOperation
// ----------------------------------------------------------------------------
class RDOTracerOperation: public RDOTracerOperationBase
{
public:
	RDOTracerOperation( RDOTracerPattern* const pat );
	virtual ~RDOTracerOperation();

	void start( RDOTracerTimeNow* const time, const int eventIndex );
	void accomplish( RDOTracerTimeNow* const time, const int eventIndex );
};

// ----------------------------------------------------------------------------
// ---------- RDOTracerEvent
// ----------------------------------------------------------------------------
class RDOTracerEvent: public RDOTracerOperationBase
{
public:
	RDOTracerEvent( RDOTracerPattern* const pat );
	virtual ~RDOTracerEvent();
	
	void occurs( RDOTracerTimeNow* const time, const int eventIndex );
	virtual void monitorTime( RDOTracerTimeNow* const time, const int eventIndex );
};

#endif // RDOTRACEROPERATION_H