#include "stdafx.h"
#include "rdostudiomodel.h"
#include "rdostudioapp.h"
#include "rdostudiomainfrm.h"
#include "rdostudiochildfrm.h"
#include "rdostudiomodeldoc.h"
#include "rdostudiomodelview.h"
#include "rdostudioframedoc.h"
#include "rdostudioframeview.h"
#include "resource.h"
#include "rdo_edit/rdoeditortabctrl.h"

#include <rdokernel.h>
#include <rdorepository.h>

using namespace std;
using namespace rdoEditor;
using namespace rdoRepository;
using namespace RDOSimulatorNS;

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
	modelTime( 0 ),
	showMode( SM_NoShow )
{
	model = this;

	modelDocTemplate = new CMultiDocTemplate( IDR_MODELTYPE, RUNTIME_CLASS(RDOStudioModelDoc), RUNTIME_CLASS(RDOStudioChildFrame), RUNTIME_CLASS(RDOStudioModelView) );
	AfxGetApp()->AddDocTemplate( modelDocTemplate );

	kernel.setNotifyReflect( RDOKernel::newModel, newModelNotify );
	kernel.setNotifyReflect( RDOKernel::openModel, openModelNotify );
	kernel.setNotifyReflect( RDOKernel::saveModel, saveModelNotify );
	kernel.setNotifyReflect( RDOKernel::canCloseModel, canCloseModelNotify );
	kernel.setNotifyReflect( RDOKernel::closeModel, closeModelNotify );
	kernel.setNotifyReflect( RDOKernel::canNotCloseModel, canNotCloseModelNotify );

	kernel.setNotifyReflect( RDOKernel::beforeModelStart, beforeModelStartNotify );
	kernel.setNotifyReflect( RDOKernel::afterModelStart, afterModelStartNotify );
	kernel.setNotifyReflect( RDOKernel::endExecuteModel, stopModelNotify );
	kernel.setNotifyReflect( RDOKernel::modelStopped, stopModelNotify );
	kernel.setNotifyReflect( RDOKernel::parseError, parseErrorModelNotify );
	kernel.setNotifyReflect( RDOKernel::showFrame, showFrameNotify );

	kernel.setNotifyReflect( RDOKernel::buildString, buildNotify );
	kernel.setNotifyReflect( RDOKernel::debugString, debugNotify );
}

RDOStudioModel::~RDOStudioModel()
{
	closeModel();
	model = NULL;
}

void RDOStudioModel::newModel( const bool _useTemplate )
{
	useTemplate = _useTemplate;
	studioApp.mainFrame->output.clearBuild();
	studioApp.mainFrame->output.clearDebug();
	studioApp.mainFrame->output.clearResults();
	studioApp.mainFrame->output.clearFind();
	kernel.getRepository()->newModel();
	studioApp.mainFrame->output.updateLogConnection();
}

bool RDOStudioModel::openModel( const string& modelName ) const
{
	studioApp.mainFrame->output.clearBuild();
	studioApp.mainFrame->output.clearDebug();
	studioApp.mainFrame->output.clearResults();
	studioApp.mainFrame->output.clearFind();
	bool flag = kernel.getRepository()->openModel( modelName );
	if ( flag ) {
		studioApp.mainFrame->output.updateLogConnection();
	}
	return flag;
}

bool RDOStudioModel::saveModel() const
{
	return kernel.getRepository()->saveModel();
}

void RDOStudioModel::saveAsModel() const
{
	kernel.getRepository()->saveAsModel();
}

void RDOStudioModel::closeModel() const
{
	stopModel();
	studioApp.mainFrame->output.clearBuild();
	studioApp.mainFrame->output.clearDebug();
	studioApp.mainFrame->output.clearResults();
	studioApp.mainFrame->output.clearFind();
	kernel.getRepository()->closeModel();
}

void RDOStudioModel::buildModel() const
{
	if ( saveModel() ) {
		studioApp.mainFrame->output.clearBuild();
		studioApp.mainFrame->output.clearDebug();
		studioApp.mainFrame->output.clearResults();
		studioApp.mainFrame->output.showBuild();
		kernel.getSimulator()->parseModel();
	}
}

