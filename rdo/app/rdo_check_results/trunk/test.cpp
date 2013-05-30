/*!
  \copyright (c) RDO-Team, 2011
  \file      /app/rdo_check_results/test.cpp
  \author    Пройдаков Евгений (lord.tiran@gmail.com)
  \date      10.05.2009
  \brief     Тест приложения rdo_studio_console
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include <stdlib.h>

#define BOOST_TEST_MODULE RDOStudioConsoleAppTest
#include <boost/test/included/unit_test.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdocommon.h"
#include "app/rdo_studio_console/test/controller_console_options_config.h"
// --------------------------------------------------------------------------------

const tstring RDO_TEST_APP_PATH_STRING = RDO_TEST_APP_PATH;
const tstring RDO_TEST_APP_STRING = RDO_TEST_APP;
const tstring INPUT_COMMAND = " --" + INPUT_MODEL_COMMAND + " ";

int run(CREF(tstring) model)
{
	tstring query = RDO_TEST_APP_STRING + INPUT_COMMAND + RDO_TEST_APP_PATH + model + "/" + model + ".rdox";
	std::cout << "test model : " << query << std::endl;
	return system(query.c_str());
}

BOOST_AUTO_TEST_SUITE(RDOStudioConsoleTest)

BOOST_AUTO_TEST_CASE(RDOStudioConsoleTestCheckInputData)
{
	if(RDO_TEST_APP_STRING == "NULL" || RDO_TEST_APP_PATH == "NULL") 
	{
		BOOST_ERROR("Invalid input data");
		exit(1);
	}
}

BOOST_AUTO_TEST_CASE(RDOStudioConsoleModelSimpleQS)
{
	BOOST_CHECK(!run("simple_qs"));
}

BOOST_AUTO_TEST_CASE(RDOStudioConsoleModelMultichannelQS)
{
	BOOST_CHECK(!run("multichannel_qs"));
}

BOOST_AUTO_TEST_CASE(RDOStudioConsoleModelArray)
{
	BOOST_CHECK(!run("array"));
}

BOOST_AUTO_TEST_CASE(RDOStudioConsoleModelCreateRes)
{
	BOOST_CHECK(!run("create_res"));
}

BOOST_AUTO_TEST_CASE(RDOStudioConsoleModelFmsEvent)
{
	BOOST_CHECK(!run("fms_event"));
}

BOOST_AUTO_TEST_CASE(RDOStudioConsoleModelFmsPlaning)
{
	BOOST_CHECK(!run("fms_planing"));
}

BOOST_AUTO_TEST_CASE(RDOStudioConsoleModelHeidel)
{
	BOOST_CHECK(!run("heidel"));
}
/*
BOOST_AUTO_TEST_CASE(RDOStudioConsoleModelEventQS)
{
	BOOST_CHECK(!run("event_qs"));
}

BOOST_AUTO_TEST_CASE(RDOStudioConsoleModelPoliclinic)
{
	BOOST_CHECK(!run("policlinic"));
}
*/
BOOST_AUTO_TEST_SUITE_END() // RDOStudioConsoleTest
