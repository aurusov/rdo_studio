#include "stdafx.h"
#include "rdostudiomodel.h"
#include "rdostudioapp.h"
#include "rdostudiothread.h"
#include "rdostudiomainfrm.h"
#include "rdostudiochildfrm.h"
#include "rdostudioframedoc.h"
#include "rdostudioframeview.h"
#include "rdostudioplugins.h"
#include "rdo_edit/rdoeditortabctrl.h"
#include "edit_ctrls/rdobuildedit.h"
#include "edit_ctrls/rdodebugedit.h"
#include "rdo_tracer/rdotracer.h"
#include "resource.h"

#include <rdokernel.h>
#include <rdorepository.h>
#include <rdostream.h>
#include <rdoplugin.h>
#include <rdothread.h>
#include <rdo_exception.h>
#include <rdoanimation.h>
#include <limits>

using namespace rdoEditor;
using namespace rdoSimulator;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RDOStudioModel
// ----------------------------------------------------------------------------
RDOStudioModel* model = NULL;

RDOStudioModel::RDOStudioModel():
	RDOThreadGUI( "RDOThreadModelGUI", static_cast<RDOKernelGUI*>(studioApp.studioGUI) ),
	modelDocTemplate( NULL ),
	useTemplate( -1 ),
	autoDeleteDoc( true ),
	showCanNotCloseModelMessage( true ),
	GUI_HAS_MODEL( false ),
	GUI_CAN_RUN( true ),
	GUI_IS_RUNING( false ),
	GUI_ACTION_NEW( true ),
	GUI_ACTION_OPEN( true ),
	GUI_ACTION_SAVE( true ),
	GUI_ACTION_CLOSE( true ),
	GUI_ACTION_BUILD( true ),
	GUI_ACTION_RUN( true ),
	openError( false ),
	smrEmptyError( false ),
	modelClosed( true ),
	frmDescribed( false ),
	timeNow( 0 ),
	speed( 1 ),
	showRate( 60 ),
	tempPause( false ),
	runtimeMode( rdoRuntime::RTM_MaxSpeed ),
	runtimeMode_prev( rdoRuntime::RTM_MaxSpeed ),
	exitCode( rdoSimulator::EC_ModelNotFound ),
	prevModify( false )
{
	modelDocTemplate = new CMultiDocTemplate( IDR_MODEL_TYPE, RUNTIME_CLASS(RDOStudioModelDoc), RUNTIME_CLASS(RDOStudioChildFrame), RUNTIME_CLASS(RDOStudioModelView) );
	AfxGetApp()->AddDocTemplate( modelDocTemplate );

	model = this;

	std::map< rdoModelObjects::RDOFileType, TemplateData > template_id;
	template_id[ rdoModelObjects::SMR ] = TemplateData( IDR_MODEL_TMP0_SMR, 0 );
	model_templates[0] = template_id;

	template_id.clear();
	template_id[ rdoModelObjects::SMR ] = TemplateData( IDR_MODEL_TMP1_SMR, 0 );
	model_templates[1] = template_id;

	template_id.clear();
	template_id[ rdoModelObjects::PAT ] = TemplateData( IDR_MODEL_TMP2_PAT, 9  );
	template_id[ rdoModelObjects::RTP ] = TemplateData( IDR_MODEL_TMP2_RTP, 15 );
	template_id[ rdoModelObjects::RSS ] = TemplateData( IDR_MODEL_TMP2_RSS, 12 );
	template_id[ rdoModelObjects::OPR ] = TemplateData( IDR_MODEL_TMP2_OPR, 13 );
	template_id[ rdoModelObjects::FRM ] = TemplateData( IDR_MODEL_TMP2_FRM, 7  );
	template_id[ rdoModelObjects::FUN ] = TemplateData( IDR_MODEL_TMP2_FUN, 10 );
	template_id[ rdoModelObjects::SMR ] = TemplateData( IDR_MODEL_TMP2_SMR, 0  );
	model_templates[2] = template_id;

	template_id.clear();
	template_id[ rdoModelObjects::PAT ] = TemplateData( IDR_MODEL_TMP3_PAT, 131 );
	template_id[ rdoModelObjects::RTP ] = TemplateData( IDR_MODEL_TMP3_RTP, 98  );
	template_id[ rdoModelObjects::RSS ] = TemplateData( IDR_MODEL_TMP3_RSS, 0   );
	template_id[ rdoModelObjects::OPR ] = TemplateData( IDR_MODEL_TMP3_OPR, 0   );
	template_id[ rdoModelObjects::FRM ] = TemplateData( IDR_MODEL_TMP3_FRM, 84  );
	template_id[ rdoModelObjects::FUN ] = TemplateData( IDR_MODEL_TMP3_FUN, 44  );
	template_id[ rdoModelObjects::SMR ] = TemplateData( IDR_MODEL_TMP3_SMR, 0   );
	model_templates[3] = template_id;

	template_id.clear();
	template_id[ rdoModelObjects::PAT ] = TemplateData( IDR_MODEL_TMP4_PAT, 0 );
	template_id[ rdoModelObjects::RTP ] = TemplateData( IDR_MODEL_TMP4_RTP, 0 );
	template_id[ rdoModelObjects::RSS ] = TemplateData( IDR_MODEL_TMP4_RSS, 0 );
	template_id[ rdoModelObjects::OPR ] = TemplateData( IDR_MODEL_TMP4_OPR, 0 );
	template_id[ rdoModelObjects::FUN ] = TemplateData( IDR_MODEL_TMP4_FUN, 0 );
	template_id[ rdoModelObjects::SMR ] = TemplateData( IDR_MODEL_TMP4_SMR, 0 );
	template_id[ rdoModelObjects::PMD ] = TemplateData( IDR_MODEL_TMP4_PMD, 0 );
	model_templates[4] = template_id;

	template_id.clear();
	template_id[ rdoModelObjects::PAT ] = TemplateData( IDR_MODEL_TMP5_PAT, 0 );
	template_id[ rdoModelObjects::RTP ] = TemplateData( IDR_MODEL_TMP5_RTP, 0 );
	template_id[ rdoModelObjects::RSS ] = TemplateData( IDR_MODEL_TMP5_RSS, 0 );
	template_id[ rdoModelObjects::OPR ] = TemplateData( IDR_MODEL_TMP5_OPR, 0 );
	template_id[ rdoModelObjects::FUN ] = TemplateData( IDR_MODEL_TMP5_FUN, 0 );
	template_id[ rdoModelObjects::SMR ] = TemplateData( IDR_MODEL_TMP5_SMR, 0 );
	template_id[ rdoModelObjects::PMD ] = TemplateData( IDR_MODEL_TMP5_PMD, 0 );
	model_templates[5] = template_id;

	template_id.clear();
	template_id[ rdoModelObjects::PAT ] = TemplateData( IDR_MODEL_TMP6_PAT, 0 );
	template_id[ rdoModelObjects::RTP ] = TemplateData( IDR_MODEL_TMP6_RTP, 0 );
	template_id[ rdoModelObjects::RSS ] = TemplateData( IDR_MODEL_TMP6_RSS, 0 );
	template_id[ rdoModelObjects::OPR ] = TemplateData( IDR_MODEL_TMP6_OPR, 0 );
	template_id[ rdoModelObjects::FUN ] = TemplateData( IDR_MODEL_TMP6_FUN, 0 );
	template_id[ rdoModelObjects::SMR ] = TemplateData( IDR_MODEL_TMP6_SMR, 0 );
	template_id[ rdoModelObjects::PMD ] = TemplateData( IDR_MODEL_TMP6_PMD, 0 );
	model_templates[6] = template_id;

	template_id.clear();
	template_id[ rdoModelObjects::PAT ] = TemplateData( IDR_MODEL_TMP7_PAT, 0 );
	template_id[ rdoModelObjects::RTP ] = TemplateData( IDR_MODEL_TMP7_RTP, 0 );
	template_id[ rdoModelObjects::RSS ] = TemplateData( IDR_MODEL_TMP7_RSS, 0 );
	template_id[ rdoModelObjects::OPR ] = TemplateData( IDR_MODEL_TMP7_OPR, 0 );
	template_id[ rdoModelObjects::FUN ] = TemplateData( IDR_MODEL_TMP7_FUN, 0 );
	template_id[ rdoModelObjects::SMR ] = TemplateData( IDR_MODEL_TMP7_SMR, 0 );
	template_id[ rdoModelObjects::PMD ] = TemplateData( IDR_MODEL_TMP7_PMD, 0 );
	model_templates[7] = template_id;

	notifies.push_back( RT_STUDIO_MODEL_GET_TEXT );
	notifies.push_back( RT_REPOSITORY_MODEL_NEW );
	notifies.push_back( RT_REPOSITORY_MODEL_OPEN );
	notifies.push_back( RT_REPOSITORY_MODEL_OPEN_GET_NAME );
	notifies.push_back( RT_REPOSITORY_MODEL_OPEN_ERROR );
	notifies.push_back( RT_REPOSITORY_MODEL_CLOSE );
	notifies.push_back( RT_REPOSITORY_MODEL_CLOSE_CAN_CLOSE );
	notifies.push_back( RT_REPOSITORY_MODEL_CLOSE_ERROR );
	notifies.push_back( RT_REPOSITORY_MODEL_SAVE );
	notifies.push_back( RT_REPOSITORY_MODEL_SAVE_GET_NAME );
	notifies.push_back( RT_SIMULATOR_PARSE_STRING );
	notifies.push_back( RT_SIMULATOR_PARSE_OK );
	notifies.push_back( RT_SIMULATOR_PARSE_ERROR );
	notifies.push_back( RT_SIMULATOR_PARSE_ERROR_SMR_EMPTY );
	notifies.push_back( RT_SIMULATOR_MODEL_STOP_OK );
	notifies.push_back( RT_SIMULATOR_MODEL_STOP_BY_USER );
	notifies.push_back( RT_SIMULATOR_MODEL_STOP_RUNTIME_ERROR );
	notifies.push_back( RT_RUNTIME_MODEL_START_BEFORE );
	notifies.push_back( RT_RUNTIME_MODEL_START_AFTER );
	notifies.push_back( RT_RUNTIME_MODEL_STOP_BEFORE );
	notifies.push_back( RT_DEBUG_STRING );

	after_constructor();
}