void RDOStudioModel::runModel() const
{
	if ( saveModel() ) {
		studioApp.mainFrame->output.clearBuild();
		studioApp.mainFrame->output.clearDebug();
		studioApp.mainFrame->output.clearResults();
		studioApp.mainFrame->output.showBuild();
		kernel.getSimulator()->runModel();
	}
}

void RDOStudioModel::stopModel() const
{
	kernel.getSimulator()->stopModel();
}

void RDOStudioModel::newModelNotify()
{
	model->newModelFromRepository();
}

void RDOStudioModel::openModelNotify()
{
	model->openModelFromRepository();
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
}

void RDOStudioModel::canNotCloseModelNotify()
{
	model->canNotCloseModelFromRepository();
}

void RDOStudioModel::beforeModelStartNotify()
{
	model->beforeModelStart();
}

void RDOStudioModel::afterModelStartNotify()
{
	studioApp.mainFrame->output.clearDebug();
	studioApp.mainFrame->output.showDebug();
	RDOStudioModelDoc* doc = model->getModelDoc();
	if ( doc ) {
		doc->running = true;
	}
}

void RDOStudioModel::stopModelNotify()
{
	RDOStudioModelDoc* doc = model->getModelDoc();
	if ( doc ) {
		doc->running = false;
	}
	model->frameManager.clear();
}

void RDOStudioModel::parseErrorModelNotify()
{
	vector<RDOSyntaxError>* errors = kernel.getSimulator()->getErrors();
	int i = 0;
	for ( vector<RDOSyntaxError>::iterator it = errors->begin(); it != errors->end(); it++ ) {
		studioApp.mainFrame->output.appendStringToBuild( it->message, it->file, it->lineNo - 1 );
		i++;
	}
	if ( i ) {
		studioApp.mainFrame->output.appendStringToBuild( format( "%d error(s) found.", i ) );
	}
}

void RDOStudioModel::showFrameNotify()
{
	model->showFrame();
}

void RDOStudioModel::buildNotify( string str )
{
	studioApp.mainFrame->output.appendStringToBuild( str );
}

void RDOStudioModel::debugNotify( string str )
{
	studioApp.mainFrame->output.appendStringToDebug( str );
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
					switch ( i ) {
						case RDOEDIT_PAT: resID = ID_INSERT_PAT_PATOPERATION; break;
						case RDOEDIT_RTP: resID = ID_INSERT_RTP_RTPPERMANENT; break;
						case RDOEDIT_RSS: resID = ID_INSERT_RSS_RSS; break;
						case RDOEDIT_OPR: resID = ID_INSERT_OPR_OPR; break;
						case RDOEDIT_FRM: resID = ID_INSERT_FRM_FRM; break;
						case RDOEDIT_FUN: resID = ID_INSERT_FUN_FUN; break;
						case RDOEDIT_SMR: resID = ID_INSERT_SMR_SMR; break;
					}
					if ( resID != - 1 ) {
						string s = format( resID );
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
				edit->setModifyFalse();
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

		RDOEditorTabCtrl* tab = getTab();
		if ( tab ) {
			for ( int i = 0; i < tab->getItemCount(); i++ ) {
				RDOEditorEdit* edit = tab->getItemEdit( i );
				edit->setReadOnly( false );
				edit->clearAll();
				stringstream stream;
				bool canLoad = true;
				switch ( i ) {
					case RDOEDIT_PAT: kernel.getRepository()->loadPAT( stream ); break;
					case RDOEDIT_RTP: kernel.getRepository()->loadRTP( stream ); break;
					case RDOEDIT_RSS: kernel.getRepository()->loadRSS( stream ); break;
					case RDOEDIT_OPR: kernel.getRepository()->loadOPR( stream ); break;
					case RDOEDIT_FRM: kernel.getRepository()->loadFRM( stream ); break;
					case RDOEDIT_FUN: kernel.getRepository()->loadFUN( stream ); break;
					case RDOEDIT_DPT: kernel.getRepository()->loadDPT( stream ); break;
					case RDOEDIT_SMR: kernel.getRepository()->loadSMR( stream ); break;
					case RDOEDIT_PMD: kernel.getRepository()->loadPMD( stream ); break;
					default: canLoad = false; break;
				}
				if ( canLoad ) {
					edit->load( stream );
					edit->setReadOnly( kernel.getRepository()->isReadOnly() );
				}
				edit->setCurrentPos( 0 );
				edit->setModifyFalse();
			}
		}

		CWnd* wnd = studioApp.mainFrame->GetActiveFrame();
		if ( maximize && wnd && wnd != studioApp.mainFrame ) {
			studioApp.mainFrame->MDIMaximize( wnd );
		}
	}
}

