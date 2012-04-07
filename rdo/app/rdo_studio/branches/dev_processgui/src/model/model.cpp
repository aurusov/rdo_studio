/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio_mfc/src/model/model.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <limits>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdostream.h"
#include "utils/rdoanimation.h"
#include "kernel/rdokernel.h"
#include "kernel/rdothread.h"
#include "repository/rdorepository.h"
#include "simulator/runtime/rdo_exception.h"
#include "app/rdo_studio_mfc/src/model/model.h"
#include "app/rdo_studio_mfc/src/thread.h"
#include "app/rdo_studio_mfc/src/main_frm.h"
#include "app/rdo_studio_mfc/src/child_frm.h"
#include "app/rdo_studio_mfc/src/frame/document.h"
#include "app/rdo_studio_mfc/src/frame/view.h"
#include "app/rdo_studio_mfc/src/plugins.h"
#include "app/rdo_studio_mfc/rdo_edit/rdoeditortabctrl.h"
#include "app/rdo_studio_mfc/edit_ctrls/rdobuildedit.h"
#include "app/rdo_studio_mfc/edit_ctrls/rdodebugedit.h"
#include "app/rdo_studio_mfc/rdo_edit/rdoeditorresults.h"
#include "app/rdo_studio_mfc/rdo_tracer/rdotracer.h"
#include "app/rdo_studio_mfc/resource.h"
#include "app/rdo_studio_mfc/plugins/common/rdoplugin.h"
#include "thirdparty/pugixml/src/pugixml.hpp"
// --------------------------------------------------------------------------------

using namespace rdoEditor;
using namespace rdoSimulator;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// -------------------- RDOStudioModel
// --------------------------------------------------------------------------------
PTR(RDOStudioModel) model = NULL;

