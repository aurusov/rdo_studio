/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio/src/model/model.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include "utils/warning_disable.h"
#include <limits>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <QMessageBox>
#include <QFileDialog>
#include "utils/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdostream.h"
#include "utils/rdoanimation.h"
#include "kernel/rdokernel.h"
#include "kernel/rdothread.h"
#include "repository/rdorepository.h"
#include "simulator/runtime/rdo_exception.h"
#include "app/rdo_studio/src/model/model.h"
#include "app/rdo_studio/src/model/model_view.h"
#include "app/rdo_studio/src/thread.h"
#include "app/rdo_studio/src/main_window.h"
#include "app/rdo_studio/src/frame/frame_view.h"
#include "app/rdo_studio/src/dialog/new_model_dialog.h"
#include "app/rdo_studio/src/model/model_tab_ctrl.h"
#include "app/rdo_studio/src/editor/build_edit.h"
#include "app/rdo_studio/src/editor/debug_edit.h"
#include "app/rdo_studio/src/editor/results_edit.h"
#include "app/rdo_studio/src/tracer/tracer.h"
// --------------------------------------------------------------------------------

using namespace rdo::gui::model;
using namespace rdo::service::simulation;
using namespace rdo::simulation::report;

// --------------------------------------------------------------------------------
// -------------------- Model::ModelTemplateItem
// --------------------------------------------------------------------------------
Model::ModelTemplateItem::ModelTemplateItem()
{}

Model::ModelTemplateItem::ModelTemplateItem(CREF(ModelTemplateItem) copy)
	: resName (copy.resName )
	, position(copy.position)
{}

Model::ModelTemplateItem::ModelTemplateItem(CREF(QString) resName)
	: resName(resName)
{}

Model::ModelTemplateItem::ModelTemplateItem(CREF(QString) resName, ruint position)
	: resName (resName )
	, position(position)
{}

// --------------------------------------------------------------------------------
// -------------------- Model
// --------------------------------------------------------------------------------
PTR(Model) g_pModel = NULL;

