#include "stdafx.h"
#include "chatsound.h"
#include "resource.h"

#include <mmsystem.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- CChatSound
// ----------------------------------------------------------------------------
CChatSound::CChatSound()
{
}

CChatSound::CChatSound( const CChatSound& snd )
{
	*this = snd;
}

CChatSound::~CChatSound()
{
}

CChatSound& CChatSound::operator= ( const CChatSound& snd )
{
	type       = snd.type;
	useSound   = snd.useSound;
	useDefault = snd.useDefault;
	res        = snd.res;
	file       = snd.file;
	return *this;
}

bool CChatSound::operator== ( const CChatSound& snd )
{
	return type       == snd.type &&
	       useSound   == snd.useSound &&
	       useDefault == snd.useDefault &&
	       res        == snd.res &&
	       file       == snd.file;
}

bool CChatSound::operator!= ( const CChatSound& snd )
{
	return !( *this == snd );
}

// ----------------------------------------------------------------------------
// ---------- CChatSoundList
// ----------------------------------------------------------------------------
CChatSoundList::CChatSoundList()
{
}

CChatSoundList::~CChatSoundList()
{
	clear();
}

void CChatSoundList::clear()
{
	std::vector< CChatSound* >::iterator it = list.begin();
	while ( it != list.end() ) {
		delete *it++;
	}
	list.clear();
}

CChatSoundList& CChatSoundList::operator=( const CChatSoundList& soundList )
{
	clear();
	std::vector< CChatSound* >::const_iterator it = soundList.list.begin();
	while ( it != soundList.list.end() ) {
		CChatSound* snd = *it++;
		list.push_back( new CChatSound( *snd ) );
	}
	return *this;
}

bool CChatSoundList::operator== ( const CChatSoundList& soundList )
{
	std::vector< CChatSound* >::const_iterator it1 = list.begin();
	std::vector< CChatSound* >::const_iterator it2 = soundList.list.begin();
	while ( it1 != list.end() && it2 != soundList.list.end() ) {
		CChatSound* snd1 = *it1++;
		CChatSound* snd2 = *it2++;
		if ( *snd1 != *snd2 ) {
			return false;
		};
	}
	return true;
}

bool CChatSoundList::operator!= ( const CChatSoundList& soundList )
{
	return !( *this == soundList );
}