RDOStudioModel::~RDOStudioModel()
{
	model = NULL;
//	closeModel();
}

void RDOStudioModel::proc( RDOThread::RDOMessageInfo& msg )
{
	switch ( msg.message ) {
		case RDOThread::RT_STUDIO_MODEL_GET_TEXT: {
			msg.lock();
			rdoRepository::RDOThreadRepository::FileData* fdata = static_cast<rdoRepository::RDOThreadRepository::FileData*>(msg.param);
			RDOEditorTabCtrl* tab = getTab();
			if ( tab ) {
				RDOEditorEdit* edit = tab->getItemEdit(fdata->m_type);
				if ( edit ) {
					edit->save(fdata->m_stream);
				}
			}
			msg.unlock();
			break;
		}
		case RDOThread::RT_REPOSITORY_MODEL_NEW: {
			newModelFromRepository();
			if ( plugins ) plugins->pluginProc( rdoPlugin::PM_MODEL_NEW );
			break;
		}
		case RDOThread::RT_REPOSITORY_MODEL_OPEN: {
			openModelFromRepository();
			if ( plugins ) plugins->pluginProc( rdoPlugin::PM_MODEL_OPEN );
			break;
		}
		case RDOThread::RT_REPOSITORY_MODEL_OPEN_ERROR: {
			AfxMessageBox( rdo::format( ID_MSG_MODELOPEN_ERROR, static_cast<std::string*>(msg.param)->c_str() ).c_str(), MB_ICONSTOP | MB_OK );
			break;
		}
		case RDOThread::RT_REPOSITORY_MODEL_SAVE: {
			saveModelToRepository();
			break;
		}
		case RDOThread::RT_REPOSITORY_MODEL_OPEN_GET_NAME: {
			msg.lock();
			rdoRepository::RDOThreadRepository::OpenFile* data = static_cast<rdoRepository::RDOThreadRepository::OpenFile*>(msg.param);
			CString filter;
			filter.LoadString( ID_MODEL_FILETYPE );
			CFileDialog dlg( true, "smr", "", 0, filter, AfxGetMainWnd() );
			data->m_result   = dlg.DoModal() == IDOK;
			data->m_name     = dlg.GetPathName();
			data->m_readonly = dlg.GetReadOnlyPref() == TRUE;
			msg.unlock();
			break;
		}
		case RDOThread::RT_REPOSITORY_MODEL_SAVE_GET_NAME: {
			msg.lock();
			rdoRepository::RDOThreadRepository::OpenFile* data = static_cast<rdoRepository::RDOThreadRepository::OpenFile*>(msg.param);
			CString filter;
			filter.LoadString( ID_MODEL_FILETYPE );
			CFileDialog dlg( false, "smr", "", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter, AfxGetMainWnd() );
			data->m_result = dlg.DoModal() == IDOK;
			data->m_name   = dlg.GetPathName();
			msg.unlock();
			break;
		}
		case RDOThread::RT_REPOSITORY_MODEL_CLOSE: {
			closeModelFromRepository();
			if ( plugins ) plugins->pluginProc( rdoPlugin::PM_MODEL_CLOSE );
			break;
		}
		case RDOThread::RT_REPOSITORY_MODEL_CLOSE_CAN_CLOSE: {
			msg.lock();
			if ( *static_cast<bool*>(msg.param) == false ) {
				msg.unlock();
				break;
			}
			msg.unlock();
			bool res = canCloseModel();
			msg.lock();
			if ( *static_cast<bool*>(msg.param) == true ) {
				*static_cast<bool*>(msg.param) = res;
			}
			msg.unlock();
			break;
		}
		case RDOThread::RT_REPOSITORY_MODEL_CLOSE_ERROR: {
			if ( showCanNotCloseModelMessage ) {
				AfxMessageBox( rdo::format( ID_MSG_MODELCLOSE_ERROR ).c_str(), MB_ICONSTOP | MB_OK );
			}
			break;
		}
		case RDOThread::RT_RUNTIME_MODEL_START_BEFORE: {
			if ( plugins ) {
				plugins->modelStart();
				plugins->pluginProc( rdoPlugin::PM_MODEL_BEFORE_START );
			}
			break;
		}
		case RDOThread::RT_RUNTIME_MODEL_START_AFTER: {
			GUI_IS_RUNING = true;
			sendMessage( kernel->runtime(), RT_RUNTIME_GET_MODE, &runtimeMode );
			sendMessage( kernel->runtime(), RT_RUNTIME_GET_SPEED, &speed );
			setSpeed( studioApp.mainFrame->getSpeed() );
			sendMessage( kernel->runtime(), RT_RUNTIME_GET_SHOWRATE, &showRate );
			afterModelStart();
			RDOStudioOutput* output = &studioApp.mainFrame->output;
			output->showDebug();
			output->appendStringToDebug( rdo::format( IDS_MODEL_STARTED ) );
			const_cast<rdoEditCtrl::RDODebugEdit*>(output->getDebug())->UpdateWindow();
			int index = frameManager.getLastShowedFrame();
			if ( index != -1 ) {
				RDOStudioFrameView* view = frameManager.getFrameView( index );
				if ( view ) view->SetFocus();
			}
			studioApp.mainFrame->update_start();
			if ( plugins ) plugins->pluginProc( rdoPlugin::PM_MODEL_AFTER_START );
			break;
		}
		case RDOThread::RT_RUNTIME_MODEL_STOP_BEFORE: {
			studioApp.mainFrame->update_stop();
			sendMessage( kernel->runtime(), RT_RUNTIME_GET_TIMENOW, &timeNow );
			frameManager.clear();
			frameManager.bmp_clear();
			SYSTEMTIME time_stop;
			::GetSystemTime( &time_stop );
			unsigned int delay = -1;
			if ( time_start.wYear == time_stop.wYear && time_start.wMonth == time_stop.wMonth ) {
				delay = (time_stop.wDay - time_start.wDay) * 24 * 60 * 60 * 1000 + (time_stop.wHour - time_start.wHour) * 60 * 60 * 1000 + (time_stop.wMinute - time_start.wMinute) * 60 * 1000 + (time_stop.wSecond - time_start.wSecond) * 1000 + (time_stop.wMilliseconds - time_start.wMilliseconds );
			} else if ( time_stop.wYear - time_start.wYear == 1 && time_start.wMonth == 12 && time_stop.wMonth == 1 ) {
				delay = (time_stop.wDay + 31 - time_start.wDay) * 24 * 60 * 60 * 1000 + (time_stop.wHour - time_start.wHour) * 60 * 60 * 1000 + (time_stop.wMinute - time_start.wMinute) * 60 * 1000 + (time_stop.wSecond - time_start.wSecond) * 1000 + (time_stop.wMilliseconds - time_start.wMilliseconds );
			}
			if ( delay != -1 ) {
				studioApp.mainFrame->output.appendStringToDebug( rdo::format(IDS_MODEL_DURATION, delay ) );
			}
			GUI_CAN_RUN   = true;
			GUI_IS_RUNING = false;
			break;
		}
		case RDOThread::RT_SIMULATOR_MODEL_STOP_OK: {
			RDOStudioOutput* output = &studioApp.mainFrame->output;
			output->appendStringToDebug( rdo::format( IDS_MODEL_FINISHED ) );
			const_cast<rdoEditCtrl::RDODebugEdit*>(output->getDebug())->UpdateWindow();

			show_result();

			if ( plugins ) {
				plugins->pluginProc( rdoPlugin::PM_MODEL_FINISHED );
				plugins->modelStop();
			}
			studioApp.autoCloseByModel();
			break;
		}
		case RDOThread::RT_SIMULATOR_MODEL_STOP_BY_USER: {
			sendMessage( kernel->simulator(), RT_SIMULATOR_GET_MODEL_EXITCODE, &exitCode );
			RDOStudioOutput* output = &studioApp.mainFrame->output;
			output->appendStringToDebug( rdo::format( IDS_MODEL_STOPED ) );
			const_cast<rdoEditCtrl::RDODebugEdit*>(output->getDebug())->UpdateWindow();

			show_result();

			if ( plugins ) {
				plugins->pluginProc( rdoPlugin::PM_MODEL_STOP_CANCEL );
				plugins->modelStop( false );
			}
			break;
		}
		case RDOThread::RT_SIMULATOR_MODEL_STOP_RUNTIME_ERROR: {
			sendMessage( kernel->simulator(), RT_SIMULATOR_GET_MODEL_EXITCODE, &exitCode );
			show_result();
			RDOStudioOutput* output = &studioApp.mainFrame->output;
			output->appendStringToDebug( rdo::format( IDS_MODEL_RUNTIMEERROR_STOPED ) );
			output->clearBuild();
			output->showBuild();
			output->appendStringToBuild( rdo::format( IDS_MODEL_RUNTIMEERROR ) );
			std::vector< RDOSyntaxError > errors;
			studioApp.studioGUI->sendMessage( kernel->simulator(), RDOThread::RT_SIMULATOR_GET_ERRORS, &errors );
			int errors_cnt   = 0;
			int warnings_cnt = 0;
			for ( std::vector< RDOSyntaxError >::iterator it = errors.begin(); it != errors.end(); it++ ) {
				output->appendStringToBuild( it->error_code, it->message, it->file, it->error_line, it->error_pos, it->warning );
				if ( it->warning ) {
					warnings_cnt++;
				} else {
					errors_cnt++;
				}
			}
			if ( errors_cnt || warnings_cnt ) {
//				const_cast<rdoEditCtrl::RDOBuildEdit*>(output->getBuild())->showFirstError();
			}

			if ( plugins ) {
				plugins->pluginProc( rdoPlugin::PM_MODEL_STOP_RUNTIME_ERROR );
				plugins->modelStop( false );
			}
			studioApp.autoCloseByModel();
			break;
		}
		case RDOThread::RT_SIMULATOR_PARSE_OK: {
			::GetSystemTime( &time_start );
			sendMessage( kernel->simulator(), RT_SIMULATOR_GET_MODEL_EXITCODE, &exitCode );
			RDOStudioOutput* output = &studioApp.mainFrame->output;
			std::vector< RDOSyntaxError > errors;
			studioApp.studioGUI->sendMessage( kernel->simulator(), RDOThread::RT_SIMULATOR_GET_ERRORS, &errors );
			int errors_cnt   = 0;
			int warnings_cnt = 0;
			for ( std::vector< RDOSyntaxError >::iterator it = errors.begin(); it != errors.end(); it++ ) {
				output->appendStringToBuild( it->error_code, it->message, it->file, it->error_line, it->error_pos, it->warning );
				if ( it->warning ) {
					warnings_cnt++;
				} else {
					errors_cnt++;
				}
			}
			output->appendStringToBuild( rdo::format( IDS_MODEL_BUILDING_RESULTS, errors_cnt, warnings_cnt ) );
			if ( errors_cnt || warnings_cnt ) {
//				const_cast<rdoEditCtrl::RDOBuildEdit*>(output->getBuild())->showFirstError();
			}
			if ( plugins ) plugins->pluginProc( rdoPlugin::PM_MODEL_BUILD_OK );
			break;
		}
		case RDOThread::RT_SIMULATOR_PARSE_ERROR: {
			GUI_IS_RUNING = false;
			sendMessage( kernel->simulator(), RT_SIMULATOR_GET_MODEL_EXITCODE, &exitCode );
			RDOStudioOutput* output = &studioApp.mainFrame->output;
			std::vector< RDOSyntaxError > errors;
			studioApp.studioGUI->sendMessage( kernel->simulator(), RDOThread::RT_SIMULATOR_GET_ERRORS, &errors );
			int errors_cnt   = 0;
			int warnings_cnt = 0;
			for ( std::vector< RDOSyntaxError >::iterator it = errors.begin(); it != errors.end(); it++ ) {
				output->appendStringToBuild( it->error_code, it->message, it->file, it->error_line, it->error_pos, it->warning );
				if ( it->warning ) {
					warnings_cnt++;
				} else {
					errors_cnt++;
				}
			}
			output->appendStringToBuild( rdo::format( IDS_MODEL_BUILDING_RESULTS, errors_cnt, warnings_cnt ) );
			if ( errors_cnt || warnings_cnt ) {
				const_cast<rdoEditCtrl::RDOBuildEdit*>(output->getBuild())->showFirstError();
			}

			if ( plugins ) plugins->pluginProc( rdoPlugin::PM_MODEL_BUILD_FAILD );

			GUI_CAN_RUN = true;
			studioApp.autoCloseByModel();
			break;
		}
		case RDOThread::RT_SIMULATOR_PARSE_ERROR_SMR_EMPTY: {
			smrEmptyError = true;
			break;
		}
		case RDOThread::RT_SIMULATOR_PARSE_STRING: {
			msg.lock();
			studioApp.mainFrame->output.appendStringToBuild( *static_cast<std::string*>(msg.param) );
			msg.unlock();
			break;
		}
		case RDOThread::RT_DEBUG_STRING: {
			msg.lock();
			studioApp.mainFrame->output.appendStringToDebug( *static_cast<std::string*>(msg.param) );
			msg.unlock();
			break;
		}
	}
}

