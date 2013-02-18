/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdotraceroperation.h
  \author    Захаров Павел
  \date      11.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_TRACER_RDOTRACEROPERATION_H_
#define _RDO_STUDIO_TRACER_RDOTRACEROPERATION_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/rdo_tracer/rdotracerserie.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- TracerOperationBase
// --------------------------------------------------------------------------------
class TracerPattern;

class TracerOperationBase: public TracerSerie
{
public:
	TracerOperationBase(TracerPattern* const pat);
	virtual ~TracerOperationBase();

	TracerPattern* getPattern() const
	{
		return pattern;
	}

	//void start( TracerTimeNow* const time, const int eventIndex );
	//void accomplish( TracerTimeNow* const time, const int eventIndex );
	void setName( CREF(tstring) name )
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
	virtual void monitorTime(TracerTimeNow* const time, const int eventIndex);

protected:
	TracerPattern* pattern;
	tstring Name;
	void incOperationsCount(TracerTimeNow* const time, const int eventIndex);
};

// --------------------------------------------------------------------------------
// -------------------- TracerOperation
// --------------------------------------------------------------------------------
class TracerOperation: public TracerOperationBase
{
public:
	TracerOperation(TracerPattern* const pat);
	virtual ~TracerOperation();

	void start(TracerTimeNow* const time, const int eventIndex);
	void accomplish(TracerTimeNow* const time, const int eventIndex);
};

// --------------------------------------------------------------------------------
// -------------------- TracerEvent
// --------------------------------------------------------------------------------
class TracerEvent: public TracerOperationBase
{
public:
	TracerEvent(TracerPattern* const pat);
	virtual ~TracerEvent();

	void occurs(TracerTimeNow* const time, const int eventIndex);
	virtual void monitorTime(TracerTimeNow* const time, const int eventIndex);
};

#endif // _RDO_STUDIO_TRACER_RDOTRACEROPERATION_H_