Model::Model()
	: RDOThreadGUI(_T("RDOThreadModelGUI"), static_cast<PTR(RDOKernelGUI)>(g_pApp->m_pStudioGUI))
	, m_frameManager   (boost::bind(&Model::onChangeFrame, this, _1))
	, m_GUI_HAS_MODEL  (false                     )
	, m_GUI_CAN_RUN    (true                      )
	, m_GUI_IS_RUNNING (false                     )
	, m_openError      (false                     )
	, m_smrEmptyError  (false                     )
	, m_modelClosed    (true                      )
	, m_frmDescribed   (false                     )
	, m_timeNow        (0                         )
	, m_speed          (1                         )
	, m_showRate       (60                        )
	, m_runtimeMode    (rdo::runtime::RTM_MaxSpeed)
	, m_exitCode       (rdo::simulation::report::EC_ModelNotFound)
	, m_modify         (false                     )
	, m_buildState     (BS_UNDEFINED              )
	, m_pView     (NULL                      )
	, m_name           ("")
{
	g_pModel = this;

	Ui::MainWindow* pMainWindow = g_pApp->getMainWndUI();
	ASSERT(pMainWindow);

	connect(pMainWindow->actFileNew,     SIGNAL(triggered(bool)), this, SLOT(onFileNew    ()));
	connect(pMainWindow->actFileOpen,    SIGNAL(triggered(bool)), this, SLOT(onFileOpen   ()));
	connect(pMainWindow->actFileClose,   SIGNAL(triggered(bool)), this, SLOT(onFileClose  ()));
	connect(pMainWindow->actFileSaveAll, SIGNAL(triggered(bool)), this, SLOT(onFileSaveAll()));

	connect(pMainWindow->actModelBuild, SIGNAL(triggered(bool)), this, SLOT(onModelBuild()));
	connect(pMainWindow->actModelRun,   SIGNAL(triggered(bool)), this, SLOT(onModelRun  ()));
	connect(pMainWindow->actModelStop,  SIGNAL(triggered(bool)), this, SLOT(onModelStop ()));

	connect(pMainWindow->actModelRuntimeMaxSpeed, SIGNAL(triggered(bool)), this, SLOT(onModelRuntimeMaxSpeed()));
	connect(pMainWindow->actModelRuntimeJump,     SIGNAL(triggered(bool)), this, SLOT(onModelRuntimeJump()));
	connect(pMainWindow->actModelRuntimeSync,     SIGNAL(triggered(bool)), this, SLOT(onModelRuntimeSync()));
	connect(pMainWindow->actModelRuntimePause,    SIGNAL(triggered(bool)), this, SLOT(onModelRuntimePause()));

	connect(pMainWindow->actModelShowRateInc,     SIGNAL(triggered(bool)), this, SLOT(onModelShowRateInc()));
	connect(pMainWindow->actModelShowRateIncFour, SIGNAL(triggered(bool)), this, SLOT(onModelShowRateIncFour()));
	connect(pMainWindow->actModelShowRateDecFour, SIGNAL(triggered(bool)), this, SLOT(onModelShowRateDecFour()));
	connect(pMainWindow->actModelShowRateDec,     SIGNAL(triggered(bool)), this, SLOT(onModelShowRateDec()));

	connect(g_pApp->getMainWndUI()->m_pModelSpeedSlider, SIGNAL(valueChanged(int)), this, SLOT(onModelSpeedValueChanged(int)));

	QActionGroup* runtimeGroup = new QActionGroup(this);
	runtimeGroup->addAction(pMainWindow->actModelRuntimeMaxSpeed);
	runtimeGroup->addAction(pMainWindow->actModelRuntimeJump);
	runtimeGroup->addAction(pMainWindow->actModelRuntimeSync);
	runtimeGroup->addAction(pMainWindow->actModelRuntimePause);

	connect(pMainWindow->actModelFrameNext, SIGNAL(triggered(bool)), this, SLOT(onShowNextFrame()));
	connect(pMainWindow->actModelFramePrev, SIGNAL(triggered(bool)), this, SLOT(onShowPrevFrame()));

	setHasModel (m_GUI_HAS_MODEL );
	setCanRun   (m_GUI_CAN_RUN   );
	setIsRunning(m_GUI_IS_RUNNING);

	ModelTemplate modelTemplate;
	modelTemplate[rdoModelObjects::SMR] = ModelTemplateItem(":/new_model_template/new_model_template/template_0/template_0.smr");
	m_modelTemplates[0] = modelTemplate;

	modelTemplate.clear();
	modelTemplate[rdoModelObjects::SMR] = ModelTemplateItem(":/new_model_template/new_model_template/template_1/template_1.smr");
	m_modelTemplates[1] = modelTemplate;

	modelTemplate.clear();
	modelTemplate[rdoModelObjects::RTP] = ModelTemplateItem(":/new_model_template/new_model_template/template_2/template_2.rtp", 15);
	modelTemplate[rdoModelObjects::RSS] = ModelTemplateItem(":/new_model_template/new_model_template/template_2/template_2.rss", 12);
	modelTemplate[rdoModelObjects::EVN] = ModelTemplateItem(":/new_model_template/new_model_template/template_2/template_2.evn", 9);
	modelTemplate[rdoModelObjects::PAT] = ModelTemplateItem(":/new_model_template/new_model_template/template_2/template_2.pat", 9);
	modelTemplate[rdoModelObjects::DPT] = ModelTemplateItem(":/new_model_template/new_model_template/template_2/template_2.dpt", 16);
	modelTemplate[rdoModelObjects::PRC] = ModelTemplateItem(":/new_model_template/new_model_template/template_2/template_2.prc", 10);
	modelTemplate[rdoModelObjects::FRM] = ModelTemplateItem(":/new_model_template/new_model_template/template_2/template_2.frm", 7);
	modelTemplate[rdoModelObjects::FUN] = ModelTemplateItem(":/new_model_template/new_model_template/template_2/template_2.fun", 10);
	modelTemplate[rdoModelObjects::SMR] = ModelTemplateItem(":/new_model_template/new_model_template/template_2/template_2.smr");
	modelTemplate[rdoModelObjects::PMD] = ModelTemplateItem(":/new_model_template/new_model_template/template_2/template_2.pmd", 10);
	m_modelTemplates[2] = modelTemplate;

	modelTemplate.clear();
	modelTemplate[rdoModelObjects::RTP] = ModelTemplateItem(":/new_model_template/new_model_template/template_3/template_3.rtp", 98);
	modelTemplate[rdoModelObjects::RSS] = ModelTemplateItem(":/new_model_template/new_model_template/template_3/template_3.rss", 58);
	modelTemplate[rdoModelObjects::EVN] = ModelTemplateItem(":/new_model_template/new_model_template/template_3/template_3.evn", 67);
	modelTemplate[rdoModelObjects::PAT] = ModelTemplateItem(":/new_model_template/new_model_template/template_3/template_3.pat", 89);
	modelTemplate[rdoModelObjects::DPT] = ModelTemplateItem(":/new_model_template/new_model_template/template_3/template_3.dpt", 132);
	modelTemplate[rdoModelObjects::PRC] = ModelTemplateItem(":/new_model_template/new_model_template/template_3/template_3.prc", 75);
	modelTemplate[rdoModelObjects::FRM] = ModelTemplateItem(":/new_model_template/new_model_template/template_3/template_3.frm", 84);
	modelTemplate[rdoModelObjects::FUN] = ModelTemplateItem(":/new_model_template/new_model_template/template_3/template_3.fun", 44);
	modelTemplate[rdoModelObjects::SMR] = ModelTemplateItem(":/new_model_template/new_model_template/template_3/template_3.smr");
	modelTemplate[rdoModelObjects::PMD] = ModelTemplateItem(":/new_model_template/new_model_template/template_3/template_3.pmd", 43);
	m_modelTemplates[3] = modelTemplate;

	modelTemplate.clear();
	modelTemplate[rdoModelObjects::RTP] = ModelTemplateItem(":/new_model_template/new_model_template/template_4/template_4.rtp");
	modelTemplate[rdoModelObjects::RSS] = ModelTemplateItem(":/new_model_template/new_model_template/template_4/template_4.rss");
	modelTemplate[rdoModelObjects::EVN] = ModelTemplateItem(":/new_model_template/new_model_template/template_4/template_4.evn");
	modelTemplate[rdoModelObjects::PAT] = ModelTemplateItem(":/new_model_template/new_model_template/template_4/template_4.pat");
	modelTemplate[rdoModelObjects::DPT] = ModelTemplateItem(":/new_model_template/new_model_template/template_4/template_4.dpt");
	modelTemplate[rdoModelObjects::FUN] = ModelTemplateItem(":/new_model_template/new_model_template/template_4/template_4.fun");
	modelTemplate[rdoModelObjects::SMR] = ModelTemplateItem(":/new_model_template/new_model_template/template_4/template_4.smr");
	modelTemplate[rdoModelObjects::PMD] = ModelTemplateItem(":/new_model_template/new_model_template/template_4/template_4.pmd");
	m_modelTemplates[4] = modelTemplate;

	modelTemplate.clear();
	modelTemplate[rdoModelObjects::RTP] = ModelTemplateItem(":/new_model_template/new_model_template/template_5/template_5.rtp");
	modelTemplate[rdoModelObjects::RSS] = ModelTemplateItem(":/new_model_template/new_model_template/template_5/template_5.rss");
	modelTemplate[rdoModelObjects::EVN] = ModelTemplateItem(":/new_model_template/new_model_template/template_5/template_5.evn");
	modelTemplate[rdoModelObjects::PAT] = ModelTemplateItem(":/new_model_template/new_model_template/template_5/template_5.pat");
	modelTemplate[rdoModelObjects::DPT] = ModelTemplateItem(":/new_model_template/new_model_template/template_5/template_5.dpt");
	modelTemplate[rdoModelObjects::FUN] = ModelTemplateItem(":/new_model_template/new_model_template/template_5/template_5.fun");
	modelTemplate[rdoModelObjects::SMR] = ModelTemplateItem(":/new_model_template/new_model_template/template_5/template_5.smr");
	modelTemplate[rdoModelObjects::PMD] = ModelTemplateItem(":/new_model_template/new_model_template/template_5/template_5.pmd");
	m_modelTemplates[5] = modelTemplate;

	modelTemplate.clear();
	modelTemplate[rdoModelObjects::RTP] = ModelTemplateItem(":/new_model_template/new_model_template/template_6/template_6.rtp");
	modelTemplate[rdoModelObjects::RSS] = ModelTemplateItem(":/new_model_template/new_model_template/template_6/template_6.rss");
	modelTemplate[rdoModelObjects::EVN] = ModelTemplateItem(":/new_model_template/new_model_template/template_6/template_6.evn");
	modelTemplate[rdoModelObjects::PAT] = ModelTemplateItem(":/new_model_template/new_model_template/template_6/template_6.pat");
	modelTemplate[rdoModelObjects::DPT] = ModelTemplateItem(":/new_model_template/new_model_template/template_6/template_6.dpt");
	modelTemplate[rdoModelObjects::FUN] = ModelTemplateItem(":/new_model_template/new_model_template/template_6/template_6.fun");
	modelTemplate[rdoModelObjects::SMR] = ModelTemplateItem(":/new_model_template/new_model_template/template_6/template_6.smr");
	modelTemplate[rdoModelObjects::PMD] = ModelTemplateItem(":/new_model_template/new_model_template/template_6/template_6.pmd");
	m_modelTemplates[6] = modelTemplate;

	modelTemplate.clear();
	modelTemplate[rdoModelObjects::RTP] = ModelTemplateItem(":/new_model_template/new_model_template/template_7/template_7.rtp");
	modelTemplate[rdoModelObjects::RSS] = ModelTemplateItem(":/new_model_template/new_model_template/template_7/template_7.rss");
	modelTemplate[rdoModelObjects::EVN] = ModelTemplateItem(":/new_model_template/new_model_template/template_7/template_7.evn");
	modelTemplate[rdoModelObjects::PAT] = ModelTemplateItem(":/new_model_template/new_model_template/template_7/template_7.pat");
	modelTemplate[rdoModelObjects::DPT] = ModelTemplateItem(":/new_model_template/new_model_template/template_7/template_7.dpt");
	modelTemplate[rdoModelObjects::FUN] = ModelTemplateItem(":/new_model_template/new_model_template/template_7/template_7.fun");
	modelTemplate[rdoModelObjects::SMR] = ModelTemplateItem(":/new_model_template/new_model_template/template_7/template_7.smr");
	modelTemplate[rdoModelObjects::PMD] = ModelTemplateItem(":/new_model_template/new_model_template/template_7/template_7.pmd");
	m_modelTemplates[7] = modelTemplate;

	notifies.push_back(RT_STUDIO_MODEL_GET_TEXT             );
	notifies.push_back(RT_REPOSITORY_MODEL_NEW              );
	notifies.push_back(RT_REPOSITORY_MODEL_OPEN             );
	notifies.push_back(RT_REPOSITORY_MODEL_OPEN_GET_NAME    );
	notifies.push_back(RT_REPOSITORY_MODEL_OPEN_ERROR       );
	notifies.push_back(RT_REPOSITORY_MODEL_CLOSE            );
	notifies.push_back(RT_REPOSITORY_MODEL_SAVE             );
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

Model::~Model()
{
	g_pModel = NULL;
//	closeModel();
}

rbool Model::init()
{
	PTR(IInit) pFrameManagerInit = dynamic_cast<PTR(IInit)>(&m_frameManager);
	ASSERT(pFrameManagerInit);
	pFrameManagerInit->init();

	return true;
}

void Model::proc(REF(RDOThread::RDOMessageInfo) msg)
{
	switch (msg.message)
	{
		case RDOThread::RT_STUDIO_MODEL_GET_TEXT:
		{
			msg.lock();
			PTR(rdo::repository::RDOThreadRepository::FileData) fdata = static_cast<PTR(rdo::repository::RDOThreadRepository::FileData)>(msg.param);
			PTR(editor::Model) pEdit = m_pView->getTab().getItemEdit(fdata->m_type);
			if (pEdit)
			{
				pEdit->save(fdata->m_stream);
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
			QMessageBox::critical(
				g_pApp->getMainWnd(),
				QString::fromStdWString(L"Ошибка открытия модели"),
				QString::fromStdWString(L"Невозможно открыть модель '%1'.").arg(QString::fromLocal8Bit(static_cast<PTR(tstring)>(msg.param)->c_str()))
			);
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
				QString::fromStdWString(L"Открыть модель"),
				QString(),
				QString::fromStdWString(L"РДО-проект (*.rdox);;РДО-конвертор (*.smr);;Все файлы (*.*)")
			);
			data->m_result   = !modelName.isEmpty();
			data->m_name     = modelName.toLocal8Bit().constData();
			data->m_readOnly = false;

			msg.unlock();
			break;
		}
		case RDOThread::RT_REPOSITORY_MODEL_CLOSE:
		{
			closeModelFromRepository();
			break;
		}
		case RDOThread::RT_RUNTIME_MODEL_START_BEFORE:
		{
			break;
		}
		case RDOThread::RT_RUNTIME_MODEL_START_AFTER:
		{
			setIsRunning(true);
			sendMessage(kernel->runtime(), RT_RUNTIME_GET_MODE, &m_runtimeMode);
			setRuntimeMode(m_runtimeMode);
			sendMessage(kernel->runtime(), RT_RUNTIME_GET_SHOWRATE, &m_showRate);
			setSpeed(m_speed);
			afterModelStart();
			g_pApp->getIMainWnd()->getDockDebug().raise();
			g_pApp->getIMainWnd()->getDockDebug().appendString(QString::fromStdWString(L"Модель запущена\n"));
			g_pApp->getIMainWnd()->getDockDebug().getContext().update();
			int index = m_frameManager.getLastShowedFrame();
			if (index != -1)
			{
				PTR(FrameAnimationWnd) pView = m_frameManager.getFrameView(index);
				if (pView) pView->setFocus();
			}
			g_pApp->getIMainWnd()->update_start();
			break;
		}
		case RDOThread::RT_RUNTIME_MODEL_STOP_BEFORE:
		{
			g_pApp->getIMainWnd()->update_stop();
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
				g_pApp->getIMainWnd()->getDockDebug().appendString(QString::fromStdWString(L"Длительность прогона: %1 мсек.\n").arg(delay));
			}
			setCanRun   (true );
			setIsRunning(false);
			break;
		}
		case RDOThread::RT_SIMULATOR_MODEL_STOP_OK:
		{
			g_pApp->getIMainWnd()->getDockDebug().appendString(QString::fromStdWString(L"Модель завершена\n"));
			g_pApp->getIMainWnd()->getDockDebug().getContext().update();

			show_result();

			g_pApp->autoCloseByModel();
			break;
		}
		case RDOThread::RT_SIMULATOR_MODEL_STOP_BY_USER:
		{
			sendMessage(kernel->simulator(), RT_SIMULATOR_GET_MODEL_EXITCODE, &m_exitCode);
			g_pApp->getIMainWnd()->getDockDebug().appendString(QString::fromStdWString(L"Модель завершена\n"));
			g_pApp->getIMainWnd()->getDockDebug().getContext().update();

			show_result();

			break;
		}
		case RDOThread::RT_SIMULATOR_MODEL_STOP_RUNTIME_ERROR:
		{
			sendMessage(kernel->simulator(), RT_SIMULATOR_GET_MODEL_EXITCODE, &m_exitCode);
			show_result();
			g_pApp->getIMainWnd()->getDockDebug().appendString(QString::fromStdWString(L"Модель остановлена из-за ошибки прогона"));
			g_pApp->getIMainWnd()->getDockBuild().clear();
			g_pApp->getIMainWnd()->getDockBuild().raise();
			g_pApp->getIMainWnd()->getDockBuild().appendString(QString::fromStdWString(L"Ошибка прогона..."));
			std::vector<FileMessage> errors;
			g_pApp->m_pStudioGUI->sendMessage(kernel->simulator(), RDOThread::RT_SIMULATOR_GET_ERRORS, &errors);
			int errors_cnt   = 0;
			int warnings_cnt = 0;
			BOOST_FOREACH(const FileMessage& message, errors)
			{
				g_pApp->getIMainWnd()->getDockBuild().appendString(message);
				if (message.getType() == FileMessage::MT_WARNING)
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
//				const_cast<PTR(rdo::gui::editor::Build)>(output->getBuild())->showFirstError();
			}

			g_pApp->autoCloseByModel();
			break;
		}
		case RDOThread::RT_SIMULATOR_PARSE_OK:
		{
			sendMessage(kernel->simulator(), RT_SIMULATOR_GET_MODEL_EXITCODE, &m_exitCode);
			std::vector<FileMessage> errors;
			g_pApp->m_pStudioGUI->sendMessage(kernel->simulator(), RDOThread::RT_SIMULATOR_GET_ERRORS, &errors);
			int errors_cnt   = 0;
			int warnings_cnt = 0;
			BOOST_FOREACH(const FileMessage& message, errors)
			{
				g_pApp->getIMainWnd()->getDockBuild().appendString(message);
				if (message.getType() == FileMessage::MT_WARNING)
				{
					warnings_cnt++;
				}
				else
				{
					errors_cnt++;
				}
			}
			g_pApp->getIMainWnd()->getDockBuild().appendString(
				QString::fromStdWString(L"найдено ошибок: %1, предупреждений: %2").arg(errors_cnt).arg(warnings_cnt)
			);
			if (errors_cnt || warnings_cnt)
			{
//				g_pApp->getIMainWnd()->getDockBuild().getContext().showFirstError();
			}
			m_buildState = BS_COMPLETE;
			::GetSystemTime(&m_timeStart);
			break;
		}
		case RDOThread::RT_SIMULATOR_PARSE_ERROR:
		{
			setIsRunning(false);
			sendMessage(kernel->simulator(), RT_SIMULATOR_GET_MODEL_EXITCODE, &m_exitCode);
			std::vector<FileMessage> errors;
			g_pApp->m_pStudioGUI->sendMessage(kernel->simulator(), RDOThread::RT_SIMULATOR_GET_ERRORS, &errors);
			int errors_cnt   = 0;
			int warnings_cnt = 0;
			BOOST_FOREACH(const FileMessage& message, errors)
			{
				g_pApp->getIMainWnd()->getDockBuild().appendString(message);
				if (message.getType() == FileMessage::MT_WARNING)
				{
					warnings_cnt++;
				}
				else
				{
					errors_cnt++;
				}
			}
			g_pApp->getIMainWnd()->getDockBuild().appendString(
				QString::fromStdWString(L"найдено ошибок: %1, предупреждений: %2").arg(errors_cnt).arg(warnings_cnt)
			);
			if (errors_cnt || warnings_cnt)
			{
				g_pApp->getIMainWnd()->getDockBuild().getContext().showFirstError();
			}

			setCanRun(true);
			m_buildState  = BS_ERROR;
			g_pApp->autoCloseByModel();
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
			g_pApp->getIMainWnd()->getDockBuild().appendString(QString::fromLocal8Bit(static_cast<PTR(tstring)>(msg.param)->c_str()));
			msg.unlock();
			break;
		}
		case RDOThread::RT_DEBUG_STRING:
		{
			msg.lock();
			g_pApp->getIMainWnd()->getDockDebug().appendString(QString::fromLocal8Bit(static_cast<PTR(tstring)>(msg.param)->c_str()));
			msg.unlock();
			break;
		}
		case RDOThread::RT_RESULT_STRING:
		{
			msg.lock();
			g_pApp->getIMainWnd()->getDockResults().appendString(QString::fromLocal8Bit(static_cast<PTR(tstring)>(msg.param)->c_str()));
			msg.unlock();
			break;
		}
	}
}