void RDOStudioModel::show_result()
{
	rdo::textstream model_results;
	sendMessage( kernel->simulator(), RT_SIMULATOR_GET_MODEL_RESULTS, &model_results );
	std::string str = model_results.str();
	if ( !str.empty() ) {
		RDOStudioOutput* output = &studioApp.mainFrame->output;
		rdoRepository::RDOThreadRepository::FileInfo data( rdoModelObjects::PMV );
		studioApp.studioGUI->sendMessage( kernel->repository(), RDOThread::RT_REPOSITORY_MODEL_GET_FILEINFO, &data );
		if ( !data.m_described ) {
			output->appendStringToDebug( "Результаты не будут записаны в файл, т.к. в SMR не определен Results_file\n" );
		}
		output->showResults();
		output->appendStringToResults( str );
	}
}

bool RDOStudioModel::newModel( std::string _model_name, std::string _model_path, const int _useTemplate  )
{
	if ( !plugins->canAction( rdoPlugin::maCreate ) ) return false;
	useTemplate = _useTemplate;
	RDOStudioOutput* output = &studioApp.mainFrame->output;
	output->clearBuild();
	output->clearDebug();
	output->clearResults();
	output->clearFind();
	rdoRepository::RDOThreadRepository::NewModel data;
	data.m_name = _model_name;
	data.m_path = _model_path;
	studioApp.broadcastMessage( RDOThread::RT_STUDIO_MODEL_NEW, &data );
	output->updateLogConnection();
	return true;
}