RDOStudioModel::RDOStudioModel()
	: RDOThreadGUI(_T("RDOThreadModelGUI"), static_cast<PTR(RDOKernelGUI)>(studioApp.m_pStudioGUI))
	, m_pModelDocTemplate(NULL                          )
	, m_pFlowchartDocTemplate( NULL                     )
	, m_useTemplate      (-1                            )
	, m_autoDeleteDoc    (true                          )
	, m_showCanNotCloseModelMessage(true                )
	, m_GUI_HAS_MODEL    (false                         )
	, m_GUI_CAN_RUN      (true                          )
	, m_GUI_IS_RUNING    (false                         )
	, m_GUI_ACTION_NEW   (true                          )
	, m_GUI_ACTION_OPEN  (true                          )
	, m_GUI_ACTION_SAVE  (true                          )
	, m_GUI_ACTION_CLOSE (true                          )
	, m_GUI_ACTION_BUILD (true                          )
	, m_GUI_ACTION_RUN   (true                          )
	, m_openError        (false                         )
	, m_smrEmptyError    (false                         )
	, m_modelClosed      (true                          )
	, m_frmDescribed     (false                         )
	, m_timeNow          (0                             )
	, m_speed            (1                             )
	, m_showRate         (60                            )
	, m_tempPause        (false                         )
	, m_runtimeMode      (rdoRuntime::RTM_MaxSpeed      )
	, m_runtimeModePrev  (rdoRuntime::RTM_MaxSpeed      )
	, m_exitCode         (rdoSimulator::EC_ModelNotFound)
	, m_prevModify       (false                         )
	, m_buildState       (BS_UNDEFINED                  )
{
	m_pModelDocTemplate = new CMultiDocTemplate(IDR_MODEL_TYPE, RUNTIME_CLASS(RDOStudioModelDoc), RUNTIME_CLASS(RDOStudioChildFrame), RUNTIME_CLASS(RDOStudioModelView));
	AfxGetApp()->AddDocTemplate(m_pModelDocTemplate);

#ifdef PROCGUI_ENABLE
	m_pFlowchartDocTemplate = new CMultiDocTemplate(IDR_FLOWCHART_TYPE, RUNTIME_CLASS(RPDoc), RUNTIME_CLASS(RPChildFrame), RUNTIME_CLASS(RPView));
	AfxGetApp()->AddDocTemplate(m_pFlowchartDocTemplate);
#endif

	model = this;

	ModelTemplate modelTemplate;
	modelTemplate[ rdoModelObjects::SMR ] = ModelTemplateItem(IDR_MODEL_TMP0_SMR, 0);
	m_modelTemplates[0] = modelTemplate;

	modelTemplate.clear();
	modelTemplate[ rdoModelObjects::SMR ] = ModelTemplateItem(IDR_MODEL_TMP1_SMR, 0);
	m_modelTemplates[1] = modelTemplate;

	modelTemplate.clear();
	modelTemplate[ rdoModelObjects::EVN ] = ModelTemplateItem(IDR_MODEL_TMP2_EVN, 0 );
	modelTemplate[ rdoModelObjects::PRC ] = ModelTemplateItem(IDR_MODEL_TMP2_PRC, 0 );
	modelTemplate[ rdoModelObjects::PMD ] = ModelTemplateItem(IDR_MODEL_TMP2_PMD, 0 );
	modelTemplate[ rdoModelObjects::PAT ] = ModelTemplateItem(IDR_MODEL_TMP2_PAT, 9 );
	modelTemplate[ rdoModelObjects::RTP ] = ModelTemplateItem(IDR_MODEL_TMP2_RTP, 15);
	modelTemplate[ rdoModelObjects::RSS ] = ModelTemplateItem(IDR_MODEL_TMP2_RSS, 12);
	modelTemplate[ rdoModelObjects::DPT ] = ModelTemplateItem(IDR_MODEL_TMP2_DPT, 13);
	modelTemplate[ rdoModelObjects::FRM ] = ModelTemplateItem(IDR_MODEL_TMP2_FRM, 7 );
	modelTemplate[ rdoModelObjects::FUN ] = ModelTemplateItem(IDR_MODEL_TMP2_FUN, 10);
	modelTemplate[ rdoModelObjects::SMR ] = ModelTemplateItem(IDR_MODEL_TMP2_SMR, 0 );
	m_modelTemplates[2] = modelTemplate;

	modelTemplate.clear();
	modelTemplate[ rdoModelObjects::PMD ] = ModelTemplateItem(IDR_MODEL_TMP3_PMD, 0  );
	modelTemplate[ rdoModelObjects::PRC ] = ModelTemplateItem(IDR_MODEL_TMP3_PRC, 0  );
	modelTemplate[ rdoModelObjects::EVN ] = ModelTemplateItem(IDR_MODEL_TMP3_EVN, 0  );
	modelTemplate[ rdoModelObjects::PAT ] = ModelTemplateItem(IDR_MODEL_TMP3_PAT, 131);
	modelTemplate[ rdoModelObjects::RTP ] = ModelTemplateItem(IDR_MODEL_TMP3_RTP, 98 );
	modelTemplate[ rdoModelObjects::RSS ] = ModelTemplateItem(IDR_MODEL_TMP3_RSS, 0  );
	modelTemplate[ rdoModelObjects::DPT ] = ModelTemplateItem(IDR_MODEL_TMP3_DPT, 0  );
	modelTemplate[ rdoModelObjects::FRM ] = ModelTemplateItem(IDR_MODEL_TMP3_FRM, 84 );
	modelTemplate[ rdoModelObjects::FUN ] = ModelTemplateItem(IDR_MODEL_TMP3_FUN, 44 );
	modelTemplate[ rdoModelObjects::SMR ] = ModelTemplateItem(IDR_MODEL_TMP3_SMR, 0  );
	m_modelTemplates[3] = modelTemplate;

	modelTemplate.clear();
	modelTemplate[ rdoModelObjects::EVN ] = ModelTemplateItem( IDR_MODEL_TMP4_EVN, 0 );
	modelTemplate[ rdoModelObjects::PAT ] = ModelTemplateItem( IDR_MODEL_TMP4_PAT, 0 );
	modelTemplate[ rdoModelObjects::RTP ] = ModelTemplateItem( IDR_MODEL_TMP4_RTP, 0 );
	modelTemplate[ rdoModelObjects::RSS ] = ModelTemplateItem( IDR_MODEL_TMP4_RSS, 0 );
	modelTemplate[ rdoModelObjects::DPT ] = ModelTemplateItem( IDR_MODEL_TMP4_DPT, 0 );
	modelTemplate[ rdoModelObjects::FUN ] = ModelTemplateItem( IDR_MODEL_TMP4_FUN, 0 );
	modelTemplate[ rdoModelObjects::SMR ] = ModelTemplateItem( IDR_MODEL_TMP4_SMR, 0 );
	modelTemplate[ rdoModelObjects::PMD ] = ModelTemplateItem( IDR_MODEL_TMP4_PMD, 0 );
	m_modelTemplates[4] = modelTemplate;

	modelTemplate.clear();
	modelTemplate[ rdoModelObjects::EVN ] = ModelTemplateItem(IDR_MODEL_TMP5_EVN, 0);
	modelTemplate[ rdoModelObjects::PAT ] = ModelTemplateItem(IDR_MODEL_TMP5_PAT, 0);
	modelTemplate[ rdoModelObjects::RTP ] = ModelTemplateItem(IDR_MODEL_TMP5_RTP, 0);
	modelTemplate[ rdoModelObjects::RSS ] = ModelTemplateItem(IDR_MODEL_TMP5_RSS, 0);
	modelTemplate[ rdoModelObjects::DPT ] = ModelTemplateItem(IDR_MODEL_TMP5_DPT, 0);
	modelTemplate[ rdoModelObjects::FUN ] = ModelTemplateItem(IDR_MODEL_TMP5_FUN, 0);
	modelTemplate[ rdoModelObjects::SMR ] = ModelTemplateItem(IDR_MODEL_TMP5_SMR, 0);
	modelTemplate[ rdoModelObjects::PMD ] = ModelTemplateItem(IDR_MODEL_TMP5_PMD, 0);
	m_modelTemplates[5] = modelTemplate;

	modelTemplate.clear();
	modelTemplate[ rdoModelObjects::EVN ] = ModelTemplateItem(IDR_MODEL_TMP6_EVN, 0);
	modelTemplate[ rdoModelObjects::PAT ] = ModelTemplateItem(IDR_MODEL_TMP6_PAT, 0);
	modelTemplate[ rdoModelObjects::RTP ] = ModelTemplateItem(IDR_MODEL_TMP6_RTP, 0);
	modelTemplate[ rdoModelObjects::RSS ] = ModelTemplateItem(IDR_MODEL_TMP6_RSS, 0);
	modelTemplate[ rdoModelObjects::DPT ] = ModelTemplateItem(IDR_MODEL_TMP6_DPT, 0);
	modelTemplate[ rdoModelObjects::FUN ] = ModelTemplateItem(IDR_MODEL_TMP6_FUN, 0);
	modelTemplate[ rdoModelObjects::SMR ] = ModelTemplateItem(IDR_MODEL_TMP6_SMR, 0);
	modelTemplate[ rdoModelObjects::PMD ] = ModelTemplateItem(IDR_MODEL_TMP6_PMD, 0);
	m_modelTemplates[6] = modelTemplate;

	modelTemplate.clear();
	modelTemplate[ rdoModelObjects::EVN ] = ModelTemplateItem(IDR_MODEL_TMP7_EVN, 0);
	modelTemplate[ rdoModelObjects::PAT ] = ModelTemplateItem(IDR_MODEL_TMP7_PAT, 0);
	modelTemplate[ rdoModelObjects::RTP ] = ModelTemplateItem(IDR_MODEL_TMP7_RTP, 0);
	modelTemplate[ rdoModelObjects::RSS ] = ModelTemplateItem(IDR_MODEL_TMP7_RSS, 0);
	modelTemplate[ rdoModelObjects::DPT ] = ModelTemplateItem(IDR_MODEL_TMP7_DPT, 0);
	modelTemplate[ rdoModelObjects::FUN ] = ModelTemplateItem(IDR_MODEL_TMP7_FUN, 0);
	modelTemplate[ rdoModelObjects::SMR ] = ModelTemplateItem(IDR_MODEL_TMP7_SMR, 0);
	modelTemplate[ rdoModelObjects::PMD ] = ModelTemplateItem(IDR_MODEL_TMP7_PMD, 0);
	m_modelTemplates[7] = modelTemplate;

	notifies.push_back(RT_STUDIO_MODEL_GET_TEXT             );
	notifies.push_back(RT_REPOSITORY_MODEL_NEW              );
	notifies.push_back(RT_REPOSITORY_MODEL_OPEN             );
	notifies.push_back(RT_REPOSITORY_MODEL_OPEN_GET_NAME    );
	notifies.push_back(RT_REPOSITORY_MODEL_OPEN_ERROR       );
	notifies.push_back(RT_REPOSITORY_MODEL_CLOSE            );
	notifies.push_back(RT_REPOSITORY_MODEL_CLOSE_CAN_CLOSE  );
	notifies.push_back(RT_REPOSITORY_MODEL_CLOSE_ERROR      );
	notifies.push_back(RT_REPOSITORY_MODEL_SAVE             );
	notifies.push_back(RT_REPOSITORY_MODEL_SAVE_GET_NAME    );
	notifies.push_back(RT_SIMULATOR_PARSE_STRING            );
	notifies.push_back(RT_SIMULATOR_PARSE_OK                );
	notifies.push_back(RT_SIMULATOR_PARSE_ERROR             );
	notifies.push_back(RT_SIMULATOR_PARSE_ERROR_SMR_EMPTY   );
	notifies.push_back(RT_SIMULATOR_MODEL_STOP_OK           );
	notifies.push_back(RT_SIMULATOR_MODEL_STOP_BY_USER      );
	notifies.push_back(RT_SIMULATOR_MODEL_STOP_RUNTIME_ERROR);
	notifies.push_back(RT_RUNTIME_MODEL_START_BEFORE        );
	notifies.push_back(RT_RUNTIME_MODEL_START_AFTER         );
	notifies.push_back(RT_RUNTIME_MODEL_STOP_BEFORE         );
	//notifies.push_back(RT_RUNTIME_TRANSACT_COUNTER          );
	notifies.push_back(RT_DEBUG_STRING                      );
	notifies.push_back(RT_RESULT_STRING                     );

	after_constructor();
}

RDOStudioModel::~RDOStudioModel()
{
	model = NULL;
//	closeModel();
}

