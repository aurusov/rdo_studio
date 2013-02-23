/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdotracerresult.h
  \author    Захаров Павел
  \date      11.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_TRACER_RDOTRACERRESULT_H_
#define _RDO_STUDIO_TRACER_RDOTRACERRESULT_H_

// ----------------------------------------------------------------------- INCLUDES
#include <vector>
#include <string>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/rdo_tracer/rdotracerserie.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- TracerResult
// --------------------------------------------------------------------------------
enum TracerResultKind
{
	RDORK_UNDEFINED  = ~0,
	RDORK_WATCHPAR   = 0,
	RDORK_WATCHSTATE,
	RDORK_WATCHQUANT,
	RDORK_WATCHVALUE
};

class TracerResult: public TracerSerie
{
DECLARE_FACTORY(TracerResult)
public:
	int id;
	TracerResultKind getResultKind() const
	{
		return resultKind;
	}
	void setName(CREF(tstring) name)
	{
		Name = name;
		if (title.empty())
		{
			title = Name;
		}
	}
	const tstring getName() const
	{
		return Name;
	}

	virtual void getCaptions(std::vector<tstring> &captions, const int val_count) const;
	void setValue(tstring& line, TracerTimeNow* const time, const int eventIndex);

protected:
	TracerResultKind resultKind;
	tstring Name;

private:
	TracerResult(const TracerResultKind kind);
	virtual ~TracerResult();
};

typedef  rdo::intrusive_ptr<TracerResult>  LPTracerResult;

#endif // _RDO_STUDIO_TRACER_RDOTRACERRESULT_H_