bool RDOStudioModel::openModel( const std::string& modelName ) const
{
	if ( !plugins->canAction( rdoPlugin::maOpen ) ) return false;
	if ( isRunning() ) {
		AfxGetMainWnd()->MessageBox( rdo::format( ID_MSG_MODEL_NEED_STOPED_FOR_OPEN ).c_str(), NULL, MB_ICONEXCLAMATION | MB_OK );
		return false;
	}
	if ( !closeModel() ) return false;
	RDOStudioOutput* output = &studioApp.mainFrame->output;
	output->clearBuild();
	output->clearDebug();
	output->clearResults();
	output->clearFind();
	output->showDebug();
	output->appendStringToDebug( rdo::format( IDS_MODEL_LOADING_BEGIN ) );
	const_cast<rdoEditCtrl::RDODebugEdit*>(output->getDebug())->UpdateWindow();
	openError     = false;
	smrEmptyError = false;
	modelClosed   = false;
	rdoRepository::RDOThreadRepository::OpenFile data( modelName );
	studioApp.broadcastMessage( RDOThread::RT_STUDIO_MODEL_OPEN, &data );
	if ( data.m_result && !openError && !smrEmptyError ) {
		rdo::binarystream stream;
		studioApp.studioGUI->sendMessage( kernel->repository(), RDOThread::RT_REPOSITORY_LOAD, &rdoRepository::RDOThreadRepository::FileData( rdoModelObjects::PMV, stream ) );
		output->appendStringToResults( stream.str() );
		output->updateLogConnection();
		output->appendStringToDebug( rdo::format( IDS_MODEL_LOADING_OK ) );
		studioApp.setLastProjectName( getFullName() );
	} else {
		if ( smrEmptyError ) {
			studioApp.mainFrame->MessageBox( "В smr-файле не найдено имя модели", "Ошибка открытия модели", MB_OK | MB_ICONERROR );
			closeModel();
		} else {
			output->updateLogConnection();
			output->appendStringToDebug( rdo::format( IDS_MODEL_LOADING_FAILD ) );
		}
	}
	return data.m_result;
}

