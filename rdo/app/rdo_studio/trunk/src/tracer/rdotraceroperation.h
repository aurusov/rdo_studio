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
#include "app/rdo_studio/src/tracer/rdotracerserie.h"
#include "app/rdo_studio/src/tracer/rdotracerpattern.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- TracerOperationBase
// --------------------------------------------------------------------------------
class TracerOperationBase: public TracerSerie
{
DECLARE_FACTORY(TracerOperationBase)
public:
	CREF(QString) getName() const;

	LPTracerPattern getPattern() const;

	virtual void getCaptions(std::vector<tstring>& captions, const int valueCount) const;
	virtual void monitorTime(TracerTimeNow* const pTime, const int eventIndex);

protected:
	TracerOperationBase(CREF(LPTracerPattern) pPattern, CREF(QString) name);
	virtual ~TracerOperationBase();

	void incOperationsCount(TracerTimeNow* const pTime, const int eventIndex);

private:
	LPTracerPattern  m_pPattern;
	QString          m_name;
};

typedef  rdo::intrusive_ptr<TracerOperationBase>  LPTracerOperationBase;

// --------------------------------------------------------------------------------
// -------------------- TracerOperation
// --------------------------------------------------------------------------------
class TracerOperation: public TracerOperationBase
{
DECLARE_FACTORY(TracerOperation)
public:
	void start(TracerTimeNow* const pTime, const int eventIndex);
	void accomplish(TracerTimeNow* const pTime, const int eventIndex);

private:
	TracerOperation(CREF(LPTracerPattern) pPattern, CREF(QString) name);
	virtual ~TracerOperation();
};

typedef  rdo::intrusive_ptr<TracerOperation>  LPTracerOperation;

// --------------------------------------------------------------------------------
// -------------------- TracerEvent
// --------------------------------------------------------------------------------
class TracerEvent: public TracerOperationBase
{
DECLARE_FACTORY(TracerEvent)
public:
	void occurs(TracerTimeNow* const pTime, const int eventIndex);
	virtual void monitorTime(TracerTimeNow* const pTime, const int eventIndex);

private:
	TracerEvent(CREF(LPTracerPattern) pPattern, CREF(QString) name);
	virtual ~TracerEvent();
};

typedef  rdo::intrusive_ptr<TracerEvent>  LPTracerEvent;

#endif // _RDO_STUDIO_TRACER_RDOTRACEROPERATION_H_
