#ifndef RDOTRACERRESULT_H
#define RDOTRACERRESULT_H
#pragma once

#include "rdotracerserie.h"

// ----------------------------------------------------------------------------
// ---------- RDOTracerResult
// ----------------------------------------------------------------------------
enum RDOTracerResultKind {
	RDORK_WATCHPAR = 0,
	RDORK_WATCHSTATE,
	RDORK_WATCHQUANT,
	RDORK_WATCHVALUE
};

class RDOTracerResult: public RDOTracerSerie
{
protected:
	RDOTracerResultKind resultKind;
	std::string Name;
public:
	RDOTracerResult( const RDOTracerResultKind kind );
	virtual ~RDOTracerResult();

	int id;
	RDOTracerResultKind getResultKind() const { return resultKind; };
	void setName( const std::string& name ) { Name = name; if ( title.empty() ) title = Name; };
	const std::string getName() const { return Name; };
};

#endif // RDOTRACERRESULT_H