void CChatSoundList::init()
{
	useSound = AfxGetApp()->GetProfileInt( "Sound", "useSound", true ) ? true : false;

	CWinApp* app = AfxGetApp();
	CChatSound* snd;
	snd = new CChatSound();
	snd->type       = CST_Connect;
	snd->useSound   = app->GetProfileInt( "Sound\\Connect", "useSound", true ) ? true : false;
	snd->useDefault = app->GetProfileInt( "Sound\\Connect", "useDefault", true ) ? true : false;
	snd->res        = IDR_CONNECT_WAVE;
	snd->file       = app->GetProfileString( "Sound\\Connect", "file", "" );
	list.push_back( snd );
	snd = new CChatSound();
	snd->type       = CST_Disconnect;
	snd->useSound   = app->GetProfileInt( "Sound\\Disconnect", "useSound", true ) ? true : false;
	snd->useDefault = app->GetProfileInt( "Sound\\Disconnect", "useDefault", true ) ? true : false;
	snd->res        = IDR_DISCONNECT_WAVE;
	snd->file       = app->GetProfileString( "Sound\\Disconnect", "file", "" );
	list.push_back( snd );
	snd = new CChatSound();
	snd->type       = CST_ChangeStatusMode;
	snd->useSound   = app->GetProfileInt( "Sound\\ChangeStatusMode", "useSound", true ) ? true : false;
	snd->useDefault = app->GetProfileInt( "Sound\\ChangeStatusMode", "useDefault", true ) ? true : false;
	snd->res        = IDR_CHANGESTATUSMODE_WAVE;
	snd->file       = app->GetProfileString( "Sound\\ChangeStatusMode", "file", "" );
	list.push_back( snd );
	snd = new CChatSound();
	snd->type       = CST_IncomingMessage;
	snd->useSound   = app->GetProfileInt( "Sound\\IncomingMessage", "useSound", true ) ? true : false;
	snd->useDefault = app->GetProfileInt( "Sound\\IncomingMessage", "useDefault", true ) ? true : false;
	snd->res        = IDR_INCOMINGMESSAGE_WAVE;
	snd->file       = app->GetProfileString( "Sound\\IncomingMessage", "file", "" );
	list.push_back( snd );
	snd = new CChatSound();
	snd->type       = CST_ChatType;
	snd->useSound   = app->GetProfileInt( "Sound\\ChatType", "useSound", true ) ? true : false;
	snd->useDefault = app->GetProfileInt( "Sound\\ChatType", "useDefault", true ) ? true : false;
	snd->res        = IDR_CHATTYPE_WAVE;
	snd->file       = app->GetProfileString( "Sound\\ChatType", "file", "" );
	list.push_back( snd );
	snd = new CChatSound();
	snd->type       = CST_ChatBack;
	snd->useSound   = app->GetProfileInt( "Sound\\ChatBack", "useSound", true ) ? true : false;
	snd->useDefault = app->GetProfileInt( "Sound\\ChatBack", "useDefault", true ) ? true : false;
	snd->res        = IDR_CHATBACK_WAVE;
	snd->file       = app->GetProfileString( "Sound\\ChatBack", "file", "" );
	list.push_back( snd );
	snd = new CChatSound();
	snd->type       = CST_ChatRet;
	snd->useSound   = app->GetProfileInt( "Sound\\ChatRet", "useSound", true ) ? true : false;
	snd->useDefault = app->GetProfileInt( "Sound\\ChatRet", "useDefault", true ) ? true : false;
	snd->res        = IDR_CHATRET_WAVE;
	snd->file       = app->GetProfileString( "Sound\\ChatRet", "file", "" );
	list.push_back( snd );
	snd = new CChatSound();
	snd->type       = CST_PrivateMsg;
	snd->useSound   = app->GetProfileInt( "Sound\\PrivateMessage", "useSound", true ) ? true : false;
	snd->useDefault = app->GetProfileInt( "Sound\\PrivateMessage", "useDefault", true ) ? true : false;
	snd->res        = IDR_PRIVATEMSG_WAVE;
	snd->file       = app->GetProfileString( "Sound\\PrivateMessage", "file", "" );
	list.push_back( snd );
}

void CChatSoundList::saveSetting() const
{
	CWinApp* app = AfxGetApp();
	CChatSound* snd;
	snd = getSound( CST_Connect );
	app->WriteProfileInt( "Sound\\Connect", "useSound", snd->useSound );
	app->WriteProfileInt( "Sound\\Connect", "useDefault", snd->useDefault );
	app->WriteProfileString( "Sound\\Connect", "file", snd->file.c_str() );
	snd = getSound( CST_Disconnect );
	app->WriteProfileInt( "Sound\\Disconnect", "useSound", snd->useSound );
	app->WriteProfileInt( "Sound\\Disconnect", "useDefault", snd->useDefault );
	app->WriteProfileString( "Sound\\Disconnect", "file", snd->file.c_str() );
	snd = getSound( CST_ChangeStatusMode );
	app->WriteProfileInt( "Sound\\ChangeStatusMode", "useSound", snd->useSound );
	app->WriteProfileInt( "Sound\\ChangeStatusMode", "useDefault", snd->useDefault );
	app->WriteProfileString( "Sound\\ChangeStatusMode", "file", snd->file.c_str() );
	snd = getSound( CST_IncomingMessage );
	app->WriteProfileInt( "Sound\\IncomingMessage", "useSound", snd->useSound );
	app->WriteProfileInt( "Sound\\IncomingMessage", "useDefault", snd->useDefault );
	app->WriteProfileString( "Sound\\IncomingMessage", "file", snd->file.c_str() );
	snd = getSound( CST_ChatType );
	app->WriteProfileInt( "Sound\\ChatType", "useSound", snd->useSound );
	app->WriteProfileInt( "Sound\\ChatType", "useDefault", snd->useDefault );
	app->WriteProfileString( "Sound\\ChatType", "file", snd->file.c_str() );
	snd = getSound( CST_ChatBack );
	app->WriteProfileInt( "Sound\\ChatBack", "useSound", snd->useSound );
	app->WriteProfileInt( "Sound\\ChatBack", "useDefault", snd->useDefault );
	app->WriteProfileString( "Sound\\ChatBack", "file", snd->file.c_str() );
	snd = getSound( CST_ChatRet );
	app->WriteProfileInt( "Sound\\ChatRet", "useSound", snd->useSound );
	app->WriteProfileInt( "Sound\\ChatRet", "useDefault", snd->useDefault );
	app->WriteProfileString( "Sound\\ChatRet", "file", snd->file.c_str() );
	snd = getSound( CST_PrivateMsg );
	app->WriteProfileInt( "Sound\\PrivateMessage", "useSound", snd->useSound );
	app->WriteProfileInt( "Sound\\PrivateMessage", "useDefault", snd->useDefault );
	app->WriteProfileString( "Sound\\PrivateMessage", "file", snd->file.c_str() );
}