bool RDOStudioModel::saveModel() const
{
	if ( !plugins->canAction( rdoPlugin::maSave ) ) return false;
	bool res = true;
	studioApp.broadcastMessage( RDOThread::RT_STUDIO_MODEL_SAVE, &res );
	return res;
}

void RDOStudioModel::saveAsModel() const
{
	studioApp.broadcastMessage( RDOThread::RT_STUDIO_MODEL_SAVE_AS );
}

bool RDOStudioModel::closeModel() const
{
	if ( !plugins->canAction( rdoPlugin::maClose ) ) return false;
	if ( !isRunning() ) {
		stopModel();
		RDOStudioOutput* output = &studioApp.mainFrame->output;
		if ( output && output->GetSafeHwnd() ) {
			output->clearBuild();
			output->clearDebug();
			output->clearResults();
			output->clearFind();
		}
		studioApp.broadcastMessage( RDOThread::RT_STUDIO_MODEL_CLOSE );
		return true;
	} else {
		AfxGetMainWnd()->MessageBox( rdo::format( ID_MSG_MODEL_NEED_STOPED_FOR_CLOSE ).c_str(), NULL, MB_ICONWARNING | MB_OK );
		return false;
	}
}

bool RDOStudioModel::buildModel() const
{
	if ( !plugins->canAction( rdoPlugin::maBuild ) ) return false;
	if ( hasModel() && !isRunning() && saveModel() ) {
		RDOStudioOutput* output = &studioApp.mainFrame->output;
		output->clearBuild();
		output->clearDebug();
		output->clearResults();
		output->showBuild();
		output->appendStringToBuild( rdo::format( IDS_MODEL_BUILDING_BEGIN ) );
		const_cast<rdoEditCtrl::RDOBuildEdit*>(output->getBuild())->UpdateWindow();
		studioApp.broadcastMessage( RDOThread::RT_STUDIO_MODEL_BUILD );
		return true;
	}
	return false;
}

bool RDOStudioModel::runModel()
{
	if ( !plugins->canAction( rdoPlugin::maRun ) ) return false;
	if ( hasModel() && !isRunning() && saveModel() ) {
		GUI_CAN_RUN = false;
		RDOStudioOutput* output = &studioApp.mainFrame->output;
		output->clearBuild();
		output->clearDebug();
		output->clearResults();
		output->showBuild();
		output->appendStringToBuild( rdo::format( IDS_MODEL_BUILDING_BEGIN ) );
		const_cast<rdoEditCtrl::RDOBuildEdit*>(output->getBuild())->UpdateWindow();
		studioApp.broadcastMessage( RDOThread::RT_STUDIO_MODEL_RUN );
		return true;
	}
	return false;
}

bool RDOStudioModel::stopModel() const
{
	if ( hasModel() && isRunning() ) {
		studioApp.broadcastMessage( RDOThread::RT_STUDIO_MODEL_STOP );
		return true;
	}
	return false;
}