void RDOStudioModel::proc(REF(RDOThread::RDOMessageInfo) msg)
{
	switch (msg.message)
	{
		case RDOThread::RT_STUDIO_MODEL_GET_TEXT:
		{
			msg.lock();
			PTR(rdoRepository::RDOThreadRepository::FileData) fdata = static_cast<PTR(rdoRepository::RDOThreadRepository::FileData)>(msg.param);
			PTR(RDOEditorTabCtrl) pTab = getTab();
			if (pTab)
			{
				PTR(RDOEditorEdit) edit = pTab->getItemEdit(fdata->m_type);
				if (edit)
				{
					edit->save(fdata->m_stream);
				}
			}
			msg.unlock();
			break;
		}
		case RDOThread::RT_REPOSITORY_MODEL_NEW:
		{
			newModelFromRepository();
			if (plugins)
			{
				plugins->pluginProc(rdoPlugin::PM_MODEL_NEW);
			}
			break;
		}
		case RDOThread::RT_REPOSITORY_MODEL_OPEN:
		{
			openModelFromRepository();
			if (plugins)
			{
				plugins->pluginProc(rdoPlugin::PM_MODEL_OPEN);
			}
			break;
		}
		case RDOThread::RT_REPOSITORY_MODEL_OPEN_ERROR:
		{
			AfxMessageBox(rdo::format(ID_MSG_MODELOPEN_ERROR, static_cast<PTR(tstring)>(msg.param)->c_str()).c_str(), MB_ICONSTOP | MB_OK);
			break;
		}
		case RDOThread::RT_REPOSITORY_MODEL_SAVE:
		{
			saveModelToRepository();
			break;
		}
		case RDOThread::RT_REPOSITORY_MODEL_OPEN_GET_NAME:
		{
			msg.lock();
			PTR(rdoRepository::RDOThreadRepository::OpenFile) data = static_cast<PTR(rdoRepository::RDOThreadRepository::OpenFile)>(msg.param);
			CString filter;
			filter.LoadString(ID_MODEL_FILETYPE);
			CFileDialog dlg(true, _T(""), _T(""), 0, filter, AfxGetMainWnd());
			data->m_result   = dlg.DoModal() == IDOK;
			data->m_name     = dlg.GetPathName();
			data->m_readOnly = dlg.GetReadOnlyPref() == TRUE;
			msg.unlock();
			break;
		}
		case RDOThread::RT_REPOSITORY_MODEL_SAVE_GET_NAME:
		{
			msg.lock();
			PTR(rdoRepository::RDOThreadRepository::OpenFile) data = static_cast<PTR(rdoRepository::RDOThreadRepository::OpenFile)>(msg.param);
			CString filter;
			filter.LoadString(ID_MODEL_FILETYPE);
			CFileDialog dlg(false, _T("smr"), _T(""), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter, AfxGetMainWnd());
			data->m_result = dlg.DoModal() == IDOK;
			data->m_name   = dlg.GetPathName();
			msg.unlock();
			break;
		}
		case RDOThread::RT_REPOSITORY_MODEL_CLOSE:
		{
			closeModelFromRepository();
			if (plugins)
			{
				plugins->pluginProc(rdoPlugin::PM_MODEL_CLOSE);
			}
			break;
		}
		case RDOThread::RT_REPOSITORY_MODEL_CLOSE_CAN_CLOSE:
		{
			msg.lock();
			if (*static_cast<PTR(rbool)>(msg.param) == false)
			{
				msg.unlock();
				break;
			}
			msg.unlock();
			rbool res = canCloseModel();
			msg.lock();
			if (*static_cast<PTR(rbool)>(msg.param) == true)
			{
				*static_cast<PTR(rbool)>(msg.param) = res;
			}
			msg.unlock();
			break;
		}
		case RDOThread::RT_REPOSITORY_MODEL_CLOSE_ERROR:
		{
			if (m_showCanNotCloseModelMessage)
			{
				AfxMessageBox(rdo::format(ID_MSG_MODELCLOSE_ERROR).c_str(), MB_ICONSTOP | MB_OK);
			}
			break;
		}
		case RDOThread::RT_RUNTIME_MODEL_START_BEFORE:
		{
			if (plugins)
			{
				plugins->modelStart();
				plugins->pluginProc(rdoPlugin::PM_MODEL_BEFORE_START);
			}
			break;
		}
		case RDOThread::RT_RUNTIME_MODEL_START_AFTER:
		{
			m_GUI_IS_RUNING = true;
			sendMessage(kernel->runtime(), RT_RUNTIME_GET_MODE, &m_runtimeMode);
			sendMessage(kernel->runtime(), RT_RUNTIME_GET_SPEED, &m_speed);
			setSpeed(studioApp.m_pMainFrame->getSpeed());
			sendMessage(kernel->runtime(), RT_RUNTIME_GET_SHOWRATE, &m_showRate);
			afterModelStart();
			PTR(RDOStudioOutput) output = &studioApp.m_pMainFrame->output;
			output->showDebug();
			output->appendStringToDebug(rdo::format(IDS_MODEL_STARTED));
			const_cast<PTR(rdoEditCtrl::RDODebugEdit)>(output->getDebug())->UpdateWindow();
			int index = m_frameManager.getLastShowedFrame();
			if (index != -1)
			{
				PTR(RDOStudioFrameView) pView = m_frameManager.getFrameView(index);
				if (pView) pView->SetFocus();
			}
			studioApp.m_pMainFrame->update_start();
			if (plugins)
			{
				plugins->pluginProc(rdoPlugin::PM_MODEL_AFTER_START);
			}
			break;
		}
		case RDOThread::RT_RUNTIME_MODEL_STOP_BEFORE:
		{
			studioApp.m_pMainFrame->update_stop();
			sendMessage(kernel->runtime(), RT_RUNTIME_GET_TIMENOW, &m_timeNow);
			m_frameManager.clear();
			SYSTEMTIME time_stop;
			::GetSystemTime(&time_stop);
			ruint delay = ruint(~0);
			if (m_timeStart.wYear == time_stop.wYear && m_timeStart.wMonth == time_stop.wMonth)
			{
				delay = (time_stop.wDay - m_timeStart.wDay) * 24 * 60 * 60 * 1000 + (time_stop.wHour - m_timeStart.wHour) * 60 * 60 * 1000 + (time_stop.wMinute - m_timeStart.wMinute) * 60 * 1000 + (time_stop.wSecond - m_timeStart.wSecond) * 1000 + (time_stop.wMilliseconds - m_timeStart.wMilliseconds);
			}
			else if (time_stop.wYear - m_timeStart.wYear == 1 && m_timeStart.wMonth == 12 && time_stop.wMonth == 1)
			{
				delay = (time_stop.wDay + 31 - m_timeStart.wDay) * 24 * 60 * 60 * 1000 + (time_stop.wHour - m_timeStart.wHour) * 60 * 60 * 1000 + (time_stop.wMinute - m_timeStart.wMinute) * 60 * 1000 + (time_stop.wSecond - m_timeStart.wSecond) * 1000 + (time_stop.wMilliseconds - m_timeStart.wMilliseconds);
			}
			if (delay != -1)
			{
				studioApp.m_pMainFrame->output.appendStringToDebug(rdo::format(IDS_MODEL_DURATION, delay));
			}
			m_GUI_CAN_RUN   = true;
			m_GUI_IS_RUNING = false;
			break;
		}
		case RDOThread::RT_RUNTIME_TRANSACT_COUNTER:
		{
			msg.lock();
			int count = *static_cast<PTR(int)>(msg.param);
			PTR(RPShapeDataBlockCreate) pRawParam = static_cast<PTR(RPShapeDataBlockCreate)>(msg.param);
			pRawParam->setAmount(count);
			msg.unlock();
			break;
		}
		case RDOThread::RT_SIMULATOR_MODEL_STOP_OK:
		{
			PTR(RDOStudioOutput) output = &studioApp.m_pMainFrame->output;
			output->appendStringToDebug(rdo::format(IDS_MODEL_FINISHED));
			const_cast<PTR(rdoEditCtrl::RDODebugEdit)>(output->getDebug())->UpdateWindow();

			show_result();

			if (plugins)
			{
				plugins->pluginProc(rdoPlugin::PM_MODEL_FINISHED);
				plugins->modelStop();
			}
			studioApp.autoCloseByModel();
			break;
		}
		case RDOThread::RT_SIMULATOR_MODEL_STOP_BY_USER:
		{
			sendMessage(kernel->simulator(), RT_SIMULATOR_GET_MODEL_EXITCODE, &m_exitCode);
			PTR(RDOStudioOutput) output = &studioApp.m_pMainFrame->output;
			output->appendStringToDebug(rdo::format(IDS_MODEL_STOPED));
			const_cast<PTR(rdoEditCtrl::RDODebugEdit)>(output->getDebug())->UpdateWindow();

			show_result();

			if (plugins)
			{
				plugins->pluginProc(rdoPlugin::PM_MODEL_STOP_CANCEL);
				plugins->modelStop(false);
			}
			break;
		}
		case RDOThread::RT_SIMULATOR_MODEL_STOP_RUNTIME_ERROR:
		{
			sendMessage(kernel->simulator(), RT_SIMULATOR_GET_MODEL_EXITCODE, &m_exitCode);
			show_result();
			PTR(RDOStudioOutput) output = &studioApp.m_pMainFrame->output;
			output->appendStringToDebug(rdo::format(IDS_MODEL_RUNTIMEERROR_STOPED));
			output->clearBuild();
			output->showBuild();
			output->appendStringToBuild(rdo::format(IDS_MODEL_RUNTIMEERROR));
			std::vector< RDOSyntaxError > errors;
			studioApp.m_pStudioGUI->sendMessage(kernel->simulator(), RDOThread::RT_SIMULATOR_GET_ERRORS, &errors);
			int errors_cnt   = 0;
			int warnings_cnt = 0;
			STL_FOR_ALL_CONST(errors, it)
			{
				output->appendStringToBuild(it->m_code, it->m_message, it->m_file, it->m_line, it->m_pos, it->m_warning);
				if (it->m_warning)
				{
					warnings_cnt++;
				}
				else
				{
					errors_cnt++;
				}
			}
			if (errors_cnt || warnings_cnt)
			{
//				const_cast<PTR(rdoEditCtrl::RDOBuildEdit)>(output->getBuild())->showFirstError();
			}

			if (plugins)
			{
				plugins->pluginProc(rdoPlugin::PM_MODEL_STOP_RUNTIME_ERROR);
				plugins->modelStop(false);
			}
			studioApp.autoCloseByModel();
			break;
		}
		case RDOThread::RT_SIMULATOR_PARSE_OK:
		{
			sendMessage(kernel->simulator(), RT_SIMULATOR_GET_MODEL_EXITCODE, &m_exitCode);
			PTR(RDOStudioOutput) output = &studioApp.m_pMainFrame->output;
			std::vector<RDOSyntaxError> errors;
			studioApp.m_pStudioGUI->sendMessage(kernel->simulator(), RDOThread::RT_SIMULATOR_GET_ERRORS, &errors);
			int errors_cnt   = 0;
			int warnings_cnt = 0;
			STL_FOR_ALL_CONST(errors, it)
			{
				output->appendStringToBuild(it->m_code, it->m_message, it->m_file, it->m_line, it->m_pos, it->m_warning);
				if (it->m_warning)
				{
					warnings_cnt++;
				}
				else
				{
					errors_cnt++;
				}
			}
			output->appendStringToBuild(rdo::format(IDS_MODEL_BUILDING_RESULTS, errors_cnt, warnings_cnt));
			if (errors_cnt || warnings_cnt)
			{
//				const_cast<PTR(rdoEditCtrl::RDOBuildEdit)>(output->getBuild())->showFirstError();
			}
			if (plugins)
			{
				plugins->pluginProc(rdoPlugin::PM_MODEL_BUILD_OK);
			}
			m_buildState = BS_COMPLETE;
			PTR(RPMethodProc2RDO_MJ) pMethod = getProc2rdo();
			if (pMethod && pMethod->checkModelStructure())
			{
				pMethod->generate();
			}
			::GetSystemTime(&m_timeStart);
			break;
		}
		case RDOThread::RT_SIMULATOR_PARSE_ERROR:
		{
			m_GUI_IS_RUNING = false;
			sendMessage(kernel->simulator(), RT_SIMULATOR_GET_MODEL_EXITCODE, &m_exitCode);
			PTR(RDOStudioOutput) output = &studioApp.m_pMainFrame->output;
			std::vector<RDOSyntaxError> errors;
			studioApp.m_pStudioGUI->sendMessage(kernel->simulator(), RDOThread::RT_SIMULATOR_GET_ERRORS, &errors);
			int errors_cnt   = 0;
			int warnings_cnt = 0;
			STL_FOR_ALL_CONST(errors, it)
			{
				output->appendStringToBuild(it->m_code, it->m_message, it->m_file, it->m_line, it->m_pos, it->m_warning);
				if (it->m_warning)
				{
					warnings_cnt++;
				}
				else
				{
					errors_cnt++;
				}
			}
			output->appendStringToBuild(rdo::format(IDS_MODEL_BUILDING_RESULTS, errors_cnt, warnings_cnt));
			if (errors_cnt || warnings_cnt)
			{
				const_cast<PTR(rdoEditCtrl::RDOBuildEdit)>(output->getBuild())->showFirstError();
			}

			if (plugins)
			{
				plugins->pluginProc(rdoPlugin::PM_MODEL_BUILD_FAILD);
			}

			m_GUI_CAN_RUN = true;
			m_buildState  = BS_ERROR;
			studioApp.autoCloseByModel();
			break;
		}
		case RDOThread::RT_SIMULATOR_PARSE_ERROR_SMR_EMPTY:
		{
			m_smrEmptyError = true;
			m_buildState    = BS_ERROR;
			break;
		}
		case RDOThread::RT_SIMULATOR_PARSE_STRING:
		{
			msg.lock();
			studioApp.m_pMainFrame->output.appendStringToBuild(*static_cast<PTR(tstring)>(msg.param));
			msg.unlock();
			break;
		}
		case RDOThread::RT_DEBUG_STRING:
		{
			msg.lock();
			studioApp.m_pMainFrame->output.appendStringToDebug(*static_cast<PTR(tstring)>(msg.param));
			msg.unlock();
			break;
		}
		case RDOThread::RT_RESULT_STRING:
		{
			msg.lock();
			studioApp.m_pMainFrame->output.appendStringToResults(*static_cast<PTR(tstring)>(msg.param));
			msg.unlock();
			break;
		}
	}
}

