#include "stdafx.h"
#include "rdostudioframemanager.h"
#include "rdostudiomodel.h"
#include "rdostudioframedoc.h"
#include "rdostudioframeview.h"
#include "rdostudioapp.h"
#include "rdostudiomainfrm.h"
#include "rdostudiochildfrm.h"
#include "rdostudioworkspace.h"
#include "rdostudioframetreectrl.h"
#include "resource.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RDOStudioFrameManager
// ----------------------------------------------------------------------------
vector< RDOStudioFrameManager::Frame* > RDOStudioFrameManager::frames;

RDOStudioFrameManager::RDOStudioFrameManager():
	frameDocTemplate( NULL )
{
	frameDocTemplate = new CMultiDocTemplate( IDR_FRAMETYPE, RUNTIME_CLASS(RDOStudioFrameDoc), RUNTIME_CLASS(RDOStudioChildFrame), RUNTIME_CLASS(RDOStudioFrameView) );
	AfxGetApp()->AddDocTemplate( frameDocTemplate );
}

RDOStudioFrameManager::~RDOStudioFrameManager()
{
	vector< Frame* >::iterator it = frames.begin();
	while ( it != frames.end() ) {
		delete *it++;
	};
}

void RDOStudioFrameManager::insertItem( const string& name )
{
	Frame* item = new Frame;
	item->hitem = studioApp.mainFrame->workspace.frames->InsertItem( name.c_str(), 1, 1, studioApp.mainFrame->workspace.frames->GetRootItem() );
	item->name  = name;
	item->doc   = NULL;
	item->view  = NULL;
	frames.push_back( item );
}

int RDOStudioFrameManager::findFrameIndex( const HTREEITEM hitem ) const
{
	vector< Frame* >::iterator it = frames.begin();
	int index = 0;
	while ( it != frames.end() ) {
		if ( (*it)->hitem == hitem ) {
			return index;
		}
		it++;
		index++;
	};
	return -1;
}

int RDOStudioFrameManager::findFrameIndex( const RDOStudioFrameDoc* doc ) const
{
	vector< Frame* >::iterator it = frames.begin();
	int index = 0;
	while ( it != frames.end() ) {
		if ( (*it)->doc == doc ) {
			return index;
		}
		it++;
		index++;
	};
	return -1;
}

int RDOStudioFrameManager::findFrameIndex( const RDOStudioFrameView* view ) const
{
	vector< Frame* >::iterator it = frames.begin();
	int index = 0;
	while ( it != frames.end() ) {
		if ( (*it)->view == view ) {
			return index;
		}
		it++;
		index++;
	};
	return -1;
}

RDOStudioFrameDoc* RDOStudioFrameManager::connectFrameDoc( const HTREEITEM hitem ) const
{
	return connectFrameDoc( findFrameIndex( hitem ) );
}

RDOStudioFrameDoc* RDOStudioFrameManager::connectFrameDoc( const int index ) const
{
	RDOStudioFrameDoc* doc = NULL;
	if ( index != -1 ) {
		CSingleLock lock( getFrameUsed( index ) );
		lock.Lock();

		doc = static_cast<RDOStudioFrameDoc*>(frameDocTemplate->OpenDocumentFile( NULL ));
		frames[index]->doc  = doc;
		frames[index]->view = doc->getView();

		lock.Unlock();
	}
	return doc;
}

void RDOStudioFrameManager::disconnectFrameDoc( const RDOStudioFrameDoc* doc ) const
{
	int index = findFrameIndex( doc );
	if ( index != -1 ) {

		CSingleLock lock( getFrameUsed( index ) );
		lock.Lock();

		frames[index]->doc  = NULL;
		frames[index]->view = NULL;

		lock.Unlock();
	}
}

void RDOStudioFrameManager::closeAll() const
{
	vector< Frame* >::iterator it = frames.begin();
	while ( it != frames.end() ) {
		if ( isValidFrameDoc( (*it)->doc ) ) {
			(*it)->doc->OnCloseDocument();
		}
		it++;
	};
}

void RDOStudioFrameManager::clear() const
{
	studioApp.mainFrame->workspace.frames->deleteChildren( studioApp.mainFrame->workspace.frames->GetRootItem() );
	vector< Frame* >::iterator it = frames.begin();
	while ( it != frames.end() ) {
		if ( isValidFrameDoc( (*it)->doc ) ) {
			(*it)->doc->OnCloseDocument();
		}
		delete *it++;
	};
	frames.clear();
}

void RDOStudioFrameManager::expand() const
{
	studioApp.mainFrame->workspace.frames->expand();
}

bool RDOStudioFrameManager::isValidFrameDoc( const RDOStudioFrameDoc* const frame ) const
{
	POSITION pos = frameDocTemplate->GetFirstDocPosition();
	while ( pos ) {
		RDOStudioFrameDoc* doc = static_cast<RDOStudioFrameDoc*>(frameDocTemplate->GetNextDoc( pos ));
		if ( frame == doc ) {
			return true;
		}
	}

	return false;
}
