#ifndef RDOTRACERRESULT_H
#define RDOTRACERRESULT_H

#include <vector>
#include <string>
#include "app/rdo_studio_mfc/rdo_tracer/rdotracerserie.h"

// --------------------------------------------------------------------------------
// -------------------- RDOTracerResult
// --------------------------------------------------------------------------------
enum RDOTracerResultKind {
	RDORK_UNDEFINED  = ~0,
	RDORK_WATCHPAR   = 0,
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
	virtual void getCaptions( std::vector<std::string> &captions, const int val_count ) const;
	void setValue( std::string& line, RDOTracerTimeNow* const time, const int eventIndex );
};

#endif // RDOTRACERRESULT_H