void RDOStudioModel::saveModelToRepository()
{
	RDOEditorTabCtrl* tab = getTab();
	if ( tab ) {
		for ( int i = 0; i < tab->getItemCount(); i++ ) {
			RDOEditorEdit* edit = tab->getItemEdit( i );
			stringstream stream;
			edit->save( stream );
			switch ( i ) {
				case RDOEDIT_PAT: kernel.getRepository()->savePAT( stream ); break;
				case RDOEDIT_RTP: kernel.getRepository()->saveRTP( stream ); break;
				case RDOEDIT_RSS: kernel.getRepository()->saveRSS( stream ); break;
				case RDOEDIT_OPR: kernel.getRepository()->saveOPR( stream ); break;
				case RDOEDIT_FRM: kernel.getRepository()->saveFRM( stream ); break;
				case RDOEDIT_FUN: kernel.getRepository()->saveFUN( stream ); break;
				case RDOEDIT_DPT: kernel.getRepository()->saveDPT( stream ); break;
				case RDOEDIT_SMR: kernel.getRepository()->saveSMR( stream ); break;
				case RDOEDIT_PMD: kernel.getRepository()->savePMD( stream ); break;
			}
			edit->setModifyFalse();
		}
	}
	setName( kernel.getRepository()->getName() );
	studioApp.insertReopenItem( kernel.getRepository()->getFullName() );
}

