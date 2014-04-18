/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      tracer_result.h
  \author    Захаров Павел
  \date      11.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_TRACER_RESULT_H_
#define _RDO_STUDIO_TRACER_RESULT_H_

// ----------------------------------------------------------------------- INCLUDES
#include <vector>
#include <string>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/tracer/tracer_serie.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace tracer {

class Result: public Serie
{
DECLARE_FACTORY(Result)
public:
	enum Kind
	{
		RK_UNDEFINED  = ~0,
		RK_WATCHPAR   = 0,
		RK_WATCHSTATE,
		RK_WATCHQUANT,
		RK_WATCHVALUE
	};

	const QString& getName() const;
	Kind           getKind() const;
	int            getID  () const;

	virtual void getCaptions(std::vector<std::string>& captions, const int valueCount) const;
	void setValue(std::string& line, Time* const time, const int eventIndex);

private:
	Result(const QString& name, Kind kind, int id);
	virtual ~Result();

	QString  m_name;
	Kind     m_kind;
	int      m_id;
};

typedef  rdo::intrusive_ptr<Result>  LPResult;

}}} // namespace rdo::gui::tracer

#endif // _RDO_STUDIO_TRACER_RESULT_H_