void RDOStudioModel::newModelFromRepository()
{
	if ( modelDocTemplate ) {

		GUI_HAS_MODEL = true;

		BOOL maximize = false;
		if ( !studioApp.mainFrame->MDIGetActive( &maximize ) ) {
			maximize = true;
		}

		modelDocTemplate->OpenDocumentFile( NULL );
		rdoRepository::RDOThreadRepository::FileInfo data_smr;
		studioApp.studioGUI->sendMessage( kernel->repository(), RDOThread::RT_REPOSITORY_MODEL_GET_FILEINFO, &data_smr );
		setName( data_smr.m_name );

		RDOEditorTabCtrl* tab = getTab();
		if ( tab ) {
			for ( int i = 0; i < tab->getItemCount(); i++ ) {
				RDOEditorEdit* edit = tab->getItemEdit( i );
				edit->setReadOnly( false );
				edit->clearAll();
				if ( useTemplate != -1 && model_templates.find( useTemplate ) != model_templates.end() ) {
					std::map< rdoModelObjects::RDOFileType, TemplateData >::const_iterator it = model_templates[ useTemplate ].find( tab->indexToType( i ) );
					if ( it != model_templates[ useTemplate ].end() ) {
						int resID = it->second.res_id;
						if ( resID != - 1 ) {
							// Пытаемся загрузить из String Table
							std::string s = rdo::format( resID );
							if ( s.empty() ) {
								// Загрузка из String Table не удалась, пытаемся загрузить из MODEL_TMP (a-la RCDATA)
								HRSRC res = ::FindResource( studioApp.m_hInstance, MAKEINTRESOURCE(resID), "MODEL_TMP" );
								if ( res ) {
									HGLOBAL res_global = ::LoadResource( studioApp.m_hInstance, res );
									if ( res_global ) {
										LPTSTR res_data = static_cast<LPTSTR>(::LockResource( res_global ));
										if ( res_data ) {
											DWORD dwSize = ::SizeofResource( studioApp.m_hInstance, res );
											CString s_res;
											LPTSTR s_res_data = s_res.GetBuffer( dwSize + 1 );
											memcpy( s_res_data, res_data, dwSize );
											s_res_data[dwSize] = NULL;
											s_res.ReleaseBuffer();
											s = s_res;
										}
									}
								}
							}
							if ( tab->indexToType( i ) == rdoModelObjects::SMR ) {
								std::string name = getName();
								s = rdo::format( s.c_str(), name.c_str(), name.c_str(), name.c_str(), name.c_str(), name.c_str(), name.c_str(), name.c_str() );
							}
							if ( !s.empty() ) {
								edit->replaceCurrent( s );
								edit->updateEditGUI();
								edit->scrollToLine( 0 );
								edit->setCurrentPos( it->second.position );
							}
						}
					}
				}
//				edit->setModifyFalse();
//				edit->clearUndoBuffer();
			}
		}

		CWnd* wnd = studioApp.mainFrame->GetActiveFrame();
		if ( maximize && wnd && wnd != studioApp.mainFrame ) {
			studioApp.mainFrame->MDIMaximize( wnd );
		}
		studioApp.setLastProjectName( getFullName() );
		if ( useTemplate != -1 ) {
			saveModel();
		}
	}
}

void RDOStudioModel::openModelFromRepository()
{
	if ( modelDocTemplate ) {

		GUI_HAS_MODEL = true;

		BOOL maximize = false;
		if ( !studioApp.mainFrame->MDIGetActive( &maximize ) ) {
			maximize = true;
		}

		CWnd* active = CWnd::GetActiveWindow();
		modelDocTemplate->OpenDocumentFile( NULL );
		rdoRepository::RDOThreadRepository::FileInfo data_smr;
		studioApp.studioGUI->sendMessage( kernel->repository(), RDOThread::RT_REPOSITORY_MODEL_GET_FILEINFO, &data_smr );
		setName( data_smr.m_name );

		RDOStudioOutput* output = &studioApp.mainFrame->output;
		RDOEditorTabCtrl* tab = getTab();
		if ( tab ) {
			int cnt = tab->getItemCount();
			studioApp.mainFrame->beginProgress( 0, cnt * 2 + 1 );
			studioApp.mainFrame->stepProgress();
			for ( int i = 0; i < cnt; i++ ) {
				RDOEditorEdit* edit = tab->getItemEdit( i );
				edit->setReadOnly( false );
				edit->clearAll();
				rdo::binarystream stream;
				bool canLoad = true;
				rdoModelObjects::RDOFileType type = tab->indexToType( i );
				if ( tab->typeSupported( type ) ) {
					studioApp.studioGUI->sendMessage( kernel->repository(), RDOThread::RT_REPOSITORY_LOAD, &rdoRepository::RDOThreadRepository::FileData( type, stream ) );
				} else {
					canLoad = false;
				}
				studioApp.mainFrame->stepProgress();
				if ( canLoad ) {
					rdoRepository::RDOThreadRepository::FileInfo data( type );
					studioApp.studioGUI->sendMessage( kernel->repository(), RDOThread::RT_REPOSITORY_MODEL_GET_FILEINFO, &data );
					bool stream_error = stream.rdstate() & std::ios_base::failbit ? true : false;
					if ( !stream_error ) {
						edit->load( stream );
						edit->setReadOnly( data.m_readonly );
						if ( data.m_readonly ) {
							output->appendStringToDebug( rdo::format( IDS_MODEL_FILE_READONLY, std::string( data.m_name + data.m_extention).c_str() ) );
						}
					} else {
						int obj = 0;
						switch ( type ) {
							case rdoModelObjects::PAT: obj = IDS_MODEL_PATTERNS;      break;
							case rdoModelObjects::RTP: obj = IDS_MODEL_RESOURCETYPES; break;
							case rdoModelObjects::RSS: obj = IDS_MODEL_RESOURCES;     break;
							case rdoModelObjects::OPR: obj = IDS_MODEL_OPERATIONS;    break;
							case rdoModelObjects::FRM: obj = IDS_MODEL_FRAMES;        break;
							case rdoModelObjects::FUN: obj = IDS_MODEL_FUNCTIONS;     break;
							case rdoModelObjects::DPT: obj = IDS_MODEL_DPTS;          break;
							case rdoModelObjects::PMD: obj = IDS_MODEL_PMDS;          break;
						}
						if ( obj ) {
							output->appendStringToDebug( rdo::format( IDS_MODEL_CANNOT_LOAD, rdo::format( obj ).c_str(), data.m_full_name.c_str() ) );
							const_cast<rdoEditCtrl::RDODebugEdit*>(output->getDebug())->UpdateWindow();
						}
						openError = true;
					}
				}
				edit->setCurrentPos( 0 );
				edit->setModifyFalse();
				edit->clearUndoBuffer();
				studioApp.mainFrame->stepProgress();
			}
			studioApp.mainFrame->endProgress();
		}

		CWnd* wnd = studioApp.mainFrame->GetActiveFrame();
		if ( maximize && wnd && wnd != studioApp.mainFrame ) {
			studioApp.mainFrame->MDIMaximize( wnd );
		}

		updateFrmDescribed();

		if ( active ) active->SetFocus();
	}
}