void CChatSoundList::play( const int resID )
{
	::sndPlaySound( NULL, 0 );
	HINSTANCE hinst = AfxGetInstanceHandle();
	HRSRC hResInfo= ::FindResource( hinst, MAKEINTRESOURCE( resID ), "WAVE" );
	if( hResInfo != NULL ) {

		HANDLE hRes = ::LoadResource( hinst, hResInfo );
		if ( hRes != NULL ) {

			LPSTR lpRes = (LPSTR)::LockResource( hRes );
			if( lpRes != NULL ) {

				::sndPlaySound( lpRes, SND_MEMORY | SND_ASYNC );
				::FreeResource( hRes );
			}
		}
	}
}

void CChatSoundList::play( const std::string& file )
{
	bool fileExist = true;
	HANDLE hFile;
	hFile = ::CreateFile( file.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if ( hFile == INVALID_HANDLE_VALUE ) fileExist = false;
	::CloseHandle(hFile);
	if ( fileExist ) {
		::sndPlaySound( NULL, 0 );
		::sndPlaySound( file.c_str(), SND_ASYNC );
	}
}

CChatSound* CChatSoundList::getSound( const CChatSoundType soundType ) const
{
	std::vector< CChatSound* >::const_iterator it = list.begin();
	while ( it != list.end() ) {
		if ( (*it)->type == soundType ) {
			return *it;
		}
		it++;
	}
	return NULL;
}

void CChatSoundList::play( const CChatSoundType soundType )
{
	if ( useSound ) {
		CChatSound* snd = getSound( soundType );
		if ( snd && snd->useSound ) {
			if ( snd->useDefault ) {
				play( snd->res );
			} else {
				if ( !snd->file.empty() ) {
					play( snd->file );
				}
			}
		}
	}
}

void CChatSoundList::setUseSound( const bool value )
{
	if ( useSound != value ) {
		useSound = value;
		if ( !useSound ) {
			::sndPlaySound( NULL, 0 );
		}
		AfxGetApp()->WriteProfileInt( "Sound", "useSound", useSound );
	}
}

std::string CChatSoundList::getName( const CChatSoundType soundType ) const
{
	std::string s;
	int res;
	switch ( soundType ) {
		case CST_Connect         : res = IDS_CONNECT_WAVE; break;
		case CST_Disconnect      : res = IDS_DISCONNECT_WAVE; break;
		case CST_ChangeStatusMode: res = IDS_CHANGESTATUSMODE_WAVE; break;
		case CST_IncomingMessage : res = IDS_INCOMINGMESSAGE_WAVE; break;
		case CST_ChatType        : res = IDS_CHATTYPE_WAVE; break;
		case CST_ChatBack        : res = IDS_CHATBACK_WAVE; break;
		case CST_ChatRet         : res = IDS_CHATRET_WAVE; break;
		case CST_PrivateMsg      : res = IDS_PRIVATEMESSAGE_WAVE; break;
		default                  : res = -1;
	}
	if ( res != -1 ) {
		s = format( res );
	} else {
		s = "";
	}
	return s;
}
