/*!
  \copyright (c) RDO-Team, 2011
  \file      /app/rdo_check_results/main.cpp
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
#include "app/rdo_check_results/controller_console_options.h"
// --------------------------------------------------------------------------------

#ifdef COMPILER_VISUAL_STUDIO
#	pragma warning (disable:4127)
#endif // COMPILER_VISUAL_STUDIO

#define TERMINATION_NORMAL	               0
#define TERMINATION_ERROR_FILE_NOT_FOUND   1
#define TERMINATION_ERROR_INVALID_INPUT    2
#define TERMINATION_ERROR_RESULT           3
#define TERMINATION_ERROR_TRACE            4
#define TERMINATION_ERROR_RESULT_AND_TRACE 5

ruint g_state = TERMINATION_NORMAL;

typedef std::list<tstring> file_data_list;

void TEST_ERROR(rbool condition, CREF(tstring) message, ruint exit_code)
{
    if(!condition) {
	    std::cout << "test error : " << message << std::endl;
	    exit(exit_code);
    }
}

void read_trace(CREF(tstring) file, REF(file_data_list) list)
{
	std::fstream stream(file.c_str(), std::ios::in);
	if (!stream.is_open()) 
	{
		TEST_ERROR(false, "Can't open file " + file, TERMINATION_ERROR_FILE_NOT_FOUND);
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

bool compare_trace(CREF(tstring) etalon_trace, CREF(tstring) trace)
{
	file_data_list etalon_trace_list;
	file_data_list trace_list;
	
	read_trace(etalon_trace, etalon_trace_list);
	read_trace(trace, trace_list);
	
	rbool trace_result_size_check = etalon_trace_list.size() == trace_list.size();
	rbool trace_result_check = etalon_trace_list == trace_list;

    return trace_result_size_check && trace_result_check;
}

void read_result(CREF(tstring) file, REF(file_data_list) list)
{
	std::fstream stream(file.c_str(), std::ios::in);
	if (!stream.is_open()) 
	{
		TEST_ERROR(false, "Can't open file " + file, TERMINATION_ERROR_FILE_NOT_FOUND);
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
			key = temp_string.find("BExpCalcCounter") == -1 ? false : true;
	}
}

bool compare_result(CREF(tstring) etalon_result, CREF(tstring) result)
{
	file_data_list etalon_result_list;
	file_data_list result_list;
	
	read_result(etalon_result, etalon_result_list);
	read_result(result, result_list);

	rbool trace_result_size_check = etalon_result_list.size() == result_list.size();
	rbool trace_result_chech = etalon_result_list == result_list;

    return trace_result_size_check && trace_result_chech;
}

void test_model(CREF(tstring) etalon_trace, CREF(tstring) etalon_result, CREF(tstring) simulation_trace, CREF(tstring) simulation_result)
{	
	TEST_ERROR(fs::exists(etalon_trace), "etalon_trace not found", TERMINATION_ERROR_FILE_NOT_FOUND);
	TEST_ERROR(fs::exists(etalon_result), "etalon_result not found", TERMINATION_ERROR_FILE_NOT_FOUND);

	TEST_ERROR(fs::exists(simulation_trace), "simulation_trace not found", TERMINATION_ERROR_FILE_NOT_FOUND);
	TEST_ERROR(fs::exists(simulation_result), "simulation_result not found", TERMINATION_ERROR_FILE_NOT_FOUND);
	
	rbool compare_trace_status = compare_trace(etalon_trace, simulation_trace);
	rbool compare_result_status = compare_result(etalon_result, simulation_result);

    if(!compare_trace_status && !compare_result_status) {
        g_state = TERMINATION_ERROR_RESULT_AND_TRACE;
    }
    else if(!compare_trace_status) {
        g_state = TERMINATION_ERROR_TRACE;
    }
    else if(!compare_result_status) {
        g_state = TERMINATION_ERROR_RESULT;
    }
}

int main(int argc, PTR(char) argv[])
{
	RDOControllerConsoleOptions options_controller(argc, argv);
	options_controller.parseOptions();
    
    if(options_controller.helpQuery()) {
		return TERMINATION_NORMAL;
    }
	
	tstring etalon_trace, etalon_result, simulation_trace, simulation_result;
	
	rbool res = true;
	
	res &= options_controller.getEtalonTraceFileName(etalon_trace);
	res &= options_controller.getEtalonResultFileName(etalon_result);
	res &= options_controller.getTraceFileName(simulation_trace);
	res &= options_controller.getResultFileName(simulation_result);
	
	if(res) 
	{
		test_model(etalon_trace, etalon_result, simulation_trace, simulation_result);
	}
	else
	{
		std::cout << "Not enough of input data" << std::endl;
		g_state = TERMINATION_ERROR_INVALID_INPUT;
	}
	
	if(!g_state)
	{
		std::cout <<  "Test is completed successfully" << std::endl;
	}
	else
	{
		std::cout <<  "Test is completed with an error" << std::endl;
	}

	return g_state;
}

#ifdef COMPILER_VISUAL_STUDIO
#	pragma warning (default:4127)
#endif // COMPILER_VISUAL_STUDIO
