#pragma once

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

    const QString& getName() const;
    void           setName(const QString& name);

    Kind getKind() const;

private:
    Pattern(Kind kind);
    virtual ~Pattern();

    QString  m_name;
    Kind     m_kind;
};

typedef  rdo::intrusive_ptr<Pattern>  LPPattern;

}}} // namespace rdo::gui::tracer