void RDOStudioModel::saveModelToRepository()
{
	bool smr_modified = false;
	bool wasSaved = false;
	RDOEditorTabCtrl* tab = getTab();
	if ( tab ) {
		RDOEditorEdit* smr_edit = tab->getItemEdit( rdoModelObjects::SMR );
		if ( smr_edit->isModify() ) {
			rdo::binarystream stream;
			smr_edit->save( stream );
			smrEmptyError = false;
			studioApp.studioGUI->sendMessage( kernel->repository(), RDOThread::RT_REPOSITORY_SAVE, &rdoRepository::RDOThreadRepository::FileData( rdoModelObjects::SMR, stream ) );
			if ( smrEmptyError ) {
				studioApp.mainFrame->MessageBox( "В smr-файле не найдено имя модели, модель не будет записана", "Ошибка записи модели", MB_OK | MB_ICONERROR );
				return;
			}
			smr_modified = true;
		}
		int cnt = tab->getItemCount();
		int progress_cnt = 0;
		for ( int i = 0; i < cnt; i++ ) {
			if ( smr_modified || tab->getItemEdit( i )->isModify() ) progress_cnt++;
		}
		if ( progress_cnt ) {
			studioApp.mainFrame->beginProgress( 0, progress_cnt * 2 + 1 );
			studioApp.mainFrame->stepProgress();
			for ( int i = 0; i < cnt; i++ ) {
				RDOEditorEdit* edit = tab->getItemEdit( i );
				if ( smr_modified || edit->isModify() ) {
					if ( !edit->isEmpty() ) {
						rdo::binarystream stream;
						edit->save( stream );
						studioApp.mainFrame->stepProgress();
						rdoModelObjects::RDOFileType type = tab->indexToType( i );
						switch ( type ) {
							case rdoModelObjects::PAT:
							case rdoModelObjects::RTP:
							case rdoModelObjects::RSS:
							case rdoModelObjects::OPR:
							case rdoModelObjects::FRM:
							case rdoModelObjects::FUN:
							case rdoModelObjects::DPT:
							case rdoModelObjects::PMD: studioApp.studioGUI->sendMessage( kernel->repository(), RDOThread::RT_REPOSITORY_SAVE, &rdoRepository::RDOThreadRepository::FileData( type, stream ) ); break;
							default: break;
						}
					} else {
						studioApp.mainFrame->stepProgress();
					}
					edit->setModifyFalse();
				}
				studioApp.mainFrame->stepProgress();
			}
			studioApp.mainFrame->endProgress();
			wasSaved = true;
		}
	}
	rdoRepository::RDOThreadRepository::FileInfo data;
	studioApp.studioGUI->sendMessage( kernel->repository(), RDOThread::RT_REPOSITORY_MODEL_GET_FILEINFO, &data );
	setName( data.m_name );

	studioApp.insertReopenItem( getFullName() );

	if ( smr_modified ) updateFrmDescribed();

	if ( wasSaved && plugins ) {
		plugins->pluginProc( rdoPlugin::PM_MODEL_SAVE );
	}
}

std::string RDOStudioModel::getFullName() const
{
	rdoRepository::RDOThreadRepository::FileInfo data;
	studioApp.studioGUI->sendMessage( kernel->repository(), RDOThread::RT_REPOSITORY_MODEL_GET_FILEINFO, &data );
	return data.m_full_name;
}

void RDOStudioModel::updateFrmDescribed()
{
//	frmDescribed = kernel->repository()->isDescribed( rdoModelObjects::FRM );
	frmDescribed = true;
}

bool RDOStudioModel::canCloseModel()
{
	bool flag = true;
	if ( isModify() && autoDeleteDoc ) {
		int res = AfxGetMainWnd()->MessageBox( rdo::format( ID_MSG_MODELSAVE_QUERY ).c_str(), NULL, MB_ICONQUESTION | MB_YESNOCANCEL );
		switch ( res ) {
			case IDYES   : flag = saveModel(); break;
			case IDNO    : flag = true; break;
			case IDCANCEL: flag = false; break;
		}
	}
	if ( !flag ) {
		showCanNotCloseModelMessage = false;
	}
	return flag;
}

void RDOStudioModel::closeModelFromRepository()
{
	if ( autoDeleteDoc ) {
		RDOStudioModelDoc* doc = getModelDoc();
		if ( doc ) {
			doc->OnCloseDocument();
		}
	}
	GUI_HAS_MODEL = false;
	if ( !showCanNotCloseModelMessage ) {
		showCanNotCloseModelMessage = true;
	}
	modelClosed = true;
}

void RDOStudioModel::setName( const std::string& str )
{
	RDOStudioModelDoc* doc = getModelDoc();
	bool flag = false;
	if ( doc ) {
		flag = doc->getName() != str;
		doc->setName( str );
	}
	if ( flag && plugins ) {
		plugins->pluginProc( rdoPlugin::PM_MODEL_NAME_CHANGED );
	}
}

