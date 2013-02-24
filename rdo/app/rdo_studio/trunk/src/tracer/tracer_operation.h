/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      tracer_operation.h
  \author    Захаров Павел
  \date      11.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_TRACER_OPERATION_H_
#define _RDO_STUDIO_TRACER_OPERATION_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/tracer/tracer_serie.h"
#include "app/rdo_studio/src/tracer/tracer_pattern.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace tracer {

// --------------------------------------------------------------------------------
// -------------------- OperationBase
// --------------------------------------------------------------------------------
class OperationBase: public Serie
{
DECLARE_FACTORY(OperationBase)
public:
	CREF(QString) getName() const;

	LPPattern getPattern() const;

	virtual void getCaptions(std::vector<tstring>& captions, const int valueCount) const;
	virtual void monitorTime(Time* const pTime, const int eventIndex);

protected:
	OperationBase(CREF(LPPattern) pPattern, CREF(QString) name);
	virtual ~OperationBase();

	void incOperationsCount(Time* const pTime, const int eventIndex);

private:
	LPPattern  m_pPattern;
	QString    m_name;
};

typedef  rdo::intrusive_ptr<OperationBase>  LPOperationBase;

// --------------------------------------------------------------------------------
// -------------------- Operation
// --------------------------------------------------------------------------------
class Operation: public OperationBase
{
DECLARE_FACTORY(Operation)
public:
	void start(Time* const pTime, const int eventIndex);
	void accomplish(Time* const pTime, const int eventIndex);

private:
	Operation(CREF(LPPattern) pPattern, CREF(QString) name);
	virtual ~Operation();
};

typedef  rdo::intrusive_ptr<Operation>  LPOperation;

// --------------------------------------------------------------------------------
// -------------------- Event
// --------------------------------------------------------------------------------
class Event: public OperationBase
{
DECLARE_FACTORY(Event)
public:
	void occurs(Time* const pTime, const int eventIndex);
	virtual void monitorTime(Time* const pTime, const int eventIndex);

private:
	Event(CREF(LPPattern) pPattern, CREF(QString) name);
	virtual ~Event();
};

typedef  rdo::intrusive_ptr<Event>  LPEvent;

}}} // namespace rdo::gui::tracer

#endif // _RDO_STUDIO_TRACER_OPERATION_H_