void RDOStudioModel::show_result()
{
	rdo::textstream model_results;
	sendMessage(kernel->simulator(), RT_SIMULATOR_GET_MODEL_RESULTS, &model_results);
	tstring str = model_results.str();
	if (!str.empty())
	{
		PTR(RDOStudioOutput) output = &studioApp.m_pMainFrame->output;
		rdoRepository::RDOThreadRepository::FileInfo data(rdoModelObjects::PMV);
		studioApp.m_pStudioGUI->sendMessage(kernel->repository(), RDOThread::RT_REPOSITORY_MODEL_GET_FILEINFO, &data);
		if (!data.m_described)
		{
			output->appendStringToDebug(_T("Результаты не будут записаны в файл, т.к. в SMR не определен Results_file\n"));
		}
		output->getResults()->clearAll();
		output->showResults();
		output->appendStringToResults(str);
	}
}

rbool RDOStudioModel::newModel(tstring _model_name, tstring _model_path, const int _useTemplate )
{
	if (!plugins->canAction(rdoPlugin::maCreate))
		return false;

	m_useTemplate = _useTemplate;
	PTR(RDOStudioOutput) output = &studioApp.m_pMainFrame->output;
	output->clearBuild();
	output->clearDebug();
	output->clearResults();
	output->clearFind();
	rdoRepository::RDOThreadRepository::NewModel data;
	data.m_name = _model_name;
	data.m_path = _model_path;
	studioApp.broadcastMessage(RDOThread::RT_STUDIO_MODEL_NEW, &data);
	output->updateLogConnection();
	return true;
}