void RDOStudioModel::afterModelStart()
{
	frameManager.clear();
	frameManager.bmp_clear();

	if ( isFrmDescribed() ) {
		RDOStudioOutput* output = &studioApp.mainFrame->output;
		output->showDebug();
		output->appendStringToDebug( rdo::format( IDS_MODEL_RESOURCE_LOADING_BEGIN ) );
		const_cast<rdoEditCtrl::RDODebugEdit*>(output->getDebug())->UpdateWindow();

		std::list< std::string > frames;
		std::list< std::string > bitmaps;
		sendMessage( kernel->simulator(), RT_SIMULATOR_GET_LIST, &rdoSimulator::RDOThreadSimulator::GetList(rdoSimulator::RDOThreadSimulator::GetList::frames, &frames) );
		sendMessage( kernel->simulator(), RT_SIMULATOR_GET_LIST, &rdoSimulator::RDOThreadSimulator::GetList(rdoSimulator::RDOThreadSimulator::GetList::bitmaps, &bitmaps) );
		std::list< std::string >::iterator bmp_it = bitmaps.begin();
		while ( bmp_it != bitmaps.end() ) {
			frameManager.bmp_insert( *bmp_it );
			bmp_it++;
		}
		std::list< std::string >::iterator frame_it = frames.begin();
		while ( frame_it != frames.end() ) {
			frameManager.insertItem( *frame_it );
			frame_it++;
		}
		frameManager.expand();
		int initFrameNumber = kernel->simulator()->getInitialFrameNumber() - 1;
		timeNow = 0;
		frameManager.setLastShowedFrame( initFrameNumber );
		if ( getRuntimeMode() != rdoRuntime::RTM_MaxSpeed && initFrameNumber >= 0 && initFrameNumber < frameManager.count() ) {
			RDOStudioFrameDoc* doc = frameManager.connectFrameDoc( initFrameNumber );
			if ( doc ) {
				frameManager.getFrameView( initFrameNumber )->SetFocus();
			}
		}
		output->appendStringToDebug( rdo::format( IDS_MODEL_RESOURCE_LOADING_OK ) );
		const_cast<rdoEditCtrl::RDODebugEdit*>(output->getDebug())->UpdateWindow();
	} else {
		timeNow = 0;
		frameManager.setLastShowedFrame( -1 );
	}
}

void RDOStudioModel::updateStyleOfAllModel() const
{
	POSITION pos = modelDocTemplate->GetFirstDocPosition();
	while ( pos ) {
		RDOStudioModelDoc* doc = static_cast<RDOStudioModelDoc*>(modelDocTemplate->GetNextDoc( pos ));
		RDOEditorTabCtrl* tab = doc->getView()->tab;
		for ( int i = 0; i < tab->getItemCount(); i++ ) {
			tab->getItemEdit( i )->setEditorStyle( &studioApp.mainFrame->style_editor );
		}
	}
	frameManager.updateStyles();
}

void RDOStudioModel::setRuntimeMode( const rdoRuntime::RunTimeMode value )
{
	if ( isRunning() ) {
		if ( tempPause ) {
			rdoRuntime::RunTimeMode _value = value;
			sendMessage( kernel->runtime(), RT_RUNTIME_SET_MODE, &_value );
			return;
		}
		runtimeMode = value;
		sendMessage( kernel->runtime(), RT_RUNTIME_SET_MODE, &runtimeMode );
		tracer->setRuntimeMode( runtimeMode );
		if ( plugins ) plugins->pluginProc( rdoPlugin::PM_MODEL_RUNTIMEMODE );
		switch ( runtimeMode ) {
			case rdoRuntime::RTM_MaxSpeed: closeAllFrame(); break;
			default: {
				RDOStudioFrameDoc* doc = frameManager.getFirstExistDoc();
				if ( !doc ) {
					frameManager.connectFrameDoc( frameManager.getLastShowedFrame() );
				}
				break;
			}
		}
	}
}

std::string RDOStudioModel::getLastBreakPointName()
{
	std::string str;
	sendMessage( kernel->runtime(), RT_RUNTIME_GET_LAST_BREAKPOINT, &str );
	return str;
}

void RDOStudioModel::setSpeed( double persent )
{
	if ( persent >= 0 && persent <= 1 && speed != persent ) {
		speed = persent;
		if ( isRunning() ) {
			sendMessage( kernel->runtime(), RT_RUNTIME_SET_SPEED, &speed );
		}
	}
}

void RDOStudioModel::setShowRate( double value )
{
	if ( !isRunning() ) return;
	if ( value >= DBL_MIN && value <= DBL_MAX ) {
		showRate = value;
		sendMessage( kernel->runtime(), RT_RUNTIME_SET_SHOWRATE, &showRate );
	}
}

void RDOStudioModel::update()
{
	sendMessage( kernel->runtime(), RT_RUNTIME_GET_TIMENOW, &timeNow );
	rdoRuntime::RunTimeMode rm;
	sendMessage( kernel->runtime(), RT_RUNTIME_GET_MODE, &rm );
	if ( rm != runtimeMode ) {
		if ( rm == rdoRuntime::RTM_BreakPoint ) {
			RDOStudioOutput* output = &studioApp.mainFrame->output;
			output->appendStringToDebug( rdo::format("Пауза в %f из-за точки '%s'\n", getTimeNow(), getLastBreakPointName().c_str()) );
		}
		setRuntimeMode( rm );
	}
	if ( getRuntimeMode() == rdoRuntime::RTM_MaxSpeed ) return;
	int frames = getFrameCount();
	for ( int i = 0; i < frames; i++ ) {
		if ( frameManager.isChanged() ) break;
		RDOStudioFrameView* view = frameManager.getFrameView( i );
		if ( view ) {
			CDC* dc = view->GetDC();
			if ( dc->RectVisible( view->getClientRect() ) ) {
				view->ReleaseDC( dc );
				try {
					rdoAnimation::RDOFrame frame;
					sendMessage( kernel->runtime(), RT_RUNTIME_GET_FRAME, &rdoRuntime::RDOThreadRunTime::GetFrame(&frame, i) );
					frameManager.showFrame( &frame, i );
				} catch ( rdoRuntime::RDORuntimeException& ) {
					sendMessage( kernel->runtime(), RT_SIMULATOR_MODEL_STOP_RUNTIME_DELAY );
					return;
				}
			} else {
				view->ReleaseDC( dc );
			}
		}
	}
}

void RDOStudioModel::setGUIPause()
{
	tempPause = true;
	if ( isRunning() ) {
		sendMessage( kernel->runtime(), RT_RUNTIME_GET_MODE, &runtimeMode_prev );
		setRuntimeMode( rdoRuntime::RTM_Pause );
	}
}

void RDOStudioModel::setGUIContinue()
{
	tempPause = false;
	if ( isRunning() ) {
		setRuntimeMode( runtimeMode_prev );
	}
}
