#include "stdafx.h"
#include "bkemulvideo.h"
#include "bkemulapp.h"
#include "bkemulmainfrm.h"
#include "bkemulchildview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace bkemul;

// --------------------------------------------------------------
// ---------- BKEmulVideo
// --------------------------------------------------------------
BKEmulVideo::BKEmulVideo(): colorMonitor( true )
{
}

BKEmulVideo::~BKEmulVideo()
{
}

void BKEmulVideo::updateMonitor() const
{
	enulApp.mainFrame->childView.updateMonitor();
}

void BKEmulVideo::updateScrolling( BYTE delta ) const
{
	enulApp.mainFrame->childView.updateScrolling( delta );
}

void BKEmulVideo::updateVideoMemoryByte( WORD address ) const
{
	enulApp.mainFrame->childView.updateVideoMemoryByte( address );
}

void BKEmulVideo::updateVideoMemoryWord( WORD address ) const
{
	enulApp.mainFrame->childView.updateVideoMemoryWord( address );
}
