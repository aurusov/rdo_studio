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
#include <iostream>
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

#include <stdlib.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdocommon.h"
#include "utils/rdofile.h"
#include "app/rdo_studio_console/test/controller_console_options.h"
// --------------------------------------------------------------------------------

#ifdef COMPILER_VISUAL_STUDIO
#	pragma warning (disable:4127)
#endif // COMPILER_VISUAL_STUDIO

#define TERMINATION_NORMAL	0
#define TERMINATION_ERROR	1

ruint g_state = TERMINATION_NORMAL;

const tstring RDO_STUDIO_CONSOLE_APP_STRING = RDO_STUDIO_CONSOLE_APP;

typedef std::list<tstring> file_data_list;

void TEST_ERROR(CREF(tstring) message)
{ 
	std::cout << "test error : " << message << std::endl;
	exit(TERMINATION_ERROR);
}

void TEST_CHECK(rbool condition, CREF(tstring) error_message)
{
	if(!condition)
	{
		std::cout << "test error : " << error_message << std::endl;
		g_state = TERMINATION_ERROR;
	}
}

void read_trace(CREF(tstring) file, REF(file_data_list) list)
{
	std::fstream stream(file.c_str(), std::ios::in);
	if (!stream.is_open()) 
	{
		TEST_ERROR("Can't open file " + file);
	}
	tstring temp_string;
	rbool key = false;
	while(true) 
	{
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
	
	TEST_CHECK(etalon_trace_list.size() == trace_list.size(), _T("etalon_trace_list size != trace_list size"));
	TEST_CHECK(etalon_trace_list == trace_list, _T("etalon_trace_list != trace_list"));
}

void read_result(CREF(tstring) file, REF(file_data_list) list)
{
	std::fstream stream(file.c_str(), std::ios::in);
	if (!stream.is_open()) 
	{
		TEST_ERROR("Can't open file " + file);
	}
	tstring temp_string;
	rbool key = false;
	while(true) 
	{
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
	
	TEST_CHECK(etalon_result_list.size() == result_list.size(), _T("etalon result list size != result_list size"));
	TEST_CHECK(etalon_result_list == result_list, _T("etalon_result_list != result_list"));
}

void test_model(CREF(tstring) etalon_trace, CREF(tstring) etalon_result, CREF(tstring) simulation_trace, CREF(tstring) simulation_result)
{	
	TEST_CHECK(fs::exists(etalon_trace), _T("etalon_trace not found"));
	TEST_CHECK(fs::exists(etalon_result), _T("etalon_result not found"));
	
	TEST_CHECK(fs::exists(simulation_trace), _T("simulation_trace not found"));
	TEST_CHECK(fs::exists(simulation_result), _T("simulation_result not found"));
	
	compare_trace(etalon_trace, simulation_trace);
	compare_result(etalon_result, simulation_result);
}

int main(int argc, PTR(char) argv[])
{
	RDOControllerConsoleOptions options_controller(argc, argv);
	options_controller.parseOptions();
	if(options_controller.helpQuery())
		return TERMINATION_NORMAL;
	
	if(RDO_STUDIO_CONSOLE_APP_STRING == "NULL") 
	{
		TEST_ERROR(_T("Invalid compile app"));
	}
	
	tstring etalon_trace, etalon_result, simulation_trace, simulation_result;
	
	bool res = true;
	
	res *= options_controller.getEtalonTraceFileName(etalon_trace);
	res *= options_controller.getEtalonResultFileName(etalon_result);
	res *= options_controller.getTraceFileName(simulation_trace);
	res *= options_controller.getResultFileName(simulation_result);
	
	if(res) 
	{
		test_model(etalon_trace, etalon_result, simulation_trace, simulation_result);
	}
	else
	{
		std::cout << _T("Not enough of input data") << std::endl;
		g_state = TERMINATION_ERROR;
	}
	
	if(!g_state)
	{
		std::cout <<  _T("Test is completed successfully") << std::endl;
	}
	else
	{
		std::cout <<  _T("Test is completed with an error") << std::endl;
	}
	
	return g_state;
}

#ifdef COMPILER_VISUAL_STUDIO
#	pragma warning (default:4127)
#endif // COMPILER_VISUAL_STUDIO
