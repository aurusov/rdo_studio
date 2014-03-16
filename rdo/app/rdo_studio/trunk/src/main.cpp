/*!
  \copyright (c) RDO-Team, 2013
  \file      app/rdo_studio/src/main.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      28.02.2013
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/locale/rdolocale.h"
#include "app/rdo_studio/src/application.h"
// --------------------------------------------------------------------------------

int main(int argc, char** argv)
{
	rdo::loc ale::init();

	Application app(argc, argv);
	Q_INIT_RESOURCE(rdo_studio);

	return app.exec();
}
