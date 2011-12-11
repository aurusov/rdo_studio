/*!
  \copyright (c) RDO-Team, 2011
  \file      main.cpp
  \author    Пройдаков Евгений (lord.tiran@gmail.com)
  \date      10.05.2009
  \brief     Тест приложения rdo_studio_console
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include <list>
#include <fstream>
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

#include <stdlib.h>

#define BOOST_TEST_MODULE RDOStudioConsoleTest
#include <boost/test/included/unit_test.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdocommon.h"
// --------------------------------------------------------------------------------

typedef std::list<tstring> file_data_list;

const tstring RDO_STUDIO_CONSOLE_TEST_PATH_STRING = RDO_STUDIO_CONSOLE_TEST_PATH;
const tstring RDO_STUDIO_CONSOLE_APP_STRING = RDO_STUDIO_CONSOLE_APP;

void read_trace(CREF(tstring) file, REF(file_data_list) list)
{
	std::fstream stream(file.c_str(), std::ios::in);
	if (!stream.is_open()) {
		BOOST_ERROR("Can't open file " + file);
	}
	tstring temp_string;
	bool key = false;
	while(true) {
		std::getline(stream, temp_string);
		if (stream.fail())
			break;
		
		if (temp_string.find("DPS_MM") != -1)
			break;
		
		if (!key)
			key = temp_string.find("$Changes") == -1 ? false : true;
		
		if (key)
			list.push_back(temp_string);
	}
}

void compare_trace(CREF(tstring) etalon_trace, CREF(tstring) trace)
{
	file_data_list etalon_trace_list;
	file_data_list trace_list;
	
	read_trace(etalon_trace, etalon_trace_list);
	read_trace(trace, trace_list);
	
	BOOST_CHECK(etalon_trace_list.size() == trace_list.size());
	BOOST_CHECK(etalon_trace_list == trace_list);
}

void read_result(CREF(tstring) file, REF(file_data_list) list)
{
	std::fstream stream(file.c_str(), std::ios::in);
	if (!stream.is_open()) {
		BOOST_ERROR("Can't open file " + file);
	}
	tstring temp_string;
	bool key = false;
	while(true) {
		std::getline(stream, temp_string);
		if (stream.fail())
			break;
		
		if (key)
			list.push_back(temp_string);

		if (!key)
			key = temp_string.find("$BExpCalcCounter") == -1 ? false : true;
	}
}

void compare_result(CREF(tstring) etalon_result, CREF(tstring) result)
{
	file_data_list etalon_result_list;
	file_data_list result_list;
	
	read_result(etalon_result, etalon_result_list);
	read_result(result, result_list);
	
	BOOST_CHECK(etalon_result_list.size() == result_list.size());
	BOOST_CHECK(etalon_result_list == result_list);
}

void test_model(CREF(tstring) path, CREF(tstring) model_name)
{	
	tstring dir = path;
	if(dir[dir.size() - 1] != '/')
		dir += '/';
	fs::path directory = dir.c_str();
	tstring file = dir + model_name + ".rdox";
	
	if (!fs::exists(directory)) {
		tstring message("file \"" + file + "\" does not exist");
		std::cerr << message.c_str() << std::endl;
		BOOST_REQUIRE(false);
	}
	if(!fs::exists(file)) {
		tstring message("directory \"" + dir + "\" does not exist");
		std::cerr << message.c_str() << std::endl;
		BOOST_REQUIRE(false);
	}
	tstring etalon_mark("_etalon");
	tstring etalon_trace = dir + model_name + etalon_mark + ".trc";
	tstring etalon_result = dir + model_name + etalon_mark + ".pmv";
	
	BOOST_REQUIRE(fs::exists(etalon_trace));
	BOOST_REQUIRE(fs::exists(etalon_result));
	
	tstring simulation_trace = dir + model_name + ".trc";
	tstring simulation_result = dir + model_name + ".pmv";
	
	boost::filesystem::remove(simulation_trace);
	boost::filesystem::remove(simulation_result);
	
	tstring command(RDO_STUDIO_CONSOLE_APP_STRING + tstring(" -i ") + file);
	system(command.c_str());
	
	BOOST_REQUIRE(fs::exists(simulation_trace));
	BOOST_REQUIRE(fs::exists(simulation_result));
	
	compare_trace(etalon_trace, simulation_trace);
	compare_result(etalon_result, simulation_result);
	
	boost::filesystem::remove(simulation_trace);
	boost::filesystem::remove(simulation_result);
}

BOOST_AUTO_TEST_SUITE(RDOStudioConsoleTest)

BOOST_AUTO_TEST_CASE(RDOStudioConsoleTestCheckInputData)
{
	if(RDO_STUDIO_CONSOLE_APP_STRING == "NULL") 
	{
		BOOST_ERROR("Invalid input data");
		exit(1);
	}
}

BOOST_AUTO_TEST_CASE(RDOStudioConsoleModelSimpleQS)
{
	tstring string("simple_qs");
	test_model(RDO_STUDIO_CONSOLE_TEST_PATH_STRING + string, string);
}

BOOST_AUTO_TEST_CASE(RDOStudioConsoleModelMultichannelQS)
{
	tstring string("multichannel_qs");
	test_model(RDO_STUDIO_CONSOLE_TEST_PATH_STRING + string, string);
}

BOOST_AUTO_TEST_CASE(RDOStudioConsoleModelArray)
{
	tstring string("array");
	test_model(RDO_STUDIO_CONSOLE_TEST_PATH_STRING + string, string);
}

BOOST_AUTO_TEST_CASE(RDOStudioConsoleModelCreateRes)
{
	tstring string("create_res");
	test_model(RDO_STUDIO_CONSOLE_TEST_PATH_STRING + string, string);
}

BOOST_AUTO_TEST_CASE(RDOStudioConsoleModelFmsEvent)
{
	tstring string("fms_event");
	test_model(RDO_STUDIO_CONSOLE_TEST_PATH_STRING + string, string);
}

BOOST_AUTO_TEST_CASE(RDOStudioConsoleModelFmsPlaning)
{
	tstring string("fms_planing");
	test_model(RDO_STUDIO_CONSOLE_TEST_PATH_STRING + string, string);
}

BOOST_AUTO_TEST_CASE(RDOStudioConsoleModelHeidel)
{
	tstring string("heidel");
	test_model(RDO_STUDIO_CONSOLE_TEST_PATH_STRING + string, string);
}

/*
BOOST_AUTO_TEST_CASE(RDOStudioConsoleModelEventQS)
{
	tstring string("event_qs");
	test_model(RDO_STUDIO_CONSOLE_TEST_PATH_STRING + string, string);
}

BOOST_AUTO_TEST_CASE(RDOStudioConsoleModelPoliclinic)
{
	tstring string("policlinic");
	test_model(RDO_STUDIO_CONSOLE_TEST_PATH_STRING + string, string);
}
*/
BOOST_AUTO_TEST_SUITE_END() // RDOStudioConsoleTest