rbool RDOStudioModel::openModel(CREF(tstring) modelName) const
{
	if (!plugins->canAction(rdoPlugin::maOpen))
		return false;

	if (isRunning())
	{
		AfxGetMainWnd()->MessageBox(rdo::format(ID_MSG_MODEL_NEED_STOPED_FOR_OPEN).c_str(), NULL, MB_ICONEXCLAMATION | MB_OK);
		return false;
	}
	if (!closeModel())
	{
		return false;
	}
	PTR(RDOStudioOutput) output = &studioApp.m_pMainFrame->output;
	output->clearBuild();
	output->clearDebug();
	output->clearResults();
	output->clearFind();
	output->showDebug();
	output->appendStringToDebug(rdo::format(IDS_MODEL_LOADING_BEGIN));
	const_cast<PTR(rdoEditCtrl::RDODebugEdit)>(output->getDebug())->UpdateWindow();
	m_openError     = false;
	m_smrEmptyError = false;
	m_modelClosed   = false;
	rdoRepository::RDOThreadRepository::OpenFile data(modelName);
	studioApp.broadcastMessage(RDOThread::RT_STUDIO_MODEL_OPEN, &data);
	if (data.m_result && !m_openError && !m_smrEmptyError)
	{
		rdo::binarystream stream;
		rdoRepository::RDOThreadRepository::FileData fileData(rdoModelObjects::PMV, stream);
		studioApp.m_pStudioGUI->sendMessage(kernel->repository(), RDOThread::RT_REPOSITORY_LOAD, &fileData);
		output->appendStringToResults(stream.str());
		output->updateLogConnection();
		output->appendStringToDebug(rdo::format(IDS_MODEL_LOADING_OK));
		studioApp.setLastProjectName(getFullName());
	}
	else
	{
		if (m_smrEmptyError)
		{
			studioApp.m_pMainFrame->MessageBox(_T("В smr-файле не найдено имя модели"), _T("Ошибка открытия модели"), MB_OK | MB_ICONERROR);
			closeModel();
		}
		else
		{
			output->updateLogConnection();
			output->appendStringToDebug(rdo::format(IDS_MODEL_LOADING_FAILD));
		}
	}
	return data.m_result;
}

rbool RDOStudioModel::saveModel() const
{
	if (!plugins->canAction(rdoPlugin::maSave))
		return false;

	rbool res = true;
	studioApp.broadcastMessage(RDOThread::RT_STUDIO_MODEL_SAVE, &res);
	return res;
}

void RDOStudioModel::saveAsModel() const
{
	studioApp.broadcastMessage(RDOThread::RT_STUDIO_MODEL_SAVE_AS);
}

rbool RDOStudioModel::closeModel() const
{
	if (!plugins->canAction(rdoPlugin::maClose))
		return false;

	if (!isRunning())
	{
		stopModel();
		PTR(RDOStudioOutput) output = &studioApp.m_pMainFrame->output;
		if (output && output->GetSafeHwnd())
		{
			output->clearBuild();
			output->clearDebug();
			output->clearResults();
			output->clearFind();
		}
		studioApp.broadcastMessage(RDOThread::RT_STUDIO_MODEL_CLOSE);
		return true;
	}
	else
	{
		AfxGetMainWnd()->MessageBox(rdo::format(ID_MSG_MODEL_NEED_STOPED_FOR_CLOSE).c_str(), NULL, MB_ICONWARNING | MB_OK);
		return false;
	}
}

rbool RDOStudioModel::buildModel()
{
	if (!plugins->canAction(rdoPlugin::maBuild))
		return false;

	if (hasModel() && !isRunning() && saveModel())
	{
		PTR(RDOStudioOutput) output = &studioApp.m_pMainFrame->output;
		output->clearBuild();
		output->clearDebug();
		output->clearResults();
		output->showBuild();
		output->appendStringToBuild(rdo::format(IDS_MODEL_BUILDING_BEGIN));
		const_cast<PTR(rdoEditCtrl::RDOBuildEdit)>(output->getBuild())->UpdateWindow();
		m_buildState = BS_UNDEFINED;
		studioApp.broadcastMessage(RDOThread::RT_STUDIO_MODEL_BUILD);
		return m_buildState == BS_COMPLETE;
	}
	return false;
}

rbool RDOStudioModel::runModel()
{
	if (buildModel())
	{
		if (!plugins->canAction(rdoPlugin::maRun))
			return false;

		m_GUI_CAN_RUN = false;
		PTR(RDOStudioOutput) output = &studioApp.m_pMainFrame->output;
		output->clearBuild();
		output->clearDebug();
		output->clearResults();
		output->showBuild();
		const_cast<PTR(rdoEditCtrl::RDOBuildEdit)>(output->getBuild())->UpdateWindow();
		studioApp.broadcastMessage(RDOThread::RT_STUDIO_MODEL_RUN);
		return true;
	}
	return false;
}

rbool RDOStudioModel::stopModel() const
{
	if (hasModel() && isRunning())
	{
		studioApp.broadcastMessage(RDOThread::RT_STUDIO_MODEL_STOP);
		return true;
	}
	return false;
}

