#include "stdafx.h"
#include "rdostudiomodel.h"
#include "rdostudioapp.h"
#include "rdostudiomainfrm.h"
#include "rdostudiochildfrm.h"
#include "rdostudiomodeldoc.h"
#include "rdostudiomodelview.h"
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
#include <rdobinarystream.h>
#include <rdoplugin.h>

using namespace rdoEditor;
using namespace rdosim;

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
	modelDocTemplate( NULL ),
	useTemplate( false ),
	closeWithDocDelete( true ),
	showCanNotCloseModelMessage( true ),
	openError( false ),
	modelClosed( true ),
	saveAsFlag( false ),
	frmDescribed( false ),
	modelTime( 0 ),
	showMode( SM_NoShow ),
	prevModify( false )
{
	model = this;

	modelDocTemplate = new CMultiDocTemplate( IDR_MODEL_TYPE, RUNTIME_CLASS(RDOStudioModelDoc), RUNTIME_CLASS(RDOStudioChildFrame), RUNTIME_CLASS(RDOStudioModelView) );
	AfxGetApp()->AddDocTemplate( modelDocTemplate );

	kernel.setNotifyReflect( RDOKernel::newModel, newModelNotify );
	kernel.setNotifyReflect( RDOKernel::openModel, openModelNotify );
	kernel.setNotifyReflect( RDOKernel::saveModel, saveModelNotify );
	kernel.setNotifyReflect( RDOKernel::canCloseModel, canCloseModelNotify );
	kernel.setNotifyReflect( RDOKernel::closeModel, closeModelNotify );
	kernel.setNotifyReflect( RDOKernel::canNotCloseModel, canNotCloseModelNotify );

	kernel.setNotifyReflect( RDOKernel::beforeModelStart, beforeModelStartNotify );
	kernel.setNotifyReflect( RDOKernel::afterModelStart, afterModelStartNotify );
	kernel.setNotifyReflect( RDOKernel::endExecuteModel, endExecuteModelNotify );
	kernel.setNotifyReflect( RDOKernel::modelStopped, stopModelNotify );
	kernel.setNotifyReflect( RDOKernel::parseSuccess, parseSuccessNotify );
	kernel.setNotifyReflect( RDOKernel::parseError, parseErrorNotify );
	kernel.setNotifyReflect( RDOKernel::executeError, executeErrorNotify );
	kernel.setNotifyReflect( RDOKernel::showFrame, showFrameNotify );

	kernel.setNotifyReflect( RDOKernel::buildString, buildNotify );
	kernel.setNotifyReflect( RDOKernel::debugString, debugNotify );

	kernel.setCallbackReflect( RDOKernel::modelExit, modelExitCallback );
}

RDOStudioModel::~RDOStudioModel()
{
	closeModel();
	model = NULL;
}

void RDOStudioModel::newModel( const bool _useTemplate )
{
	useTemplate = _useTemplate;
	RDOStudioOutput* output = &studioApp.mainFrame->output;
	output->clearBuild();
	output->clearDebug();
	output->clearResults();
	output->clearFind();
	kernel.getRepository()->newModel();
	output->updateLogConnection();
}

bool RDOStudioModel::openModel( const std::string& modelName ) const
{
	RDOStudioOutput* output = &studioApp.mainFrame->output;
	output->clearBuild();
	output->clearDebug();
	output->clearResults();
	output->clearFind();
	output->showBuild();
	output->appendStringToBuild( rdo::format( IDS_MODEL_LOADING_BEGIN ) );
	const_cast<rdoEditCtrl::RDOBuildEdit*>(output->getBuild())->UpdateWindow();
	openError   = false;
	modelClosed = false;
	bool flag = kernel.getRepository()->openModel( modelName );
	if ( flag && !openError ) {
		rdo::binarystream stream;
		kernel.getRepository()->load( rdoModelObjects::PMV, stream );
		output->appendStringToResults( stream.str() );
		output->updateLogConnection();
		output->appendStringToBuild( rdo::format( IDS_MODEL_LOADING_OK ) );
		studioApp.setLastProjectName( kernel.getRepository()->getFullName() );
	} else {
		output->updateLogConnection();
		output->appendStringToBuild( rdo::format( IDS_MODEL_LOADING_FAILD ) );
		studioApp.setLastProjectName( kernel.getRepository()->getFullName() );
	}
	return flag;
}