void Model::show_result()
{
	rdo::textstream modelResults;
	sendMessage(kernel->simulator(), RT_SIMULATOR_GET_MODEL_RESULTS, &modelResults);
	QString str = QString::fromLocal8Bit(modelResults.str().c_str());
	if (!str.isEmpty())
	{
		rdo::repository::RDOThreadRepository::FileInfo data(rdoModelObjects::PMV);
		g_pApp->m_pStudioGUI->sendMessage(kernel->repository(), RDOThread::RT_REPOSITORY_MODEL_GET_FILEINFO, &data);
		if (!data.m_described)
		{
			g_pApp->getIMainWnd()->getDockDebug().appendString(QString::fromStdWString(L"Результаты не будут записаны в файл, т.к. в SMR не определен Results_file\n"));
		}
		g_pApp->getIMainWnd()->getDockResults().getContext().clearAll();
		g_pApp->getIMainWnd()->getDockResults().raise();
		g_pApp->getIMainWnd()->getDockResults().appendString(str);
	}
}

rbool Model::newModel(CREF(QString) modelName, CREF(QString) modelPath, ruint templateIndex)
{
	m_templateIndex = templateIndex;
	g_pApp->getIMainWnd()->getDockBuild  ().clear();
	g_pApp->getIMainWnd()->getDockDebug  ().clear();
	g_pApp->getIMainWnd()->getDockResults().clear();
	g_pApp->getIMainWnd()->getDockFind   ().clear();
	rdo::repository::RDOThreadRepository::NewModel data;
	data.m_name = modelName.toLocal8Bit().constData();
	data.m_path = modelPath.toLocal8Bit().constData();
	g_pApp->broadcastMessage(RDOThread::RT_STUDIO_MODEL_NEW, &data);
	return true;
}