bool RDOStudioModel::canCloseModel()
{
	bool flag = true;
	if ( isModify() && closeWithDocDelete ) {
		int res = AfxGetMainWnd()->MessageBox( format( ID_MSG_MODELSAVE_QUERY ).c_str(), NULL, MB_ICONQUESTION | MB_YESNOCANCEL );
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
}

void RDOStudioModel::canNotCloseModelFromRepository() const
{
	if ( showCanNotCloseModelMessage ) {
		AfxMessageBox( format( ID_MSG_MODELCLOSE_ERROR ).c_str(), MB_ICONSTOP | MB_OK );
	}
}

string RDOStudioModel::getName() const
{
	RDOStudioModelDoc* doc = getModelDoc();
	if ( doc ) {
		return doc->getName();
	}
	return "";
}

void RDOStudioModel::setName( const string& str )
{
	RDOStudioModelDoc* doc = getModelDoc();
	if ( doc ) {
		doc->setName( str );
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
	frameManager.clear();
	vector< const string* > frames = kernel.getSimulator()->getAllFrames();
	vector< const string* >::iterator it = frames.begin();
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
		RDOStudioFrameDoc* doc = model->frameManager.connectFrameDoc( initFrameNumber );
		if ( doc ) {
			doc->SetTitle( format( IDS_FRAMENAME, frameManager.getFrameName( initFrameNumber ).c_str() ).c_str() );
		}
	}
}

void RDOStudioModel::showFrame()
{
	modelTime = kernel.getSimulator()->getModelTime();
	studioApp.mainFrame->showNewModelTime( modelTime );
	const vector<RDOFrame *>& frames = kernel.getSimulator()->getFrames();
	vector<RDOFrame *>::const_iterator it = frames.begin();
	int index = 0;
	while ( it != frames.end() ) {
		if ( *it && index < frameManager.count() ) {

			CSingleLock lock_used( frameManager.getFrameUsed( index ) );
			lock_used.Lock();

			RDOStudioFrameDoc* doc = frameManager.getFrameDoc( index );
			if ( doc ) {

				SYSTEMTIME time1;
				::GetSystemTime( &time1 );

				CSingleLock lock_draw( frameManager.getFrameDraw( index ) );
				lock_draw.Lock();

				RDOStudioFrameView* view = frameManager.getFrameView( index );
				if ( view->mustBeInit ) {
					view->frameBmpRect.right  = (*it)->width;
					view->frameBmpRect.bottom = (*it)->height;
					view->frameBmp.CreateCompatibleBitmap( view->GetDC(), view->frameBmpRect.Width(), view->frameBmpRect.Height() );
					view->mustBeInit = false;
				}

				CDC dc;
				dc.CreateCompatibleDC( view->GetDC() );
				CBitmap* pOldBitmap = dc.SelectObject( &view->frameBmp );

				if( !(*it)->hasBackPicture ) {
					CBrush brush( RGB( (*it)->r, (*it)->g, (*it)->b ) );
					CBrush* pOldBrush = dc.SelectObject( &brush );
					CPen pen( PS_SOLID, 0, RGB( 0x00, 0x00, 0x00 ) );
					CPen* pOldPen = dc.SelectObject( &pen );
					CRect rect( 0, 0, (*it)->width, (*it)->height );
					dc.Rectangle( rect );
					dc.SelectObject( pOldBrush );
					dc.SelectObject( pOldPen );
				}

				int size = (*it)->elements.size();
				for(int i = 0; i < size; i++)
				{
					RDOFrameElement *currElement = (*it)->elements.at(i);
					switch(currElement->type)
					{
					case RDOFrameElement::text_type:
						{				
							int oldBkMode;
							COLORREF oldBkColor;
							COLORREF oldTextColor;
							bool restoreBkColor   = false;
							bool restoreTextColor = false;
							RDOTextElement *textEl = (RDOTextElement *)currElement;
							if( !textEl->background.isTransparent ) {
								oldBkMode  = dc.SetBkMode(OPAQUE);
								oldBkColor = dc.SetBkColor(RGB(textEl->background.r, textEl->background.g, textEl->background.b));
								restoreBkColor = true;
							} else {
								oldBkMode = dc.SetBkMode(TRANSPARENT);
							}

							if( !textEl->foreground.isTransparent ) {
								oldTextColor = dc.SetTextColor(RGB(textEl->foreground.r, textEl->foreground.g, textEl->foreground.b));
								restoreTextColor = true;
							}

							UINT nFormat = DT_SINGLELINE;
							switch(textEl->align)
							{
							case RDOTextElement::left:
								nFormat |= DT_LEFT; break;
							case RDOTextElement::right:
								nFormat |= DT_RIGHT; break;
							case RDOTextElement::center:
								nFormat |= DT_CENTER; break;
							}

							dc.DrawText(textEl->strText.c_str(), textEl->strText.length(),
								CRect(textEl->x, textEl->y, textEl->x + textEl->w, textEl->y + textEl->h),
								nFormat);

							dc.SetBkMode( oldBkMode );
							if ( restoreBkColor ) dc.SetBkColor( oldBkColor );
							if ( restoreTextColor ) dc.SetTextColor( oldTextColor );
						}
						break;

					case RDOFrameElement::rect_type:
						{																		
							RDORectElement *rectEl = (RDORectElement *)currElement;
							CBrush brush( RGB(rectEl->background.r, rectEl->background.g, rectEl->background.b) );
							CBrush* pOldBrush;
							if( !rectEl->background.isTransparent ) {
								pOldBrush = dc.SelectObject( &brush );
							} else {
								pOldBrush = static_cast<CBrush*>(dc.SelectStockObject( NULL_BRUSH ));
							}

							CPen pen( PS_SOLID, 0, RGB(rectEl->foreground.r, rectEl->foreground.g, rectEl->foreground.b) );
							CPen* pOldPen;
							bool restorePen = false;
							if( !rectEl->foreground.isTransparent ) {
								pOldPen = dc.SelectObject( &pen );
								restorePen = true;
							}

							dc.Rectangle(rectEl->x, rectEl->y, rectEl->x + rectEl->w, rectEl->y + rectEl->h);

							dc.SelectObject( pOldBrush );
							if ( restorePen ) dc.SelectObject( pOldPen );
						}
						break;

					case RDOFrameElement::line_type:
						{																		
							RDOLineElement *lineEl = (RDOLineElement *)currElement;
							CPen pen( PS_SOLID, 0, RGB(lineEl->foreground.r, lineEl->foreground.g, lineEl->foreground.b) );
							CPen* pOldPen;
							bool restorePen = false;
							if( !lineEl->foreground.isTransparent ) {
								pOldPen = dc.SelectObject( &pen );
								restorePen = true;
							}

							dc.MoveTo(lineEl->x, lineEl->y);
							dc.LineTo(lineEl->w, lineEl->h);

							if ( restorePen ) dc.SelectObject( pOldPen );
						}
						break;

					case RDOFrameElement::s_bmp_type:
						{																		
							RDOSBmpElement *sbmpEl = (RDOSBmpElement *)currElement;
							CBrush brush(RGB(196, 196, 0));
							CBrush* pOldBrush = dc.SelectObject( &brush );
							CPen pen(PS_SOLID, 3, RGB(0, 196, 196));
							CPen* pOldPen = dc.SelectObject( &pen );
							dc.Rectangle(sbmpEl->x, sbmpEl->y, sbmpEl->x + sbmpEl->w, sbmpEl->y + sbmpEl->h);
							dc.MoveTo(sbmpEl->x, sbmpEl->y);
							dc.LineTo(sbmpEl->x + sbmpEl->w, sbmpEl->y + sbmpEl->h);
							dc.MoveTo(sbmpEl->x + sbmpEl->w, sbmpEl->y);
							dc.LineTo(sbmpEl->x, sbmpEl->y + sbmpEl->h);
							dc.SelectObject( pOldBrush );
							dc.SelectObject( pOldPen );
						}
						break;

					case RDOFrameElement::active_type:
						{																		
							RDOActiveElement *activeEl = (RDOActiveElement *)currElement;
							CBrush brush( RGB(196, 0, 196) );
							CBrush* pOldBrush = dc.SelectObject( &brush );
							CPen pen( PS_SOLID, 2, RGB(196, 196, 0) );
							CPen* pOldPen = dc.SelectObject( &pen );
							dc.Rectangle(activeEl->x, activeEl->y, activeEl->x + activeEl->w, activeEl->y + activeEl->h);
							dc.MoveTo(activeEl->x, activeEl->y);
							dc.LineTo(activeEl->x + activeEl->w, activeEl->y + activeEl->h);
							dc.MoveTo(activeEl->x + activeEl->w, activeEl->y);
							dc.LineTo(activeEl->x, activeEl->y + activeEl->h);
							dc.SelectObject( pOldBrush );
							dc.SelectObject( pOldPen );
						}
						break;
					}
				}
				dc.SelectObject( pOldBitmap );

				lock_draw.Unlock();

				frameManager.getFrameTimer( index )->ResetEvent();

//				CRect rect;
//				view->GetClientRect( rect );
				view->InvalidateRect( NULL );
				view->SendNotifyMessage( WM_PAINT, 0, 0 );

				CONST HANDLE events[2] = { frameManager.getFrameTimer( index )->m_hObject, frameManager.getFrameClose( index )->m_hObject };
				DWORD res = ::WaitForMultipleObjects( 2, events, FALSE, INFINITE );
				if ( res == WAIT_OBJECT_0 ) {
					SYSTEMTIME time2;
					::GetSystemTime( &time2 );
					int msec = ( time2.wSecond - time1.wSecond ) * 1000 + ( time2.wMilliseconds - time1.wMilliseconds );
					TRACE( "time = %d\r\n", msec );
				} else if ( res == WAIT_OBJECT_0 + 1 ) {
				} else {
				}
			}
			lock_used.Unlock();
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
}

void RDOStudioModel::setShowMode( const ShowMode value )
{
	if ( showMode == SM_NoShow && value == SM_Animation ) {
		RDOStudioFrameDoc* doc = frameManager.getFirstExistDoc();
		if ( doc ) {
		} else {
			doc = frameManager.connectFrameDoc( frameManager.getLastShowedFrame() );
			if ( doc ) {
				doc->SetTitle( format( IDS_FRAMENAME, frameManager.getFrameName( frameManager.getLastShowedFrame() ).c_str() ).c_str()  );
			}
		}
	}
	showMode = value;
	if ( showMode == SM_NoShow ) {
		frameManager.closeAll();
	}
	kernel.getSimulator()->setShowMode( showMode );
}