void RDOStudioModel::newModelFromRepository()
{
	if (m_pModelDocTemplate)
	{
		m_GUI_HAS_MODEL = true;

		PTR(RPMethodProc2RDO_MJ) pMethod = getProc2rdo();
		if (pMethod)
		{
			m_pFlowchartDocTemplate->OpenDocumentFile(NULL);
			pMethod->makeFlowChart(rpMethod::project);
		}

		BOOL maximize = false;
		if (!studioApp.m_pMainFrame->MDIGetActive(&maximize))
		{
			maximize = true;
		}

		m_pModelDocTemplate->OpenDocumentFile(NULL);
		rdoRepository::RDOThreadRepository::FileInfo data_smr(rdoModelObjects::RDOX);
		studioApp.m_pStudioGUI->sendMessage(kernel->repository(), RDOThread::RT_REPOSITORY_MODEL_GET_FILEINFO, &data_smr);
		setName(data_smr.m_name);

		PTR(RDOEditorTabCtrl) pTab = getTab();
		if (pTab)
		{
			for (int i = 0; i < pTab->getItemCount(); i++)
			{
				PTR(RDOEditorEdit) edit = pTab->getItemEdit(i);
				edit->setReadOnly(false);
				edit->clearAll();
				if (m_useTemplate != -1 && m_modelTemplates.find(m_useTemplate) != m_modelTemplates.end())
				{
					ModelTemplate::const_iterator it = m_modelTemplates[ m_useTemplate ].find(pTab->indexToType(i));
					if (it != m_modelTemplates[ m_useTemplate ].end())
					{
						ruint resID = it->second.m_resID;
						if (resID != - 1)
						{
							// Пытаемся загрузить из String Table
							tstring s = rdo::format(resID);
							if (s.empty())
							{
								// Загрузка из String Table не удалась, пытаемся загрузить из MODEL_TMP (a-la RCDATA)
								HRSRC res = ::FindResource(studioApp.m_hInstance, MAKEINTRESOURCE(resID), _T("MODEL_TMP"));
								if (res)
								{
									HGLOBAL res_global = ::LoadResource(studioApp.m_hInstance, res);
									if (res_global)
									{
										LPTSTR res_data = static_cast<LPTSTR>(::LockResource(res_global));
										if (res_data)
										{
											DWORD dwSize = ::SizeofResource(studioApp.m_hInstance, res);
											CString s_res;
											LPTSTR s_res_data = s_res.GetBuffer(dwSize + 1);
											memcpy(s_res_data, res_data, dwSize);
											s_res_data[dwSize] = NULL;
											s_res.ReleaseBuffer();
											s = s_res;
										}
									}
								}
							}
							if (pTab->indexToType(i) == rdoModelObjects::SMR)
							{
								tstring name = getName();
								s = rdo::format(s.c_str(), name.c_str(), name.c_str(), name.c_str(), name.c_str(), name.c_str(), name.c_str(), name.c_str());
							}
							if (!s.empty())
							{
								edit->replaceCurrent(s);
								edit->updateEditGUI();
								edit->scrollToLine(0);
								edit->setCurrentPos(it->second.m_position);
							}
						}
					}
				}
//				edit->setModifyFalse();
//				edit->clearUndoBuffer();
			}
		}

		PTR(CWnd) wnd = studioApp.m_pMainFrame->GetActiveFrame();
		if (maximize && wnd && wnd != studioApp.m_pMainFrame)
		{
			studioApp.m_pMainFrame->MDIMaximize(wnd);
		}
		studioApp.setLastProjectName(getFullName());
		if (m_useTemplate != -1)
		{
			saveModel();
		}
	}
}

void RDOStudioModel::openModelFromRepository()
{
	if (m_pModelDocTemplate)
	{
		m_GUI_HAS_MODEL = true;

		PTR(RPMethodProc2RDO_MJ) pMethod = getProc2rdo();
		if (pMethod)
		{
			m_pFlowchartDocTemplate->OpenDocumentFile(NULL);
			loadFromXML();
		}

		BOOL maximize = false;
		if (!studioApp.m_pMainFrame->MDIGetActive(&maximize))
		{
			maximize = true;
		}

		PTR(CWnd) active = CWnd::GetActiveWindow();
		m_pModelDocTemplate->OpenDocumentFile(NULL);
		rdoRepository::RDOThreadRepository::FileInfo data_smr(rdoModelObjects::RDOX);
		studioApp.m_pStudioGUI->sendMessage(kernel->repository(), RDOThread::RT_REPOSITORY_MODEL_GET_FILEINFO, &data_smr);
		setName(data_smr.m_name);

		PTR(RDOStudioOutput) output = &studioApp.m_pMainFrame->output;
		PTR(RDOEditorTabCtrl) pTab = getTab();
		if (pTab)
		{
			int cnt = pTab->getItemCount();
			studioApp.m_pMainFrame->beginProgress(0, cnt * 2 + 1);
			studioApp.m_pMainFrame->stepProgress();
			for (int i = 0; i < cnt; i++)
			{
				PTR(RDOEditorEdit) edit = pTab->getItemEdit(i);
				edit->setReadOnly(false);
				edit->clearAll();
				rdo::binarystream stream;
				rbool canLoad = true;
				rdoModelObjects::RDOFileType type = pTab->indexToType(i);
				if (pTab->typeSupported(type))
				{
					rdoRepository::RDOThreadRepository::FileData fileData(type, stream);
					studioApp.m_pStudioGUI->sendMessage(kernel->repository(), RDOThread::RT_REPOSITORY_LOAD, &fileData);
				}
				else
				{
					canLoad = false;
				}
				studioApp.m_pMainFrame->stepProgress();
				if (canLoad)
				{
					rdoRepository::RDOThreadRepository::FileInfo data(type);
					studioApp.m_pStudioGUI->sendMessage(kernel->repository(), RDOThread::RT_REPOSITORY_MODEL_GET_FILEINFO, &data);
					rbool stream_error = stream.rdstate() & std::ios_base::failbit ? true : false;
					if (!stream_error)
					{
						edit->load(stream);
						edit->setReadOnly(data.m_readOnly);
						if (data.m_readOnly)
						{
							output->appendStringToDebug(rdo::format(IDS_MODEL_FILE_READONLY, tstring(data.m_name + data.m_extention).c_str()));
						}
					}
					else
					{
						int obj = 0;
						switch (type)
						{
						case rdoModelObjects::RTP: obj = IDS_MODEL_RESOURCETYPES; break;
						case rdoModelObjects::RSS: obj = IDS_MODEL_RESOURCES;     break;
						case rdoModelObjects::EVN: obj = IDS_MODEL_EVENTS;        break;
						case rdoModelObjects::PAT: obj = IDS_MODEL_PATTERNS;      break;
						case rdoModelObjects::DPT: obj = IDS_MODEL_DPTS;          break;
						case rdoModelObjects::FRM: obj = IDS_MODEL_FRAMES;        break;
						case rdoModelObjects::FUN: obj = IDS_MODEL_FUNCTIONS;     break;
						case rdoModelObjects::PMD: obj = IDS_MODEL_PMDS;          break;
						}
						if (obj)
						{
							output->appendStringToDebug(rdo::format(IDS_MODEL_CANNOT_LOAD, rdo::format(obj).c_str(), data.m_fullName.c_str()));
							const_cast<PTR(rdoEditCtrl::RDODebugEdit)>(output->getDebug())->UpdateWindow();
						}
						m_openError = true;
					}
				}
				edit->setCurrentPos(0);
				edit->setModifyFalse();
				edit->clearUndoBuffer();
				studioApp.m_pMainFrame->stepProgress();
			}
			studioApp.m_pMainFrame->endProgress();
		}

		PTR(CWnd) wnd = studioApp.m_pMainFrame->GetActiveFrame();
		if (maximize && wnd && wnd != studioApp.m_pMainFrame)
		{
			studioApp.m_pMainFrame->MDIMaximize(wnd);
		}

		updateFrmDescribed();

		if (active) active->SetFocus();
	}
}