rbool Model::openModel(CREF(QString) modelName)
{
	if (isRunning())
	{
		QMessageBox::warning(g_pApp->getMainWnd(), "RAO-Studio", QString::fromStdWString(L"Работает модель. Перед открытием её необходимо остановить."));
		return false;
	}
	if (!closeModel())
	{
		return false;
	}
	g_pApp->getIMainWnd()->getDockBuild  ().clear();
	g_pApp->getIMainWnd()->getDockDebug  ().clear();
	g_pApp->getIMainWnd()->getDockResults().clear();
	g_pApp->getIMainWnd()->getDockFind   ().clear();
	g_pApp->getIMainWnd()->getDockDebug().raise();
	g_pApp->getIMainWnd()->getDockDebug().appendString(QString::fromStdWString(L"Загрузка модели...\n"));
	g_pApp->getIMainWnd()->getDockDebug().getContext().update();
	m_openError     = false;
	m_smrEmptyError = false;
	m_modelClosed   = false;
	rdo::repository::RDOThreadRepository::OpenFile data(modelName.toLocal8Bit().constData());
	g_pApp->broadcastMessage(RDOThread::RT_STUDIO_MODEL_OPEN, &data);
	if (data.m_result && !m_openError && !m_smrEmptyError)
	{
		rdo::binarystream stream;
		rdo::repository::RDOThreadRepository::FileData fileData(rdoModelObjects::PMV, stream);
		g_pApp->m_pStudioGUI->sendMessage(kernel->repository(), RDOThread::RT_REPOSITORY_LOAD, &fileData);
		g_pApp->getIMainWnd()->getDockResults().appendString(QString::fromLocal8Bit(stream.str().c_str()));
		g_pApp->getIMainWnd()->getDockDebug().appendString(QString::fromStdWString(L"Загрузка модели... ok\n"));
		g_pApp->setLastProjectName(getFullName());
	}
	else
	{
		m_modelClosed = true;
		if (m_smrEmptyError)
		{
			QMessageBox::critical(g_pApp->getMainWnd(), _T("Ошибка открытия модели"), _T("В smr-файле не найдено имя модели"));
			closeModel();
		}
		else
		{
			g_pApp->getIMainWnd()->getDockDebug().appendString(QString::fromStdWString(L"Загрузка модели... failed\n"));
		}
	}
	return data.m_result;
}

