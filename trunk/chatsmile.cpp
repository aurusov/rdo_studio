#include "stdafx.h"
#include "chatsmile.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- CChatSmile
// ----------------------------------------------------------------------------
CChatSmile::CChatSmile( const Type _type, CWnd* parent )
{
	type = _type;
	UINT res;
	switch ( type ) {
		case CChatSmile::smile   : res = IDR_SMILE_SMILE; break;
		case CChatSmile::frown   : res = IDR_SMILE_FROWN; break;
		case CChatSmile::wink    : res = IDR_SMILE_WINK; break;
		case CChatSmile::smirk   : res = IDR_SMILE_SMIRK; break;
		case CChatSmile::tongue  : res = IDR_SMILE_TONGUE; break;
		case CChatSmile::biggrin : res = IDR_SMILE_BIGGRIN; break;
		case CChatSmile::gigi    : res = IDR_SMILE_GIGI; break;
		case CChatSmile::laugh   : res = IDR_SMILE_LAUGH; break;
		case CChatSmile::lol     : res = IDR_SMILE_LOL; break;
		case CChatSmile::cool    : res = IDR_SMILE_COOL; break;
		case CChatSmile::confused: res = IDR_SMILE_CONFUSED; break;
		case CChatSmile::eek     : res = IDR_SMILE_EEK; break;
		case CChatSmile::rolleyes: res = IDR_SMILE_ROLLEYES; break;
		case CChatSmile::insane  : res = IDR_SMILE_INSANE; break;
		case CChatSmile::redface : res = IDR_SMILE_REDFACE; break;
		case CChatSmile::mad     : res = IDR_SMILE_MAD; break;
		case CChatSmile::weep    : res = IDR_SMILE_WEEP; break;
		case CChatSmile::up      : res = IDR_SMILE_UP; break;
		case CChatSmile::down    : res = IDR_SMILE_DOWN; break;
		case CChatSmile::rotate  : res = IDR_SMILE_ROTATE; break;
		case CChatSmile::super   : res = IDR_SMILE_SUPER; break;
		case CChatSmile::upset   : res = IDR_SMILE_UPSET; break;
		case CChatSmile::beer    : res = IDR_SMILE_BEER; break;
		case CChatSmile::amazed  : res = IDR_SMILE_AMAZED; break;
		case CChatSmile::shuffle : res = IDR_SMILE_SHUFFLE; break;
		case CChatSmile::lamer   : res = IDR_SMILE_LAMER; break;
		case CChatSmile::spy     : res = IDR_SMILE_SPY; break;
		case CChatSmile::idea    : res = IDR_SMILE_IDEA; break;
		case CChatSmile::puke    : res = IDR_SMILE_PUKE; break;
		case CChatSmile::wink2   : res = IDR_SMILE_WINK2; break;
		case CChatSmile::smirk2  : res = IDR_SMILE_SMIRK2; break;
		case CChatSmile::think   : res = IDR_SMILE_THINK; break;
		case CChatSmile::fear    : res = IDR_SMILE_FEAR; break;
		case CChatSmile::yes     : res = IDR_SMILE_YES; break;
		case CChatSmile::no      : res = IDR_SMILE_NO; break;
		case CChatSmile::crazy   : res = IDR_SMILE_CRAZY; break;
		case CChatSmile::joke    : res = IDR_SMILE_JOKE; break;
		case CChatSmile::hah     : res = IDR_SMILE_HAH; break;
		case CChatSmile::ballet  : res = IDR_SMILE_BALLET; break;
		case CChatSmile::friday  : res = IDR_SMILE_FRIDAY; break;
		case CChatSmile::drink   : res = IDR_SMILE_DRINK; break;
		case CChatSmile::fart    : res = IDR_SMILE_FART; break;
		case CChatSmile::box     : res = IDR_SMILE_BOX; break;
		case CChatSmile::invalid : res = IDR_SMILE_INVALID; break;
		case CChatSmile::duel    : res = IDR_SMILE_DUEL; break;
		case CChatSmile::kos     : res = IDR_SMILE_KOS; break;
		case CChatSmile::gossip  : res = IDR_SMILE_GOSSIP; break;
		case CChatSmile::bis     : res = IDR_SMILE_BIS; break;
		case CChatSmile::hit     : res = IDR_SMILE_HIT; break;
		case CChatSmile::ivan    : res = IDR_SMILE_IVAN; break;
		case CChatSmile::venera  : res = IDR_SMILE_VENERA; break;
		case CChatSmile::mobile  : res = IDR_SMILE_MOBILE; break;
		case CChatSmile::kult    : res = IDR_SMILE_KULT; break;
		case CChatSmile::figa    : res = IDR_SMILE_FIGA; break;
		case CChatSmile::eyes    : res = IDR_SMILE_EYES; break;
		case CChatSmile::tomato  : res = IDR_SMILE_TOMATO; break;
		default                  : res = -1; break;
	}
	if ( res != -1 ) {
		Create( "", WS_CHILD | WS_VISIBLE, CRect( 0, 0, 1, 1 ), parent );
		if ( Load( MAKEINTRESOURCE( res ), _T("GIF") ) ) {
			Draw();
		}
	}
}