bool RDOStudioModel::saveModel() const
{
	return kernel.getRepository()->saveModel();
}

void RDOStudioModel::saveAsModel() const
{
	saveAsFlag = true;
	kernel.getRepository()->saveAsModel();
	saveAsFlag = false;;
}

void RDOStudioModel::closeModel() const
{
	stopModel();
	RDOStudioOutput* output = &studioApp.mainFrame->output;
	if ( output && output->GetSafeHwnd() ) {
		output->clearBuild();
		output->clearDebug();
		output->clearResults();
		output->clearFind();
	}
	kernel.getRepository()->closeModel();
}

void RDOStudioModel::buildModel() const
{
	if ( hasModel() && !isRunning() && saveModel() ) {
		RDOStudioOutput* output = &studioApp.mainFrame->output;
		output->clearBuild();
		output->clearDebug();
		output->clearResults();
		output->showBuild();
		output->appendStringToBuild( rdo::format( IDS_MODEL_BUILDING_BEGIN ) );
		const_cast<rdoEditCtrl::RDOBuildEdit*>(output->getBuild())->UpdateWindow();
		kernel.getSimulator()->parseModel();
	}
}

void RDOStudioModel::runModel() const
{
	if ( hasModel() && !isRunning() && saveModel() ) {
		RDOStudioOutput* output = &studioApp.mainFrame->output;
		output->clearBuild();
		output->clearDebug();
		output->clearResults();
		output->showBuild();
		output->appendStringToBuild( rdo::format( IDS_MODEL_BUILDING_BEGIN ) );
		const_cast<rdoEditCtrl::RDOBuildEdit*>(output->getBuild())->UpdateWindow();
		kernel.getSimulator()->runModel();
	}
}

void RDOStudioModel::stopModel() const
{
	if ( hasModel() && isRunning() ) {
		kernel.getSimulator()->stopModel();
	}
}

void RDOStudioModel::newModelNotify()
{
	model->newModelFromRepository();
	plugins->pluginProc( rdoPlugin::PM_MODEL_NEW );
}

void RDOStudioModel::openModelNotify()
{
	model->openModelFromRepository();
	plugins->pluginProc( rdoPlugin::PM_MODEL_OPEN );
}

void RDOStudioModel::saveModelNotify()
{
	model->saveModelToRepository();
}

bool RDOStudioModel::canCloseModelNotify()
{
	return model->canCloseModel();
}

void RDOStudioModel::closeModelNotify()
{
	model->closeModelFromRepository();
	if ( plugins ) {
		plugins->pluginProc( rdoPlugin::PM_MODEL_CLOSE );
	}
}

void RDOStudioModel::canNotCloseModelNotify()
{
	model->canNotCloseModelFromRepository();
}

void RDOStudioModel::beforeModelStartNotify()
{
	model->beforeModelStart();
	plugins->pluginProc( rdoPlugin::PM_MODEL_BEFORE_START );
}

void RDOStudioModel::afterModelStartNotify()
{
	RDOStudioModelDoc* doc = model->getModelDoc();
	if ( doc ) {
		doc->running = true;
	}
	RDOStudioOutput* output = &studioApp.mainFrame->output;
	output->showDebug();
	output->appendStringToDebug( rdo::format( IDS_MODEL_STARTED ) );
	const_cast<rdoEditCtrl::RDODebugEdit*>(output->getDebug())->UpdateWindow();
	int index = model->frameManager.getLastShowedFrame();
	if ( index != -1 ) {
		RDOStudioFrameView* view = model->frameManager.getFrameView( index );
		if ( view ) view->SetFocus();
	}
	plugins->pluginProc( rdoPlugin::PM_MODEL_AFTER_START );
}

