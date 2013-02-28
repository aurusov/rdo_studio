#include "app/rdo_studio/pch/stdpch.h"

int roundDouble(const double val)
{
	if (val >= 0)
	{
		return (val - (int)val < 0.5) ? (int)val : (int)val + 1;
	}
	else
	{
		return (val - (int)val > - 0.5) ? (int)val : (int)val - 1;
	}
}