rbool Model::saveModel() const
{
	rbool res = true;
	g_pApp->broadcastMessage(RDOThread::RT_STUDIO_MODEL_SAVE, &res);
	return res;
}

rbool Model::closeModel()
{
	if (isRunning())
	{
		QMessageBox::warning(g_pApp->getMainWnd(), "RAO-Studio", QString::fromStdWString(L"Работает модель. Перед закрытием её необходимо остановить."));
		return false;
	}

	if (!canCloseModel())
	{
		return false;
	}

	setHasModel(false);
	resetView();
	stopModel();
	g_pApp->getIMainWnd()->getDockBuild  ().clear();
	g_pApp->getIMainWnd()->getDockDebug  ().clear();
	g_pApp->getIMainWnd()->getDockResults().clear();
	g_pApp->getIMainWnd()->getDockFind   ().clear();
	g_pApp->broadcastMessage(RDOThread::RT_STUDIO_MODEL_CLOSE);
	return true;
}

rbool Model::buildModel()
{
	if (hasModel() && !isRunning() && saveModel())
	{
		g_pApp->getIMainWnd()->getDockBuild().clear();
		g_pApp->getIMainWnd()->getDockDebug().clear();
		g_pApp->getIMainWnd()->getDockResults().clear();
		g_pApp->getIMainWnd()->getDockBuild().raise();
		g_pApp->getIMainWnd()->getDockBuild().appendString(QString::fromStdWString(L"Компиляция..."));
		g_pApp->getIMainWnd()->getDockBuild().getContext().update();
		m_buildState = BS_UNDEFINED;
		g_pApp->broadcastMessage(RDOThread::RT_STUDIO_MODEL_BUILD);
		return m_buildState == BS_COMPLETE;
	}
	return false;
}

rbool Model::runModel()
{
	if (buildModel())
	{
		setCanRun(false);
		g_pApp->getIMainWnd()->getDockBuild().clear();
		g_pApp->getIMainWnd()->getDockDebug().clear();
		g_pApp->getIMainWnd()->getDockResults().clear();
		g_pApp->getIMainWnd()->getDockBuild().raise();
		g_pApp->getIMainWnd()->getDockBuild().getContext().update();
		g_pApp->broadcastMessage(RDOThread::RT_STUDIO_MODEL_RUN);
		return true;
	}
	return false;
}

rbool Model::stopModel() const
{
	if (hasModel() && isRunning())
	{
		g_pApp->broadcastMessage(RDOThread::RT_STUDIO_MODEL_STOP);
		return true;
	}
	return false;
}

void Model::createView()
{
	ASSERT(m_pView == NULL);
	m_pView = new View(NULL);
	m_pView->setModel(this);
	g_pApp->getIMainWnd()->addSubWindow(m_pView);
	m_pView->parentWidget()->setWindowIcon(QIcon(QString::fromUtf8(":/images/images/mdi_model.png")));

	for (int i = 0; i < m_pView->getTab().count(); i++)
	{
		PTR(editor::Model) pEdit = m_pView->getTab().getItemEdit(i);
		connect(pEdit, SIGNAL(modifyChanged(bool)), this, SLOT(onEditModifyChanged(bool)));
	}
}

void Model::resetView()
{
	if (m_pView)
	{
		for (int i = 0; i < m_pView->getTab().count(); i++)
		{
			PTR(editor::Model) pEdit = m_pView->getTab().getItemEdit(i);
			disconnect(pEdit, SIGNAL(modifyChanged(bool)), this, SLOT(onEditModifyChanged(bool)));
		}
		m_pView->setModel(NULL);
	}
}

void Model::newModelFromRepository()
{
	setHasModel(true);

	createView();
	rdo::repository::RDOThreadRepository::FileInfo data_smr(rdoModelObjects::RDOX);
	g_pApp->m_pStudioGUI->sendMessage(kernel->repository(), RDOThread::RT_REPOSITORY_MODEL_GET_FILEINFO, &data_smr);
	setName(QString::fromLocal8Bit(data_smr.m_name.c_str()));

	ModelTemplateList::const_iterator templateIt = m_templateIndex.is_initialized()
		? m_modelTemplates.find(*m_templateIndex)
		: m_modelTemplates.end();

	for (int i = 0; i < m_pView->getTab().count(); i++)
	{
		PTR(editor::Model) pEdit = m_pView->getTab().getItemEdit(i);
		pEdit->setReadOnly(false);
		pEdit->clearAll();
		if (templateIt != m_modelTemplates.end())
		{
			ModelTemplate::const_iterator it = templateIt->second.find(m_pView->getTab().indexToType(i));
			if (it != templateIt->second.end())
			{
				ASSERT(!it->second.resName.isEmpty())
				QString resourceData;
				QFile   file(it->second.resName);
				if (file.open(QIODevice::ReadOnly) && file.isOpen())
				{
					resourceData = QString::fromLocal8Bit(file.readAll());
				}
				if (!resourceData.isEmpty())
				{
					pEdit->replaceCurrent(resourceData.toLocal8Bit().constData());
					pEdit->scrollToLine(0);
					if (it->second.position.is_initialized())
					{
						pEdit->setCurrentPos(*it->second.position);
					}
				}
			}
		}
	}

	g_pApp->setLastProjectName(getFullName());
	if (templateIt != m_modelTemplates.end())
	{
		saveModel();
		for (int i = 0; i < m_pView->getTab().count(); i++)
		{
			PTR(editor::Model) pEdit = m_pView->getTab().getItemEdit(i);
			pEdit->clearUndoBuffer();
		}
	}
}

