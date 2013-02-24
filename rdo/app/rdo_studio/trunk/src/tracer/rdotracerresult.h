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
class TracerResult: public TracerSerie
{
DECLARE_FACTORY(TracerResult)
public:
	enum Kind
	{
		RK_UNDEFINED  = ~0,
		RK_WATCHPAR   = 0,
		RK_WATCHSTATE,
		RK_WATCHQUANT,
		RK_WATCHVALUE
	};

	CREF(QString) getName() const;
	Kind          getKind() const;
	int           getID  () const;

	virtual void getCaptions(std::vector<tstring>& captions, const int valueCount) const;
	void setValue(tstring& line, TracerTimeNow* const time, const int eventIndex);

private:
	TracerResult(CREF(QString) name, Kind kind, int id);
	virtual ~TracerResult();

	QString  m_name;
	Kind     m_kind;
	int      m_id;
};

typedef  rdo::intrusive_ptr<TracerResult>  LPTracerResult;

#endif // _RDO_STUDIO_TRACER_RDOTRACERRESULT_H_
