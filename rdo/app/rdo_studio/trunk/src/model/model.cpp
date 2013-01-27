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
#include <boost/bind.hpp>
#include <limits>
#include <QtGui/qmessagebox.h>
#include <QtGui/qfiledialog.h>
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
#include "app/rdo_studio_mfc/src/main_windows_base.h"
#include "app/rdo_studio_mfc/src/frame/view.h"
#include "app/rdo_studio_mfc/rdo_edit/rdoeditortabctrl.h"
#include "app/rdo_studio_mfc/edit_ctrls/rdobuildedit.h"
#include "app/rdo_studio_mfc/edit_ctrls/rdodebugedit.h"
#include "app/rdo_studio_mfc/rdo_edit/rdoeditorresults.h"
#include "app/rdo_studio_mfc/rdo_tracer/rdotracer.h"
#include "app/rdo_studio_mfc/resource.h"
#include "thirdparty/pugixml/src/pugixml.hpp"
// --------------------------------------------------------------------------------

using namespace rdoEditor;
using namespace rdo::service::simulation;
using namespace rdo::simulation::report;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// -------------------- RDOStudioModel::ModelTemplateItem
// --------------------------------------------------------------------------------
RDOStudioModel::ModelTemplateItem::ModelTemplateItem()
	: m_resID   (ruint(~0))
	, m_position(~0       )
{}

RDOStudioModel::ModelTemplateItem::ModelTemplateItem(CREF(ModelTemplateItem) copy)
	: m_resID   (copy.m_resID   )
	, m_position(copy.m_position)
{}

RDOStudioModel::ModelTemplateItem::ModelTemplateItem(ruint resID, int position)
	: m_resID   (resID   )
	, m_position(position)
{}

// --------------------------------------------------------------------------------
// -------------------- RDOStudioModel
// --------------------------------------------------------------------------------
PTR(RDOStudioModel) model = NULL;

RDOStudioModel::RDOStudioModel()
	: RDOThreadGUI(_T("RDOThreadModelGUI"), static_cast<PTR(RDOKernelGUI)>(studioApp.m_pStudioGUI))
	, m_useTemplate      (-1                        )
	, m_autoDeleteDoc    (true                      )
	, m_showCanNotCloseModelMessage(true            )
	, m_GUI_HAS_MODEL    (false                     )
	, m_GUI_CAN_RUN      (true                      )
	, m_GUI_IS_RUNING    (false                     )
	, m_GUI_ACTION_NEW   (true                      )
	, m_GUI_ACTION_OPEN  (true                      )
	, m_GUI_ACTION_SAVE  (true                      )
	, m_GUI_ACTION_CLOSE (true                      )
	, m_GUI_ACTION_BUILD (true                      )
	, m_GUI_ACTION_RUN   (true                      )
	, m_openError        (false                     )
	, m_smrEmptyError    (false                     )
	, m_modelClosed      (true                      )
	, m_frmDescribed     (false                     )
	, m_timeNow          (0                         )
	, m_speed            (1                         )
	, m_showRate         (60                        )
	, m_tempPause        (false                     )
	, m_runtimeMode      (rdo::runtime::RTM_MaxSpeed)
	, m_runtimeModePrev  (rdo::runtime::RTM_MaxSpeed)
	, m_exitCode         (rdo::simulation::report::EC_ModelNotFound)
	, m_modify           (false                     )
	, m_buildState       (BS_UNDEFINED              )
	, m_pModelView       (NULL                      )
	, m_pModelProcView   (NULL                      )
	, m_name             ("")
{
	model = this;

	Ui::MainWindow* pMainWindow = studioApp.getMainWndUI();
	ASSERT(pMainWindow);
	connect(pMainWindow->actModelBuild, SIGNAL(triggered(bool)), this, SLOT(onModelBuild()));
	connect(pMainWindow->actModelRun,   SIGNAL(triggered(bool)), this, SLOT(onModelRun  ()));
	connect(pMainWindow->actModelStop,  SIGNAL(triggered(bool)), this, SLOT(onModelStop ()));

	m_timeNowSignal.connect(boost::bind(&RDOStudioMainFrame::onUpdateModelTime, static_cast<RDOStudioMainFrame*>(studioApp.getIMainWnd()), _1));

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
	notifies.push_back(RT_DEBUG_STRING                      );
	notifies.push_back(RT_RESULT_STRING                     );

	after_constructor();
}

RDOStudioModel::~RDOStudioModel()
{
	model = NULL;
//	closeModel();
}

rbool RDOStudioModel::init()
{
	PTR(IInit) pFrameManagerInit = dynamic_cast<PTR(IInit)>(&m_frameManager);
	ASSERT(pFrameManagerInit);
	pFrameManagerInit->init();

	return true;
}

