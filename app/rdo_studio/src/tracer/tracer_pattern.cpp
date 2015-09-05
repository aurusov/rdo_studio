// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/tracer_pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/tracer/tracer_pattern.h"
// --------------------------------------------------------------------------------

using namespace rdo::gui::tracer;

Pattern::Pattern(Kind kind)
    : ChartTreeItem()
    , m_kind(kind)
{}

Pattern::~Pattern()
{}

const QString& Pattern::getName() const
{
    return m_name;
}

void Pattern::setName(const QString& name)
{
    m_name = name;
}

Pattern::Kind Pattern::getKind() const
{
    return m_kind;
}