CChatSmile::~CChatSmile()
{
}

void CChatSmile::setBgColor( COLORREF color )
{
	if ( GetBkColor() != color ) {
		bool flag = IsAnimatedGIF() ? true : false;
		if ( !flag ) Stop();
		SetBkColor( color );
		if ( !flag ) Draw();
	}
}

void CChatSmile::showAnimation( const bool value )
{
	ShowWindow( value ? SW_SHOW : SW_HIDE );
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

CChatSmile::Type CChatSmileList::getType( std::string str )
{
	tolower( str );
	if ( str == ":)" || str == ":-)" )        { return CChatSmile::smile; }
	else if ( str == "(:" || str == "(-:" )   { return CChatSmile::smile; }
	else if ( str == ":(" || str == ":-(" )   { return CChatSmile::frown; }
	else if ( str == ";)" || str == ";-)" )   { return CChatSmile::wink; }
	else if ( str == ":-\\" || str == ":\\" ) { return CChatSmile::smirk; }
	else if ( str == ":-/" || str == ":/" )   { return CChatSmile::smirk; }
	else if ( str == ";-\\" || str == ";\\" ) { return CChatSmile::smirk; }
	else if ( str == ";-/" || str == ";/" )   { return CChatSmile::smirk; }
	else if ( str == ":p" || str == ";p" )    { return CChatSmile::tongue; }
	else if ( str == ":d" || str == ";d")     { return CChatSmile::biggrin; }
	else if ( str == ":gigi:" )               { return CChatSmile::gigi; }
	else if ( str == ":laugh:" )              { return CChatSmile::laugh; }
	else if ( str == ":lol:" )                { return CChatSmile::lol; }
	else if ( str == ":cool:" )               { return CChatSmile::cool; }
	else if ( str == ":confused:" )           { return CChatSmile::confused; }
	else if ( str == ":eek:" )                { return CChatSmile::eek; }
	else if ( str == ":rolleyes:" )           { return CChatSmile::rolleyes; }
	else if ( str == ":insane:" )             { return CChatSmile::insane; }
	else if ( str == ":redface:" )            { return CChatSmile::redface; }
	else if ( str == ":mad:" )                { return CChatSmile::mad; }
	else if ( str == ":weep:" )               { return CChatSmile::weep; }
	else if ( str == ":up:" )                 { return CChatSmile::up; }
	else if ( str == ":down:" )               { return CChatSmile::down; }
	else if ( str == ":rotate:" )             { return CChatSmile::rotate; }
	else if ( str == ":super:" )              { return CChatSmile::super; }
	else if ( str == ":upset:" )              { return CChatSmile::upset; }
	else if ( str == ":beer:" )               { return CChatSmile::beer; }
	else if ( str == ":amazed:" )             { return CChatSmile::amazed; }
	else if ( str == ":shuffle:" )            { return CChatSmile::shuffle; }
	else if ( str == ":lamer:" )              { return CChatSmile::lamer; }
	else if ( str == ":spy:" )                { return CChatSmile::spy; }
	else if ( str == ":idea:" )               { return CChatSmile::idea; }
	else if ( str == ":puke:" )               { return CChatSmile::puke; }
	else if ( str == ":wink:" )               { return CChatSmile::wink2; }
	else if ( str == ":smirk:" )              { return CChatSmile::smirk2; }
	else if ( str == ":think:" )              { return CChatSmile::think; }
	else if ( str == ":fear:" )               { return CChatSmile::fear; }
	else if ( str == ":yes:" )                { return CChatSmile::yes; }
	else if ( str == ":no:" )                 { return CChatSmile::no; }
	else if ( str == ":crazy:" )              { return CChatSmile::crazy; }
	else if ( str == ":joke:" )               { return CChatSmile::joke; }
	else if ( str == ":hah:" )                { return CChatSmile::hah; }
	else if ( str == ":ballet:" )             { return CChatSmile::ballet; }
	else if ( str == ":friday:" )             { return CChatSmile::friday; }
	else if ( str == ":drink:" )              { return CChatSmile::drink; }
	else if ( str == ":fart:" )               { return CChatSmile::fart; }
	else if ( str == ":box:" )                { return CChatSmile::box; }
	else if ( str == ":invalid:" )            { return CChatSmile::invalid; }
	else if ( str == ":duel:" )               { return CChatSmile::duel; }
	else if ( str == ":kos:" )                { return CChatSmile::kos; }
	else if ( str == ":gossip:" )             { return CChatSmile::gossip; }
	else if ( str == ":bis:" )                { return CChatSmile::bis; }
	else if ( str == ":hit:" )                { return CChatSmile::hit; }
	else if ( str == ":ivan:" )               { return CChatSmile::ivan; }
	else if ( str == ":venera:" )             { return CChatSmile::venera; }
	else if ( str == ":mobile:" )             { return CChatSmile::mobile; }
	else if ( str == ":kult:" )               { return CChatSmile::kult; }
	else if ( str == ":figa:" )               { return CChatSmile::figa; }
	else if ( str == ":eyes:" )               { return CChatSmile::eyes; }
	else if ( str == ":tomato:" )             { return CChatSmile::tomato; }
	return CChatSmile::none;
}

std::string CChatSmileList::getStr( const CChatSmile::Type type )
{
	switch ( type ) {
		case CChatSmile::smile   : return ":)"; break;
		case CChatSmile::frown   : return ":("; break;
		case CChatSmile::wink    : return ";)"; break;
		case CChatSmile::smirk   : return ":-\\"; break;
		case CChatSmile::tongue  : return ":p"; break;
		case CChatSmile::biggrin : return ":D"; break;
		case CChatSmile::gigi    : return ":gigi:"; break;
		case CChatSmile::laugh   : return ":laugh:"; break;
		case CChatSmile::lol     : return ":lol:"; break;
		case CChatSmile::cool    : return ":cool:"; break;
		case CChatSmile::confused: return ":confused:"; break;
		case CChatSmile::eek     : return ":eek:"; break;
		case CChatSmile::rolleyes: return ":rolleyes:"; break;
		case CChatSmile::insane  : return ":insane:"; break;
		case CChatSmile::redface : return ":redface:"; break;
		case CChatSmile::mad     : return ":mad:"; break;
		case CChatSmile::weep    : return ":weep:"; break;
		case CChatSmile::up      : return ":up:"; break;
		case CChatSmile::down    : return ":down:"; break;
		case CChatSmile::rotate  : return ":rotate:"; break;
		case CChatSmile::super   : return ":super:"; break;
		case CChatSmile::upset   : return ":upset:"; break;
		case CChatSmile::beer    : return ":beer:"; break;
		case CChatSmile::amazed  : return ":amazed:"; break;
		case CChatSmile::shuffle : return ":shuffle:"; break;
		case CChatSmile::lamer   : return ":lamer:"; break;
		case CChatSmile::spy     : return ":spy:"; break;
		case CChatSmile::idea    : return ":idea:"; break;
		case CChatSmile::puke    : return ":puke:"; break;
		case CChatSmile::wink2   : return ":wink:"; break;
		case CChatSmile::smirk2  : return ":smirk:"; break;
		case CChatSmile::think   : return ":think:"; break;
		case CChatSmile::fear    : return ":fear:"; break;
		case CChatSmile::yes     : return ":yes:"; break;
		case CChatSmile::no      : return ":no:"; break;
		case CChatSmile::crazy   : return ":crazy:"; break;
		case CChatSmile::joke    : return ":joke:"; break;
		case CChatSmile::hah     : return ":hah:"; break;
		case CChatSmile::ballet  : return ":ballet:"; break;
		case CChatSmile::friday  : return ":friday:"; break;
		case CChatSmile::drink   : return ":drink:"; break;
		case CChatSmile::fart    : return ":fart:"; break;
		case CChatSmile::box     : return ":box:"; break;
		case CChatSmile::invalid : return ":invalid:"; break;
		case CChatSmile::duel    : return ":duel:"; break;
		case CChatSmile::kos     : return ":kos:"; break;
		case CChatSmile::gossip  : return ":gossip:"; break;
		case CChatSmile::bis     : return ":bis:"; break;
		case CChatSmile::hit     : return ":hit:"; break;
		case CChatSmile::ivan    : return ":ivan:"; break;
		case CChatSmile::venera  : return ":venera:"; break;
		case CChatSmile::mobile  : return ":mobile:"; break;
		case CChatSmile::kult    : return ":kult:"; break;
		case CChatSmile::figa    : return ":figa:"; break;
		case CChatSmile::eyes    : return ":eyes:"; break;
		case CChatSmile::tomato  : return ":tomato:"; break;
		default: return "";
	}
}

std::string CChatSmileList::getInfo( const CChatSmile::Type type )
{
	UINT res;
	switch ( type ) {
		case CChatSmile::smile   : res = IDS_SMILE_SMILE; break;
		case CChatSmile::frown   : res = IDS_SMILE_FROWN; break;
		case CChatSmile::wink    : res = IDS_SMILE_WINK; break;
		case CChatSmile::smirk   : res = IDS_SMILE_SMIRK; break;
		case CChatSmile::tongue  : res = IDS_SMILE_TONGUE; break;
		case CChatSmile::biggrin : res = IDS_SMILE_BIGGRIN; break;
		case CChatSmile::gigi    : res = IDS_SMILE_GIGI; break;
		case CChatSmile::laugh   : res = IDS_SMILE_LAUGH; break;
		case CChatSmile::lol     : res = IDS_SMILE_LOL; break;
		case CChatSmile::cool    : res = IDS_SMILE_COOL; break;
		case CChatSmile::confused: res = IDS_SMILE_CONFUSED; break;
		case CChatSmile::eek     : res = IDS_SMILE_EEK; break;
		case CChatSmile::rolleyes: res = IDS_SMILE_ROLLEYES; break;
		case CChatSmile::insane  : res = IDS_SMILE_INSANE; break;
		case CChatSmile::redface : res = IDS_SMILE_REDFACE; break;
		case CChatSmile::mad     : res = IDS_SMILE_MAD; break;
		case CChatSmile::weep    : res = IDS_SMILE_WEEP; break;
		case CChatSmile::up      : res = IDS_SMILE_UP; break;
		case CChatSmile::down    : res = IDS_SMILE_DOWN; break;
		case CChatSmile::rotate  : res = IDS_SMILE_ROTATE; break;
		case CChatSmile::super   : res = IDS_SMILE_SUPER; break;
		case CChatSmile::upset   : res = IDS_SMILE_UPSET; break;
		case CChatSmile::beer    : res = IDS_SMILE_BEER; break;
		case CChatSmile::amazed  : res = IDS_SMILE_AMAZED; break;
		case CChatSmile::shuffle : res = IDS_SMILE_SHUFFLE; break;
		case CChatSmile::lamer   : res = IDS_SMILE_LAMER; break;
		case CChatSmile::spy     : res = IDS_SMILE_SPY; break;
		case CChatSmile::idea    : res = IDS_SMILE_IDEA; break;
		case CChatSmile::puke    : res = IDS_SMILE_PUKE; break;
		case CChatSmile::wink2   : res = IDS_SMILE_WINK2; break;
		case CChatSmile::smirk2  : res = IDS_SMILE_SMIRK2; break;
		case CChatSmile::think   : res = IDS_SMILE_THINK; break;
		case CChatSmile::fear    : res = IDS_SMILE_FEAR; break;
		case CChatSmile::yes     : res = IDS_SMILE_YES; break;
		case CChatSmile::no      : res = IDS_SMILE_NO; break;
		case CChatSmile::crazy   : res = IDS_SMILE_CRAZY; break;
		case CChatSmile::joke    : res = IDS_SMILE_JOKE; break;
		case CChatSmile::hah     : res = IDS_SMILE_HAH; break;
		case CChatSmile::ballet  : res = IDS_SMILE_BALLET; break;
		case CChatSmile::friday  : res = IDS_SMILE_FRIDAY; break;
		case CChatSmile::drink   : res = IDS_SMILE_DRINK; break;
		case CChatSmile::fart    : res = IDS_SMILE_FART; break;
		case CChatSmile::box     : res = IDS_SMILE_BOX; break;
		case CChatSmile::invalid : res = IDS_SMILE_INVALID; break;
		case CChatSmile::duel    : res = IDS_SMILE_DUEL; break;
		case CChatSmile::kos     : res = IDS_SMILE_KOS; break;
		case CChatSmile::gossip  : res = IDS_SMILE_GOSSIP; break;
		case CChatSmile::bis     : res = IDS_SMILE_BIS; break;
		case CChatSmile::hit     : res = IDS_SMILE_HIT; break;
		case CChatSmile::ivan    : res = IDS_SMILE_IVAN; break;
		case CChatSmile::venera  : res = IDS_SMILE_VENERA; break;
		case CChatSmile::mobile  : res = IDS_SMILE_MOBILE; break;
		case CChatSmile::kult    : res = IDS_SMILE_KULT; break;
		case CChatSmile::figa    : res = IDS_SMILE_FIGA; break;
		case CChatSmile::eyes    : res = IDS_SMILE_EYES; break;
		case CChatSmile::tomato  : res = IDS_SMILE_TOMATO; break;
		default: res = -1;
	}
	if ( res != -1 ) {
		return format( res );
	}
	return "";
}

CChatSmile* CChatSmileList::addSmile( const std::string& str, CWnd* parent )
{
	CChatSmile::Type type = getType( str );
	if ( type != CChatSmile::none ) {
		return addSmile( type, parent );
	}
	return NULL;
}

CChatSmile* CChatSmileList::addSmile( const CChatSmile::Type type, CWnd* parent )
{
	CChatSmile* smile = new CChatSmile( type, parent );
	list.push_back( smile );
	return smile;
}

void CChatSmileList::setBgColor( COLORREF color )
{
	std::vector< CChatSmile* >::iterator it = list.begin();
	while ( it != list.end() ) {
		CChatSmile* smile = *it++;
		smile->setBgColor( color );
//		if ( !smile->IsAnimatedGIF() ) smile->Stop();
//		smile->SetBkColor( color );
//		if ( !smile->IsAnimatedGIF() ) smile->Draw();
	}
}

void CChatSmileList::showAnimation( const bool value )
{
	int nCmdShow = value ? SW_SHOW : SW_HIDE;
	std::vector< CChatSmile* >::iterator it = list.begin();
	while ( it != list.end() ) {
		(*it++)->ShowWindow( nCmdShow );
	}
}