void Model::openModelFromRepository()
{
	setHasModel(true);

	createView();
	rdo::repository::RDOThreadRepository::FileInfo data_smr(rdoModelObjects::RDOX);
	g_pApp->m_pStudioGUI->sendMessage(kernel->repository(), RDOThread::RT_REPOSITORY_MODEL_GET_FILEINFO, &data_smr);
	setName(QString::fromLocal8Bit(data_smr.m_name.c_str()));

	int cnt = m_pView->getTab().count();
	g_pApp->getMainWndUI()->statusBar()->beginProgress(0, cnt * 2 + 1);
	g_pApp->getMainWndUI()->statusBar()->stepProgress();
	for (int i = 0; i < cnt; i++)
	{
		PTR(editor::Model) pEdit = m_pView->getTab().getItemEdit(i);
		pEdit->setReadOnly(false);
		pEdit->clearAll();
		rdo::binarystream stream;
		rbool canLoad = true;
		rdoModelObjects::RDOFileType type = m_pView->getTab().indexToType(i);
		if (m_pView->getTab().typeSupported(type))
		{
			rdo::repository::RDOThreadRepository::FileData fileData(type, stream);
			g_pApp->m_pStudioGUI->sendMessage(kernel->repository(), RDOThread::RT_REPOSITORY_LOAD, &fileData);
		}
		else
		{
			canLoad = false;
		}
		g_pApp->getMainWndUI()->statusBar()->stepProgress();
		if (canLoad)
		{
			rdo::repository::RDOThreadRepository::FileInfo data(type);
			g_pApp->m_pStudioGUI->sendMessage(kernel->repository(), RDOThread::RT_REPOSITORY_MODEL_GET_FILEINFO, &data);
			rbool stream_error = stream.rdstate() & std::ios_base::failbit ? true : false;
			if (!stream_error)
			{
				pEdit->load(stream);
				pEdit->setReadOnly(data.m_readOnly);
				if (data.m_readOnly)
				{
					g_pApp->getIMainWnd()->getDockDebug().appendString(QString::fromStdWString(L"%1 - только чтение\n").arg(tstring(data.m_name + data.m_extention).c_str()));
				}
			}
			else
			{
				QString objName;
				switch (type)
				{
				case rdoModelObjects::RTP: objName = QString::fromStdWString(L"типы ресурсов");          break;
				case rdoModelObjects::RSS: objName = QString::fromStdWString(L"ресурсы");                break;
				case rdoModelObjects::EVN: objName = QString::fromStdWString(L"события");                break;
				case rdoModelObjects::PAT: objName = QString::fromStdWString(L"образцы");                break;
				case rdoModelObjects::DPT: objName = QString::fromStdWString(L"точки принятия решений"); break;
				case rdoModelObjects::FRM: objName = QString::fromStdWString(L"кадры");                  break;
				case rdoModelObjects::FUN: objName = QString::fromStdWString(L"функции");                break;
				case rdoModelObjects::PMD: objName = QString::fromStdWString(L"описание показателей");   break;
				}
				if (!objName.isEmpty())
				{
					g_pApp->getIMainWnd()->getDockDebug().appendString(QString::fromStdWString(L"Невозможно загрузить %1 (%2)\n").arg(objName).arg(data.m_fullName.c_str()));
					g_pApp->getIMainWnd()->getDockDebug().getContext().update();
				}
				m_openError = true;
			}
		}
		pEdit->setCurrentPos(0);
		pEdit->setModifyFalse();
		pEdit->clearUndoBuffer();
		g_pApp->getMainWndUI()->statusBar()->stepProgress();
	}
	g_pApp->getMainWndUI()->statusBar()->endProgress();

	updateFrmDescribed();
	updateActions();
}

void Model::saveModelToRepository()
{
	rbool smr_modified = false;
	rbool wasSaved     = false;
	PTR(editor::Model) pSmrEdit = m_pView->getTab().getItemEdit(rdoModelObjects::SMR);
	if (pSmrEdit->isModify())
	{
		rdo::binarystream stream;
		pSmrEdit->save(stream);
		m_smrEmptyError = false;
		rdo::repository::RDOThreadRepository::FileData fileData(rdoModelObjects::SMR, stream);
		g_pApp->m_pStudioGUI->sendMessage(kernel->repository(), RDOThread::RT_REPOSITORY_SAVE, &fileData);
		if (m_smrEmptyError)
		{
			QMessageBox::critical(g_pApp->getMainWnd(), QString::fromStdWString(L"Ошибка записи модели"), QString::fromStdWString(L"В smr-файле не найдено имя модели, модель не будет записана"));
			return;
		}
		smr_modified = true;
	}

	int cnt = m_pView->getTab().count();
	int progress_cnt = 0;
	for (int i = 0; i < cnt; i++)
	{
		if (smr_modified || m_pView->getTab().getItemEdit(i)->isModify())
		{
			progress_cnt++;
		}
	}
	if (progress_cnt)
	{
		g_pApp->getMainWndUI()->statusBar()->beginProgress(0, progress_cnt * 2 + 1);
		g_pApp->getMainWndUI()->statusBar()->stepProgress();
		for (int i = 0; i < cnt; i++)
		{
			PTR(editor::Model) pEdit = m_pView->getTab().getItemEdit(i);
			if (smr_modified || pEdit->isModify())
			{
				rdo::binarystream stream;
				pEdit->save(stream);
				g_pApp->getMainWndUI()->statusBar()->stepProgress();
				rdoModelObjects::RDOFileType type = m_pView->getTab().indexToType(i);
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
						g_pApp->m_pStudioGUI->sendMessage(kernel->repository(), RDOThread::RT_REPOSITORY_SAVE, &fileData);
						break;
					}
				default: break;
				}
				pEdit->setModifyFalse();
			}
			g_pApp->getMainWndUI()->statusBar()->stepProgress();
		}
		g_pApp->getMainWndUI()->statusBar()->endProgress();
		wasSaved = true;
	}

	rdo::repository::RDOThreadRepository::FileInfo data(rdoModelObjects::RDOX);
	g_pApp->m_pStudioGUI->sendMessage(kernel->repository(), RDOThread::RT_REPOSITORY_MODEL_GET_FILEINFO, &data);
	setName(QString::fromLocal8Bit(data.m_name.c_str()));

	g_pApp->getMainWndUI()->insertMenuFileReopenItem(getFullName());

	if (smr_modified)
	{
		updateFrmDescribed();
	}
	updateActions();
}

QString Model::getFullName() const
{
	rdo::repository::RDOThreadRepository::FileInfo data(rdoModelObjects::RDOX);
	g_pApp->m_pStudioGUI->sendMessage(kernel->repository(), RDOThread::RT_REPOSITORY_MODEL_GET_FILEINFO, &data);
	return QString::fromLocal8Bit(data.m_fullName.c_str());
}

void Model::updateFrmDescribed()
{
//	frmDescribed = kernel->repository()->isDescribed(rdoModelObjects::FRM);
	m_frmDescribed = true;
}