void RDOStudioModel::saveModelToRepository()
{
	rbool smr_modified = false;
	rbool wasSaved     = false;
	PTR(RDOEditorTabCtrl) pTab = getTab();
	if (pTab)
	{
		PTR(RDOEditorEdit) smr_edit = pTab->getItemEdit(rdoModelObjects::SMR);
		if (smr_edit->isModify())
		{
			rdo::binarystream stream;
			smr_edit->save(stream);
			m_smrEmptyError = false;
			rdoRepository::RDOThreadRepository::FileData fileData(rdoModelObjects::SMR, stream);
			studioApp.m_pStudioGUI->sendMessage(kernel->repository(), RDOThread::RT_REPOSITORY_SAVE, &fileData);
			if (m_smrEmptyError)
			{
				studioApp.m_pMainFrame->MessageBox(_T("В smr-файле не найдено имя модели, модель не будет записана"), _T("Ошибка записи модели"), MB_OK | MB_ICONERROR);
				return;
			}
			smr_modified = true;
		}
		int cnt = pTab->getItemCount();
		int progress_cnt = 0;
		for (int i = 0; i < cnt; i++)
		{
			if (smr_modified || pTab->getItemEdit(i)->isModify())
			{
				progress_cnt++;
			}
		}
		if (progress_cnt)
		{
			studioApp.m_pMainFrame->beginProgress(0, progress_cnt * 2 + 1);
			studioApp.m_pMainFrame->stepProgress();
			for (int i = 0; i < cnt; i++)
			{
				PTR(RDOEditorEdit) edit = pTab->getItemEdit(i);
				if (smr_modified || edit->isModify())
				{
					rdo::binarystream stream;
					edit->save(stream);
					studioApp.m_pMainFrame->stepProgress();
					rdoModelObjects::RDOFileType type = pTab->indexToType(i);
					switch (type)
					{
					case rdoModelObjects::RTP:
					case rdoModelObjects::RSS:
					case rdoModelObjects::EVN:
					case rdoModelObjects::PAT:
					case rdoModelObjects::DPT:
					case rdoModelObjects::PRC:
					case rdoModelObjects::FRM:
					case rdoModelObjects::FUN:
					case rdoModelObjects::PMD:
						{
							rdoRepository::RDOThreadRepository::FileData fileData(type, stream);
							studioApp.m_pStudioGUI->sendMessage(kernel->repository(), RDOThread::RT_REPOSITORY_SAVE, &fileData);
							break;
						}
					default: break;
					}
					edit->setModifyFalse();
				}
				studioApp.m_pMainFrame->stepProgress();
			}
			studioApp.m_pMainFrame->endProgress();
			wasSaved = true;
		}
	}
	rdoRepository::RDOThreadRepository::FileInfo data(rdoModelObjects::RDOX);
	studioApp.m_pStudioGUI->sendMessage(kernel->repository(), RDOThread::RT_REPOSITORY_MODEL_GET_FILEINFO, &data);
	setName(data.m_name);

	if (getProc2rdo())
	{
		saveToXML();
	}

	studioApp.insertReopenItem(getFullName());

	if (smr_modified)
	{
		updateFrmDescribed();
	}

	if (wasSaved && plugins)
	{
		plugins->pluginProc(rdoPlugin::PM_MODEL_SAVE);
	}
}

void RDOStudioModel::saveToXML()
{
	// Заводим документ:
	pugi::xml_document doc;
	// Пишем первый узел документа:
	pugi::xml_node node = doc.append_child(_T("Model"));
	// Ссылаемся на виртуальную функцию saveToXML(parentNode), которая поэтапно запишет информацию в файл:
	rpMethod::project->saveToXML(node);

	// Создаем файл '.prcx' с помощью репозитария:
	rdoRepository::RDOThreadRepository::FileInfo fileInfo(rdoModelObjects::PRCX);
	sendMessage(kernel->repository(), RDOThread::RT_REPOSITORY_MODEL_GET_FILEINFO, &fileInfo);

	// Автоматически открываем файл при создании потока:
	std::ofstream outFile(fileInfo.m_fullName.c_str());
	// Проверяем открытый нами поток на наличие ошибок ввода-вывода:
	if (outFile.good())
	{
		doc.save(outFile);
		outFile.close();
	}
}

void RDOStudioModel::loadFromXML()
{
	// Заводим документ:
	pugi::xml_document doc;

	rdoRepository::RDOThreadRepository::FileInfo fileInfo(rdoModelObjects::PRCX);
	studioApp.m_pStudioGUI->sendMessage(kernel->repository(), RDOThread::RT_REPOSITORY_MODEL_GET_FILEINFO, &fileInfo);

	// Открываем сохраненный xml-файл и проверяем поток на ошибки ввода-вывода:
	std::ifstream inFile(fileInfo.m_fullName.c_str());
	if (inFile.good())
	{
		// Загружаем документ и проверяем на предмет ошибок парсинга и пустого узла:
		if (doc.load(inFile))
		{
			pugi::xml_node node = doc.child(_T("Model"));
			if (!node.empty())
			{
				// Ссылаемся на виртуальную функцию loadFromXML(node), которая поэтапно загрузит графику из файла:
				rpMethod::project->loadFromXML(node.first_child());
			}
		}
		inFile.close();
	}
	else
	{
		PTR(RPMethodProc2RDO_MJ) pMethod = getProc2rdo();
		pMethod->makeFlowChart(rpMethod::project);
	}
}

tstring RDOStudioModel::getFullName() const
{
	rdoRepository::RDOThreadRepository::FileInfo data(rdoModelObjects::RDOX);
	studioApp.m_pStudioGUI->sendMessage(kernel->repository(), RDOThread::RT_REPOSITORY_MODEL_GET_FILEINFO, &data);
	return data.m_fullName;
}

void RDOStudioModel::updateFrmDescribed()
{
//	frmDescribed = kernel->repository()->isDescribed(rdoModelObjects::FRM);
	m_frmDescribed = true;
}

rbool RDOStudioModel::canCloseModel()
{
	rbool flag = true;
	if (isModify() && m_autoDeleteDoc)
	{
		int res = AfxGetMainWnd()->MessageBox(rdo::format(ID_MSG_MODELSAVE_QUERY).c_str(), NULL, MB_ICONQUESTION | MB_YESNOCANCEL);
		switch (res)
		{
		case IDYES   : flag = saveModel(); break;
		case IDNO    : flag = true; break;
		case IDCANCEL: flag = false; break;
		}
	}
	if (!flag)
	{
		m_showCanNotCloseModelMessage = false;
	}
	return flag;
}

void RDOStudioModel::closeModelFromRepository()
{
	if (m_autoDeleteDoc)
	{
		PTR(RDOStudioModelDoc) pDoc = getModelDoc();
		if (pDoc)
		{
			pDoc->OnCloseDocument();
		}
	}
	PTR(RPDoc) pFlowchartDoc = getFlowchartDoc();
	if (pFlowchartDoc)
	{
		pFlowchartDoc->OnCloseDocument();
	}
	m_GUI_HAS_MODEL = false;
	if (!m_showCanNotCloseModelMessage)
	{
		m_showCanNotCloseModelMessage = true;
	}
	m_modelClosed = true;
}

void RDOStudioModel::setName(CREF(tstring) str)
{
	PTR(RDOStudioModelDoc) pDoc = getModelDoc();
	rbool flag = false;
	if (pDoc)
	{
		flag = pDoc->getName() != str;
		pDoc->setName(str);
	}
	if (flag && plugins)
	{
		plugins->pluginProc(rdoPlugin::PM_MODEL_NAME_CHANGED);
	}
}

