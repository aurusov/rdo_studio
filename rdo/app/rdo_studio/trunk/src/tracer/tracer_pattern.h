/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      tracer_pattern.h
  \author    Захаров Павел
  \date      11.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_TRACER_PATTERN_H_
#define _RDO_STUDIO_TRACER_PATTERN_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/tracer/chart/chart_tree_item.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace tracer {

class Pattern: public ChartTreeItem
{
DECLARE_FACTORY(Pattern)
public:
	enum Kind
	{
		PK_UNDEFINED = ~0,
		PK_OPERATION = 0,
		PK_EVENT,
		PK_RULE,
		PK_KEYBOARD
	};

	CREF(QString) getName() const;
	void          setName(CREF(QString) name);

	Kind getKind() const;

private:
	Pattern(Kind kind);
	virtual ~Pattern();

	QString  m_name;
	Kind     m_kind;
};

typedef  rdo::intrusive_ptr<Pattern>  LPPattern;

}}} // namespace rdo::gui::tracer

#endif // _RDO_STUDIO_TRACER_PATTERN_H_
