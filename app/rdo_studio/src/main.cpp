// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/locale/rdolocale.h"
#include "app/rdo_studio/src/application.h"
// --------------------------------------------------------------------------------

int main(int argc, char** argv)
{
	rdo::locale::init();

	Application app(argc, argv);
	Q_INIT_RESOURCE(rdo_studio);

	return app.exec();
}