void RDOStudioModel::endExecuteModelNotify()
{
	model->stopModelFromSimulator();

	RDOStudioOutput* output = &studioApp.mainFrame->output;
	output->appendStringToDebug( rdo::format( IDS_MODEL_FINISHED ) );
	const_cast<rdoEditCtrl::RDODebugEdit*>(output->getDebug())->UpdateWindow();

	std::string str = kernel.getSimulator()->getResults().str();
	if ( str.length() ) {
		rdo::binarystream stream;
		stream.write( str.c_str(), str.length() );
		kernel.getRepository()->save( rdoModelObjects::PMV, stream );
		output->showResults();
		output->appendStringToResults( str );
	}

	plugins->pluginProc( rdoPlugin::PM_MODEL_FINISHED );
}

void RDOStudioModel::stopModelNotify()
{
	model->stopModelFromSimulator();

	RDOStudioOutput* output = &studioApp.mainFrame->output;
	output->appendStringToDebug( rdo::format( IDS_MODEL_STOPED ) );
	const_cast<rdoEditCtrl::RDODebugEdit*>(output->getDebug())->UpdateWindow();

	plugins->pluginProc( rdoPlugin::PM_MODEL_STOP_CANCEL );
}

void RDOStudioModel::stopModelFromSimulator()
{
	RDOStudioModelDoc* doc = getModelDoc();
	if ( doc ) {
		doc->running = false;
	}
	frameManager.clear();
	frameManager.bmp_clear();
}

