#ifndef RDOTRACEROPERATION_H
#define RDOTRACEROPERATION_H
#pragma once

#include "rdotracerserie.h"

// ----------------------------------------------------------------------------
// ---------- RDOTracerOperation
// ----------------------------------------------------------------------------
class RDOTracerPattern;

class RDOTracerOperation: public RDOTracerSerie
{
protected:
	RDOTracerPattern* pattern;
	string Name;
public:
	RDOTracerOperation( RDOTracerPattern* const pat );
	virtual ~RDOTracerOperation();

	RDOTracerPattern* getPattern() const { return pattern; };
	void start( RDOTracerTimeNow* const time, const int eventIndex );
	void accomplish( RDOTracerTimeNow* const time, const int eventIndex );
	void setName( const string& name ) { Name = name; if ( title.empty() ) title = Name; };
	const string getName() const { return Name; };
};

#endif // RDOTRACEROPERATION_H