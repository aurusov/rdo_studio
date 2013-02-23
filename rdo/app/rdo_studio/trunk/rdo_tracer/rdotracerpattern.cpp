/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdotracerpattern.cpp
  \author    Захаров Павел
  \date      11.03.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/rdo_tracer/rdotracerpattern.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

TracerPattern::TracerPattern(Kind kind)
	: ChartTreeItem()
	, m_kind(kind)
{}

TracerPattern::~TracerPattern()
{}

TracerPattern::Kind TracerPattern::getKind() const
{
	return m_kind;
}

CREF(QString) TracerPattern::getName() const
{
	return m_name;
}

void TracerPattern::setName(CREF(QString) name)
{
	m_name = name;
}
