/*!
  \copyright (c) RDO-Team, 2013
  \file      main.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      28.02.2013
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/application.h"
// --------------------------------------------------------------------------------

int main(int argc, char** argv)
{
	Application app(argc, argv);

	return app.exec();
}
