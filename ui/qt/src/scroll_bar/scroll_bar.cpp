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

bool ScrollMetric::applyInc(int delta)
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