rbool Model::canCloseModel()
{
	rbool result = true;
	if (isModify())
	{
		switch (QMessageBox::question(g_pApp->getMainWnd(), "RAO-Studio", QString::fromStdWString(L"Сохранить модель ?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel))
		{
			case QMessageBox::Yes   : result = saveModel(); break;
			case QMessageBox::No    : result = true; break;
			case QMessageBox::Cancel: result = false; break;
		}
	}
	return result;
}

void Model::closeModelFromRepository()
{
	setHasModel(false);
	m_pView->parentWidget()->close();
	m_pView  = NULL;
	m_modelClosed = true;
	setName("");
}

CREF(QString) Model::getName() const
{
	return m_name;
}

void Model::setName(CREF(QString) name)
{
	QString newName = name.trimmed();

	if (m_name != newName)
	{
		m_name = newName;

		if (m_pView)
		{
			m_pView->parentWidget()->setWindowTitle(QString::fromLocal8Bit("модель: %1").arg(
				g_pApp->getShowCaptionFullName()
					? getFullName()
					: m_name
			));
		}
	}
}

void Model::afterModelStart()
{
	m_frameManager.clear();

	if (isFrmDescribed())
	{
		g_pApp->getIMainWnd()->getDockDebug().raise();
		g_pApp->getIMainWnd()->getDockDebug().appendString(QString::fromStdWString(L"Загрузка ресурсов для анимации...\n"));
		g_pApp->getIMainWnd()->getDockDebug().getContext().update();

		std::list<tstring> frames;
		std::list<tstring> bitmaps;
		rdo::service::simulation::RDOThreadSimulator::GetList getListFrames (rdo::service::simulation::RDOThreadSimulator::GetList::frames,  &frames );
		rdo::service::simulation::RDOThreadSimulator::GetList getListBitmaps(rdo::service::simulation::RDOThreadSimulator::GetList::bitmaps, &bitmaps);
		sendMessage(kernel->simulator(), RT_SIMULATOR_GET_LIST, &getListFrames );
		sendMessage(kernel->simulator(), RT_SIMULATOR_GET_LIST, &getListBitmaps);
		BOOST_FOREACH(const tstring& name, bitmaps)
		{
			m_frameManager.insertBitmap(QString::fromLocal8Bit(name.c_str()));
		}
		BOOST_FOREACH(const tstring& name, frames)
		{
			m_frameManager.insertFrame(QString::fromLocal8Bit(name.c_str()));
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
		g_pApp->getIMainWnd()->getDockDebug().appendString(QString::fromStdWString(L"Загрузка ресурсов для анимации... ok\n"));
		g_pApp->getIMainWnd()->getDockDebug().getContext().update();
	}
	else
	{
		m_timeNow = 0;
		m_frameManager.setLastShowedFrame(ruint(~0));
	}
}

void Model::updateStyleOfAllModel() const
{
	if (m_pView)
	{
		for (int i = 0; i < m_pView->getTab().count(); i++)
		{
			m_pView->getTab().getItemEdit(i)->setEditorStyle(&g_pApp->getStyle()->style_editor);
		}
	}

	m_frameManager.updateStyles();
}

rbool Model::isPrevModelClosed() const
{
	return m_modelClosed;
}

void Model::setRuntimeMode(const rdo::runtime::RunTimeMode value)
{
	if (isRunning())
	{
		m_runtimeMode = value;
		sendMessage(kernel->runtime(), RT_RUNTIME_SET_MODE, &m_runtimeMode);
		g_pTracer->setRuntimeMode(getRuntimeMode());
		switch (getRuntimeMode())
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
	updateActions();
}

QString Model::getLastBreakPointName()
{
	tstring str;
	sendMessage(kernel->runtime(), RT_RUNTIME_GET_LAST_BREAKPOINT, &str);
	return QString::fromLocal8Bit(str.c_str());
}

double Model::getSpeed() const
{
	return m_speed;
}

void Model::setSpeed(double persent)
{
	if (persent >= 0 && persent <= 1)
	{
		m_speed = persent;
		if (isRunning())
		{
			sendMessage(kernel->runtime(), RT_RUNTIME_SET_SPEED, &m_speed);
		}
		updateActions();
	}
}

double Model::getShowRate() const
{
	return m_showRate;
}

void Model::setShowRate(double value)
{
	if (!isRunning())
		return;

	if (value >= DBL_MIN && value <= DBL_MAX)
	{
		m_showRate = value;
		sendMessage(kernel->runtime(), RT_RUNTIME_SET_SHOWRATE, &m_showRate);
		updateActions();
	}
}

void Model::onShowNextFrame()
{
	m_frameManager.showNextFrame();
}

void Model::onShowPrevFrame()
{
	m_frameManager.showPrevFrame();
}

int Model::getFrameCount() const
{
	return m_frameManager.count();
}

CREF(QString) Model::getFrameName(int index) const
{
	return m_frameManager.getFrameName(index);
}

void Model::showFrame(int index)
{
	m_frameManager.showFrame(index);
}

void Model::closeAllFrame()
{
	m_frameManager.closeAll();
}

rbool Model::hasModel() const
{
	return m_GUI_HAS_MODEL;
}

void Model::setHasModel(rbool value)
{
	m_GUI_HAS_MODEL = value;
	updateActions();
}

void Model::setCanRun(rbool value)
{
	m_GUI_CAN_RUN = value;
	updateActions();
}

void Model::updateActions()
{
	Ui::MainWindow* pMainWindow = g_pApp->getMainWndUI();
	if (!pMainWindow)
		return;

	pMainWindow->actFileNew->setEnabled(canNew());
	pMainWindow->actFileOpen->setEnabled(canOpen());
	pMainWindow->actFileSaveAll->setEnabled(canSave());
	pMainWindow->actFileClose->setEnabled(canClose());

	pMainWindow->actModelBuild->setEnabled(canBuild());
	pMainWindow->actModelRun->setEnabled(canRun());
	pMainWindow->actModelStop->setEnabled(isRunning());

	pMainWindow->actModelRuntimeMaxSpeed->setEnabled(isRunning());
	pMainWindow->actModelRuntimeJump->setEnabled    (isRunning());
	pMainWindow->actModelRuntimeSync->setEnabled    (isRunning());
	pMainWindow->actModelRuntimePause->setEnabled   (isRunning());
	pMainWindow->actModelRuntimeMaxSpeed->setChecked(getRuntimeMode() == rdo::runtime::RTM_MaxSpeed);
	pMainWindow->actModelRuntimeJump->setChecked    (getRuntimeMode() == rdo::runtime::RTM_Jump);
	pMainWindow->actModelRuntimeSync->setChecked    (getRuntimeMode() == rdo::runtime::RTM_Sync);
	pMainWindow->actModelRuntimePause->setChecked   (getRuntimeMode() == rdo::runtime::RTM_Pause || getRuntimeMode() == rdo::runtime::RTM_BreakPoint);

	rbool canShowRate = isRunning() && getRuntimeMode() == rdo::runtime::RTM_Sync;
	pMainWindow->actModelShowRateInc->setEnabled    (canShowRate && getShowRate() * 1.5 <= DBL_MAX);
	pMainWindow->actModelShowRateIncFour->setEnabled(canShowRate && getShowRate() * 4.0 <= DBL_MAX);
	pMainWindow->actModelShowRateDecFour->setEnabled(canShowRate && getShowRate() / 4.0 >= DBL_MIN);
	pMainWindow->actModelShowRateDec->setEnabled    (canShowRate && getShowRate() / 1.5 >= DBL_MIN);

	pMainWindow->actModelFrameNext->setEnabled(m_frameManager.canShowNextFrame());
	pMainWindow->actModelFramePrev->setEnabled(m_frameManager.canShowPrevFrame());

	QString runTimeMode;
	if (isRunning())
	{
		switch (getRuntimeMode())
		{
		case rdo::runtime::RTM_MaxSpeed  : runTimeMode = QString::fromLocal8Bit("Без анимации"); break;
		case rdo::runtime::RTM_Jump      : runTimeMode = QString::fromLocal8Bit("Дискретная имитация"); break;
		case rdo::runtime::RTM_Sync      : runTimeMode = QString::fromLocal8Bit("Синхронная имитация"); break;
		case rdo::runtime::RTM_Pause     : runTimeMode = QString::fromLocal8Bit("Пауза"); break;
		case rdo::runtime::RTM_BreakPoint: runTimeMode = QString::fromLocal8Bit("Точка останова: %1").arg(getLastBreakPointName()); break;
		}
	}
	g_pApp->getMainWndUI()->statusBar()->update<StatusBar::SB_MODEL_RUNTYPE>(runTimeMode);

	g_pApp->getMainWndUI()->statusBar()->update<StatusBar::SB_MODEL_SPEED>(
		getRuntimeMode() != rdo::runtime::RTM_MaxSpeed || !isRunning()
			? QString::fromStdWString(L"Скорость: %1%").arg(rsint(getSpeed() * 100))
			: ""
	);

	QString showRateStr;
	if (isRunning())
	{
		showRateStr = QString::fromStdWString(L"Масштаб: ");
		switch (getRuntimeMode())
		{
		case rdo::runtime::RTM_MaxSpeed:
		case rdo::runtime::RTM_Jump    :
			showRateStr += QString::fromStdWString(L"Бесконечность");
			break;

		case rdo::runtime::RTM_Pause     :
		case rdo::runtime::RTM_BreakPoint:
			showRateStr += "0.0";
			break;

		case rdo::runtime::RTM_Sync:
			{
				double showRate = g_pModel->getShowRate();
				if (showRate < 1e-10 || showRate > 1e10)
				{
					showRateStr += QString::number(showRate, 'e');
				}
				else if (showRate >= 1)
				{
					showRateStr += QString::number(showRate, 'f', 1);
				}
				else
				{
					showRateStr += QString::number(showRate, 'f', 10);
				}
			}
			break;
		}
	}
	g_pApp->getMainWndUI()->statusBar()->update<StatusBar::SB_MODEL_SHOWRATE>(showRateStr);
}

void Model::update()
{
	sendMessage(kernel->runtime(), RT_RUNTIME_GET_TIMENOW, &m_timeNow);

	g_pApp->getMainWndUI()->statusBar()->update<StatusBar::SB_MODEL_TIME>(QString::fromStdWString(L"Время: %1").arg(m_timeNow));

	rdo::runtime::RunTimeMode rm;
	sendMessage(kernel->runtime(), RT_RUNTIME_GET_MODE, &rm);
	if (rm != getRuntimeMode())
	{
		if (rm == rdo::runtime::RTM_BreakPoint)
		{
			g_pApp->getIMainWnd()->getDockDebug().appendString(QString::fromStdWString(L"Пауза в %1 из-за точки '%2'\n").arg(getTimeNow()).arg(getLastBreakPointName()));
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

rbool Model::isModify() const
{
	if (!m_pView)
		return false;

	rbool result = false;

	for (int i = 0; i < getTab()->count(); i++)
	{
		if (getTab()->getItemEdit(i)->isModify())
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

rbool Model::canNew() const
{
	return (canRun() || !hasModel());
}

rbool Model::canOpen() const
{
	return (canRun() || !hasModel());
}

rbool Model::canSave() const
{
	return hasModel() && isModify();
}

rbool Model::canClose() const
{
	return hasModel() && !isRunning();
}

rbool Model::canBuild() const
{
	return canRun();
}

rbool Model::canRun() const
{
	return hasModel() && m_GUI_CAN_RUN;
}

rbool Model::isRunning() const
{
	return m_GUI_IS_RUNNING;
}

void Model::setIsRunning(rbool value)
{
	m_GUI_IS_RUNNING = value;
	updateActions();
}

rbool Model::isFrmDescribed() const
{
	return m_frmDescribed;
}

double Model::getTimeNow() const
{
	return m_timeNow;
}

rdo::simulation::report::RDOExitCode Model::getExitCode() const
{
	return m_exitCode;
}

rdo::runtime::RunTimeMode Model::getRuntimeMode() const
{
	return m_runtimeMode;
}

REF(FrameManager) Model::getFrameManager()
{
	return m_frameManager;
}

void Model::onChangeFrame(ruint)
{
	updateActions();
}

PTR(TabCtrl) Model::getTab()
{
	if (!m_pView)
		return NULL;

	return &m_pView->getTab();
}

CPTR(TabCtrl) Model::getTab() const
{
	if (!m_pView)
		return NULL;

	return &m_pView->getTab();
}

void Model::onFileNew()
{
	NewModelDialog dlg(g_pApp->getMainWndUI());
	if (dlg.exec() == QDialog::Accepted)
	{
		newModel(dlg.getModelName(), dlg.getModelPath() + dlg.getModelName(), dlg.getTemplateIndex());
	}
}

void Model::onFileOpen()
{
	openModel();
}

void Model::onFileClose()
{
	closeModel();
}

void Model::onFileSaveAll()
{
	saveModel();
}

void Model::onModelBuild()
{
	buildModel();
}

void Model::onModelRun()
{
	runModel();
}

void Model::onModelStop()
{
	stopModel();
}

void Model::onModelRuntimeMaxSpeed()
{
	setRuntimeMode(rdo::runtime::RTM_MaxSpeed);
}

void Model::onModelRuntimeJump()
{
	setRuntimeMode(rdo::runtime::RTM_Jump);
}

void Model::onModelRuntimeSync()
{
	setRuntimeMode(rdo::runtime::RTM_Sync);
}

void Model::onModelRuntimePause()
{
	setRuntimeMode(rdo::runtime::RTM_Pause);
}

void Model::onModelShowRateInc()
{
	setShowRate(getShowRate() * 1.5);
}

void Model::onModelShowRateIncFour()
{
	setShowRate(getShowRate() * 4);
}

void Model::onModelShowRateDecFour()
{
	setShowRate(getShowRate() / 4);
}

void Model::onModelShowRateDec()
{
	setShowRate(getShowRate() / 1.5);
}

void Model::onModelSpeedValueChanged(int value)
{
	setSpeed(log( double(value + 1) ) / log(101.0));
}

void Model::onEditModifyChanged(bool)
{
	updateActions();
}