void RDOStudioModel::afterModelStart()
{
	m_frameManager.clear();

	if (isFrmDescribed())
	{
		PTR(RDOStudioOutput) output = &studioApp.m_pMainFrame->output;
		output->showDebug();
		output->appendStringToDebug(rdo::format(IDS_MODEL_RESOURCE_LOADING_BEGIN));
		const_cast<PTR(rdoEditCtrl::RDODebugEdit)>(output->getDebug())->UpdateWindow();

		std::list< tstring > frames;
		std::list< tstring > bitmaps;
		rdoSimulator::RDOThreadSimulator::GetList getListFrames (rdoSimulator::RDOThreadSimulator::GetList::frames,  &frames );
		rdoSimulator::RDOThreadSimulator::GetList getListBitmaps(rdoSimulator::RDOThreadSimulator::GetList::bitmaps, &bitmaps);
		sendMessage(kernel->simulator(), RT_SIMULATOR_GET_LIST, &getListFrames );
		sendMessage(kernel->simulator(), RT_SIMULATOR_GET_LIST, &getListBitmaps);
		STL_FOR_ALL_CONST(bitmaps, bmp_it)
		{
			m_frameManager.insertBitmap(*bmp_it);
		}
		STL_FOR_ALL_CONST(frames, frame_it)
		{
			m_frameManager.insertFrame(*frame_it);
		}
		m_frameManager.expand();
		m_timeNow = 0;
		ruint initFrameNumber = kernel->simulator()->getInitialFrameNumber();
		if (initFrameNumber != ruint(~0))
		{
			--initFrameNumber;
		}
		m_frameManager.setLastShowedFrame(initFrameNumber);
		if (getRuntimeMode() != rdoRuntime::RTM_MaxSpeed && initFrameNumber < m_frameManager.count())
		{
			PTR(RDOStudioFrameDoc) pDoc = m_frameManager.connectFrameDoc(initFrameNumber);
			if (pDoc)
			{
				m_frameManager.getFrameView(initFrameNumber)->SetFocus();
			}
		}
		output->appendStringToDebug(rdo::format(IDS_MODEL_RESOURCE_LOADING_OK));
		const_cast<PTR(rdoEditCtrl::RDODebugEdit)>(output->getDebug())->UpdateWindow();
	}
	else
	{
		m_timeNow = 0;
		m_frameManager.setLastShowedFrame(ruint(~0));
	}
}

void RDOStudioModel::updateStyleOfAllModel() const
{
	POSITION pos = m_pModelDocTemplate->GetFirstDocPosition();
	while (pos)
	{
		PTR(RDOStudioModelDoc) pDoc = static_cast<PTR(RDOStudioModelDoc)>(m_pModelDocTemplate->GetNextDoc(pos));
		PTR(RDOEditorTabCtrl) pTab = pDoc->getView()->tab;
		for (int i = 0; i < pTab->getItemCount(); i++)
		{
			pTab->getItemEdit(i)->setEditorStyle(&studioApp.m_pMainFrame->style_editor);
		}
	}
	m_frameManager.updateStyles();
}

void RDOStudioModel::setRuntimeMode(const rdoRuntime::RunTimeMode value)
{
	if (isRunning())
	{
		if (m_tempPause)
		{
			rdoRuntime::RunTimeMode _value = value;
			sendMessage(kernel->runtime(), RT_RUNTIME_SET_MODE, &_value);
			return;
		}
		m_runtimeMode = value;
		sendMessage(kernel->runtime(), RT_RUNTIME_SET_MODE, &m_runtimeMode);
		tracer->setRuntimeMode(m_runtimeMode);
		if (plugins)
		{
			plugins->pluginProc(rdoPlugin::PM_MODEL_RUNTIMEMODE);
		}
		switch (m_runtimeMode)
		{
			case rdoRuntime::RTM_MaxSpeed: closeAllFrame(); break;
			default:
			{
				PTR(RDOStudioFrameDoc) pDoc = m_frameManager.getFirstExistDoc();
				if (!pDoc)
				{
					m_frameManager.connectFrameDoc(m_frameManager.getLastShowedFrame());
				}
				break;
			}
		}
	}
}

tstring RDOStudioModel::getLastBreakPointName()
{
	tstring str;
	sendMessage(kernel->runtime(), RT_RUNTIME_GET_LAST_BREAKPOINT, &str);
	return str;
}

void RDOStudioModel::setSpeed(double persent)
{
	if (persent >= 0 && persent <= 1 && m_speed != persent)
	{
		m_speed = persent;
		if (isRunning())
		{
			sendMessage(kernel->runtime(), RT_RUNTIME_SET_SPEED, &m_speed);
		}
	}
}

void RDOStudioModel::setShowRate(double value)
{
	if (!isRunning())
		return;

	if (value >= DBL_MIN && value <= DBL_MAX)
	{
		m_showRate = value;
		sendMessage(kernel->runtime(), RT_RUNTIME_SET_SHOWRATE, &m_showRate);
	}
}

void RDOStudioModel::update()
{
	sendMessage(kernel->runtime(), RT_RUNTIME_GET_TIMENOW, &m_timeNow);
	rdoRuntime::RunTimeMode rm;
	sendMessage(kernel->runtime(), RT_RUNTIME_GET_MODE, &rm);
	if (rm != m_runtimeMode)
	{
		if (rm == rdoRuntime::RTM_BreakPoint)
		{
			PTR(RDOStudioOutput) output = &studioApp.m_pMainFrame->output;
			output->appendStringToDebug(rdo::format(_T("Пауза в %f из-за точки '%s'\n"), getTimeNow(), getLastBreakPointName().c_str()));
		}
		setRuntimeMode(rm);
	}
	if (getRuntimeMode() == rdoRuntime::RTM_MaxSpeed)
	{
		return;
	}
	int frames = getFrameCount();
	for (int i = 0; i < frames; i++)
	{
		if (m_frameManager.isChanged())
		{
			break;
		}
		PTR(RDOStudioFrameView) pView = m_frameManager.getFrameView(i);
		if (pView)
		{
			PTR(CDC) dc = pView->GetDC();
			if (dc->RectVisible(pView->getClientRect()))
			{
				pView->ReleaseDC(dc);
				try
				{
					rdoAnimation::RDOFrame frame;
					rdoRuntime::RDOThreadRunTime::GetFrame getFrame(&frame, i);
					sendMessage(kernel->runtime(), RT_RUNTIME_GET_FRAME, &getFrame);
					m_frameManager.showFrame(&frame, i);
				}
				catch (REF(rdoRuntime::RDORuntimeException))
				{
					sendMessage(kernel->runtime(), RT_SIMULATOR_MODEL_STOP_RUNTIME_DELAY);
					return;
				}
			}
			else
			{
				pView->ReleaseDC(dc);
			}
		}
	}
}

void RDOStudioModel::setGUIPause()
{
	m_tempPause = true;
	if (isRunning())
	{
		sendMessage(kernel->runtime(), RT_RUNTIME_GET_MODE, &m_runtimeModePrev);
		setRuntimeMode(rdoRuntime::RTM_Pause);
	}
}

void RDOStudioModel::setGUIContinue()
{
	m_tempPause = false;
	if (isRunning())
	{
		setRuntimeMode(m_runtimeModePrev);
	}
}
