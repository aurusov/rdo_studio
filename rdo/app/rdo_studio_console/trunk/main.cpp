/*!
  \copyright (c) RDO-Team, 2011
  \file      main.cpp
  \author    Пройдаков Евгений (lord.tiran@gmail.com)
  \date      26.10.2011
  \brief     Консольная версия RDO
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- PLATFORM
#include "utils/platform.h"
// ----------------------------------------------------------------------- INCLUDES
#include <iostream>
#include <boost/program_options.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdocommon.h"
#include "kernel/rdothread.h"
#include "repository/rdorepository.h"
#include "simulator/service/rdosimwin.h"
// --------------------------------------------------------------------------------

#ifdef COMPILER_VISUAL_STUDIO
#	pragma warning (disable:4512)
#endif // COMPILER_VISUAL_STUDIO

#ifdef OST_WINDOWS
const tstring SYSTEM_OS = _T("Windows");
#endif // OST_WINDOWS
#ifdef OST_LINUX
const tstring SYSTEM_OS = _T("Linux");
#endif // OST_LINUX

// Input param constant
const tstring MODEL_COMMAND                 = _T("model");
const tstring CHELP_COMMAND                 = _T("help");
const tstring VERSION_COMMAND               = _T("version");
const tstring LANGUAGE_COMMAND              = _T("language");
const tstring AUTO_RUN_COMMAND              = _T("autorun");
const tstring AUTO_EXIT_COMMAND             = _T("autoexit");
const tstring DONT_CLOSE_IF_ERROR_COMMAND   = _T("dont_close_if_error");
const tstring PLUGIN_START_COMMAND          = _T("pluginstart");
const tstring PLUGIN_AUTO_EXIT_COMMAND      = _T("pluginautoexit");

const tstring MODEL_COMMAND_SHORT           = _T("m");
const tstring HELP_COMMAND_SHORT            = _T("h");
const tstring VERSION_COMMAND_SHORT         = _T("v");
const tstring LANGUAGE_COMMAND_SHORT        = _T("l");

// Input param comment
const tstring MODEL_COMMENT                 = _T("path to RDO model");
const tstring HELP_COMMENT                  = _T("display help message");
const tstring VERSION_COMMENT               = _T("display program version");
const tstring LANGUAGE_COMMENT              = _T("display language version of RDO");
const tstring AUTO_RUN_COMMENT              = _T("autostart program, skipped in console version");
const tstring AUTO_EXIT_COMMENT             = _T("autoexit program, skipped in console version");
const tstring DONT_CLOSE_IF_ERROR_COMMENT   = _T("dont close if error, skipped in console version");
const tstring PLUGIN_START_COMMENT          = _T("start plugin, skipped in console version");
const tstring PLUGIN_AUTO_EXIT_COMMENT      = _T("plugin auto exit, skipped in console version");

// Auxiliary constants
const tstring RDO_STUDIO_CONSOLE_VERSION       = _T("0.1.5");
const tstring RDO_LANGUAGE_VERSION             = _T("1.5");

const tstring RDO_SITE                         = _T("( http://rdo.rk9.bmstu.ru )");
const tstring RDO_STUDIO_CONSOLE_PROGRAMM_NAME = _T("rdo console studio");

const tstring NOP_STRING   = _T(" ");
const tstring COMMA_STRING = _T(",");

namespace po = boost::program_options;

tstring create_description_message();
void create_description(po::options_description& description);

struct rdo_studio_console_param
{
	rdo_studio_console_param()
		: m_model_file_name("")
		, autorun(true)
		, autoexit(true)
		, dont_close_if_error(false)
		, pluginstart(false)
		, pluginautoexit(false)
	{}

	tstring m_model_file_name;
	rbool autorun;
	rbool autoexit;
	rbool dont_close_if_error;
	rbool pluginstart;
	rbool pluginautoexit;
};

class ModelController: public RDOThread
{
public:
	ModelController();
	virtual ~ModelController();

private:
	void proc(REF(RDOMessageInfo) msg);
};

ModelController::ModelController()
	: RDOThread(_T("RDOThreadModelController"))
{
	notifies.push_back(RT_REPOSITORY_MODEL_OPEN             );
	notifies.push_back(RT_REPOSITORY_MODEL_OPEN_GET_NAME    );
	notifies.push_back(RT_REPOSITORY_MODEL_OPEN_ERROR       );
	notifies.push_back(RT_REPOSITORY_MODEL_CLOSE            );
	notifies.push_back(RT_REPOSITORY_MODEL_CLOSE_CAN_CLOSE  );
	notifies.push_back(RT_REPOSITORY_MODEL_CLOSE_ERROR      );
	notifies.push_back(RT_REPOSITORY_MODEL_SAVE             );
	notifies.push_back(RT_SIMULATOR_PARSE_STRING            );
	notifies.push_back(RT_SIMULATOR_PARSE_OK                );
	notifies.push_back(RT_SIMULATOR_PARSE_ERROR             );
	notifies.push_back(RT_SIMULATOR_MODEL_STOP_OK           );
	notifies.push_back(RT_SIMULATOR_MODEL_STOP_RUNTIME_ERROR);
	notifies.push_back(RT_RUNTIME_MODEL_START_BEFORE        );
	notifies.push_back(RT_RUNTIME_MODEL_START_AFTER         );
	notifies.push_back(RT_RUNTIME_MODEL_STOP_BEFORE         );
	notifies.push_back(RT_DEBUG_STRING                      );
	notifies.push_back(RT_RESULT_STRING                     );

	after_constructor();
}

ModelController::~ModelController()
{}

void ModelController::proc(REF(RDOThread::RDOMessageInfo) msg)
{
	switch (msg.message)
	{
		case RDOThread::RT_REPOSITORY_MODEL_OPEN:
		{
			int i = 1;
			//openModelFromRepository();
			break;
		}
		case RDOThread::RT_REPOSITORY_MODEL_OPEN_ERROR:
		{
//			AfxMessageBox(rdo::format(ID_MSG_MODELOPEN_ERROR, static_cast<PTR(tstring)>(msg.param)->c_str()).c_str(), MB_ICONSTOP | MB_OK);
			int i = 1;
			break;
		}
		case RDOThread::RT_REPOSITORY_MODEL_SAVE:
		{
			int i = 1;
			break;
		}
		case RDOThread::RT_REPOSITORY_MODEL_OPEN_GET_NAME:
		{
			//msg.lock();
			//PTR(rdoRepository::RDOThreadRepository::OpenFile) data = static_cast<PTR(rdoRepository::RDOThreadRepository::OpenFile)>(msg.param);
			//CString filter;
			//filter.LoadString(ID_MODEL_FILETYPE);
			//CFileDialog dlg(true, _T(""), _T(""), 0, filter, AfxGetMainWnd());
			//data->m_result   = dlg.DoModal() == IDOK;
			//data->m_name     = dlg.GetPathName();
			//data->m_readOnly = dlg.GetReadOnlyPref() == TRUE;
			//msg.unlock();
			int i = 1;
			break;
		}
		case RDOThread::RT_REPOSITORY_MODEL_CLOSE:
		{
//			closeModelFromRepository();
			int i = 1;
			break;
		}
		case RDOThread::RT_REPOSITORY_MODEL_CLOSE_CAN_CLOSE:
		{
			int i = 1;
			break;
		}
		case RDOThread::RT_REPOSITORY_MODEL_CLOSE_ERROR:
		{
			int i = 1;
			break;
		}
		case RDOThread::RT_RUNTIME_MODEL_START_BEFORE:
		{
			int i = 1;
			break;
		}
		case RDOThread::RT_RUNTIME_MODEL_START_AFTER:
		{
			int i = 1;
			break;
		}
		case RDOThread::RT_RUNTIME_MODEL_STOP_BEFORE:
		{
			int i = 1;
			break;
		}
		case RDOThread::RT_SIMULATOR_MODEL_STOP_OK:
		{
			//! @todo вызывается после завершения
			int i = 1;
			break;
		}
		case RDOThread::RT_SIMULATOR_MODEL_STOP_RUNTIME_ERROR:
		{
//			sendMessage(kernel->simulator(), RT_SIMULATOR_GET_MODEL_EXITCODE, &m_exitCode);
//			show_result();
//			PTR(RDOStudioOutput) output = &studioApp.mainFrame->output;
//			output->appendStringToDebug(rdo::format(IDS_MODEL_RUNTIMEERROR_STOPED));
//			output->clearBuild();
//			output->showBuild();
//			output->appendStringToBuild(rdo::format(IDS_MODEL_RUNTIMEERROR));
//			std::vector< RDOSyntaxError > errors;
//			studioApp.studioGUI->sendMessage(kernel->simulator(), RDOThread::RT_SIMULATOR_GET_ERRORS, &errors);
//			int errors_cnt   = 0;
//			int warnings_cnt = 0;
//			STL_FOR_ALL_CONST(errors, it)
//			{
//				output->appendStringToBuild(it->m_code, it->m_message, it->m_file, it->m_line, it->m_pos, it->m_warning);
//				if (it->m_warning)
//				{
//					warnings_cnt++;
//				}
//				else
//				{
//					errors_cnt++;
//				}
//			}
//			studioApp.autoCloseByModel();
			int i = 1;
			break;
		}
		case RDOThread::RT_SIMULATOR_PARSE_OK:
		{
			int i = 1;
			break;
		}
		case RDOThread::RT_SIMULATOR_PARSE_ERROR:
		{
			int i = 1;
			break;
		}
		case RDOThread::RT_SIMULATOR_PARSE_STRING:
		{
			int i = 1;
			break;
		}
		case RDOThread::RT_DEBUG_STRING:
		{
			int i = 1;
			break;
		}
		case RDOThread::RT_RESULT_STRING:
		{
			int i = 1;
			break;
		}
	}
}

int main(int argc, char *argv[])
{
	po::options_description description(create_description_message().c_str());
	create_description(description);

	po::variables_map options;
	try
	{
		po::store(po::parse_command_line(argc, argv, description), options);
	}
	catch (CREF(std::exception) e)
	{
		std::cout << _T("command line error: ") << e.what() << std::endl;
	}
	if (options.empty() || options.count(CHELP_COMMAND))
	{
		std::cout << description << std::endl;
	}
	else if (options.count(LANGUAGE_COMMAND))
	{
		std::cout << _T("rdo language v") + RDO_LANGUAGE_VERSION << std::endl;
	}
	else if (options.count(VERSION_COMMAND))
	{
		std::cout << RDO_STUDIO_CONSOLE_PROGRAMM_NAME + NOP_STRING + VERSION_COMMAND_SHORT + RDO_STUDIO_CONSOLE_VERSION << std::endl;
	}
	//else
	{
		// Init
		RDOKernel::init();
		new rdoSimulator::RDOThreadSimulator();
		new rdoRepository::RDOThreadRepository();

		ModelController* modelController = new ModelController();

		rdoRepository::RDOThreadRepository::OpenFile data("C:\\mymodel\\mymodel.rdox");
		modelController->broadcastMessage(RDOThread::RT_STUDIO_MODEL_OPEN, &data);
		modelController->broadcastMessage(RDOThread::RT_STUDIO_MODEL_BUILD);
		modelController->broadcastMessage(RDOThread::RT_STUDIO_MODEL_RUN  );

		while (true)
		{
			kernel->idle();
		}

		// Close
		RDOKernel::close();
	}
	return 0;
}

tstring create_description_message()
{
	return  RDO_STUDIO_CONSOLE_PROGRAMM_NAME + NOP_STRING + VERSION_COMMAND_SHORT +
			RDO_STUDIO_CONSOLE_VERSION + NOP_STRING + 
			SYSTEM_OS + RDO_SITE;
}

void create_description(po::options_description& description)
{
	description.add_options()
			((MODEL_COMMAND + COMMA_STRING + MODEL_COMMAND_SHORT).c_str(), MODEL_COMMENT.c_str())
			((VERSION_COMMAND + COMMA_STRING + VERSION_COMMAND_SHORT).c_str(), VERSION_COMMENT.c_str())
			((CHELP_COMMAND + COMMA_STRING + HELP_COMMAND_SHORT).c_str(), HELP_COMMENT.c_str())
			((LANGUAGE_COMMAND + COMMA_STRING + LANGUAGE_COMMAND_SHORT).c_str(), LANGUAGE_COMMENT.c_str())
			(AUTO_RUN_COMMAND.c_str(), AUTO_RUN_COMMENT.c_str())
			(AUTO_EXIT_COMMAND.c_str(), AUTO_EXIT_COMMENT.c_str())
			(DONT_CLOSE_IF_ERROR_COMMAND.c_str(), DONT_CLOSE_IF_ERROR_COMMENT.c_str())
			(PLUGIN_START_COMMAND.c_str(), PLUGIN_START_COMMENT.c_str())
			(PLUGIN_AUTO_EXIT_COMMAND.c_str(), PLUGIN_AUTO_EXIT_COMMENT.c_str());	
}