void RDOStudioModel::proc(REF(RDOThread::RDOMessageInfo) msg)
{
	switch (msg.message)
	{
		case RDOThread::RT_STUDIO_MODEL_GET_TEXT:
		{
			msg.lock();
			PTR(rdo::repository::RDOThreadRepository::FileData) fdata = static_cast<PTR(rdo::repository::RDOThreadRepository::FileData)>(msg.param);
			PTR(RDOEditorEdit) edit = m_pModelView->getTab().getItemEdit(fdata->m_type);
			if (edit)
			{
				edit->save(fdata->m_stream);
			}
			msg.unlock();
			break;
		}
		case RDOThread::RT_REPOSITORY_MODEL_NEW:
		{
			newModelFromRepository();
			break;
		}
		case RDOThread::RT_REPOSITORY_MODEL_OPEN:
		{
			openModelFromRepository();
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
			PTR(rdo::repository::RDOThreadRepository::OpenFile) data = static_cast<PTR(rdo::repository::RDOThreadRepository::OpenFile)>(msg.param);

			QString modelName = QFileDialog::getOpenFileName(
				NULL,
				"Открыть модель",
				QString(),
				"РДО-проект (*.rdox);;РДО-конвертор (*.smr);;Все файлы (*.*)"
			);
			data->m_result   = !modelName.isEmpty();
			data->m_name     = modelName.toStdString();
			data->m_readOnly = false;

			msg.unlock();
			break;
		}
		case RDOThread::RT_REPOSITORY_MODEL_SAVE_GET_NAME:
		{
			msg.lock();
			PTR(rdo::repository::RDOThreadRepository::OpenFile) data = static_cast<PTR(rdo::repository::RDOThreadRepository::OpenFile)>(msg.param);
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
			break;
		}
		case RDOThread::RT_RUNTIME_MODEL_START_AFTER:
		{
			m_GUI_IS_RUNING = true;
			sendMessage(kernel->runtime(), RT_RUNTIME_GET_MODE, &m_runtimeMode);
			sendMessage(kernel->runtime(), RT_RUNTIME_GET_SPEED, &m_speed);
			setSpeed(studioApp.getIMainWnd()->getSpeed());
			sendMessage(kernel->runtime(), RT_RUNTIME_GET_SHOWRATE, &m_showRate);
			afterModelStart();
			studioApp.getIMainWnd()->getDockDebug().raise();
			studioApp.getIMainWnd()->getDockDebug().appendString(rdo::format(IDS_MODEL_STARTED));
			studioApp.getIMainWnd()->getDockDebug().getContext().update();
			int index = m_frameManager.getLastShowedFrame();
			if (index != -1)
			{
				PTR(FrameAnimationWnd) pView = m_frameManager.getFrameView(index);
				if (pView) pView->setFocus();
			}
			studioApp.getIMainWnd()->update_start();
			break;
		}
		case RDOThread::RT_RUNTIME_MODEL_STOP_BEFORE:
		{
			studioApp.getIMainWnd()->update_stop();
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
				studioApp.getIMainWnd()->getDockDebug().appendString(rdo::format(IDS_MODEL_DURATION, delay));
			}
			m_GUI_CAN_RUN   = true;
			m_GUI_IS_RUNING = false;
			break;
		}
		case RDOThread::RT_SIMULATOR_MODEL_STOP_OK:
		{
			studioApp.getIMainWnd()->getDockDebug().appendString(rdo::format(IDS_MODEL_FINISHED));
			studioApp.getIMainWnd()->getDockDebug().getContext().update();

			show_result();

			studioApp.autoCloseByModel();
			break;
		}
		case RDOThread::RT_SIMULATOR_MODEL_STOP_BY_USER:
		{
			sendMessage(kernel->simulator(), RT_SIMULATOR_GET_MODEL_EXITCODE, &m_exitCode);
			studioApp.getIMainWnd()->getDockDebug().appendString(rdo::format(IDS_MODEL_STOPED));
			studioApp.getIMainWnd()->getDockDebug().getContext().update();

			show_result();

			break;
		}
		case RDOThread::RT_SIMULATOR_MODEL_STOP_RUNTIME_ERROR:
		{
			sendMessage(kernel->simulator(), RT_SIMULATOR_GET_MODEL_EXITCODE, &m_exitCode);
			show_result();
			studioApp.getIMainWnd()->getDockDebug().appendString(rdo::format(IDS_MODEL_RUNTIMEERROR_STOPED));
			studioApp.getIMainWnd()->getDockBuild().clear();
			studioApp.getIMainWnd()->getDockBuild().raise();
			studioApp.getIMainWnd()->getDockBuild().appendString(rdo::format(IDS_MODEL_RUNTIMEERROR));
			std::vector< FileMessage > errors;
			studioApp.m_pStudioGUI->sendMessage(kernel->simulator(), RDOThread::RT_SIMULATOR_GET_ERRORS, &errors);
			int errors_cnt   = 0;
			int warnings_cnt = 0;
			STL_FOR_ALL_CONST(errors, it)
			{
				studioApp.getIMainWnd()->getDockBuild().appendString(*it);
				if (it->getType() == FileMessage::MT_WARNING)
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

			studioApp.autoCloseByModel();
			break;
		}
		case RDOThread::RT_SIMULATOR_PARSE_OK:
		{
			sendMessage(kernel->simulator(), RT_SIMULATOR_GET_MODEL_EXITCODE, &m_exitCode);
			std::vector<FileMessage> errors;
			studioApp.m_pStudioGUI->sendMessage(kernel->simulator(), RDOThread::RT_SIMULATOR_GET_ERRORS, &errors);
			int errors_cnt   = 0;
			int warnings_cnt = 0;
			STL_FOR_ALL_CONST(errors, it)
			{
				studioApp.getIMainWnd()->getDockBuild().appendString(*it);
				if (it->getType() == FileMessage::MT_WARNING)
				{
					warnings_cnt++;
				}
				else
				{
					errors_cnt++;
				}
			}
			studioApp.getIMainWnd()->getDockBuild().appendString(rdo::format(IDS_MODEL_BUILDING_RESULTS, errors_cnt, warnings_cnt));
			if (errors_cnt || warnings_cnt)
			{
//				studioApp.getIMainWnd()->getDockBuild().getContext().showFirstError();
			}
			m_buildState = BS_COMPLETE;
			PTR(RPMethodProc2RDO) pMethod = getProc2rdo();
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
			std::vector<FileMessage> errors;
			studioApp.m_pStudioGUI->sendMessage(kernel->simulator(), RDOThread::RT_SIMULATOR_GET_ERRORS, &errors);
			int errors_cnt   = 0;
			int warnings_cnt = 0;
			STL_FOR_ALL_CONST(errors, it)
			{
				studioApp.getIMainWnd()->getDockBuild().appendString(*it);
				if (it->getType() == FileMessage::MT_WARNING)
				{
					warnings_cnt++;
				}
				else
				{
					errors_cnt++;
				}
			}
			studioApp.getIMainWnd()->getDockBuild().appendString(rdo::format(IDS_MODEL_BUILDING_RESULTS, errors_cnt, warnings_cnt));
			if (errors_cnt || warnings_cnt)
			{
				studioApp.getIMainWnd()->getDockBuild().getContext().showFirstError();
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
			studioApp.getIMainWnd()->getDockBuild().appendString(*static_cast<PTR(tstring)>(msg.param));
			msg.unlock();
			break;
		}
		case RDOThread::RT_DEBUG_STRING:
		{
			msg.lock();
			studioApp.getIMainWnd()->getDockDebug().appendString(*static_cast<PTR(tstring)>(msg.param));
			msg.unlock();
			break;
		}
		case RDOThread::RT_RESULT_STRING:
		{
			msg.lock();
			studioApp.getIMainWnd()->getDockResults().appendString(*static_cast<PTR(tstring)>(msg.param));
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
		rdo::repository::RDOThreadRepository::FileInfo data(rdoModelObjects::PMV);
		studioApp.m_pStudioGUI->sendMessage(kernel->repository(), RDOThread::RT_REPOSITORY_MODEL_GET_FILEINFO, &data);
		if (!data.m_described)
		{
			studioApp.getIMainWnd()->getDockDebug().appendString(_T("Результаты не будут записаны в файл, т.к. в SMR не определен Results_file\n"));
		}
		studioApp.getIMainWnd()->getDockResults().getContext().clearAll();
		studioApp.getIMainWnd()->getDockResults().raise();
		studioApp.getIMainWnd()->getDockResults().appendString(str);
	}
}

rbool RDOStudioModel::newModel(tstring _model_name, tstring _model_path, const int _useTemplate )
{
	m_useTemplate = _useTemplate;
	studioApp.getIMainWnd()->getDockBuild  ().clear();
	studioApp.getIMainWnd()->getDockDebug  ().clear();
	studioApp.getIMainWnd()->getDockResults().clear();
	studioApp.getIMainWnd()->getDockFind   ().clear();
	rdo::repository::RDOThreadRepository::NewModel data;
	data.m_name = _model_name;
	data.m_path = _model_path;
	studioApp.broadcastMessage(RDOThread::RT_STUDIO_MODEL_NEW, &data);
	return true;
}

rbool RDOStudioModel::openModel(CREF(tstring) modelName) const
{
	if (isRunning())
	{
		AfxGetMainWnd()->MessageBox(rdo::format(ID_MSG_MODEL_NEED_STOPED_FOR_OPEN).c_str(), NULL, MB_ICONEXCLAMATION | MB_OK);
		return false;
	}
	if (!closeModel())
	{
		return false;
	}
	studioApp.getIMainWnd()->getDockBuild  ().clear();
	studioApp.getIMainWnd()->getDockDebug  ().clear();
	studioApp.getIMainWnd()->getDockResults().clear();
	studioApp.getIMainWnd()->getDockFind   ().clear();
	studioApp.getIMainWnd()->getDockDebug().raise();
	studioApp.getIMainWnd()->getDockDebug().appendString(rdo::format(IDS_MODEL_LOADING_BEGIN));
	studioApp.getIMainWnd()->getDockDebug().getContext().update();
	m_openError     = false;
	m_smrEmptyError = false;
	m_modelClosed   = false;
	rdo::repository::RDOThreadRepository::OpenFile data(modelName);
	studioApp.broadcastMessage(RDOThread::RT_STUDIO_MODEL_OPEN, &data);
	if (data.m_result && !m_openError && !m_smrEmptyError)
	{
		rdo::binarystream stream;
		rdo::repository::RDOThreadRepository::FileData fileData(rdoModelObjects::PMV, stream);
		studioApp.m_pStudioGUI->sendMessage(kernel->repository(), RDOThread::RT_REPOSITORY_LOAD, &fileData);
		studioApp.getIMainWnd()->getDockResults().appendString(stream.str());
		studioApp.getIMainWnd()->getDockDebug().appendString(rdo::format(IDS_MODEL_LOADING_OK));
		studioApp.setLastProjectName(getFullName());
	}
	else
	{
		m_modelClosed = true;
		if (m_smrEmptyError)
		{
			QMessageBox::critical(studioApp.getMainWnd(), _T("Ошибка открытия модели"), _T("В smr-файле не найдено имя модели"));
			closeModel();
		}
		else
		{
			studioApp.getIMainWnd()->getDockDebug().appendString(rdo::format(IDS_MODEL_LOADING_FAILD));
		}
	}
	return data.m_result;
}

rbool RDOStudioModel::saveModel() const
{
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
	if (!isRunning())
	{
		stopModel();
		studioApp.getIMainWnd()->getDockBuild  ().clear();
		studioApp.getIMainWnd()->getDockDebug  ().clear();
		studioApp.getIMainWnd()->getDockResults().clear();
		studioApp.getIMainWnd()->getDockFind   ().clear();
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
	if (hasModel() && !isRunning() && saveModel())
	{
		studioApp.getIMainWnd()->getDockBuild().clear();
		studioApp.getIMainWnd()->getDockDebug().clear();
		studioApp.getIMainWnd()->getDockResults().clear();
		studioApp.getIMainWnd()->getDockBuild().raise();
		studioApp.getIMainWnd()->getDockBuild().appendString(rdo::format(IDS_MODEL_BUILDING_BEGIN));
		studioApp.getIMainWnd()->getDockBuild().getContext().update();
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
		m_GUI_CAN_RUN = false;
		studioApp.getIMainWnd()->getDockBuild().clear();
		studioApp.getIMainWnd()->getDockDebug().clear();
		studioApp.getIMainWnd()->getDockResults().clear();
		studioApp.getIMainWnd()->getDockBuild().raise();
		studioApp.getIMainWnd()->getDockBuild().getContext().update();
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

void RDOStudioModel::createView()
{
	ASSERT(m_pModelView == NULL);
	m_pModelView = new RDOStudioModelView(NULL);
	studioApp.getIMainWnd()->addSubWindow(m_pModelView);
	m_pModelView->parentWidget()->setWindowIcon(QIcon(QString::fromUtf8(":/images/images/model.png")));
}

void RDOStudioModel::createProcView()
{
	ASSERT(m_pModelProcView == NULL);
	m_pModelProcView = new RPViewQt();
	studioApp.getIMainWnd()->addSubWindow(m_pModelProcView);
	m_pModelProcView->parentWidget()->setWindowIcon(QIcon(QString::fromUtf8(":/images/images/flowchart.png")));
}

void RDOStudioModel::newModelFromRepository()
{
	m_GUI_HAS_MODEL = true;

	PTR(RPMethodProc2RDO) pMethod = getProc2rdo();
	if (pMethod)
	{
		createProcView();
		pMethod->makeFlowChart(rpMethod::project);
	}

	createView();
	rdo::repository::RDOThreadRepository::FileInfo data_smr(rdoModelObjects::RDOX);
	studioApp.m_pStudioGUI->sendMessage(kernel->repository(), RDOThread::RT_REPOSITORY_MODEL_GET_FILEINFO, &data_smr);
	setName(data_smr.m_name);

	for (int i = 0; i < m_pModelView->getTab().count(); i++)
	{
		PTR(RDOEditorEdit) edit = m_pModelView->getTab().getItemEdit(i);
		edit->setReadOnly(false);
		edit->clearAll();
		if (m_useTemplate != -1 && m_modelTemplates.find(m_useTemplate) != m_modelTemplates.end())
		{
			ModelTemplate::const_iterator it = m_modelTemplates[ m_useTemplate ].find(m_pModelView->getTab().indexToType(i));
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
					if (m_pModelView->getTab().indexToType(i) == rdoModelObjects::SMR)
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
//			edit->setModifyFalse();
//			edit->clearUndoBuffer();
		}
	}

	studioApp.setLastProjectName(getFullName());
	if (m_useTemplate != -1)
	{
		saveModel();
	}
}

void RDOStudioModel::openModelFromRepository()
{
	m_GUI_HAS_MODEL = true;

	PTR(RPMethodProc2RDO) pMethod = getProc2rdo();
	if (pMethod)
	{
		createProcView();
		loadFromXML();
	}

	PTR(CWnd) active = CWnd::GetActiveWindow();
	createView();
	rdo::repository::RDOThreadRepository::FileInfo data_smr(rdoModelObjects::RDOX);
	studioApp.m_pStudioGUI->sendMessage(kernel->repository(), RDOThread::RT_REPOSITORY_MODEL_GET_FILEINFO, &data_smr);
	setName(data_smr.m_name);

	int cnt = m_pModelView->getTab().count();
	studioApp.getIMainWnd()->beginProgress(0, cnt * 2 + 1);
	studioApp.getIMainWnd()->stepProgress();
	for (int i = 0; i < cnt; i++)
	{
		PTR(RDOEditorEdit) edit = m_pModelView->getTab().getItemEdit(i);
		edit->setReadOnly(false);
		edit->clearAll();
		rdo::binarystream stream;
		rbool canLoad = true;
		rdoModelObjects::RDOFileType type = m_pModelView->getTab().indexToType(i);
		if (m_pModelView->getTab().typeSupported(type))
		{
			rdo::repository::RDOThreadRepository::FileData fileData(type, stream);
			studioApp.m_pStudioGUI->sendMessage(kernel->repository(), RDOThread::RT_REPOSITORY_LOAD, &fileData);
		}
		else
		{
			canLoad = false;
		}
		studioApp.getIMainWnd()->stepProgress();
		if (canLoad)
		{
			rdo::repository::RDOThreadRepository::FileInfo data(type);
			studioApp.m_pStudioGUI->sendMessage(kernel->repository(), RDOThread::RT_REPOSITORY_MODEL_GET_FILEINFO, &data);
			rbool stream_error = stream.rdstate() & std::ios_base::failbit ? true : false;
			if (!stream_error)
			{
				edit->load(stream);
				edit->setReadOnly(data.m_readOnly);
				if (data.m_readOnly)
				{
					studioApp.getIMainWnd()->getDockDebug().appendString(rdo::format(IDS_MODEL_FILE_READONLY, tstring(data.m_name + data.m_extention).c_str()));
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
					studioApp.getIMainWnd()->getDockDebug().appendString(rdo::format(IDS_MODEL_CANNOT_LOAD, rdo::format(obj).c_str(), data.m_fullName.c_str()));
					studioApp.getIMainWnd()->getDockDebug().getContext().update();
				}
				m_openError = true;
			}
		}
		edit->setCurrentPos(0);
		edit->setModifyFalse();
		edit->clearUndoBuffer();
		studioApp.getIMainWnd()->stepProgress();
	}
	studioApp.getIMainWnd()->endProgress();

	updateFrmDescribed();

	if (active) active->SetFocus();
}

void RDOStudioModel::saveModelToRepository()
{
	rbool smr_modified = false;
	rbool wasSaved     = false;
	PTR(RDOEditorEdit) smr_edit = m_pModelView->getTab().getItemEdit(rdoModelObjects::SMR);
	if (smr_edit->isModify())
	{
		rdo::binarystream stream;
		smr_edit->save(stream);
		m_smrEmptyError = false;
		rdo::repository::RDOThreadRepository::FileData fileData(rdoModelObjects::SMR, stream);
		studioApp.m_pStudioGUI->sendMessage(kernel->repository(), RDOThread::RT_REPOSITORY_SAVE, &fileData);
		if (m_smrEmptyError)
		{
			QMessageBox::critical(studioApp.getMainWnd(), _T("Ошибка записи модели"), _T("В smr-файле не найдено имя модели, модель не будет записана"));
			return;
		}
		smr_modified = true;
	}

	int cnt = m_pModelView->getTab().count();
	int progress_cnt = 0;
	for (int i = 0; i < cnt; i++)
	{
		if (smr_modified || m_pModelView->getTab().getItemEdit(i)->isModify())
		{
			progress_cnt++;
		}
	}
	if (progress_cnt)
	{
		studioApp.getIMainWnd()->beginProgress(0, progress_cnt * 2 + 1);
		studioApp.getIMainWnd()->stepProgress();
		for (int i = 0; i < cnt; i++)
		{
			PTR(RDOEditorEdit) edit = m_pModelView->getTab().getItemEdit(i);
			if (smr_modified || edit->isModify())
			{
				rdo::binarystream stream;
				edit->save(stream);
				studioApp.getIMainWnd()->stepProgress();
				rdoModelObjects::RDOFileType type = m_pModelView->getTab().indexToType(i);
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
						rdo::repository::RDOThreadRepository::FileData fileData(type, stream);
						studioApp.m_pStudioGUI->sendMessage(kernel->repository(), RDOThread::RT_REPOSITORY_SAVE, &fileData);
						break;
					}
				default: break;
				}
				edit->setModifyFalse();
			}
			studioApp.getIMainWnd()->stepProgress();
		}
		studioApp.getIMainWnd()->endProgress();
		wasSaved = true;
	}

	rdo::repository::RDOThreadRepository::FileInfo data(rdoModelObjects::RDOX);
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
	rdo::repository::RDOThreadRepository::FileInfo fileInfo(rdoModelObjects::PRCX);
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

	rdo::repository::RDOThreadRepository::FileInfo fileInfo(rdoModelObjects::PRCX);
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
		PTR(RPMethodProc2RDO) pMethod = getProc2rdo();
		pMethod->makeFlowChart(rpMethod::project);
	}
}

tstring RDOStudioModel::getFullName() const
{
	rdo::repository::RDOThreadRepository::FileInfo data(rdoModelObjects::RDOX);
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
	rbool result = true;
	if (isModify() && m_autoDeleteDoc)
	{
		switch (QMessageBox::question(studioApp.getMainWnd(), "RAO-Studio", rdo::format(ID_MSG_MODELSAVE_QUERY).c_str(), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel))
		{
			case QMessageBox::Yes   : result = saveModel(); break;
			case QMessageBox::No    : result = true; break;
			case QMessageBox::Cancel: result = false; break;
		}
	}
	if (!result)
	{
		m_showCanNotCloseModelMessage = false;
	}
	return result;
}

void RDOStudioModel::closeModelFromRepository()
{
	if (m_autoDeleteDoc)
	{
		if (isModify())
		{
			if (!saveModified())
				return;
		}
	}
	if (m_pModelProcView)
	{
		m_pModelProcView->parentWidget()->close();
		m_pModelProcView = NULL;
	}
	m_GUI_HAS_MODEL = false;
	if (!m_showCanNotCloseModelMessage)
	{
		m_showCanNotCloseModelMessage = true;
	}
	m_pModelView->parentWidget()->close();
	m_pModelView  = NULL;
	m_modelClosed = true;
	setName("");
}

CREF(tstring) RDOStudioModel::getName() const
{
	return m_name;
}

void RDOStudioModel::setName(CREF(tstring) str)
{
	tstring newName(str);
	rdo::trim(newName);

	if (m_name != newName)
	{
		m_name = newName;

		if (m_pModelView)
		{
			if (studioApp.getShowCaptionFullName())
			{
				m_pModelView->parentWidget()->setWindowTitle(QString::fromStdString(rdo::format(IDS_MODEL_NAME, getFullName().c_str())));
			}
			else
			{
				m_pModelView->parentWidget()->setWindowTitle(QString::fromStdString(rdo::format(IDS_MODEL_NAME, m_name.c_str())));
			}
		}
	}
}

void RDOStudioModel::afterModelStart()
{
	m_frameManager.clear();

	if (isFrmDescribed())
	{
		studioApp.getIMainWnd()->getDockDebug().raise();
		studioApp.getIMainWnd()->getDockDebug().appendString(rdo::format(IDS_MODEL_RESOURCE_LOADING_BEGIN));
		studioApp.getIMainWnd()->getDockDebug().getContext().update();

		std::list< tstring > frames;
		std::list< tstring > bitmaps;
		rdo::service::simulation::RDOThreadSimulator::GetList getListFrames (rdo::service::simulation::RDOThreadSimulator::GetList::frames,  &frames );
		rdo::service::simulation::RDOThreadSimulator::GetList getListBitmaps(rdo::service::simulation::RDOThreadSimulator::GetList::bitmaps, &bitmaps);
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
		m_timeNow = 0;
		ruint initFrameNumber = kernel->simulator()->getInitialFrameNumber();
		if (initFrameNumber != ruint(~0))
		{
			--initFrameNumber;
		}
		m_frameManager.setLastShowedFrame(initFrameNumber);
		if (getRuntimeMode() != rdo::runtime::RTM_MaxSpeed && initFrameNumber < m_frameManager.count())
		{
			PTR(FrameAnimationWnd) pView = m_frameManager.createView(initFrameNumber);
			if (pView)
			{
				m_frameManager.getFrameView(initFrameNumber)->setFocus();
			}
		}
		studioApp.getIMainWnd()->getDockDebug().appendString(rdo::format(IDS_MODEL_RESOURCE_LOADING_OK));
		studioApp.getIMainWnd()->getDockDebug().getContext().update();
	}
	else
	{
		m_timeNow = 0;
		m_frameManager.setLastShowedFrame(ruint(~0));
	}
}

PTR(RPMethodProc2RDO) RDOStudioModel::getProc2rdo() const
{
	RPMethodManager::MethodList::const_iterator it = studioApp.getMethodManager().getList().begin();
	while (it != studioApp.getMethodManager().getList().end())
	{
		PTR(rpMethod::RPMethod) pMethod = *it;
		ASSERT(pMethod);
		if (pMethod->getClassName() == _T("RPMethodProc2RDO"))
		{
			PTR(RPMethodProc2RDO) pProc2RDO = dynamic_cast<PTR(RPMethodProc2RDO)>(pMethod);
			ASSERT(pProc2RDO);
			return pProc2RDO;
		}
		it++;
	}
	return NULL;
}

void RDOStudioModel::updateStyleOfAllModel() const
{
	if (m_pModelView)
	{
		for (int i = 0; i < m_pModelView->getTab().count(); i++)
		{
			m_pModelView->getTab().getItemEdit(i)->setEditorStyle(&studioApp.getStyle()->style_editor);
		}
	}

	m_frameManager.updateStyles();
}

rbool RDOStudioModel::isPrevModelClosed() const
{
	return m_modelClosed;
}

void RDOStudioModel::setRuntimeMode(const rdo::runtime::RunTimeMode value)
{
	if (isRunning())
	{
		if (m_tempPause)
		{
			rdo::runtime::RunTimeMode _value = value;
			sendMessage(kernel->runtime(), RT_RUNTIME_SET_MODE, &_value);
			return;
		}
		m_runtimeMode = value;
		sendMessage(kernel->runtime(), RT_RUNTIME_SET_MODE, &m_runtimeMode);
		g_pTracer->setRuntimeMode(m_runtimeMode);
		switch (m_runtimeMode)
		{
			case rdo::runtime::RTM_MaxSpeed: closeAllFrame(); break;
			default:
			{
				PTR(FrameAnimationWnd) pView = m_frameManager.getFrameViewFirst();
				if (!pView)
				{
					m_frameManager.createView(m_frameManager.getLastShowedFrame());
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

double RDOStudioModel::getSpeed() const
{
	return m_speed;
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

double RDOStudioModel::getShowRate() const
{
	return m_showRate;
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

void RDOStudioModel::showNextFrame()
{
	m_frameManager.showNextFrame();
}

void RDOStudioModel::showPrevFrame()
{
	m_frameManager.showPrevFrame();
}

rbool RDOStudioModel::canShowNextFrame() const
{
	return m_frameManager.canShowNextFrame();
}

rbool RDOStudioModel::canShowPrevFrame() const
{
	return m_frameManager.canShowPrevFrame();
}

int RDOStudioModel::getFrameCount() const
{
	return m_frameManager.count();
}

CPTR(char) RDOStudioModel::getFrameName(int index) const
{
	return m_frameManager.getFrameName(index).c_str();
}

void RDOStudioModel::showFrame(int index)
{
	m_frameManager.showFrame(index);
}

void RDOStudioModel::closeAllFrame()
{
	m_frameManager.closeAll();
}

rbool RDOStudioModel::hasModel() const
{
	return m_GUI_HAS_MODEL;
}

void RDOStudioModel::update()
{
	sendMessage(kernel->runtime(), RT_RUNTIME_GET_TIMENOW, &m_timeNow);
	m_timeNowSignal(m_timeNow);
	rdo::runtime::RunTimeMode rm;
	sendMessage(kernel->runtime(), RT_RUNTIME_GET_MODE, &rm);
	if (rm != m_runtimeMode)
	{
		if (rm == rdo::runtime::RTM_BreakPoint)
		{
			studioApp.getIMainWnd()->getDockDebug().appendString(rdo::format(_T("Пауза в %f из-за точки '%s'\n"), getTimeNow(), getLastBreakPointName().c_str()));
		}
		setRuntimeMode(rm);
	}
	if (getRuntimeMode() == rdo::runtime::RTM_MaxSpeed)
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
		PTR(FrameAnimationWnd) pView = m_frameManager.getFrameView(i);
		if (pView)
		{
			//! @todo qt: переделать модель отрисовки.
			//! В pView::paintEvent запрашивать RT_RUNTIME_GET_FRAME, если был тик от таймера.
			//! Иначе отрисовывать текущий буфер.
			if (pView->isVisible())
			{
				try
				{
					rdo::animation::Frame frame;
					rdo::runtime::RDOThreadRunTime::GetFrame getFrame(&frame, i);
					sendMessage(kernel->runtime(), RT_RUNTIME_GET_FRAME, &getFrame);
					m_frameManager.showFrame(&frame, i);
				}
				catch (REF(rdo::runtime::RDORuntimeException))
				{
					sendMessage(kernel->runtime(), RT_SIMULATOR_MODEL_STOP_RUNTIME_DELAY);
					return;
				}
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
		setRuntimeMode(rdo::runtime::RTM_Pause);
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

rbool RDOStudioModel::isModify() const
{
	if (!m_pModelView)
		return false;

	rbool result = false;

	for (int i = 0; i < getTab()->count(); i++)
	{
		if (getTab()->getItemEdit( i )->isModify())
		{
			result = true;
			break;
		}
	}

	if (m_modify != result)
	{
		m_modify = result;
	}

	return result;
}

rbool RDOStudioModel::canNew() const
{
	return ((hasModel() && m_GUI_CAN_RUN) || !hasModel()) && m_GUI_ACTION_NEW;
}

rbool RDOStudioModel::canOpen() const
{
	return ((hasModel() && m_GUI_CAN_RUN) || !hasModel()) && m_GUI_ACTION_OPEN;
}

rbool RDOStudioModel::canSave() const
{
	return isModify() && m_GUI_ACTION_SAVE;
}

rbool RDOStudioModel::canClose() const
{
	return hasModel() && !isRunning() && m_GUI_ACTION_CLOSE;
}

rbool RDOStudioModel::canBuild() const
{
	return hasModel() && m_GUI_CAN_RUN && m_GUI_ACTION_BUILD;
}

rbool RDOStudioModel::canRun() const
{
	return hasModel() && m_GUI_CAN_RUN && m_GUI_ACTION_RUN;
}

rbool RDOStudioModel::isRunning() const
{
	return m_GUI_IS_RUNING;
}

rbool RDOStudioModel::isFrmDescribed() const
{
	return m_frmDescribed;
}

double RDOStudioModel::getTimeNow() const
{
	return m_timeNow;
}

rdo::simulation::report::RDOExitCode RDOStudioModel::getExitCode() const
{
	return m_exitCode;
}

rdo::runtime::RunTimeMode RDOStudioModel::getRuntimeMode() const
{
	return m_runtimeMode;
}

rbool RDOStudioModel::saveModified()
{
	if (isRunning())
	{
		QMessageBox::warning(studioApp.getMainWnd(), "RAO-Studio", rdo::format(ID_MSG_MODEL_NEED_STOPED_FOR_CLOSE).c_str());
		return false;
	}
	rbool result = true;

	if (isModify())
	{
		switch (QMessageBox::question(studioApp.getMainWnd(), "RAO-Studio", rdo::format(ID_MSG_MODELSAVE_QUERY).c_str(), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel))
		{
			case QMessageBox::Yes   : result = saveModel(); break;
			case QMessageBox::No    : result = true; break;
			case QMessageBox::Cancel: result = false; break;
		}
	}
	else if (!MainWindowBase::is_close_mode())
	{
		switch (QMessageBox::question(studioApp.getMainWnd(), "RAO-Studio", rdo::format(ID_MSG_MODELCLOSE_QUERY).c_str(), QMessageBox::Yes | QMessageBox::No))
		{
			case QMessageBox::Yes: result = true; break;
			case QMessageBox::No : result = false; break;
		}
	}

	if (result)
	{
		m_autoDeleteDoc = false;
		closeModel();
		m_autoDeleteDoc = true;
		m_GUI_HAS_MODEL = false;
	}
	return result;
}

REF(RDOStudioFrameManager) RDOStudioModel::getFrameManager()
{
	return m_frameManager;
}

PTR(RPViewQt) RDOStudioModel::getProcView()
{
	return m_pModelProcView;
}

PTR(rdoEditor::RDOEditorTabCtrl) RDOStudioModel::getTab()
{
	if (!m_pModelView)
		return NULL;

	return &m_pModelView->getTab();
}

CPTR(rdoEditor::RDOEditorTabCtrl) RDOStudioModel::getTab() const
{
	if (!m_pModelView)
		return NULL;

	return &m_pModelView->getTab();
}

void RDOStudioModel::onModelBuild()
{
	buildModel();
}

void RDOStudioModel::onModelRun()
{
	runModel();
}

void RDOStudioModel::onModelStop()
{
	stopModel();
}