void RDOStudioModel::parseSuccessNotify()
{
	RDOStudioOutput* output = &studioApp.mainFrame->output;
	std::vector<RDOSyntaxError>* errors = kernel.getSimulator()->getErrors();
	int errors_cnt   = 0;
	int warnings_cnt = 0;
	for ( std::vector<RDOSyntaxError>::iterator it = errors->begin(); it != errors->end(); it++ ) {
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
	plugins->pluginProc( rdoPlugin::PM_MODEL_BUILD_OK );
}

void RDOStudioModel::parseErrorNotify()
{
	RDOStudioOutput* output = &studioApp.mainFrame->output;
	std::vector<RDOSyntaxError>* errors = kernel.getSimulator()->getErrors();
	int errors_cnt   = 0;
	int warnings_cnt = 0;
	for ( std::vector<RDOSyntaxError>::iterator it = errors->begin(); it != errors->end(); it++ ) {
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

	plugins->pluginProc( rdoPlugin::PM_MODEL_BUILD_FAILD );
}

void RDOStudioModel::executeErrorNotify()
{
	RDOStudioOutput* output = &studioApp.mainFrame->output;
	output->clearBuild();
	output->showBuild();
	output->appendStringToBuild( rdo::format( IDS_MODEL_RUNTIMEERROR ) );
	std::vector<RDOSyntaxError>* errors = kernel.getSimulator()->getErrors();
	int errors_cnt   = 0;
	int warnings_cnt = 0;
	for ( std::vector<RDOSyntaxError>::iterator it = errors->begin(); it != errors->end(); it++ ) {
		output->appendStringToBuild( it->error_code, it->message, it->file, it->error_line, it->error_pos, it->warning );
		if ( it->warning ) {
			warnings_cnt++;
		} else {
			errors_cnt++;
		}
	}
	model->stopModelFromSimulator();
	if ( errors_cnt || warnings_cnt ) {
		const_cast<rdoEditCtrl::RDOBuildEdit*>(output->getBuild())->showFirstError();
	}

	plugins->pluginProc( rdoPlugin::PM_MODEL_STOP_RUNTIME_ERROR );
}

void RDOStudioModel::showFrameNotify()
{
	model->showFrame();
}

void RDOStudioModel::buildNotify( const std::string& str )
{
	studioApp.mainFrame->output.appendStringToBuild( str );
}

void RDOStudioModel::debugNotify( const std::string& str )
{
	studioApp.mainFrame->output.appendStringToDebug( str );
}

void RDOStudioModel::modelExitCallback( int exitCode )
{
	kernel.callbackNext( RDOKernel::modelExit, RDOStudioModel::modelExitCallback, exitCode );
	studioApp.autoClose( exitCode );
}

RDOStudioModelDoc* RDOStudioModel::getModelDoc() const
{
	POSITION pos = modelDocTemplate->GetFirstDocPosition();
	if ( pos ) {
		return static_cast<RDOStudioModelDoc*>(modelDocTemplate->GetNextDoc( pos ));
	}
	return NULL;
}

void RDOStudioModel::updateModify() const
{
	RDOStudioModelDoc* doc = getModelDoc();
	if ( doc ) {
		doc->updateModify();
	}
	if ( prevModify != isModify() ) {
		static_cast<bool>(prevModify) = isModify();
		plugins->pluginProc( rdoPlugin::PM_MODEL_MODIFY );
	}
}

RDOEditorTabCtrl* RDOStudioModel::getTab() const
{
	RDOStudioModelDoc* doc = getModelDoc();
	if ( doc ) {
		RDOStudioModelView* view = doc->getView();
		if ( view ) {
			return view->tab;
		}
	}
	return NULL;
}

void RDOStudioModel::newModelFromRepository()
{
	if ( modelDocTemplate ) {

		BOOL maximize = false;
		if ( !studioApp.mainFrame->MDIGetActive( &maximize ) ) {
			maximize = true;
		}

		modelDocTemplate->OpenDocumentFile( NULL );
		setName( kernel.getRepository()->getName() );

		RDOEditorTabCtrl* tab = getTab();
		if ( tab ) {
			for ( int i = 0; i < tab->getItemCount(); i++ ) {
				RDOEditorEdit* edit = tab->getItemEdit( i );
				edit->setReadOnly( false );
				edit->clearAll();
				if ( useTemplate ) {
					int resID = -1;
					switch ( tab->indexToType( i ) ) {
						case rdoModelObjects::PAT: resID = ID_INSERT_PAT_PATOPERATION; break;
						case rdoModelObjects::RTP: resID = ID_INSERT_RTP_RTPPERMANENT; break;
						case rdoModelObjects::RSS: resID = ID_INSERT_RSS_RSS; break;
						case rdoModelObjects::OPR: resID = ID_INSERT_OPR_OPR; break;
						case rdoModelObjects::FRM: resID = ID_INSERT_FRM_FRM; break;
						case rdoModelObjects::FUN: resID = ID_INSERT_FUN_FUN; break;
						case rdoModelObjects::SMR: resID = ID_INSERT_SMR_SMR; break;
					}
					if ( resID != - 1 ) {
						std::string s = rdo::format( resID );
						if ( !s.empty() ) {
							int incPos = -1;
							switch ( resID ) {
								case ID_INSERT_PAT_PATOPERATION : incPos = 9; break;
								case ID_INSERT_RTP_RTPPERMANENT : incPos = 15; break;
								case ID_INSERT_RSS_RSS          : incPos = 13; break;
								case ID_INSERT_OPR_OPR          : incPos = 14; break;
								case ID_INSERT_FRM_FRM          : incPos = 7; break;
								case ID_INSERT_FUN_FUN          : incPos = 10; break;
								case ID_INSERT_SMR_SMR          : incPos = 17; break;
							}
							edit->replaceCurrent( s, incPos );
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
	}
}

void RDOStudioModel::openModelFromRepository()
{
	if ( modelDocTemplate ) {

		BOOL maximize = false;
		if ( !studioApp.mainFrame->MDIGetActive( &maximize ) ) {
			maximize = true;
		}

		modelDocTemplate->OpenDocumentFile( NULL );
		setName( kernel.getRepository()->getName() );

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
					kernel.getRepository()->load( type, stream );
				} else {
					canLoad = false;
				}
				studioApp.mainFrame->stepProgress();
				if ( canLoad ) {
					bool stream_error = stream.rdstate() & std::ios_base::failbit ? true : false;
					if ( !stream_error ) {
						bool readonly = kernel.getRepository()->isReadOnly( type );
						edit->load( stream );
						edit->setReadOnly( readonly );
						if ( readonly ) {
							output->appendStringToBuild( rdo::format( IDS_MODEL_FILE_READONLY, kernel.getRepository()->getFileExtName( type ).c_str() ) );
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
							output->appendStringToBuild( rdo::format( IDS_MODEL_CANNOT_LOAD, rdo::format( obj ).c_str(), kernel.getRepository()->getFullFileName( type ).c_str() ) );
							const_cast<rdoEditCtrl::RDOBuildEdit*>(output->getBuild())->UpdateWindow();
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
	}
}

void RDOStudioModel::saveModelToRepository()
{
	bool smr_modified = false;
	bool wasSaved = false;
	RDOEditorTabCtrl* tab = getTab();
	if ( tab ) {
		RDOEditorEdit* smr_edit = tab->getItemEdit( rdoModelObjects::SMR );
		if ( smr_edit->isModify() || saveAsFlag ) {
			rdo::binarystream stream;
			smr_edit->save( stream );
			kernel.getRepository()->save( rdoModelObjects::SMR, stream );
			smr_modified = true;
		}
		int cnt = tab->getItemCount();
		int progress_cnt = 0;
		for ( int i = 0; i < cnt; i++ ) {
			if ( tab->getItemEdit( i )->isModify() || saveAsFlag ) progress_cnt++;
		}
		if ( progress_cnt ) {
			studioApp.mainFrame->beginProgress( 0, progress_cnt * 2 + 1 );
			studioApp.mainFrame->stepProgress();
			for ( int i = 0; i < cnt; i++ ) {
				RDOEditorEdit* edit = tab->getItemEdit( i );
				if ( edit->isModify() || saveAsFlag ) {
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
						case rdoModelObjects::PMD: kernel.getRepository()->save( type, stream ); break;
						default: break;
					}
					edit->setModifyFalse();
				}
				studioApp.mainFrame->stepProgress();
			}
			studioApp.mainFrame->endProgress();
			wasSaved = true;
		}
	}
	setName( kernel.getRepository()->getName() );
	studioApp.insertReopenItem( kernel.getRepository()->getFullName() );

	if ( smr_modified ) updateFrmDescribed();

	if ( wasSaved ) {
		plugins->pluginProc( rdoPlugin::PM_MODEL_SAVE );
	}
}

void RDOStudioModel::updateFrmDescribed()
{
//	frmDescribed = kernel.getRepository()->isDescribed( rdoModelObjects::FRM );
	frmDescribed = true;
}

bool RDOStudioModel::canCloseModel()
{
	bool flag = true;
	if ( isModify() && closeWithDocDelete ) {
		int res = AfxGetMainWnd()->MessageBox( rdo::format( ID_MSG_MODELSAVE_QUERY ).c_str(), NULL, MB_ICONQUESTION | MB_YESNOCANCEL );
		switch ( res ) {
			case IDYES   : flag = true; break;
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
	if ( closeWithDocDelete ) {
		RDOStudioModelDoc* doc = getModelDoc();
		if ( doc ) {
			doc->OnCloseDocument();
		}
	}
	if ( !showCanNotCloseModelMessage ) {
		showCanNotCloseModelMessage = true;
	}
	modelClosed = true;
}

void RDOStudioModel::canNotCloseModelFromRepository() const
{
	if ( showCanNotCloseModelMessage ) {
		AfxMessageBox( rdo::format( ID_MSG_MODELCLOSE_ERROR ).c_str(), MB_ICONSTOP | MB_OK );
	}
}

std::string RDOStudioModel::getName() const
{
	RDOStudioModelDoc* doc = getModelDoc();
	if ( doc ) {
		return doc->getName();
	}
	return "";
}

void RDOStudioModel::setName( const std::string& str )
{
	RDOStudioModelDoc* doc = getModelDoc();
	bool flag = false;
	if ( doc ) {
		flag = doc->getName() != str;
		doc->setName( str );
	}
	if ( flag ) {
		plugins->pluginProc( rdoPlugin::PM_MODEL_NAME_CHANGED );
	}
}

bool RDOStudioModel::isModify() const
{
	RDOStudioModelDoc* doc = getModelDoc();
	if ( doc ) {
		return doc->isModify();
	}
	return false;
}

bool RDOStudioModel::isRunning() const
{
	RDOStudioModelDoc* doc = getModelDoc();
	if ( doc ) {
		return doc->isRunning();
	}
	return false;
}

void RDOStudioModel::beforeModelStart()
{
	frameManager.bmp_clear();
	frameManager.clear();

	if ( isFrmDescribed() ) {
		RDOStudioOutput* output = &studioApp.mainFrame->output;
		output->showDebug();
		output->appendStringToDebug( rdo::format( IDS_MODEL_RESOURCE_LOADING_BEGIN ) );
		const_cast<rdoEditCtrl::RDODebugEdit*>(output->getDebug())->UpdateWindow();

		std::vector< const std::string* > bitmaps = kernel.getSimulator()->getAllBitmaps();
		std::vector< const std::string* >::iterator bmp = bitmaps.begin();
		while ( bmp != bitmaps.end() ) {
			frameManager.bmp_insert( *(*bmp) );
			bmp++;
		}

		std::vector< const std::string* > frames = kernel.getSimulator()->getAllFrames();
		std::vector< const std::string* >::iterator it = frames.begin();
		while ( it != frames.end() ) {
			frameManager.insertItem( *(*it) );
			it++;
		}
		frameManager.expand();
		int initFrameNumber = kernel.getSimulator()->getInitialFrameNumber() - 1;
		modelTime = 0;
		showMode  = kernel.getSimulator()->getInitialShowMode();
		frameManager.setLastShowedFrame( initFrameNumber );
		if ( showMode == SM_Animation && initFrameNumber >= 0 && initFrameNumber < frameManager.count() ) {
			RDOStudioFrameDoc* doc = frameManager.connectFrameDoc( initFrameNumber );
			if ( doc ) {
				frameManager.getFrameView( initFrameNumber )->SetFocus();
			}
		}
		output->appendStringToDebug( rdo::format( IDS_MODEL_RESOURCE_LOADING_OK ) );
		const_cast<rdoEditCtrl::RDODebugEdit*>(output->getDebug())->UpdateWindow();
	} else {
		modelTime = 0;
		showMode  = rdosim::SM_NoShow;
		frameManager.setLastShowedFrame( -1 );
	}
}

void RDOStudioModel::showFrame()
{
	modelTime = kernel.getSimulator()->getModelTime();
	studioApp.mainFrame->showNewModelTime( modelTime );
//	while ( getShowMode() == SM_Monitor ) {
//		::Sleep( 500 );
//	}
	const std::vector<RDOFrame *>& frames = kernel.getSimulator()->getFrames();
	std::vector<RDOFrame *>::const_iterator it = frames.begin();
	int index = 0;
	while ( it != frames.end() ) {
		if ( *it ) {
			frameManager.showFrame( *it, index );
		}
		it++;
		index++;
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

void RDOStudioModel::setShowMode( const ShowMode value )
{
	if ( isRunning() ) {
		showMode = value;
		if ( showMode == SM_Animation ) {
			RDOStudioFrameDoc* doc = frameManager.getFirstExistDoc();
			if ( !doc ) {
				frameManager.connectFrameDoc( frameManager.getLastShowedFrame() );
			}
		}
		if ( showMode == SM_NoShow ) {
			frameManager.closeAll();
		}
		kernel.getSimulator()->setShowMode( showMode );
		tracer->setShowMode( showMode );
		plugins->pluginProc( rdoPlugin::PM_MODEL_SHOWMODE );
	}
}

double RDOStudioModel::getShowRate() const
{
	return kernel.getSimulator()->getShowRate();
}

void RDOStudioModel::setShowRate( const double value ) const
{
	kernel.getSimulator()->setShowRate( value );
}
