#include "stdafx.h"
#include "chatsmile.h"
#include "chatapp.h"
#include "chatmainfrm.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- CChatSmile
// ----------------------------------------------------------------------------
CChatSmile::CChatSmile()
{
}

CChatSmile::~CChatSmile()
{
}

// ----------------------------------------------------------------------------
// ---------- CChatSmileList
// ----------------------------------------------------------------------------
CChatSmileList::CChatSmileList()
{
}

CChatSmileList::~CChatSmileList()
{
	std::vector< CChatSmile* >::iterator it = list.begin();
	while ( it != list.end() ) {
		delete *it++;
	}
	list.clear();
}

CChatSmile* CChatSmileList::addSmile( const CChatSmile::Type type )
{
	CChatSmile* smile = new CChatSmile;
	smile->type = type;
	list.push_back( smile );
	UINT res;
	switch ( type ) {
		case CChatSmile::smile  : res = IDR_SMILE_SMILE; break;
		case CChatSmile::frown  : res = IDR_SMILE_FROWN; break;
		case CChatSmile::wink   : res = IDR_SMILE_WINK; break;
		case CChatSmile::smirk  : res = IDR_SMILE_SMIRK; break;
		case CChatSmile::tongue : res = IDR_SMILE_TONGUE; break;
		case CChatSmile::biggrin: res = IDR_SMILE_BIGGRIN; break;
		case CChatSmile::gigi   : res = IDR_SMILE_GIGI; break;
		case CChatSmile::eyes   : res = IDR_SMILE_EYES; break;
		case CChatSmile::beer1  : res = IDR_SMILE_BEER_1; break;
		case CChatSmile::beer2  : res = IDR_SMILE_BEER_2; break;
		case CChatSmile::lamer  : res = IDR_SMILE_LAMER; break;
		case CChatSmile::tomato : res = IDR_SMILE_TOMATO; break;
		default                 : res = -1; break;
	}
	if ( res != -1 ) {
		smile->Create( "", WS_CHILD | WS_VISIBLE, CRect( 0, 0, 1, 1 ), &chatApp.mainFrame->childView.viewer );
		if ( smile->Load( MAKEINTRESOURCE( res ), _T("GIF") ) ) {
			smile->Draw();
		}
	}
	return smile;
}

void CChatSmileList::setBgColor( COLORREF color )
{
	std::vector< CChatSmile* >::iterator it = list.begin();
	while ( it != list.end() ) {
		CChatSmile* smile = *it++;
		if ( !smile->IsAnimatedGIF() ) smile->Stop();
		smile->SetBkColor( color );
		if ( !smile->IsAnimatedGIF() ) smile->Draw();
	}
}
