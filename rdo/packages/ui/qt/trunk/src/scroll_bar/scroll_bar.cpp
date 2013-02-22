/*!
  \copyright (c) RDO-Team, 2013
  \file      scroll_bar.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      22.02.2013
  \brief     Информация о скролбаре
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "ui/qt/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "ui/qt/headers/scroll_bar/scroll_bar.h"
// --------------------------------------------------------------------------------

using namespace rdo::gui;

ScrollMetric::ScrollMetric()
	: position(0)
	, posMax  (0)
	, pageSize(0)
{}

rbool ScrollMetric::applyInc(rsint delta)
{
	if (delta == 0)
	{
		return false;
	}

	if (position + delta < 0 || position + delta > posMax)
	{
		return false;
	}

	position += delta;
	return true;
}
